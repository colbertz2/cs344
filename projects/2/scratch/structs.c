#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct room {
  int id;
  char name[9];
};

struct room* room_create() {
  struct room* r = malloc(sizeof(struct room));

  r->id = 1;
  strcpy(r->name, "Zach");

  return r;
}

int main() {
  struct room* r = room_create();

  printf("Room %d with name %s\n", r->id, r->name);

  free(r);

  return 0;
}

