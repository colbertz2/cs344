#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define CMD_MAX 2048
#define NARG_MAX 512

// Parses command string and runs exec()
void _execute_cmd(char* cmd);
