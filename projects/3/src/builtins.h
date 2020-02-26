#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

// status_t indicates if the status is a return value or signal
enum status_t {RETURN = 0, SIGNAL = 1};

// Prints the given status to the console
// Message varies with argument "type"
void _status(enum status_t type, int status);

// Given string cmd containing "cd ARG"
// Changes working directory to ARG
// Blank arg changes cwd to env var HOME
int _changedir(char* cmd, char* buffer);
