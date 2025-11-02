#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static int max_primenumber = 0;
static int check_number = 0;
void show_report(int signum);

int main(void) {
  void show_report(int signum);

  int num;
  signal(SIGINT, show_report);

  printf("入力された数値までの素数を求めます\n");
  scanf("%d", &num);
  for (int i = 2; i <= num; i++) {
    printf("%dを検証します : ", i);
    check_number = i;
    for (int j = 2; j <= i; j++) {
      if (i == j) {
        printf("%dは素数です\n", i);
        max_primenumber = i;
      }
      if (i % j == 0) {
        break;
      }
    }
    printf("\n");
  }
}

void show_report(int signum) {
  int c;
  printf("\n現在判明している最大の素数は %d です。\n", max_primenumber);
  printf("現在%dまで検証しています。\n", check_number);

  printf("続行しますか？ (y/n)\n");

  do {
    c = getchar();
  } while (c == '\n');

  if (c == 'y' || c == 'Y') {
    printf("続行します\n");
  } else {
    printf("処理を終了します\n");
    exit(2);
  }
}
