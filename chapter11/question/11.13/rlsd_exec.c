/* rlsd.c - リモートlsサーバー
 */

#include <assert.h>
#include <limits.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define PORTNUM 15000
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

    /* 双方向ストリームとしてオープン*/
    if ((sock_fp = fdopen(sock_fd, "r+")) == NULL)
      oops("fdopen reading");

    if (fgets(dirname, sizeof(dirname), sock_fp) == NULL)
      oops("reading dirname");

    dirname[strcspn(dirname, "\r\n")] = '\0'; // 改行削除

    int pid = -1;

    int fromChild[2];
    if (pipe(fromChild) == -1) {
      perror("pipe");
      fclose(sock_fp);
      continue;
    }

    if ((pid = fork()) == -1) {
      perror("fork");
      close(fromChild[0]);
      close(fromChild[1]);
      fclose(sock_fp);
      continue;
    }

    if (pid == 0) {
      close(fromChild[0]); // 不要なfdを閉じる

      dup2(fromChild[1], STDOUT_FILENO);
      close(fromChild[1]);
      fclose(sock_fp);
      char *argv[] = {"ls", dirname, NULL};
      execvp("ls", argv);
      perror("exec");
      exit(1);
    }

    close(fromChild[1]);

    FILE *fromChildFp = fdopen(fromChild[0], "r");
    if (fromChildFp == NULL) {
      perror("fdopen");
      fclose(sock_fp);
      continue;
    }

    char buf[BUFSIZ];

    while ((fgets(buf, sizeof(buf), fromChildFp) != NULL)) {
      fputs(buf, sock_fp);
      fflush(sock_fp);
    }

    fclose(fromChildFp);

    waitpid(pid, NULL, 0);

    fclose(sock_fp);
  }
}
