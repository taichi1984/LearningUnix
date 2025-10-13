/* write0.c
 * 目的：他の端末にメッセージを送る。
 * 方法：他の端末を書き込みようにオープンし、標準入力をその端末にコピーする。
 * 学習ポイント: 端末は、通常の入出力をサポートするファイルにすぎないこと
 * 使い方 : write0 ttyname
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <utmp.h>
#define MAX_UTMP 1024

int main(int ac, char *av[]) {
  int fd_utmp;
  int fd_device;
  char buf[BUFSIZ];
  char *username = getlogin();
  char *targetuser;
  char *device[MAX_UTMP];
  char hello[BUFSIZ];
  struct utmp utmpent[MAX_UTMP];

  /*引数をチェックする*/
  if (ac != 2) {
    fprintf(stderr, "usage: write username\n");
    exit(1);
  }

  targetuser = av[1];

  if ((fd_utmp = open(UTMP_FILE, O_RDONLY)) == -1) {
    perror("open");
    exit(1);
  }

  int i = 0;
  while (read(fd_utmp, &utmpent[i], sizeof(struct utmp)) > 0) {
    // printf("%s %s \n", utmpent[i].ut_user, utmpent[i].ut_line);
    i++;
  }

  close(fd_utmp);

  int j = 0;
  int k = 0;
  for (j = 0; j < i; j++) {
    if (utmpent[j].ut_type == USER_PROCESS &&
        strcmp(utmpent[j].ut_user, targetuser) == 0) {
      device[k] = utmpent[j].ut_line;
      k++;
    }
  }

  int l = 0;
  if (k > 0) {
    printf("user : %s は 以下の端末でログインしています。\n", username);
    for (l = 0; l < k; l++) {
      printf("%d : %s \n", l, device[l]);
    }
  } else {
    printf("user : %s は現在ログインをしていません\n", targetuser);
    exit(0);
  }

  printf("どの端末を選択しますか\n");
  int ans;
  do {
    scanf("%d", &ans);
  } while (ans < 0 || ans >= k);

  printf("\n");

  char filename[FILENAME_MAX];
  snprintf(filename, sizeof(filename), "/dev/%s", device[ans]);

  /* デバイスをオープンする。*/
  fd_device = open(filename, O_WRONLY);
  if (fd_device == -1) {
    perror("open");
    exit(1);
  }
  snprintf(hello, sizeof(hello), "\nMessage from %s \n", username);

  if (write(fd_device, hello, strlen(hello)) == -1) {
    perror("write");
    exit(1);
  }

  /* 入力にEOFが現れるまでループ */
  while (fgets(buf, BUFSIZ, stdin) != NULL)
    if (write(fd_device, buf, strlen(buf)) == -1)
      break;
  close(fd_device);
}
