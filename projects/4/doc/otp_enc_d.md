# `otp_enc_d`

Daemon receives plaintext and key from `otp_enc` via socket connection, and returns the resulting OTP ciphertext.

## Inputs
Command-line: Listening port number

Socket connection: Key, plaintext

## Outputs
Ciphertext string, written to `otp_enc` via socket connection.

## Return Value
Return 0 on success. Non-zero otherwise.

## Notes
+ Run parent process on a loop, listening on specified port
+ When a connection is made, `accept()` and enter child process
+ Child:
    + Check if connected to `otp_enc`
    + Receive plaintext
    + Receive key
    + Check key is at least as long as plaintext
    + Calculate ciphertext
    + Send ciphertext
    + Terminate or Recycle?

+ Must support 5 concurrent socket connections
+ Output errors to stderr, but keep running
    + Unless error is on startup, i.e. network calls fail
+ Use `localhost` as target host

## Dependencies
Copy from `src/start/server.c` and add `otp.h`

## Complete Spec
This program will run in the background as a daemon. Upon execution, `otp_enc_d` must output an error if it cannot be run due to a network error, such as the ports being unavailable. Its function is to perform the actual encoding, as described above in the Wikipedia quote. This program will listen on a particular port/socket, assigned when it is first ran (see syntax below). When a connection is made, `otp_enc_d` must call `accept()` to generate the socket used for actual communication, and then use a separate process to handle the rest of the transaction (see below), which will occur on the newly accepted socket.

This child process of `otp_enc_d` must first check to make sure it is communicating with `otp_enc` (see [otp_enc](otp_enc.md)). After verifying that the connection to `otp_enc_d` is coming from `otp_enc`, then this child receives from `otp_enc` plaintext and a key via the communication socket (not the original listen socket). The `otp_enc_d` child will then write back the ciphertext to the `otp_enc` process that it is connected to via the same communication socket. Note that the key passed in must be at least as big as the plaintext.

Your version of `otp_enc_d` must support up to five concurrent socket connections running at the same time; this is different than the number of processes that could queue up on your listening socket (which is specified in the second parameter of the `listen()` call). Again, only in the child process will the actual encryption take place, and the ciphertext be written back: the original server daemon process continues listening for new connections, not encrypting data.

In terms of creating that child process as described above, you may either create with `fork()` a new process every time a connection is made, or set up a pool of five processes at the beginning of the program, before connections are allowed, to handle your encryption tasks. As above, your system must be able to do five separate encryptions at once, using either method you choose.

Use this syntax for `otp_enc_d`:
```
otp_enc_d listening_port
```

_listening_port_ is the port that `otp_enc_d` should listen on. You will always start `otp_enc_d` in the background, as follows (the port 57171 is just an example; yours should be able to use any port):
```
$ otp_enc_d 57171 &
```

In all error situations, this program must output errors to stderr as appropriate (see grading script below for details), but should not crash or otherwise exit, unless the errors happen when the program is starting up (i.e. are part of the networking start up protocols like `bind()`). If given bad input, once running, `otp_enc_d` should recognize the bad input, report an error to stderr, and continue to run. Generally speaking, though, this daemon shouldn't receive bad input, since that should be discovered and handled in the client first. All error text must be output to stderr.

This program, and the other 3 network programs, should use "localhost" as the target IP address/host. This makes them use the actual computer they all share as the target for the networking connections.

[Back to README](../README.md)