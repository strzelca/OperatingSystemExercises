#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sys/mman.h>

struct Node {
  int val;
  struct Node *next;
};

int main(void) {
  const char *list_shm = "LIST";
  const char *size = "SIZE";
  const char *ptr = "PTR";
  int list_length = 0;

  struct Node *list = (struct Node *)malloc(sizeof(struct Node));
  struct Node *curr = list;

  for (int i = 1; i <= 20; i++) {
    curr->val = i;
    curr->next = (struct Node *)malloc(sizeof(struct Node));
    curr = curr->next;
  }

  curr = list;

  while (curr->next != NULL) {
    list_length++;
    curr = curr->next;
  }

  curr = list;

  const int SIZE = list_length * sizeof(struct Node *);
  int list_fd, size_fd, ptr_fd;
  void *list_ptr, *size_ptr, **_ptr;

  list_fd = shm_open(list_shm, O_CREAT | O_RDWR, 0666);
  size_fd = shm_open(size, O_CREAT | O_RDWR, 0666);
  ptr_fd = shm_open(ptr, O_CREAT | O_RDWR, 0666);

  ftruncate(list_fd, SIZE);
  ftruncate(size_fd, sizeof(int));
  ftruncate(ptr_fd, sizeof(void **));

  list_ptr = (struct Node *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,
                                 list_fd, 0);
  size_ptr = (int *)mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED,
                         size_fd, 0);
  _ptr = (void **)mmap(0, sizeof(void **), PROT_READ | PROT_WRITE, MAP_SHARED,
                       ptr_fd, 0);

  if (list_ptr == MAP_FAILED) {
    printf("Error Creating mmap\n");
    return 1;
  }

  if (size_ptr == MAP_FAILED) {
    printf("Error Creating mmap\n");
    return 1;
  }

  if (_ptr == MAP_FAILED) {
    printf("Error Creating mmap\n");
    return 1;
  }

  *(int *)size_ptr = list_length;
  *_ptr = list_ptr;

  curr = list;
  struct Node *next = (struct Node *)list_ptr;
  while (curr->next != NULL) {
    next->val = curr->val;
    next->next = (struct Node *)((char *)next + sizeof(struct Node));
    printf("%d -> %p\n", next->val, next);
    curr = curr->next;
    next = next->next;
  }

  next->next = NULL;

  return 0;
}