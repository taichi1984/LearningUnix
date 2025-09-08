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

struct file_id {
  dev_t dev;
  ino_t ino;
};

struct file_id get_fid(const char *path) {
  struct stat st;
  if (stat(path, &st) == -1) {
    perror(path);
    exit(1);
  }
  struct file_id fid = {st.st_dev, st.st_ino};
  return fid;
}

void printpathto(struct file_id this_fid);
void inum_to_name(struct file_id target, char *namebuf, int buflen);

int main() {
  struct file_id cwd = get_fid(".");
  printpathto(cwd);
  putchar('\n');

  return 0;
}

void printpathto(struct file_id this_fid) {
  struct file_id parent_fid = get_fid("..");

  if (this_fid.dev == parent_fid.dev && this_fid.ino == parent_fid.ino) {
    return;
  }

  chdir("..");
  char its_name[BUFSIZ];
  inum_to_name(this_fid, its_name, BUFSIZ);

  printpathto(parent_fid);
  printf("/%s", its_name);
}

void inum_to_name(struct file_id target, char *namebuf, int buflen) {
  DIR *dir_ptr;
  struct dirent *direntp;

  dir_ptr = opendir(".");
  if (dir_ptr == NULL) {
    perror(".");
    exit(1);
  }

  while ((direntp = readdir(dir_ptr)) != NULL) {
    struct stat st;
    if (stat(direntp->d_name, &st) == 0) {
      if (st.st_dev == target.dev && st.st_ino == target.ino) {
        strncpy(namebuf, direntp->d_name, buflen);
        namebuf[buflen - 1] = '\0';
        closedir(dir_ptr);
        return;
      }
    }
  }
  closedir(dir_ptr);
  fprintf(stderr, "error: cannot find matching entry\n");
  exit(1);
}
