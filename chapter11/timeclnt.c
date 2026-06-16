/*
 *  timeclnt.c - timeserv.c のクライアント
 *  使い方：timeclnt <ホスト名> <ポート番号>
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

int main(int ac, char *av[]) {
  struct sockaddr_in servadd;
  struct hostent *hp;
  int sock_id;
  char message[BUFSIZ];
  int messlen;

  /*
   *  ソケットを作成する。
   */
  sock_id = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_id == -1)
    oops("socket");

  /*
   *
   */
  memset(&servadd, 0, sizeof(servadd));

  hp = gethostbyname(av[1]);
  if (hp == NULL)
    oops(av[1]);
  memcpy((struct sockaddr *)&servadd.sin_addr, hp->h_addr, hp->h_length);

  servadd.sin_port = htons(atoi(av[2]));

  servadd.sin_family = AF_INET;

  if (connect(sock_id, (struct sockaddr *)&servadd, sizeof(servadd)) != 0)
    oops("connect");

  /*
   *  ステップ３: サーバからデータを受け取って電話を切る。
   */

  messlen = read(sock_id, message, BUFSIZ);
  if (messlen == -1)
    oops("read");
  if (write(1, message, messlen) != messlen)
    oops("write");
  close(sock_id);
}
