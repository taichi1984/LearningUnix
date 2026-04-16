#include "hashmap.h"
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

#define oops(msg)                                                              \
  {                                                                            \
    perror(msg);                                                               \
    exit(1);                                                                   \
  }

int setup_server(int port);
int setup_cliant(char *ipaddr, int port);
ssize_t write_all(int sock_fd, char *buf, int rn);

int main(int ac, char *av[]) {
  int sock_id = -1;
  int sock_fd = -1;
  struct timeval rec_time, current_time;
  char cache[BUFSIZ];
  int cache_len = 0;
  Node *map[MAPSIZE];

  for (int i = 0; i < 256; i++) {
    map[i] = NULL;
  }

  if ((sock_id = setup_server(PORTNUM)) == -1) {
    perror("setup_server");
    exit(1);
  }
  if (gettimeofday(&rec_time, NULL) == -1) {
    perror("gettimeofday initialize");
    exit(1);
  }

  while (1) {

    char key[BUFSIZ];
    char value[BUFSIZ];
    ssize_t rn = -1;
    ssize_t wn = -1;

    sock_fd = accept(sock_id, NULL, NULL);
    if (sock_fd == -1) {
      perror("accept");
      continue;
    }

    rn = read(sock_fd, key, sizeof(key) - 1);
    if (rn <= 0) {
      close(sock_fd);
      continue;
    }
    key[rn] = '\0';
    key[strcspn(key, "\r\n")] = '\0';

    Node *tmp;

    if ((tmp = get_node(map, MAPSIZE, key)) == NULL) {
      int cliant_sockfd = -1;
      if ((cliant_sockfd = setup_cliant("192.168.0.6", FINGERPORT)) < 0) {
        perror("setup_cliant");
        close(sock_fd);
        continue;
      }

      write_all(cliant_sockfd, key, strlen(key));
      write_all(cliant_sockfd, "\r\n", 2);

      rn = read(cliant_sockfd, value, sizeof(value));
      if (rn <= 0) {
        close(sock_fd);
        continue;
      }
      value[rn] = '\0';

      wn = write_all(sock_fd, value, rn);
      if (rn == -1) {
        perror("read");
      }
      if (wn == -1) {
        perror("write_all");
        break;
      }

      close(cliant_sockfd);
      put_node(map, MAPSIZE, key, value);
    } else {
      Node *res_node;
      res_node = get_node(map, MAPSIZE, key);

      if (write_all(sock_fd, res_node->value, strlen(res_node->value)) == -1 ||
          write_all(sock_fd, "this is cash\n", sizeof("this is cash\n")) ==
              -1) {
        perror("write_all");
      }
    }

    close(sock_fd);
  }

  return 0;
}

int setup_cliant(char *ipaddr, int port)
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
      PORTNUM); // htons()関数でリトルエンディアン→ビッグエンディアンに変換する。（ネットワークはビッグエンディアンが共通語になっている。）
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

ssize_t write_all(int sock_fd, char *buf, int rn) {
  int wr = -1;
  int total = 0;
  while (total < rn) {
    wr = write(sock_fd, buf + total, rn - total);

    if (wr <= 0) {
      return -1;
    }
    total += wr;
  }
  return total;
}
