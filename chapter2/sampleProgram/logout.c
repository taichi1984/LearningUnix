/*
* logout_tty(char *line)
* utmpレコードにログアウトのマークをつける。
* ユーザー名やリモートホストの名を空にしない
* エラーの時は-1、成功したときは0を返す
*
*/ 
#include<utmp.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<time.h>


int logout_tty(char * line)
{
    int fd;
    struct utmp rec;
    int len = sizeof(struct utmp);
    int retval = -1;

    if(( fd = open(UTMP_FILE, O_RDWR)) == -1)
      return -1;
    
    /*検索して置換する*/ 
    while( read(fd, &rec , len) == len)
      if( strncmp(rec.ut_line , line , sizeof(rec.ut_line)) == 0)
      {
          rec.ut_type = DEAD_PROCESS;
          if ( time( (time_t * )&rec.ut_time  ) != -1 )
              if ( lseek(fd , -len , SEEK_CUR ) != -1 )
                if ( write(fd , &rec , len) == len)
                    retval = 0;
          break;
      }

/* ファイルをクローズする */ 
   
    if ( close(fd) == -1 )
      retval = -1;
    return retval;
}

