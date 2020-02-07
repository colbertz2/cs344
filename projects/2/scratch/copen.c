#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
  int fd = open("test.txt", O_RDONLY);
  close(fd);
  return 0;
}

