#include <alloca.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORTNUM 79
#define HOSTLEN 256
int setup_server(int port);

int main() {
  int sock_id, sock_fd;
  FILE *sock_fp;

  sock_id = setup_server(PORTNUM);
  if (sock_id == -1) {
    perror("socket");
    exit(1);
  }

  /* メインループ*/

  while (1) {

    char request[BUFSIZ];
    sock_fd = accept(sock_id, NULL, NULL);
    if (sock_fd == -1) {
      perror("accept");
      continue;
    }
    sock_fp = fdopen(sock_fd, "r");
    if (sock_fp == NULL) {
      perror("fdopen");
      close(sock_fd);
      continue;
    }

    if (fgets(request, sizeof(request), sock_fp) == NULL) {
      perror("fgets");
      fclose(sock_fp);
      continue;
    };

    request[strcspn(request, "\r\n")] = '\0';

    struct passwd *pw = getpwnam(request);

    char buf[BUFSIZ];
    int len;

    if (pw == NULL) {
      len = snprintf(buf, sizeof(buf), "user: %s is not found \n", request);
    } else {
      len = snprintf(
          buf, sizeof(buf),
          "username : %s \n uid : %d \n gid : %d \n home : %s \n shell : %s \n",
          pw->pw_name, pw->pw_uid, pw->pw_gid, pw->pw_dir, pw->pw_shell);
    }

    write(sock_fd, buf, len);

    fclose(sock_fp);
  }
  return 0;
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
