/** ls1.c
 *  目的 : ディレクトリ( 複数の場合も含む) の内容をリストアップする。
 *  動作 :
 * 引数がない場合には、"."、そうでなければ引数のディレクトリに含まれるファイルを出力する。
 *
 */
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cmp_str(const void *a, const void *b);
int cmp_str_desc(const void *a, const void *b);

void do_ls(char[], char[]);

int main(int ac, char *av[]) {
  if (ac == 1) {
    do_ls(".", "normal");
  } else if (ac == 2) {
    if (strcmp(av[1], "-r") == 0) {
      do_ls(".", "-r");
    }
  }

  else {
    while (--ac) {
      printf("%s: \n", *++av);
      do_ls(*av, "normal");
    }
  }

  return 0;
}

int cmp_str(const void *a, const void *b) {
  // a, b は "要素へのポインタへのポインタ"
  const struct dirent *pa = *(const struct dirent **)a;
  const struct dirent *pb = *(const struct dirent **)b;
  return strcmp(pa->d_name, pb->d_name); // strcmp をそのまま使える
}
int cmp_str_desc(const void *a, const void *b) {
  // a, b は "要素へのポインタへのポインタ"
  const struct dirent *pa = *(const struct dirent **)a;
  const struct dirent *pb = *(const struct dirent **)b;
  return -strcmp(pa->d_name, pb->d_name); // strcmp をそのまま使える
}

void do_ls(char dirname[], char option[])
/*
 *  dirnameというディレクトリのファイルをリストアップする。
 */
{
  DIR *dir_ptr;
  struct dirent *entry;
  struct dirent *direntp[1024];
  int i = 0;

  if ((dir_ptr = opendir(dirname)) == NULL)
    fprintf(stderr, "ls1: cannot open %s \n", dirname);
  else {
    while ((entry = readdir(dir_ptr)) != NULL) {
      direntp[i] = malloc(sizeof(struct dirent));
      memcpy(direntp[i], entry, sizeof(struct dirent));
      i++;
    }
  }

  if (strcmp(option, "-r") == 0)
    qsort(direntp, i, sizeof(direntp[0]), cmp_str_desc);
  else if (strcmp(option, "normal") == 0)
    qsort(direntp, i, sizeof(direntp[0]), cmp_str);

  for (int j = 0; j < i; j++)
    printf("%s \n", direntp[j]->d_name);

  for (int j = 0; j < i; j++)
    free(direntp[j]);

  closedir(dir_ptr);
}
