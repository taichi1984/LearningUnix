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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void read_til_crnl(FILE *fp);
void process_rq(char *rq, int fd, char *result);
void cannot_do(int fd);
int isadir(char *f);
int not_exist(char *f);
int do_cat(char *f, int fd);
int do_ls(char *dir, int fd);
void do_404(char *item, int fd);
void do_exec(char *prog, int fd);
int ends_in_cgi(char *f);

int main(int ac, char *av[]) {
  int sock, fd;
  FILE *fpin;
  struct sockaddr_in cli_data;
  socklen_t socklen = sizeof(cli_data);
  char request[BUFSIZ];
  char buf[INET_ADDRSTRLEN];
  char result[4];
  if (ac == 1) {
    fprintf(stderr, "usage: ws portnum\n");
    exit(1);
  }
  sock = make_server_socket(atoi(av[1]));
  if (sock == -1)
    exit(2);

  while (1) {
    fd = accept(sock, (struct sockaddr *)&cli_data, &socklen);
    fpin = fdopen(fd, "r");
    inet_ntop(AF_INET, &cli_data.sin_addr, buf, sizeof(buf));
    /*リクエストを読み出す*/
    fgets(request, BUFSIZ, fpin);
    request[strcspn(request, "\r\n")] = '\0';
    read_til_crnl(fpin);

    /* クライアントが要求した処理を実行する*/
    process_rq(request, fd, result);
    printf("%s %s %s\n", buf, request, result);
    fflush(stdout);

    fclose(fpin);
  }
}

void read_til_crnl(FILE *fp) {
  char buf[BUFSIZ];
  while (fgets(buf, BUFSIZ, fp) != NULL && strcmp(buf, "\r\n") != 0)
    ;
}

void process_rq(char *rq, int fd, char *result) {
  char cmd[BUFSIZ], arg[BUFSIZ];
  strcpy(arg, "./");

  if (sscanf(rq, "%s%s", cmd, arg + 2) != 2)
    return;

  if (fork() != 0) {
    if (strcmp(cmd, "GET") != 0) {
      strcpy(result, "501");
    } else if (not_exist(arg)) {
      strcpy(result, "404");
    } else if (isadir(arg)) {
      strcpy(result, "200");
    } else if (ends_in_cgi(arg)) {
      strcpy(result, "200");
    } else {
      strcpy(result, "200");
    }

    return;
  }

  if (strcmp(cmd, "GET") != 0) {
    cannot_do(fd);
  } else if (not_exist(arg)) {
    do_404(arg, fd);
  } else if (isadir(arg)) {
    do_ls(arg, fd);
  } else if (ends_in_cgi(arg)) {
    do_exec(arg, fd);
  } else {
    do_cat(arg, fd);
  }
}

void header(FILE *fp, char *content_type) {
  fprintf(fp, "HTTP/1.0 200 OK \r\n");
  if (content_type) {
    fprintf(fp, "Content-type: %s; ", content_type);
    fprintf(fp, "charset=UTF-8\r\n");
  }
}

void cannot_do(int fd) {
  FILE *fp = fdopen(fd, "w");
  fprintf(fp, "HTTP/1.0 501 Not Implemented\r\n");
  fprintf(fp, "Content-type : text/plain\r\n");
  fprintf(fp, "\r\n");

  fprintf(fp, "That command is not yet implemented \r\n");
  fclose(fp);
  _exit(0);
}

void do_404(char *item, int fd) {
  FILE *fp = fdopen(fd, "w");

  fprintf(fp, "HTTP/1.0 404 Not Found\r\n");
  fprintf(fp, "Content-type: text/plain\r\n");
  fprintf(fp, "\r\n");

  fprintf(fp, "The item you qequested : %s\r\n  is not found \r\n", item);
  fclose(fp);
  _exit(0);
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

int do_ls(char *dir, int fd) {
  FILE *fp;
  fp = fdopen(fd, "w");
  header(fp, "text/plain");
  fprintf(fp, "\r\n");
  fflush(fp);

  dup2(fd, 1);
  dup2(fd, 2);
  close(fd);
  execlp("ls", "ls", "-1", dir, NULL);
  perror(dir);
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

void do_exec(char *prog, int fd) {
  FILE *fp;

  fp = fdopen(fd, "w");
  header(fp, NULL);
  fflush(fp);
  dup2(fd, 1);
  dup2(fd, 2);
  close(fd);
  execl(prog, prog, NULL);
  perror(prog);
}

/* --------------------------------------------*
 * do_cat(filename , fd)
 * ヘッダを送ってから内容を送り返す
 * --------------------------------------------*/

int do_cat(char *f, int fd) {
  char *extension = file_type(f);
  char *content = "text/plain";
  FILE *fpsock, *fpfile;
  int c;

  if (strcmp(extension, "html") == 0)
    content = "text/html";
  else if (strcmp(extension, "gif") == 0)
    content = "image/gif";
  else if (strcmp(extension, "jpg") == 0)
    content = "image/jpeg";
  else if (strcmp(extension, "jpeg") == 0)
    content = "image/jpeg";

  fpsock = fdopen(fd, "w");
  fpfile = fopen(f, "r");
  if (fpsock != NULL && fpfile != NULL) {
    header(fpsock, content);
    fprintf(fpsock, "\r\n");
    while ((c = getc(fpfile)) != EOF)
      putc(c, fpsock);
    fclose(fpfile);
    fclose(fpsock);
  }
  _exit(0);
}
