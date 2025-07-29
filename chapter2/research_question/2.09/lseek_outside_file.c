#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>

int main()
{
  int fd;
  char buffer[100];

  if((fd = open("sampletext.txt",O_RDWR)) == -1)
  {
    perror("open error");
    exit(1);
  }
  
  lseek(fd,50,SEEK_END);

  if(read(fd,&buffer,20) != 0)
    printf("%s \n",buffer);

  lseek(fd,20000,SEEK_END);
  
  if(write(fd,"testword",9) != 0)
    printf("書き込み完了\n");
  
}
