#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int sum;

void *runner(void *param) {
  int upper = atoi(param);
  sum = 0;

  for (int i = 0; i <= upper; i++) {
    sum += i;
  }

  pthread_exit(0);
}

int main(int argc, char **argv) {
  pthread_t thread_id;
  pthread_attr_t attr;

  pthread_attr_init(&attr);

  if (pthread_create(&thread_id, &attr, runner, argv[1]) < 0) {
    printf("Failed creating thread...");
    return -1;
  }

  pthread_join(thread_id, NULL);

  printf("sum = %d\n", sum);

  return 0;
}