#include <fcntl.h>
#include <stdio.h>
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

  int list_fd, size_fd, ptr_fd;
  void *list_ptr, *size_ptr, **_ptr;

  list_fd = shm_open(list_shm, O_RDONLY, 0666);
  size_fd = shm_open(size, O_RDONLY, 0666);
  ptr_fd = shm_open(ptr, O_RDONLY, 0666);

  _ptr = (void **)mmap(0, sizeof(void **), PROT_READ, MAP_SHARED, ptr_fd, 0);
  if (_ptr == MAP_FAILED) {
    printf("Error Creating mmap\n");
    return 1;
  }

  size_ptr = (int *)mmap(0, sizeof(int), PROT_READ, MAP_SHARED, size_fd, 0);

  if (size_ptr == MAP_FAILED) {
    printf("Error Creating mmap\n");
    return 1;
  }
  const int SIZE = *(int *)size_ptr * sizeof(struct Node *);

  list_ptr = (struct Node *)mmap(*_ptr, SIZE, PROT_READ, MAP_SHARED | MAP_FIXED,
                                 list_fd, 0);
  if (list_ptr == MAP_FAILED) {
    printf("Error Creating mmap\n");
    return 1;
  }

  struct Node *curr = (struct Node *)list_ptr;
  while (curr->next != NULL) {
    printf("%d -> %p\n", curr->val, curr);
    curr = curr->next;
  }

  shm_unlink(list_shm);
  shm_unlink(size);
  shm_unlink(ptr);

  return 0;
}