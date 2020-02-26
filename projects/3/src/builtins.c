#include "builtins.h"

// Prints the given status to the console
// Message varies with argument "type"
void _status(enum status_t type, int status) {
  // Print different strings for signals vs returns
  switch (type) {
  case 0:
      printf("exit value ");
      break;
  case 1:
      printf("terminated by signal ");
      break;
  default:
      printf("status ");
  }

  printf("%d\n", status);   // Print given status
  fflush(stdout);
}

// Given string cmd containing "cd ARG"
// Changes working directory to ARG
// Blank arg changes cwd to env var HOME
int _changedir(char* cmd, char* buffer) {
  int intret = -5;

  // Edge case: If command is exactly 'cd', chdir to HOME
  memset(buffer, '\0', strlen(buffer));
  sscanf(cmd, "%*s %s", buffer);   // Tokenize all after cd
  if (strcmp(buffer, "") == 0) {
    intret = chdir(getenv("HOME"));
  } else {
    // chdir supports relative and absolute path names by default
    // so we don't have to do anything special
    memset(buffer, '\0', strlen(buffer));
    sscanf(cmd, "%*s %s %*s", buffer); // tokenize second word
    intret = chdir(buffer);
  }

  // Report chdir errors
  if (intret != 0) {
    fprintf(stderr, "cd: %s: %s\n", strerror(errno), buffer);
    fflush(stderr);
  }

  /* DEV: print cwd */
  memset(buffer, '\0', strlen(buffer));
  getcwd(buffer, (size_t) 100);
  printf("%s\n", buffer);

  return intret;
}