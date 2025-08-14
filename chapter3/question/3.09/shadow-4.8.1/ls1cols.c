
/** ls1cols.c
 *  目的 : ディレクトリ( 複数の場合も含む) の内容をリストアップする。
 *  動作 :
 * 引数がない場合には、"."、そうでなければ引数のディレクトリに含まれるファイルを出力する。
 *
 */
#define _POSIX_C_SOURCE 200809L

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

void do_ls(char[]);

int main(int ac, char *av[]) {
  if (ac == 1) {
    do_ls(".");
  } else {
    while (--ac) {
      printf("%s: \n", *++av);
      do_ls(*av);
    }
  }
  return 0;
}

void do_ls(char dirname[])
/*
 *  dirnameというディレクトリのファイル名を返す。
 */
{
  DIR *dir_ptr;
  struct dirent *direntp;
  char **filename;
  int num_of_file = 0;
  int i = 0;
  int j = 0;
  struct winsize w;
  int max_of_filename = 0;

  /*ファイルの数を数える*/
  if ((dir_ptr = opendir(dirname)) == NULL) {
    fprintf(stderr, "ls1: cannot open %s \n", dirname);
    return;

  } else {
    while ((direntp = readdir(dir_ptr)) != NULL) {
      num_of_file++;
    }
  }

  /* 文字列ポインタの配列を確保*/

  filename = malloc(num_of_file * sizeof(char *));
  if (!filename) {
    perror("malloc");
    closedir(dir_ptr);
    return;
  }

  rewinddir(dir_ptr);
  /*ファイル名のポインタをfilenameに順番に格納*/
  i = 0;
  while (i < num_of_file && (direntp = readdir(dir_ptr)) != NULL) {
    filename[i] = strdup(direntp->d_name);

    if (filename[i] == NULL) {
      perror("strdup failed");
      for (j = 0; j < i; j++)
        free(filename[j]);
      free(filename);
      closedir(dir_ptr);
      return;
    }
    i++;
  }

  closedir(dir_ptr);

  /*端末の幅を取得*/

  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
    perror("ioctl");
    return;
  }

  printf("端末の横の長さ : %d \n ", w.ws_col);

  /* 最も長いファイルの長さを測る */
  for (j = 0; j < num_of_file; j++) {
    if (strlen(filename[j]) >= max_of_filename)
      max_of_filename = strlen(filename[j]);
  }
  printf("最も長いファイル長 %d\n", max_of_filename);

  /* 表示 */

  const int padding_spaces = 2;
  const int terminal_width = (w.ws_col > 0) ? w.ws_col : 80;
  int column_width = max_of_filename + padding_spaces;

  int column_count = (terminal_width + padding_spaces) / column_width;
  if (column_width < 1)
    column_count = 1;

  int row_count = (num_of_file + column_count - 1) / column_count;
  int row_index = 0;
  int column_index = 0;
  int index_in_array;
  for (row_index = 0; row_index < row_count; row_index++) {
    for (column_index = 0; column_index < column_count; column_index++) {
      index_in_array = row_index + column_index * row_count;
      if (index_in_array < num_of_file) {
        printf("%-*s", column_width, filename[index_in_array]);
      }
    }
    putchar('\n');
  }

  for (j = 0; j < num_of_file; j++) {
    free(filename[j]);
  }
  free(filename);
}
