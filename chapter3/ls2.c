/** ls2.c
 *  目的 : ディレクトリ( 複数の場合も含む) の内容をリストアップする。 ls-l仕様
 *  動作 :
 * 引数がない場合には、"."、そうでなければ引数のディレクトリに含まれるファイルを出力する。
 *
 */
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

void showtime(struct timespec *time);
void mode_to_letter(mode_t mode, char str[]);
char *uid_to_name(uid_t uid);
char *gid_to_name(gid_t gid);
void do_ls(char[]);
void showstats(char *d_name);
int main(int ac, char *av[]) {
  if (ac == 1) {
    do_ls(".");
  } else {
    while (--ac) {
      printf("%s:\n ", *++av);
      do_ls(*av);
    }
  }
  return 0;
}

void do_ls(char dirname[])
/*
 *  dirnameというディレクトリのファイルをリストアップする。
 */
{
  DIR *dir_ptr;
  struct dirent *direntp;

  if ((dir_ptr = opendir(dirname)) == NULL)
    fprintf(stderr, "ls1: cannot open %s \n", dirname);
  else {
    while ((direntp = readdir(dir_ptr)) != NULL) {
      showstats(direntp->d_name);
      printf("\n");
    }
    closedir(dir_ptr);
  }
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
    printf("%20s ", filename);
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

  if (mode & S_IRGRP)
    str[4] = 'r';
  if (mode & S_IWGRP)
    str[5] = 'w';
  if (mode & S_IXGRP)
    str[6] = 'x';

  if (mode & S_IROTH)
    str[7] = 'r';
  if (mode & S_IWOTH)
    str[8] = 'w';
  if (mode & S_IXOTH)
    str[9] = 'x';
}

char *uid_to_name(uid_t uid) { return getpwuid(uid)->pw_name; }

char *gid_to_name(gid_t gid) { return getgrgid(gid)->gr_name; }
