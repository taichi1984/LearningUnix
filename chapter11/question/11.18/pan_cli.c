
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define FINGERPORT 7979
#define PORTNUM 12000
#define HOSTLEN 256
#define MAPSIZE 256

int setup_client(char *ipaddr, int port);
ssize_t read_all(int fd, void *buf, size_t len);

int main() {
  int sock_id = setup_client("192.168.0.6", 10000);

  int rn = 0;
  int buf = 0;
  rn = read_all(sock_id, &buf, sizeof(buf));
  if (rn == -1) {
    perror("read_all");
    exit(1);
  }
  close(sock_id);

  buf = ntohl(buf);
  printf("番号札は %d 番です。\n", buf);

  return 0;
}

int setup_client(char *ipaddr, int port)
/*
 * クライアントとして第１引数で入力したサーバーに接続する。
 * 戻り値:成功時はsock_idを、失敗時は-1を返す
 */
{

  struct sockaddr_in servadd;
  char hostname[HOSTLEN];
  struct hostent *hp;
  int sock_id = -1;

  /* ステップ１：　ソケットを作成する*/
  if ((sock_id = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
    return sock_id;
  }

  /** ステップ2 : サーバーに接続する*/

  memset(&servadd, 0, sizeof(servadd));
  hp = gethostbyname(ipaddr);
  if (hp == NULL) {
    perror("gethostbyname");
    return -1;
  }

  memcpy((struct sockaddr *)&servadd.sin_addr, hp->h_addr, hp->h_length);
  servadd.sin_port = htons(port);
  servadd.sin_family = AF_INET;

  if (connect(sock_id, (struct sockaddr *)&servadd, sizeof(servadd)) != 0) {
    perror("connect");
    return -1;
  }

  return sock_id;
}

ssize_t read_all(int fd, void *buf, size_t len) {
  ssize_t total = 0;
  ssize_t rn;
  char *p = buf;
  while (total < len) {
    rn = read(fd, p + total, len - total);
    if (rn < 0) {
      return -1;
    }
    if (rn == 0) {
      break;
    }
    total += rn;
  }

  return total;
}
