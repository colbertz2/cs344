#include <stdio.h>
#include <unistd.h>

int main() {
  
  char *wd = NULL;
 
  wd = get_current_dir_name();

  printf("Current directory: %s\n", wd);

  chdir("etc");

  wd = get_current_dir_name();

  printf("Current directory: %s\n", wd);

  return 0;
}
