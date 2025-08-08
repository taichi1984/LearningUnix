#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
void showtime(struct timespec *time);

int main(int ac, char *av[]) {
  struct stat buf;
  struct stat *bufp = &buf;

  if (ac != 2) {
    printf("invalid argument");
    exit(1);
  }

  if (stat(av[1], bufp) == 0) {
    printf("    mode : %o \n", bufp->st_mode);
    printf("   nlink : %ld \n", bufp->st_nlink);
    printf("    user : %d \n", bufp->st_uid);
    printf("   group : %d \n", bufp->st_gid);
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
