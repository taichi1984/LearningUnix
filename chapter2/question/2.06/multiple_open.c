#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
  int fd1;
  int fd2;
  int fd3;
  
  int qty = 20;

  char *buf;
  char *writing_buf = "I want write this sentence";
  buf = malloc(qty);
  
  if((fd1 = open("samplefile.txt",O_RDONLY)) == -1){
      perror("error");
      exit(1);
  }


  if((fd2 = open("samplefile.txt",O_WRONLY)) == -1){
      perror("error");
      exit(1);
  }

  if((fd3 = open("samplefile.txt",O_RDWR)) == -1){
      perror("error");
      exit(1);
  }
 
 /*4の処理*/
  if(read(fd1,buf,qty) != -1)
  {
    printf("%s \n",buf);
  }

  if(write(fd2,writing_buf,qty) != -1)
  {
    printf("書き込み完了\n");

  }
  
  
  if(read(fd3,buf,qty) != -1)
  {
    printf("%s \n",buf);
  }

  close(fd1);
  close(fd2);
  close(fd3);
  free(buf);

  
}
