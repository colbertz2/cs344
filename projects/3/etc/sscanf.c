#include <stdio.h>

int main() {
    
  char* myStr = "cd";
  char scanStr[10];

  sscanf(myStr, "%*s %s %*s", scanStr);

  printf("%s\n", scanStr);

  return 0;
}
