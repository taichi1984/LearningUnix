#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char *cp = getenv("LANG");
  if (cp != NULL && strcmp(cp, "fr") == 0)
    printf("Bonjour\n");
  else if (cp != NULL && strcmp(cp, "ja") == 0)
    printf("こんにちは\n");
  else
    printf("Hello\n");
}
