/*
 * rls.c
 * リモートディレクトリリスト表示サービスのクライアント
 * 使い方：rls <ホスト名> <ディレクトリ>
 */

#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define oops(msg)                                                              \
  {                                                                            \
    perror(msg);                                                               \
    exit(1);                                                                   \
  }
#define PORTNUM 15000

int main(int ac, char *av[]) {
  struct sockaddr_in servadd;
  struct hostent *hp;
  int sock_id;
  char buffer[BUFSIZ];
  int n_read;

  if (ac != 3)
    exit(1);

  /* ステップ１：　ソケットを作成する*/
  if ((sock_id = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    oops("socket");

  /** ステップ2 : サーバーに接続する*/

  memset(&servadd, 0, sizeof(servadd));
  hp = gethostbyname(av[1]);
  if (hp == NULL)
    oops(av[1]);

  memcpy((struct sockaddr *)&servadd.sin_addr, hp->h_addr, hp->h_length);
  servadd.sin_port = htons(PORTNUM);
  servadd.sin_family = AF_INET;

  if (connect(sock_id, (struct sockaddr *)&servadd, sizeof(servadd)) != 0)
    oops("connect");

  /* ステップ3 : ディレクトリ名を送り、結果を読み出す **/

  if (write(sock_id, av[2], strlen(av[2])) == -1)
    oops("write");
  if (write(sock_id, "\n", 1) == -1)
    oops("write");

  while ((n_read = read(sock_id, buffer, BUFSIZ)) > 0)
    if (write(1, buffer, n_read) == -1)
      oops("write");

  close(sock_id);

  return 0;
}
