#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sys/mman.h>

#define SIZE 4096

int main(void) {
  const char *name = "OS";

  int fd;
  char *ptr;

  fd = shm_open(name, O_RDONLY, 0666);

  ptr = (char *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  printf("%s", (char *)ptr);

  shm_unlink(name);

  return 0;
}