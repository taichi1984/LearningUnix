#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define CHUNK 16
#define TARGET (2 * 1024 * 1024)
int main(void) {
  char chunk[CHUNK] = "abcdefghijklmnop";
  int written = 0;
  int fd;
  fd = open("testfile", O_WRONLY | O_CREAT, 0644);

  if (fd == -1) {
    perror("open");
    exit(1);
  }

  while (written < TARGET) {
    ssize_t w = write(fd, chunk, sizeof(chunk));
    if (w == -1) {
      perror("write");
      exit(1);
    }
    if (w != CHUNK) {
      fprintf(stderr, "partial write detected \n");
      exit(1);
    }

    written += w;
    printf("%dbyte書き込み済み\n", written);
  }

  close(fd);
}
