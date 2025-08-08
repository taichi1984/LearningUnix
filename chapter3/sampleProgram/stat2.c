#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

void showtime(struct timespec *time);
void mode_to_letter(mode_t mode , char str[]);
char *uid_to_name(uid_t uid);
char *gid_to_name(gid_t gid);


int main(int ac, char *av[]) {
  struct stat buf;
  struct stat *bufp = &buf;
  char modebuf[11];

  if (ac != 2) {
    printf("invalid argument");
    exit(1);
  }
  if (stat(av[1], bufp) == 0) {
    printf("    mode : ");
    mode_to_letter(bufp->st_mode,modebuf);
    printf("%s \n",modebuf);
    printf("   nlink : %ld \n", bufp->st_nlink);
    printf("    user : %s \n", uid_to_name(bufp->st_uid));
    printf("   group : %s \n", gid_to_name(bufp->st_gid));
    printf("filesize : %ld bytes\n", bufp->st_size);
    showtime(&bufp->st_mtim);
    printf("filename : %s \n", av[1]);
  } else {
    printf("Can't stat %s \n", av[1]);
  }
}

void showtime(struct timespec *time) {
  printf("modified : %s", ctime(&time->tv_sec));
}
void mode_to_letter(mode_t mode, char str[]) {
  strcpy( str , "----------" );

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

  if( mode & S_IRUSR)
    str[1] = 'r';
  if( mode & S_IWUSR)
    str[2] = 'w';
  if( mode & S_IXUSR)
    str[3] = 'x';

  if( mode & S_IRGRP)
    str[4] = 'r';
  if( mode & S_IWGRP)
    str[5] = 'w';
  if( mode & S_IXGRP)
    str[6] = 'x';


  if( mode & S_IROTH)
    str[7] = 'r';
  if( mode & S_IWOTH)
    str[8] = 'w';
  if( mode & S_IXOTH)
    str[9] = 'x';

}

char *uid_to_name(uid_t uid)
{
  return getpwuid(uid)->pw_name;
}

char *gid_to_name( gid_t gid )
{
  return getgrgid(gid)->gr_name;
}

