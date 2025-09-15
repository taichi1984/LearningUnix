#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct {
  int option_p;

} Options;

int do_mkdir(char *filepath, Options option) {

  int n = 0;
  char *path_list[255];

  if (option.option_p == 1) {
    path_list[n] = strtok(filepath, "/");
    if (mkdir(path_list[n], S_IRUSR | S_IWUSR | S_IXUSR | S_IXGRP | S_IXOTH) !=
        0) {
      if (errno == EEXIST) {

      } else {
        perror("mkdir");
        return -1;
      }
    }

    n++;

    while ((path_list[n] = strtok(NULL, "/")) != NULL) {
      char dir_path[PATH_MAX] = "";
      for (int i = 0; i <= n; i++) {
        if (i > 0)
          strncat(dir_path, "/", sizeof(dir_path) - strlen(dir_path) - 1);
        strncat(dir_path, path_list[i],
                sizeof(dir_path) - strlen(dir_path) - 1);
      }
      // printf("dirpath : %s \n", dir_path);
      if (mkdir(dir_path, S_IRUSR | S_IWUSR | S_IXUSR | S_IXGRP | S_IXOTH) !=
          0) {
        if (errno == EEXIST) {

        } else {
          perror("mkdir");
          return -1;
        }
      }
      n++;
    }
  } else {
    if (mkdir(filepath, S_IRUSR | S_IWUSR | S_IXUSR | S_IXGRP | S_IXOTH) != 0) {
      perror("mkdir");
      exit(1);
    }
  }
  return 0;
}

int main(int ac, char *av[]) {

  Options option = {0};
  int file_argc = 0;

  for (int i = 1; i < ac; i++) {

    if (av[i][0] == '-') {
      if (strcmp(av[i], "-p") == 0) {
        option.option_p = 1;
      } else {
        fprintf(stderr, "Unknown option: %s\n", av[i]);
      }
    } else {
      do_mkdir(av[i], option);
      file_argc++;
    }
  }

  if (file_argc == 0) {
    printf("ディレクトリを正しく指定してください \n");
  }

  return 0;
}
