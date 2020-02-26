#include <stdio.h>

int main() {
    
  char* myStr = "This is a string with multiple words!";
  char scanStr[10];

  sscanf(myStr, "%s %*s", scanStr);

  printf("%s\n", scanStr);

  return 0;
}
