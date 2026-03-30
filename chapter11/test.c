#include <arpa/inet.h>
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

  struct in_addr addr;
  memcpy(&addr, hp->h_addr, sizeof(struct in_addr));
  printf("host情報表示\n");
  printf("h_addr : %s \n", inet_ntoa(addr));

  bcopy((void *)hp->h_addr, (void *)&saddr.sin_addr, hp->h_length);
  saddr.sin_port = htons(PORTNUM);
  saddr.sin_family = AF_INET;
}
