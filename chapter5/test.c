#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() { write(stdin->_fileno, "abcdef", 8); }
