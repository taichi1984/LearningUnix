#include <ctype.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int is_all_digits(const char *s) {
  for (size_t i = 0; s[i] != '\0'; i++) {
    if (!isdigit((unsigned char)s[i]))
      return 0;
  }
  return 1;
}

int main(int ac, char *av[]) {
  struct passwd *user_passwd;
  uid_t user;

  if (ac < 3) {
    fprintf(stderr, "引数が足りません\n");
    return 1;
  } else {
    if (is_all_digits(av[1])) {
      user = atoi(av[1]);
    } else {
      if ((user_passwd = getpwnam(av[1])) == NULL) {
        fprintf(stderr, "入力されたユーザー名は存在しません。\n");
        return 1;
      } else {
        user = user_passwd->pw_uid;
      }
    }

    for (int i = 2; i < ac; i++) {
      if (chown(av[i], user, -1) == -1) {
        perror(av[i]);
      }
    }
  }
}
