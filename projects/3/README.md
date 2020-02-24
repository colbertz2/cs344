# Project 3 - smallsh
[Canvas Assignment](https://oregonstate.instructure.com/courses/1780106/assignments/7770260)

## Specifications
### Prompt
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

### Command Execution
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

### Background and Foreground
#### Foreground Commands
Foreground commands don't end with `&`
1. wait for child process to terminate
2. DO NOT return prompt to the user until child terminates
3. _Note:_ Recommended to use `waitpid()` to wait on child

#### Background Commands
Background commands end with `&`
1. Do not wait for child to terminate
2. Return prompt to user immediately after `fork()`ing
3. Periodically check for child termination (with `waitpid(... NOHANG ...)`)
4. 

