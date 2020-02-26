#include <stdio.h>
#include <stdlib.h>

int main() {
  int* arr = calloc(25, sizeof(int));
  int i;

  for (i = 0; i < 25; i++) {
    arr[i] = i;
  }

  printf("sizeof arr: %lu\n", sizeof(arr));
  printf("Contents: ");
  for (i = 0; i < 25; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
  fflush(stdout);

  arr = realloc(arr, 12);

  printf("sizeof arr: %lu\n", sizeof(arr));
  printf("Contents: ");
  for (i = 0; i < 12; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
  fflush(stdout);

  free(arr);

  return 0;
}
