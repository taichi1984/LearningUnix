#include <sys/socket.h>
#include <sys/types.h>

int main() {
  int apipe[2];
  socketpair(AF_UNIX, SOCK_STREAM, PF_UNSPEC, apipe);
}
