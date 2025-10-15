/*
 *  ファイルロックのテスト
 *  第１引数のファイル名に　\n scanfで取得した任意の文字列を末尾に追加する。
 */
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int ac, char *av[]) {
  char lockfile[FILENAME_MAX];
  char line[100];

  snprintf(lockfile, sizeof(lockfile), "%s.LCK", av[1]);

  while (1) {
    if (link(av[1], lockfile) == 0) {
      printf("追加したい文字列を入力してください\n");
      break;
    }

    if (errno == EEXIST) {
      printf("ファイルはロックされています。 , ロック解除待ち中...\n");
      sleep(1);
      continue;
    }
  }

  fgets(line, sizeof(line), stdin);

  FILE *fp;
  fp = fopen(av[1], "a");
  fprintf(fp, "%s", line);
  fclose(fp);

  unlink(lockfile);
}
