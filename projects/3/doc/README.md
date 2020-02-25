# Outline for smallsh
Only meets specs for prompt and builtins

## Main Routine
### Variables Declared in Main
```
char* commandBuffer : 2048 chars allocated on the heap
char* tokenBuffer : 2048 chars allocated on the heap
int status : stack var, holds return status of last fg child
#pid_t fgspawn : stack var, Currently running foreground PID
#pid_t* bgspawn : 512 pid_t allocated on heap
#int bgcount : stack var, number of currently running background processes
```

```
init commandBuffer to null string
init tokenBuffer to null string
init status to 0
#init bgcount to 0

while true:
  Send prompt to user and read input to command buffer
  If command is blank, continue
  If command starts with #, continue
  Tokenize command buffer to token buffer
  switch (first token):
    exit: break
    cd: Tokenize command buffer again, chdir to (second token)
    status: Print status of last foreground proc

clean up and exit shell
```

