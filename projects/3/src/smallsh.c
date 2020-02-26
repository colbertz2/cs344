/*******************************************************************************
 * NAME
 *    smallsh
 *
 * DESCRIPTION
 *    Tiny shell does your bidding! Features simple redirection, foregrounding
 *    and backgrounding of processes.
 *
 * AUTHOR
 *    Zach Colbert (colbertz@oregonstate.edu)
 *
 * NOTES
 *    For CS344, W2020 (online)
 ******************************************************************************/

#include "builtins.h"
#include "exec.h"
#include <sys/wait.h>

#define BG_MAX 512

/**************************************************************************
 * MAIN ROUTINE
 *
 * "Shell" runs continuously, accepting input from user and handling child
 * processes. Builtin commands: exit  cd  status
 **************************************************************************/
int main() {
  int status, cmdBufSize, cmdReadSize, intret;
  int bg, ffg;
  enum status_t type;
  char *cmdBuffer, *tokBuffer, *prompt = ": ";
  pid_t spawnpid = -5;

  /* INITIALIZE VARIABLES */
  status = 0;   // Default status before exec'ing any child proc
  type = RETURN; // Indicates whether status is a return val or signal
  cmdBufSize = CMD_MAX + 1;   // Buffer size is 2048 chars + null term
  cmdReadSize = 0;            // Getline tells us how many chars it reads
  bg = 0;       // By default, run processes in the foreground
  ffg = 0;      // When ffg == 1, force foregrounding

  // Use CMD_MAX + 1 so that the actual capacity of each string can be CMD_MAX
  cmdBuffer = calloc(cmdBufSize, sizeof(char));
  tokBuffer = calloc(cmdBufSize, sizeof(char)); // Allocate heap space
  memset(cmdBuffer, '\0', cmdBufSize);
  memset(tokBuffer, '\0', cmdBufSize); // Init buffers to empty strings

  /* MAIN SHELL LOOP */
  // Loop until exit command is received
  while (1) {

    /** INIT BUFFERS **/
    // Clear the contents of each buffer before each prompt
    memset(cmdBuffer, '\0', strlen(cmdBuffer));
    memset(tokBuffer, '\0', strlen(tokBuffer));

    /** PROMPT **/
    printf(prompt);
    fflush(stdout);     // Print and flush user prompt
    cmdReadSize = getline(&cmdBuffer, (size_t*) &cmdBufSize, stdin);   // Get user input

    // Getline will expand our buffer if necessary
    // Enforce 2048-char command limit
    if (cmdReadSize > CMD_MAX) {
      fprintf(stderr, "smallsh: Command is too long\n");
      fflush(stderr);

      // Reallocate and initialize command buffer to original size
      cmdBufSize = CMD_MAX + 1;
      cmdBuffer = realloc(cmdBuffer, cmdBufSize);
      memset(cmdBuffer, '\0', cmdBufSize);

      continue;
    }

    // Tokenize first word
    sscanf(cmdBuffer, "%s %*s", tokBuffer);

    /** BLANKS / COMMENTS **/
    // If line is blank or comment, do nothing and continue
    if (strcmp(tokBuffer, "") == 0 || tokBuffer[0] == '#') {
      continue;
    }

    /** BULTIN COMMANDS **/
    // Process command right away if it's one of the builtins
    if (strcmp(tokBuffer, "exit") == 0) {
      break;    // Stop shell, clean up and exit
    
    } else if (strcmp(tokBuffer, "status") == 0) {
      // Print the last return value or signal
      _status(type, status);
      continue;
    
    } else if (strcmp(tokBuffer, "cd") == 0) {
      intret = _changedir(cmdBuffer, tokBuffer);
      continue;
    }

    /** COMMAND EXECUTION **/
    bg = 0;   // By default, run processes in the foreground
    spawnpid = fork();
    switch (spawnpid) {
      case -1:
        // Something went wrong, no child was created
        fprintf(stderr, "fork: %s\n", strerror(errno));
        fflush(stderr);
        status = -1;
        break;
      
      case 0:
        /* This is the child process! */
        //_set_redirects();
        _execute_cmd(cmdBuffer);
        break;

      default:
        /* This is the parent process! */
        //bg = _check_background()
        // Wait on foreground process to terminate
        waitpid(spawnpid, &intret, 0);
        
        // Interpret results of process termination
        if (WIFSIGNALED(intret)) {
          type = SIGNAL;
          status = WTERMSIG(intret);
        } else {
          type = RETURN;
          status = WEXITSTATUS(intret);
        }
    }
    continue;   // After processing command, return to prompt

    /** COMMAND NOT FOUND **/
    // If nothing has continued the loop up to this point, command failed
    fprintf(stderr, "smallsh: command not found: %s\n", tokBuffer);
    fflush(stderr);
    status = 1;
    type = RETURN;

  }

  /* CLEAN UP AND EXIT */
  free(cmdBuffer);
  free(tokBuffer);

  return 0;
}

