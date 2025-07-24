/* whoの最初のバージョン *
*/
#include<time.h>
#include<stdio.h>
#include<utmp.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include "utmplib.c"

#define SHOWHOST
void show_info(struct utmp *utbufp);
void showtime( long timeval );


int main()
{
  
  struct utmp *utbufp;

  if (utmp_open( UTMP_FILE ) == -1){
    perror(UTMP_FILE);
    exit(1);
  }

  while( ( utbufp = utmp_next() ) != (( struct utmp * ) NULL ) )
    show_info( utbufp );
  utmp_close();
  return 0;
}
void show_info(struct utmp *utbufp)
{
  if( utbufp-> ut_type != USER_PROCESS)
    return;
  printf("%-8.8s" , utbufp->ut_name);
  printf(" ");
  printf("%-8.8s" , utbufp->ut_line);
  printf(" ");
  showtime(utbufp->ut_time);

#ifdef SHOWHOST
  printf("(%s)", utbufp->ut_host);
#endif
  printf("\n");
}

void showtime( long timeval )
{
  time_t t = (time_t)timeval;
  char *cp = ctime(&timeval);
  printf("%12.12s",cp + 4);

}

