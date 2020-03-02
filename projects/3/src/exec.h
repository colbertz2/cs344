#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <errno.h>

#define CMD_MAX 2048
#define NARG_MAX 512

// Opens necessary file streams and parses command line
// Redirects stdin or stdout as directed
int _set_redirects(char* cmd);

// Parses command string and runs exec()
void _execute_cmd(char* cmd);

// Checks for the background operator at the end of the command line
int _check_background(char* cmd);
