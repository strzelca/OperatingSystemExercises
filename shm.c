#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sys/mman.h>

int main(void) {
  const int SIZE = 4096;
  const char *name = "OS";
  const char *message_0 = "Hello";
  const char *message_1 = "World!";

  int fd;
  void *ptr;

  fd = shm_open(name, O_CREAT | O_RDWR, 0666);
  ftruncate(fd, SIZE);

  ptr = (char *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if (ptr == MAP_FAILED) {
    printf("Error Creating mmap\n");
    return 1;
  }

  sprintf(ptr, "%s", message_0);
  ptr += strlen(message_0);
  sprintf(ptr, "%s", message_1);
  ptr += strlen(message_1);

  return 0;
}