/* webserv.c - 最小限のWebサーバー(バージョン0.2)
 * 使い方:webserv<ポート番号>
 * 機能 : GETコマンドのみをサポートする
 * カレントディレクトリで動作する。
 * 新しい子プロセスを作って、個々のリクエストを処理する。
 * 非常に大きなセキュリティホールがあるので、あくまでもデモ用である。
 * 他にも弱点はいくつもあるが、スタート地点としては満足できる。
 * コンパイル: cc webserv.c socklib.c -o webserv
 */

#include "socklib.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

enum http_method {
  METHOD_GET,
  METHOD_HEAD,
  METHOD_POST,
  METHOD_PUT,
  METHOD_DELETE,
  METHOD_CONNECT,
  METHOD_OPTIONS,
  METHOD_TRACE,
  METHOD_UNKNOWN,
};

struct http_header {
  char *http_version;

  int status_code;
  char *reason_phrase;

  char *content_type;
  long content_length;

  char *charset;
  long charset_length;

  char *server;
  time_t date;
  time_t last_modified;

  int connection_close;
  int accept_ranges;
};

struct http_response {
  struct http_header header;

  int body_fd;

  char *memory_body;

  size_t memory_length;
};

struct http_request {
  enum http_method method;
  char method_str[32];
  char path[BUFSIZ];
  char version[32];

  struct header *headers;

  char *body;
  size_t body_length;
};

void read_til_crnl(FILE *fp);
void process_rq(struct http_request *rq, int fd, struct http_response *response,
                char *addr);
void cannot_do(struct http_response *response);
int isadir(char *f);
int not_exist(char *f);
int do_cat(char *f, int fd, struct http_request *request,
           struct http_response *response);
int do_ls(char *dir, int fd, struct http_request *request,
          struct http_response *response);
void do_404(char *file, struct http_response *response);
void do_exec(char *prog, int fd);
int ends_in_cgi(char *f);
int in_cgi_bin(char *f);
int initialize_env(const char *cmd, const char *addr);
void child_handler(int sig);
void send_header(FILE *fp, struct http_header *head);
void send_body(FILE *fp, struct http_response *response);
enum http_method parse_method(const char *s);

int main(int ac, char *av[]) {
  int sock, fd;
  FILE *fpin;
  struct sockaddr_in cli_data;
  socklen_t socklen = sizeof(cli_data);
  char request_str[BUFSIZ];
  char buf[INET_ADDRSTRLEN];

  if (ac == 1) {
    fprintf(stderr, "usage: ws portnum\n");
    exit(1);
  }

  struct sigaction sa;
  sa.sa_handler = child_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;

  sigaction(SIGCHLD, &sa, NULL);

  sock = make_server_socket(atoi(av[1]));
  if (sock == -1)
    exit(2);

  while (1) {
    struct http_request request = {0};
    struct http_response response = {0};
    enum http_method method;
    fd = accept(sock, (struct sockaddr *)&cli_data, &socklen);
    fpin = fdopen(fd, "r");
    inet_ntop(AF_INET, &cli_data.sin_addr, buf, sizeof(buf));
    /*リクエストを読み出す*/
    fgets(request_str, BUFSIZ, fpin);
    request_str[strcspn(request_str, "\r\n")] = '\0';

    char cmd[BUFSIZ], arg[BUFSIZ], ver[BUFSIZ];
    strcpy(arg, "./");
    sscanf(request_str, "%s %s %s", cmd, arg + 2, ver);

    request.method = parse_method(cmd);
    strcpy(request.method_str, cmd);
    strcpy(request.path, arg);
    strcpy(request.version, ver);

    read_til_crnl(fpin);

    /* クライアントが要求した処理を実行する*/
    process_rq(&request, fd, &response, buf);

    fclose(fpin);
  }
}

void read_til_crnl(FILE *fp) {
  char buf[BUFSIZ];
  while (fgets(buf, BUFSIZ, fp) != NULL && strcmp(buf, "\r\n") != 0)
    ;
}

void process_rq(struct http_request *request, int fd,
                struct http_response *response, char *addr) {

  if (fork() == 0) {
    int handle = 0;
    if (request->method == METHOD_UNKNOWN) {
      cannot_do(response);
    } else if (not_exist(request->path)) {
      do_404(request->path, response);
    } else if (isadir(request->path)) {
      do_ls(request->path, fd, request, response);
      handle = 1;
    } else if (in_cgi_bin(request->path) || ends_in_cgi(request->path)) {
      initialize_env(request->method_str, addr);
      do_exec(request->path, fd);
      handle = 1;
    } else {
      do_cat(request->path, fd, request, response);
      handle = 1;
    }

    if (!handle) {
      FILE *fp = fdopen(fd, "w");
      send_header(fp, &response->header);
      if (request->method != METHOD_HEAD)
        send_body(fp, response);
      fflush(fp);
      fclose(fp);
      _exit(1);
    }
  }
}

void send_header(FILE *fp, struct http_header *h) {
  fprintf(fp, "%s %d %s\r\n", h->http_version, h->status_code,
          h->reason_phrase);
  if (h->server) {
    fprintf(fp, "Server: %s\r\n", h->server);
  }
  if (h->date) {
    struct tm *tm;
    char buf[128];
    tm = gmtime(&h->date);
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", tm);
    fprintf(fp, "Date: %s\r\n", buf);
  }
  if (h->content_type) {
    fprintf(fp, "Content-Type: %s", h->content_type);
    if (h->charset)
      fprintf(fp, "; charset=%s", h->charset);
    fprintf(fp, "\r\n");
  }
  if (h->last_modified) {
    struct tm *tm;
    char buf[128];
    tm = gmtime(&h->last_modified);
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", tm);

    fprintf(fp, "Last-Modified: %s \r\n", buf);
  }
  if (h->content_length) {
    fprintf(fp, "Content-Length : %ld \r\n", h->content_length);
  }

  if (h->accept_ranges) {
    fprintf(fp, "Accept-Ranges: bytes \r\n");
  }
  if (h->connection_close) {
    fprintf(fp, "Connection: close \r\n");
  }

  fprintf(fp, "\r\n");
}

void send_body(FILE *fp, struct http_response *response) {
  fprintf(fp, "%s", response->memory_body);
}

void cannot_do(struct http_response *response) {
  response->header.http_version = "HTTP/1.0";
  response->header.status_code = 501;
  response->header.reason_phrase = "Not Implemented";
  response->header.content_type = "text/plain";
  response->memory_body = "That command is not yet implemented \r\n";
  response->memory_length = strlen(response->memory_body);
  response->header.content_length = response->memory_length;
}

void do_404(char *filename, struct http_response *response) {
  response->header.http_version = "HTTP/1.0";
  response->header.status_code = 404;
  response->header.reason_phrase = "Not Found";
  response->header.content_type = "text/plain";
  static char buf[256];
  snprintf(buf, sizeof(buf),
           "The item you requested : %s\r\n"
           "is not found\r\n",
           filename);
  response->memory_body = buf;
  response->memory_length = strlen(response->memory_body);
  response->header.content_length = response->memory_length;
}

/* -------------------------------------------------*
 * ディレクトリリスト部
 * isadir()はstatを使う。not_exist()もstatを使う.
 * do_lsはlsを実行するが、避けるべき。
 * ---------------------------------------------------*/
int isadir(char *f) {
  struct stat info;
  return (stat(f, &info) != -1 && S_ISDIR(info.st_mode));
}

int not_exist(char *f) {
  struct stat info;
  return (stat(f, &info) == -1);
}

int do_ls(char *dir, int fd, struct http_request *request,
          struct http_response *response) {
  FILE *fp;
  fp = fdopen(fd, "w");
  response->header.http_version = "HTTP/1.0";
  response->header.status_code = 200;
  response->header.reason_phrase = "OK";
  response->header.content_type = "text/plain";
  send_header(fp, &response->header);

  fflush(fp);

  if (request->method != METHOD_HEAD) {
    dup2(fd, 1);
    dup2(fd, 2);
    close(fd);
    execlp("ls", "ls", "-1", dir, NULL);
    perror(dir);
  }
  exit(1);
}

/* -----------------------------------------------*
 * cgi処理部
 * 拡張子をチェックする関数とプログラムを実行する関数
 * -----------------------------------------------*/

char *file_type(char *f)
/* returns 'extension' of file */
{
  char *cp;
  if ((cp = strrchr(f, '.')) != NULL)
    return cp + 1;
  return "";
}

int ends_in_cgi(char *f) { return (strcmp(file_type(f), "cgi") == 0); }
int in_cgi_bin(char *f) { return (strncmp(f, ".//cgi-bin/", 11) == 0); }
void do_exec(char *prog, int fd) {

  dup2(fd, 1);
  dup2(fd, 2);
  close(fd);
  execl(prog, prog, NULL);
  perror(prog);
  _exit(1);
}

/* --------------------------------------------*
 * do_cat(filename , fd)
 * ヘッダを送ってから内容を送り返す
 * --------------------------------------------*/

int do_cat(char *f, int fd, struct http_request *request,
           struct http_response *response) {
  char *extension = file_type(f);
  struct stat st;
  char timebuf[128];
  time_t now = time(NULL);
  struct tm *tm = gmtime(&now);

  response->header.content_type = "text/plain";

  FILE *fpsock, *fpfile;
  int c;
  response->header.http_version = "HTTP/1.0";
  response->header.status_code = 200;
  response->header.reason_phrase = "OK";
  response->header.server = "MyWebServer 1.0";
  if (strcmp(extension, "html") == 0)
    response->header.content_type = "text/html";
  else if (strcmp(extension, "gif") == 0)
    response->header.content_type = "image/gif";
  else if (strcmp(extension, "jpg") == 0)
    response->header.content_type = "image/jpeg";
  else if (strcmp(extension, "jpeg") == 0)
    response->header.content_type = "image/jpeg";

  response->header.charset = "UTF-8";
  response->header.date = time(NULL);

  if (stat(f, &st) == -1) {
    perror("stat");
    do_404(f, response);
    return -1;
  }
  response->header.last_modified = st.st_mtime;
  response->header.content_length = st.st_size;
  response->header.accept_ranges = 1;
  response->header.connection_close = 1;

  fpsock = fdopen(fd, "w");
  fpfile = fopen(f, "r");
  if (fpsock != NULL && fpfile != NULL) {
    send_header(fpsock, &response->header);
    if (request->method != METHOD_HEAD)
      while ((c = getc(fpfile)) != EOF)
        putc(c, fpsock);

    fclose(fpfile);
    fclose(fpsock);
  }
  _exit(0);
}

int initialize_env(const char *cmd, const char *addr) {
  int rv = -1;
  if ((rv = setenv("REQUEST_METHOD", cmd, 1)) == -1) {
    perror("setenv");
    return rv;
  }
  if ((rv = setenv("REMOTE_ADDR", addr, 1)) == -1) {
    perror("setenv");
    return rv;
  }

  return rv;
}

void child_handler(int sig) {
  while (waitpid(-1, NULL, WNOHANG) > 0)
    ;
}

enum http_method parse_method(const char *s) {

  if (strcmp(s, "GET") == 0)
    return METHOD_GET;

  if (strcmp(s, "HEAD") == 0)
    return METHOD_HEAD;

  if (strcmp(s, "POST") == 0)
    return METHOD_POST;

  if (strcmp(s, "PUT") == 0)
    return METHOD_PUT;

  if (strcmp(s, "DELETE") == 0)
    return METHOD_DELETE;

  if (strcmp(s, "CONNECT") == 0)
    return METHOD_CONNECT;

  if (strcmp(s, "OPTIONS") == 0)
    return METHOD_OPTIONS;

  if (strcmp(s, "TRACE") == 0)
    return METHOD_TRACE;

  return METHOD_UNKNOWN;
}
