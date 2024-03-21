#include <fcntl.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sys/mman.h>

int main(void) {
  const int SIZE = 4096;
  const char *name = "OS";

  int fd;
  void *ptr;

  fd = shm_open(name, O_RDONLY, 0666);

  ptr = (char *)mmap(0, SIZE, PROT_READ, MAP_SHARED, fd, 0);
  if (ptr == MAP_FAILED) {
    printf("Error Creating mmap\n");
    return 1;
  }

  printf("%s\n", (char *)ptr);

  shm_unlink(name);

  return 0;
}