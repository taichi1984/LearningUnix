/* whoの最初のバージョン *
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <utmp.h>

#define SHOWHOST
void show_info(struct utmp *utbufp);

int main() {
  struct utmp current_record;
  int utmpfd;
  int reclen = sizeof(current_record);

  if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1) {
    perror(UTMP_FILE);
    exit(1);
  }

  while (read(utmpfd, &current_record, reclen) == reclen)
    show_info(&current_record);
  close(utmpfd);
  return 0;
}

void show_info(struct utmp *utbufp) {
  printf("%10d", utbufp->ut_type);

  printf(" ");
  printf("
  printf("%-8.8s" , utbufp->ut_name);
  printf(" ");
  printf("%-8.8s" , utbufp->ut_line);
  printf(" ");
  printf("%10d", utbufp->ut_time);
#ifdef SHOWHOST
  printf("(%s)", utbufp->ut_host);
#endif
  printf("\n");
}

struct utmp {
  short int ut_type; /* Type of login.  */
  pid_t ut_pid;      /* Process ID of login process.  */
  char ut_line[UT_LINESIZE] __attribute_nonstring__; /* Devicename.  */
  char ut_id[4] __attribute_nonstring__;             /* Inittab ID.  */
  char ut_user[UT_NAMESIZE] __attribute_nonstring__; /* Username.  */
  char ut_host[UT_HOSTSIZE] __attribute_nonstring__; /* Hostname for remote
                                                        login.  */
  struct exit_status ut_exit; /* Exit status of a process marked
                                 as DEAD_PROCESS.  */
/* The ut_session and ut_tv fields must be the same size when compiled
   32- and 64-bit.  This allows data files and shared memory to be
   shared between 32- and 64-bit applications.  */
#if __WORDSIZE_TIME64_COMPAT32
  int32_t ut_session; /* Session ID, used for windowing.  */
  struct {
    int32_t tv_sec;  /* Seconds.  */
    int32_t tv_usec; /* Microseconds.  */
  } ut_tv;           /* Time entry was made.  */
#else
  long int ut_session;  /* Session ID, used for windowing.  */
  struct timeval ut_tv; /* Time entry was made.  */
#endif

  int32_t ut_addr_v6[4];     /* Internet address of remote host.  */
  char __glibc_reserved[20]; /* Reserved for future use.  */
};
