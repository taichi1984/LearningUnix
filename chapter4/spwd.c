/** spwd.c : pwd を単純化したバージョン
 *
 * カレントディレクトリからスタートして、再帰的にファイルシステムのルートまで上り
 * 上のディレクトリから順に名前を出力する。
 * readdir()を使って個々のディレクトリについての情報を得る。
 * *
 * *
 * */

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wchar.h>

ino_t get_inode(char *);
void printpathto(ino_t);
void inum_to_name(ino_t, char *, int);

int main() {
  printpathto(get_inode("."));
  putchar('\n');

  return 0;
}

void printpathto(ino_t this_inode) {

  ino_t parent_inode;
  char its_name[BUFSIZ];
  parent_inode = get_inode("..");
  if (get_inode("..") != this_inode) {
    chdir("..");

    inum_to_name(this_inode, its_name, BUFSIZ);

    //    printf("my_inode : %ld \n", my_inode);
    printpathto(parent_inode);
    printf("/%s", its_name);
  }
}

void inum_to_name(ino_t inode_to_find, char *namebuf, int buflen) {
  DIR *dir_ptr;
  struct dirent *direntp;

  dir_ptr = opendir(".");
  if (dir_ptr == NULL) {
    perror(".");
    exit(1);
  }

  while ((direntp = readdir(dir_ptr)) != NULL)
    if (direntp->d_ino == inode_to_find) {
      strncpy(namebuf, direntp->d_name, buflen);
      namebuf[buflen - 1] = '\0';

      closedir(dir_ptr);
      return;
    }
  fprintf(stderr, "error looking for inum %ld\n", inode_to_find);
  exit(1);
}

ino_t get_inode(char *fname) {
  struct stat info;

  if (stat(fname, &info) == -1) {
    fprintf(stderr, "Cannot stat");
    perror(fname);
    exit(1);
  }
  return info.st_ino;
}
