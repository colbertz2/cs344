/******************************************************************
 * Demo declaration, definition, and access of array of strings.  *
 ******************************************************************/

#include <stdio.h>

#define NAMESPACE_SIZE 10
#define NAME_MAX_SIZE 8

char myArr[NAMESPACE_SIZE][NAME_MAX_SIZE] = {
  "1 A",
  "2 B",
  "3 C",
  "4 D",
  "5 E",
  "6 F",
  "7 G",
  "8 H",
  "9 I",
  "10 J"
};


int main() {
  int i;
  for (i = 0; i < NAMESPACE_SIZE; i++) {
    printf("%s\n", myArr[i]);
  }

  return 0;
}

