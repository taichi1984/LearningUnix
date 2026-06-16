/*
 *  socklib.c
 *
 *  このファイルには、インターネットクライアント／サーバープログラムを書くときによく使われる関数が含まれている。
 *  ２つの主要関数は次の通り。
 *
 *  int make_server_socket(portnum)
 *                 サーバーをソケットを返す。エラーのときは-1を返す。
 *  int make_server_socket_q(portnum , backlog)
 *
 *  int connect_to_server(char *hostname, int portnum)
 *                  接続されたソケットを返す。エラーのときは-1を返す。
 */

#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define HOSTLEN 256
#define BACKLOG 1
int connect_to_server(char *host, int portnum);
int make_server_socket_q(int, int);

int make_server_socket(int portnum) {
  return make_server_socket_q(portnum, BACKLOG);
}

int make_server_socket_q(int portnum, int backlog) {
  struct sockaddr_in saddr;
  struct hostent *hp;
  char hostname[HOSTLEN];
  int sock_id;

  sock_id = socket(PF_INET, SOCK_STREAM, 0);
  if (sock_id == -1)
    return -1;

  /** アドレスを構築して、ソケットにバインドする **/

  memset((void *)&saddr, 0, sizeof(saddr));
  saddr.sin_port = htons(portnum);
  saddr.sin_addr.s_addr = htonl(INADDR_ANY);
  saddr.sin_family = AF_INET;
  if (bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
    return -1;

  if (listen(sock_id, backlog) != 0)
    return -1;
  return sock_id;
}

int connect_to_server(char *host, int portnum) {
  int sock;
  struct sockaddr_in servadd;
  struct hostent *hp;

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1)
    return -1;

  memset(&servadd, 0, sizeof(servadd));
  hp = gethostbyname(host);
  if (hp == NULL)
    return -1;
  memcpy((struct sockaddr *)&servadd.sin_addr, hp->h_addr, hp->h_length);
  servadd.sin_port = htons(portnum);
  servadd.sin_family = AF_INET;

  if (connect(sock, (struct sockaddr *)&servadd, sizeof(servadd)) != 0)
    return -1;

  return sock;
}
