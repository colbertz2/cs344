# Project 3 - smallsh
[Canvas Assignment](https://oregonstate.instructure.com/courses/1780106/assignments/7770260)

## Prompt
1. Use `:` as the prompt character
2. Immediately after printing anything, flush buffers with `fflush()`
3. Syntax (in general): `command [arg1 arg2 ...] [< input_file] [> output_file] [&]` (square brackets --> optional)
4. Accept commands up to 2048 characters, max 512 arguments
5. Allow blank lines--do nothing
6. Allow comment lines (denoted by `#`)--do nothing

_Assumptions_
1. Arguments are words separated by spaces
2. Redirection operators must be surrounded with spaces
3. Background operator `&` must be surrounded by spaces, must be last thing in command. `&` anywhere else in string should be handled as text.
4. Redirection is not ordered (input can come before or after output redirection)
5. Redirection operator must be immediately followed by filename
6. Shell does not support quoting
7. Pipe operator `|` is not implemented
8. No error checking is performed on the syntax of the command line

## Command Execution
1. Use `fork()`, `exec()`, and `waitpid()`
2. Recommended routine:
  1. Parent process (the shell) runs continuously
  2. For non-builtin commands, `fork()` a child. Child handles redirection with `dup2()`, then runs `exec()` on the command
  3. _Note:_ Redirection is not passed to exec (See `ls > junk` example on Canvas)
  4. If the command can't be found, exec will throw an error. Indicate the command failed to the user, and set builtin shell status to 1.
  5. _Note:_ Make sure a failed child process terminates itself!
3. Use the system PATH variable to look for non-builtin commands
4. Allow shell scripts to be executed
5. On redirection:
  1. Input redirects should be opened for reading only
  2. Output redirects should be opened for writing only, trunc if exists or create if not exists
  3. If file can't be opened, output error message and set status to 1
  4. Input and output can be redirected simultaneously
6. Expand all instances of `$$` to the shell's PID. (This is the only variable expansion)

## Background and Foreground
### Foreground Commands
Foreground commands don't end with `&`
1. wait for child process to terminate
2. DO NOT return prompt to the user until child terminates
3. _Note:_ Recommended to use `waitpid()` to wait on child

### Background Commands
Background commands end with `&`
1. Do not wait for child to terminate
2. Return prompt to user immediately after `fork()`ing
3. Periodically check for child termination (with `waitpid(... NOHANG ...)`) and clean up.
  + _Note:_ Consider storing PIDs of incomplete background processes in an array
4. If no inputs are specified by the user, redirect input _and_ output to `/dev/null`
5. When bg process begins, shell will print its PID to stdout
6. When bg process terminates, shell will print its exit status and PID to stdout (_before_ returning to prompt)

## Signals
Use `sigaction()` to set up signal handling

### SIGINT
`Ctrl-C` sends SIGINT to shell and all child processes at the same time. 

1. SIGINT _should not_ terminate shell
2. SIGINT _only_ terminates fg command if one is running
3. Shell _should not_ attempt to terminate child when receiving SIGINT
4. Forground child must terminate itself on SIGINT
5. When child is terminated, shell must print signal number that killed its child
6. Background processes _should not_ be terminated by SIGINT

### SIGSTP
`Ctrl-Z` sends SIGSTP to shell and all child processes at the same time

1. Immediately after the current fg process terminates, print `Entering foreground-only mode (& is now ignored)`
2. State change: Commands can no longer be run in the background, ignore `&` character
3. If SIGSTP is received again, print `Exiting foreground-only mode`
4. State change: Return to normal state
5. All child processes should ignore SIGSTP

## Builtin Commands
`exit`, `cd`, `status`

1. No redirection for these commands
2. No exit status for these commands
3. Shell handles these itself, no child process
4. Forced to run in foreground (ignore `&`)
  + Don't count as foreground processes for `status`!

### `exit`
Exits the shell

1. No arguments
2. _MUST_ kill all child processes

### `cd`
Changes the working directory

Use `chdir()`

1. Given no arguments, changes to HOME envvar
2. Given 1 argument, changes to specified directory
3. Must support absolute _and_ relative paths
4. Error when given too many arguments?

### `status`
Prints the exit status or terminating signal of the last fg process

1. Before any fg process is run, this should return 0

## Tips from Assignment
1. Program builtin commands first
2. Don't forget to `fflush()`!
3. Use grading script, program on os1 server

### Re-Entrancy
`printf()` family is not re-entrant! In signal handler functions, you cannot use these!

## Submission
Submit a single zip file containing:
+ As many source files as necessary
+ README with instructions on how to compile code

