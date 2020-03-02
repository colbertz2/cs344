#include "exec.h"
#include <stdio.h>

int main() {

  char* t1 = "cat < input > output";
  char* t2 = "cat > output < input";

  printf("%s\n", t1);
  _set_redirects(t1);
  printf("\n");

  printf("%s\n", t2);
  _set_redirects("cat > output < input");

  return 0;
}
