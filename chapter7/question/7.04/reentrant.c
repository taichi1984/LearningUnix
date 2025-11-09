#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  void entrant();
  struct sigaction newhandler;
  newhandler.sa_handler = entrant;
  newhandler.sa_flags = SA_NODEFER;

  if (sigaction(SIGINT, &newhandler, NULL) == -1) {
    perror("sigaction");
  }

  while (1) {
    printf("処理行われてる?\n");
    pause();
  }
}

void entrant() {
  char sentence1[20] = "sentence1\n";
  char sentence2[20] = "sentence2\n";
  char sentence3[20] = "sentence3\n";

  char sentence4[80] = "";
  int len = snprintf(sentence4, sizeof(sentence4), "%s%s%s", sentence1,
                     sentence2, sentence3);

  printf("ハンドラが呼ばれました\n");
  int fd = open("testfile", O_APPEND | O_WRONLY);
  if (fd == -1) {
    perror("open");
    exit(1);
  }
  printf("writing sentences\n");
  sleep(3);
  write(fd, sentence4, len);

  if (close(fd) == -1) {
    perror("close");
    exit(2);
  }
}
