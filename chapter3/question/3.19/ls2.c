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
  int atime;
  int reverse;
} Options;

void showtime(struct timespec *time);
void mode_to_letter(mode_t mode, char str[]);
char *uid_to_name(uid_t uid);
char *gid_to_name(gid_t gid);
void do_ls(char[], Options);
void showstats(char *d_name, Options);

struct fileinfo {
  char *name;
  struct stat st;
};

int cmp_d_name(const void *a, const void *b) {
  // a, b は "要素へのポインタへのポインタ"
  const struct fileinfo *pa = *(const struct fileinfo **)a;
  const struct fileinfo *pb = *(const struct fileinfo **)b;
  return strcmp(pa->name, pb->name); // strcmp をそのまま使える
}

int cmp_d_name_desc(const void *a, const void *b) { return -cmp_d_name(a, b); }

int cmp_atime(const void *a, const void *b) {
  // a, b は "要素へのポインタへのポインタ"
  const struct fileinfo *pa = *(const struct fileinfo **)a;
  const struct fileinfo *pb = *(const struct fileinfo **)b;

  if (pa->st.st_atim.tv_sec != pb->st.st_atim.tv_sec)
    return (pa->st.st_atim.tv_sec > pb->st.st_atim.tv_sec) -
           (pa->st.st_atim.tv_sec < pb->st.st_atim.tv_sec);

  return (pa->st.st_atim.tv_nsec > pb->st.st_atim.tv_nsec) -
         (pa->st.st_atim.tv_nsec < pb->st.st_atim.tv_nsec);
}

int cmp_atime_desc(const void *a, const void *b) { return -cmp_atime(a, b); }

int main(int ac, char *av[]) {

  Options option = {0, 0, 0, 0, 0};
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
        } else if (strcmp(av[i], "-u") == 0) {
          option.atime = 1;
        } else if (strcmp(av[i], "-r") == 0) {
          option.reverse = 1;
        } else {
          fprintf(stderr, "Unknown option: %s\n", av[i]);
        }
      } else {
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
  struct fileinfo *fileinfo_array[1024];
  int i = 0;
  char buf[512];

  printf(": %s \n", dirname);
  if ((dir_ptr = opendir(dirname)) == NULL)
    fprintf(stderr, "ls1: cannot open %s \n", dirname);
  else {
    while ((direntp = readdir(dir_ptr)) != NULL) {
      fileinfo_array[i] = malloc(sizeof(struct fileinfo));
      fileinfo_array[i]->name = strdup(direntp->d_name);
      snprintf(buf, sizeof(buf), "%s/%s", dirname, fileinfo_array[i]->name);
      stat(buf, &fileinfo_array[i]->st);
      i++;
    }
  }

  if (option.atime == 1 && option.reverse == 1) {
    qsort(fileinfo_array, i, sizeof(fileinfo_array[0]), cmp_atime_desc);
  } else if (option.atime == 1) {
    qsort(fileinfo_array, i, sizeof(fileinfo_array[0]), cmp_atime);
  } else if (option.reverse == 1) {
    qsort(fileinfo_array, i, sizeof(fileinfo_array[0]), cmp_d_name_desc);
  } else if (option.reverse == 0) {
    qsort(fileinfo_array, i, sizeof(fileinfo_array[0]), cmp_d_name);
  }

  for (int j = 0; j < i; j++) {
    if (fileinfo_array[j]->name[0] == '.')
      continue;

    snprintf(buf, sizeof(buf), "%s/%s", dirname, fileinfo_array[j]->name);

    if (option.show_long)
      showstats(buf, option);
    else
      printf("%10s ", fileinfo_array[j]->name);
  }
  printf("\n\n");

  if (option.recursion) {
    for (int j = 0; j < i; j++) {
      if (fileinfo_array[j]->name[0] == '.')
        continue;

      if (S_ISDIR(fileinfo_array[j]->st.st_mode)) {
        char recdir[257];
        snprintf(recdir, sizeof(recdir), "%s/%s", dirname,
                 fileinfo_array[j]->name);
        do_ls(recdir, option);
      }
    }
  }
  closedir(dir_ptr);
  for (int j = 0; j < i; j++) {
    free(fileinfo_array[j]->name);
    free(fileinfo_array[j]);
  }
}

void showstats(char *filename, Options option) {
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
    if (option.atime == 1)
      showtime(&bufp->st_atim);
    else
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
