/** ls2.c
 *  目的 : ディレクトリ( 複数の場合も含む) の内容をリストアップする。 ls-l仕様
 *  動作 :
 * 引数がない場合には、"."、そうでなければ引数のディレクトリに含まれるファイルを出力する。
 *
 */

#define _DEFAULT_SOURCE
#include <assert.h>
#include <dirent.h> // DIR*, opendir/readdir（使っていれば）
#include <grp.h>    // getgrgid
#include <pwd.h>    // getpwuid
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>  // struct stat
#include <sys/types.h> // mode_t, uid_t, gid_t
#include <time.h>      // struct timespec, localtime, strftime
#include <unistd.h>    // close など（必要なら）

typedef struct {
  int show_long;
  int show_all;
  int recursion;
} Options;

void showtime(struct timespec *time);
void mode_to_letter(mode_t mode, char str[]);
char *uid_to_name(uid_t uid);
char *gid_to_name(gid_t gid);
void do_ls(char[], Options);
void showstats(char *d_name);

int main(int ac, char *av[]) {

  Options option = {0, 0, 0};
  int file_argc = 0;
  if (ac == 1) {
    do_ls(".", option);
  } else {
    for (int i = 1; i < ac; i++) {
      if (av[i][0] == '-') {
        if (strcmp(av[i], "-R") == 0) {
          option.recursion = 1;
        } else if (strcmp(av[i], "-l") == 0) {
          option.show_long = 1;
        } else {
          fprintf(stderr, "Unknown option: %s\n", av[i]);
        }
      } else {
        printf("%s:\n", av[i]);
        do_ls(av[i], option);
        file_argc++;
      }
    }
    if (file_argc == 0) {

      do_ls(".", option);
    }
    return 0;
  }
}

void do_ls(char dirname[], Options option)
/*
 *  dirnameというディレクトリのファイルをリストアップする。
 */
{
  DIR *dir_ptr;
  struct dirent *direntp;
  char buf[257];
  struct stat entry;

  if ((dir_ptr = opendir(dirname)) == NULL)
    fprintf(stderr, "ls1: cannot open %s \n", dirname);
  else {
    printf("%s :\n", dirname);
    while ((direntp = readdir(dir_ptr)) != NULL) {
      if (direntp->d_name[0] == '.')
        continue;
      strcpy(buf, dirname);
      strcat(buf, "/");
      strcat(buf, direntp->d_name);

      if (option.show_long) {
        showstats(buf);
      } else {
        printf("%s ", direntp->d_name);
      }
    }
    printf("\n\n");
    seekdir(dir_ptr, SEEK_SET);

    if (option.recursion) {
      while ((direntp = readdir(dir_ptr)) != NULL) {
        if (direntp->d_name[0] == '.')
          continue;
        if (direntp->d_type == DT_DIR) {
          char recdir[257];
          snprintf(recdir, sizeof(recdir), "%s/%s", dirname, direntp->d_name);
          do_ls(recdir, option);
        }
      }
    }
  }
  closedir(dir_ptr);
}

void showstats(char *filename) {
  struct stat buf;
  struct stat *bufp = &buf;
  char modebuf[11];

  if (stat(filename, bufp) == 0) {
    mode_to_letter(bufp->st_mode, modebuf);
    printf("%10s ", modebuf);
    printf("%2ld ", bufp->st_nlink);
    printf("%10s ", uid_to_name(bufp->st_uid));
    printf("%10s ", gid_to_name(bufp->st_gid));
    printf("%10ld bytes   ", bufp->st_size);
    showtime(&bufp->st_mtim);
    printf("%20s ", strrchr(filename, '/') + 1);
    printf("\n");
  } else {
    printf("Can't stat %s ", filename);
  }
}

void showtime(struct timespec *time) {
  char *timebuf;
  timebuf = ctime(&time->tv_sec);
  strtok(timebuf, "\n");
  printf("%s", timebuf);
}
void mode_to_letter(mode_t mode, char str[]) {
  strcpy(str, "----------");

  if (S_ISDIR(mode))
    str[0] = 'd';
  if (S_ISLNK(mode))
    str[0] = 'l';
  if (S_ISCHR(mode))
    str[0] = 'c';
  if (S_ISBLK(mode))
    str[0] = 'b';
  if (S_ISFIFO(mode))
    str[0] = 'p';
  if (S_ISSOCK(mode))
    str[0] = 's';

  if (mode & S_IRUSR)
    str[1] = 'r';
  if (mode & S_IWUSR)
    str[2] = 'w';
  if (mode & S_IXUSR)
    str[3] = 'x';
  if (mode & S_IXUSR && mode & S_ISUID)
    str[3] = 's';
  if ((mode & S_IXUSR) == 0 && mode & S_ISUID)
    str[3] = 'S';

  if (mode & S_IRGRP)
    str[4] = 'r';
  if (mode & S_IWGRP)
    str[5] = 'w';
  if (mode & S_IXGRP)
    str[6] = 'x';
  if (mode & S_IXGRP && mode & S_ISGID)
    str[6] = 's';
  if ((mode & S_IXGRP) == 0 && mode & S_ISGID)
    str[6] = 'S';

  if (mode & S_IROTH)
    str[7] = 'r';
  if (mode & S_IWOTH)
    str[8] = 'w';
  if (mode & S_IXOTH)
    str[9] = 'x';
  if (mode & S_ISVTX && mode & S_IXOTH)
    str[9] = 't';
  if ((mode & S_IXOTH) == 0 && mode & S_ISVTX)
    str[9] = 'T';
}

char *uid_to_name(uid_t uid) { return getpwuid(uid)->pw_name; }

char *gid_to_name(gid_t gid) { return getgrgid(gid)->gr_name; }
