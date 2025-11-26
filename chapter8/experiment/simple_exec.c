
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define CMDMAX 1024
int main() {
  char command[CMDMAX];
  while (1) {
    printf("$ ");
    fgets(command, sizeof(command), stdin);
    command[strcspn(command, "\n")] = '\0';

    char *argv[10];
    char *token = strtok(command, " ");
    argv[0] = token;
    int i = 1;

    while (token != NULL) {
      token = strtok(NULL, " ");
      argv[i] = token;
      i++;
    }

    pid_t pid = fork();

    if (pid == 0) {
      int result = execvp(argv[0], argv);

      if (result == -1) {
        perror("execve");
        exit(1);
      }
    } else if (pid > 0) {
      wait(NULL);
    } else {
      perror("fork");
    }
  }
  return 0;
}
