#include<stdio.h>
#include<stdlib.h>

int main(){
  int *i;
  i = malloc(sizeof(int));
  *i = 10;
  printf("%d \n", *i);
  printf("%p \n", (void *)i);

  free(i);
}

