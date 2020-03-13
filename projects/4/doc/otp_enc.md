# `otp_enc`

Client takes plaintext and key from files, and connects to otp_enc_d via socket connection to request encryption.

## Inputs
Command-line: plaintext file, key file, port number

Socket connection: Ciphertext string

## Outputs
Ciphertext string, written to stdout and terminated with newline

## Return Value
Return 0 on success. 
Return 2 on failure to connect to server.

## Notes
+ BE PREPARED TO ACCEPT LARGE INPUT
+ Outline:
    + Check input files exist
    + Check plaintext is valid
    + Check key is valid
    + Check key is at least as long as plaintext
    + Start socket connection to `otp_enc_d`
    + Send ID
    + Receive acknowledgement, stop if rejected by server
    + Send plaintext terminated with '@'
    + Receive acknowledgement
    + Send key
    + Receive ciphertext, writing buffered output to stdout

## Dependencies
Copy from `src/start/client.c`

## Complete Spec
This program connects to `otp_enc_d`, and asks it to perform a one-time pad style encryption as detailed above. By itself, `otp_enc` doesn’t do the encryption - `otp_enc_d` does. The syntax of `otp_enc` is as follows:

```
otp_enc plaintext key port
```

In this syntax, _plaintext_ is the name of a file in the current directory that contains the plaintext you wish to encrypt. Similarly, _key_ contains the encryption key you wish to use to encrypt the text. Finally, _port_ is the port that `otp_enc` should attempt to connect to `otp_enc_d` on.

When `otp_enc` receives the ciphertext back from `otp_enc_d`, it should output it to stdout. Thus, `otp_enc` can be launched in any of the following methods, and should send its output appropriately:

```
$ otp_enc myplaintext mykey 57171
$ otp_enc myplaintext mykey 57171 > myciphertext
$ otp_enc myplaintext mykey 57171 > myciphertext &
```

If `otp_enc` receives key or plaintext files with **ANY** bad characters in them, or the key file is shorter than the plaintext, then it should terminate, send appropriate error text to stderr, and set the exit value to 1.

`otp_enc` should NOT be able to connect to `otp_dec_d`, even if it tries to connect on the correct port - you'll need to have the programs reject each other. If this happens, `otp_enc` should report the rejection to stderr and then terminate itself. In more detail: if `otp_enc` cannot connect to the `otp_enc_d` server, for any reason (including that it has accidentally tried to connect to the `otp_dec_d` server), it should report this error to stderr with the attempted port, and set the exit value to 2. Otherwise, upon successfully running and terminating, `otp_enc` should set the exit value to 0.

Again, any and all error text must be output to stderr (not into the plaintext or ciphertext files).

[Back to README](../README.md)