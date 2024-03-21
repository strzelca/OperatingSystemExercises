#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main(void) {
  char write_msg[BUFFER_SIZE] = "Lezzo!";
  char read_msg[BUFFER_SIZE];
  int fd[2];
  pid_t pid;

  if (pipe(fd) < 0) {
    printf("Failed creating pipe...\n");
    return -1;
  }

  if ((pid = fork()) < 0) {
    printf("Fork failed...\n");
    return -1;
  }

  if (pid > 0) {
    close(fd[READ_END]);
    write(fd[WRITE_END], write_msg, strlen(write_msg) + 1);
    close(fd[WRITE_END]);
  } else {
    close(fd[WRITE_END]);
    read(fd[READ_END], read_msg, BUFFER_SIZE);
    printf("%s\n", read_msg);
    close(fd[READ_END]);
  }

  return 0;
}