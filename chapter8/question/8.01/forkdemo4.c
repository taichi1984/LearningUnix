/* forkdemo4.c -
 * fork()の戻り値を使って、自分が親か子かを判断するにはどうすればよいかを示す。
 */

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  int fork_rv;

  printf("Before : my pid is %d \n", getpid());
  int parentpid = getpid();
  fork_rv = fork();

  if (parentpid == getpid()) {
    printf("I am the parent. my child is %d \n", fork_rv);
    wait(NULL);
  } else {
    printf("I am the child. my pid is %d \n", getpid());
  }
}
