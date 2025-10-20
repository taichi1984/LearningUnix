#include <ctype.h>
#include <stdio.h>
int main(void) {
  int c;
  while ((c = getchar()) != EOF) {
    if (c == 'z')
      c = 'a';
    else if (islower(c))
      c++;
    putchar(c);
  }
}
