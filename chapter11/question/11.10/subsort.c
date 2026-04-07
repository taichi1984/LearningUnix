#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int ac, char *av[]) {

  char *filename;
  int toSort[2], fromSort[2];
  if (ac != 2) {
    fprintf(stderr, "invalid argument\n");
    exit(1);
  }

  filename = av[1];

  int ret = pipe(toSort);
  if (ret == -1) {
    perror("pipe");
    exit(1);
  }

  ret = pipe(fromSort);
  if (ret == -1) {
    perror("pipe");
    exit(1);
  }

  int pid = -1;
  if ((pid = fork()) == -1) {
    perror("fork");
    exit(1);
  }

  if (pid == 0) {
    // 子プロセスの処理(sort側）
    dup2(toSort[0], 0);
    dup2(fromSort[1], 1);

    close(toSort[0]);
    close(toSort[1]);
    close(fromSort[0]);
    close(fromSort[1]);

    char *args[] = {"sort", NULL};
    execvp("sort", args);
    perror("exec");
    exit(1);
  } else {

    // 親プロセスの処理

    FILE *fromSortfp = fdopen(fromSort[0], "r");

    close(toSort[0]);
    close(fromSort[1]);

    FILE *source_fp;
    char data[60000];

    source_fp = fopen(filename, "r");
    if (source_fp == NULL) {
      perror("fopen");
      exit(1);
    }

    FILE *toSortfp = fdopen(toSort[1], "w");
    if (toSortfp == NULL) {
      perror("fdopen");
      exit(1);
    }

    while (fgets(data, sizeof(data), source_fp)) {
      fputs(data, toSortfp);
    }

    fclose(source_fp);
    fclose(toSortfp);

    char sentences[255][255]; // 簡略化のため配列、文字列の長さは固定にする。
    int i = 0;
    while (fgets(sentences[i], sizeof(sentences[i]), fromSortfp)) {
      i++;
    }

    fclose(fromSortfp);

    wait(NULL);
    int num = i;

    for (int i = 0; i < num; i++) {
      printf("%s", sentences[i]);
    }
  }
}
