#include<stdio.h>
#include<stdlib.h>
int main()
{
  FILE *fp = fopen("sample.txt","r");
  int i;
  for(i=0 ; i < 20 ; i++)
  {
    printf("%c \n" , getc(fp));
  }

  char *mystr;
  mystr = malloc(sizeof(char) * 30);
  printf("%s \n" , fgets(mystr, 20 , fp));


  fclose(fp);







}

