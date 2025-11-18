/* sigdemo3.c
 * 目的:シグナルの問いに対する答えを示す。
 * 問1:シグナルが届いたあともハンドラは有効なままか。
 * 問2:シグナルXの処理中にシグナルXが届いたらどうなるか。
 * 問3:シグナルYの処理中にシグナルXが届いたらどうなるか。
 * 問4:read()の実行中にシグナルが届いたらどうなるか。
 */

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define INPUTLEN 100

int main(int ac, char *av[]) {
  void signalhandler(int);
  void inthandler(int);
  void quithandler(int);
  char input[INPUTLEN];
  int nchars;

  signal(SIGINT, signalhandler);
  signal(SIGQUIT, signalhandler);

  do {
    printf("\nType a message\n");
    nchars = read(0, input, (INPUTLEN - 1));
    if (nchars == -1)
      perror("read returned an error");
    else {
      input[nchars] = '\0';
      printf("You typed : %s", input);
    }
  } while (strncmp(input, "quit", 4) != 0);
}

void signalhandler(int signum) {
  if (signum == SIGINT) {
    printf(" Received signal%d .. waiting\n", signum);
    sleep(2);
    printf(" Leaving inthandler \n");
  }
  if (signum == SIGQUIT) {
    printf(" Received signal %d .. waiting\n", signum);
    sleep(3);
    printf(" Leaving quithandler \n");
  }
}
