#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_t workers[8];
pthread_attr_t attr;
sem_t *sem;

void _sum(void *n) {
  int *p = (int *)n;
  *p += 5;
  printf("sum: %d\n", *p);
}

void *_mul(void *n) {
  int *p = (int *)n;

  sem_wait(sem);
  _sum(n);
  *p *= 5;
  sem_post(sem);
  printf("mul: %d\n", *p);

  pthread_exit(0);
}

int main(int argc, char **argv) {
  int value = 0;
  const char *sem_n = "sem";

  sem = sem_open(sem_n, O_CREAT, 0644, 1);

  pthread_attr_init(&attr);

  for (int i = 0; i < 8; i++) {
    pthread_create(&workers[i], &attr, _mul, &value);
  }

  for (int i = 0; i < 8; i++) {
    pthread_join(workers[i], NULL);
  }

  printf("val = %d\n", value);

  return 0;
}