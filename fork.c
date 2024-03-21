#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  pid_t pid;
  int a = 5;

  if ((pid = fork()) < 0) {
    return 1;
  }

  if (pid == 0) {
    for (int i = 0; i < 10; i++) {
      a++;
    }
    printf("Processo figlio: %d\n", a);
  } else {
    wait(NULL);
    printf("Processo padre: %d\n", a);
  }

  return 0;
}