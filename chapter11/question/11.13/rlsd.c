/* rlsd.c - リモートlsサーバー
 */

#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORTNUM 15000
#define HOSTLEN 256
#define oops(msg)                                                              \
  {                                                                            \
    perror(msg);                                                               \
    exit(1);                                                                   \
  }
size_t sanitize(char *src, char *dest, size_t size);

int main(int ac, char *av[]) {
  struct sockaddr_in saddr;
  struct hostent *hp;
  char hostname[HOSTLEN];
  int sock_id, sock_fd;
  FILE *sock_fpi, *sock_fpo;
  FILE *pipe_fp;
  char dirname[PATH_MAX];

  char command[BUFSIZ];
  int dirlen, c;

  /* ステップ1 : カーネルにソケットを要求する */

  sock_id = socket(PF_INET, SOCK_STREAM, 0);
  if (sock_id == -1) {
    oops("socket");
  }

  /** ステップ２：ソケットにアドレスをバインドする。アドレスはホスト、ポート **/

  memset((void *)&saddr, 0, sizeof(saddr));
  gethostname(hostname, HOSTLEN);
  hp = gethostbyname(hostname);
  memcpy((void *)&saddr.sin_addr, hp->h_addr, hp->h_length);
  saddr.sin_addr.s_addr = htonl(INADDR_ANY);
  saddr.sin_port = htons(PORTNUM);
  saddr.sin_family = AF_INET;
  if (bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
    oops("bind");

  /** ステップ3 : キューサイズを１として、ソケットを着信できる状態にする。 */
  if (listen(sock_id, 1) != 0)
    oops("listen");
  /*
   *  メインループ: accept(), write(),close()
   */

  while (1) {
    sock_fd = accept(sock_id, NULL, NULL);
    if (sock_fd == -1)
      oops("accept");

    /* 読み出し方向をストリームとしてオープン*/
    if ((sock_fpi = fdopen(sock_fd, "r")) == NULL)
      oops("fdopen reading");

    if (fgets(dirname, sizeof(dirname), sock_fpi) == NULL)
      oops("reading dirname");

    dirname[strcspn(dirname, "\n")] = '\0';

    char sanitized_dirname[sizeof(dirname) * 2];

    size_t dir_size =
        sanitize(dirname, sanitized_dirname, sizeof(sanitized_dirname));

    /* 書き込み方向をストリームとしてオープン*/
    if ((sock_fpo = fdopen(sock_fd, "w")) == NULL)
      oops("fdopen writing");

    int ret = snprintf(command, sizeof(command), "ls %s", sanitized_dirname);
    ret = if ((pipe_fp = popen(command, "r")) == NULL) oops("popen");

    while ((c = getc(pipe_fp)) != EOF)
      putc(c, sock_fpo);
    pclose(pipe_fp);
    fclose(sock_fpo);
    fclose(sock_fpi);
  }
}

size_t sanitize(char *src, char *dest, size_t size)
/*
 *  誰かが "; rm*"のようなディレクトリ名を送ってきたときに、素直に" ls ; rm *"
 * というコマンドを作るのでは非常にまずい。
 *
 *  そこでスラッシュと英数字以外の全ての文字を取り除く。
 *  もっと良い方法がある。練習問題を参照のこと。
 */
{
  size_t used = 0;

  for (; *src; src++) {
    if (*src == '*' || *src == ';' || *src == ' ') {
      if (used + 2 >= size)
        break;
      dest[used++] = '\\';
      dest[used++] = *src;
    } else if (*src == '/' || isalnum(*src) || *src == '.' || *src == '-') {
      if (used + 1 >= size)
        break;
      dest[used++] = *src;
    }
  }

  if (size > 0)
    dest[used] = '\0';

  return used;
}
