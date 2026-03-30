
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define PORTNUM 13000
#define HOSTLEN 256
#define oops(msg)                                                              \
  {                                                                            \
    perror(msg);                                                               \
    exit(1);                                                                   \
  }

int main(int ac, char *av[]) {
  struct sockaddr_in saddr;
  struct hostent *hp;
  char hostname[HOSTLEN];
  int sock_id, sock_fd;
  FILE *sock_fp;
  time_t thetime;

  /* ステップ1:カーネルにソケットを要求する。*/

  sock_id = socket(PF_INET, SOCK_STREAM, 0); // ソケット取得
  if (sock_id == -1)
    oops("socket");

  /* ステップ２：ソケットにアドレスをバインドする。アドレスはホスト、ポート*/

  memset((void *)&saddr, 0, sizeof(saddr)); /*構造体をクリアする*/

  gethostname(hostname, HOSTLEN);
  hp = gethostbyname(hostname);

  bcopy((void *)hp->h_addr, (void *)&saddr.sin_addr, hp->h_length);
  saddr.sin_addr.s_addr = htonl(INADDR_ANY);
  saddr.sin_port = htons(
      PORTNUM); // htons()関数でリトルエンディアン→ビッグエンディアンに変換する。（ネットワークはビッグエンディアンが共通語になっている。）
  saddr.sin_family = AF_INET;

  if (bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
    oops("bind");

  /*
   * ステップ3:キューサイズを１としてソケットを着信できる状態にする。
   */

  if (listen(sock_id, 1) != 0)
    oops("listen");

  /*
   *  メインループ : accept() , write(),close()
   */

  while (1) {
    sock_fd = accept(sock_id, NULL, NULL);
    printf("Wow! got a call!\n");
    if (sock_fd == -1)
      oops("fdopen");

    sock_fp = fdopen(sock_fd, "w");
    if (sock_fp == NULL)
      oops("fdopen");

    thetime = time(NULL);

    fprintf(sock_fp, "The time here is ...");
    fprintf(sock_fp, "%s", ctime(&thetime));
    fclose(sock_fp);
  }
}
