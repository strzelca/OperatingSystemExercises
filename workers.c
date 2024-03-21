#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *_sum(void *n) {
  int *p = (int *)n;

  *p += 5;
  printf("sum: %d\n", *p);

  pthread_exit(0);
}

void *_mul(void *n) {
  int *p = (int *)n;
  sleep(2);
  *p *= 5;
  printf("mul: %d\n", *p);

  pthread_exit(0);
}

void *_div(void *n) {
  int *p = (int *)n;

  sleep(1);
  *p /= 5;
  printf("div: %d\n", *p);

  pthread_exit(0);
}

void *_sub(void *n) {
  int *p = (int *)n;

  sleep(4);
  *p -= 5;
  printf("sub: %d\n", *p);

  pthread_exit(0);
}

int main(int argc, char **argv) {
  int value = 0;
  pthread_t workers[4];
  pthread_attr_t attr;

  pthread_attr_init(&attr);

  pthread_create(&workers[0], &attr, _sum, &value);
  pthread_create(&workers[1], &attr, _mul, &value);
  pthread_create(&workers[2], &attr, _div, &value);
  pthread_create(&workers[3], &attr, _sub, &value);

  for (int i = 0; i < 4; i++) {
    pthread_join(workers[i], NULL);
  }

  printf("sum = %d\n", value);

  return 0;
}