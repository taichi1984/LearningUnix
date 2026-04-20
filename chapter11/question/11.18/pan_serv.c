#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define PORTNUM 10000
#define HOSTLEN 256

#define oops(msg)                                                              \
  {                                                                            \
    perror(msg);                                                               \
    exit(1);                                                                   \
  }
int setup_server(int port);
ssize_t write_all(int sock_fd, const void *buf, size_t rn);

int main() {
  int sock_id = setup_server(PORTNUM);
  int que_no = 1;
  ssize_t wn = -1;
  while (1) {
    int sock_fd = accept(sock_id, NULL, NULL);
    if (sock_fd == -1) {
      perror("accept");
      continue;
    }
    int net_no = htonl(que_no);
    wn = write_all(sock_fd, &net_no, sizeof(int));
    if (wn == -1) {
      perror("write");
      close(sock_fd);
      continue;
    }

    que_no++;
    close(sock_fd);
  }
}

int setup_server(int port) {
  struct sockaddr_in saddr;
  char hostname[HOSTLEN];
  int sock_id;

  /* ステップ1:カーネルにソケットを要求する。*/

  sock_id = socket(PF_INET, SOCK_STREAM, 0); // ソケット取得
  if (sock_id == -1)
    return -1;

  /* ステップ２：ソケットにアドレスをバインドする。アドレスはホスト、ポート*/

  memset((void *)&saddr, 0, sizeof(saddr)); /*構造体をクリアする*/

  saddr.sin_addr.s_addr = htonl(INADDR_ANY);
  saddr.sin_port = htons(
      port); // htons()関数でリトルエンディアン→ビッグエンディアンに変換する。（ネットワークはビッグエンディアンが共通語になっている。）
  saddr.sin_family = AF_INET;

  if (bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
    return -1;

  /*
   * ステップ3:キューサイズを１としてソケットを着信できる状態にする。
   */

  if (listen(sock_id, 5) != 0)
    return -1;

  return sock_id;
}

ssize_t write_all(int sock_fd, const void *buf, size_t rn) {
  ssize_t wr = -1;
  ssize_t total = 0;
  const char *p = buf;

  while (total < rn) {
    wr = write(sock_fd, p + total, rn - total);

    if (wr < 0) {
      return -1;
    }
    if (wr == 0) {
      break;
    }
    total += wr;
  }
  return total;
}
