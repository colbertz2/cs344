# `keygen`

Creates a key of the specified length.

## Inputs
Integer _keylength_ : Number of characters in key. Must be greater than 0.

## Outputs
"Random" key string of the specified length, terminated with newline.

## Return Value
Integer _keylength_ : Number of characters in key.
Returns -1 if key generation failed.

## Notes
+ Output has 1 char more than _keylength_ (including newline)
+ Not cryptographically secure - use `rand()` for random number generation
+ Key is not broken into blocks - It's a single string

## Dependencies
```
<stdio.h>
    printf, perror

<stdlib.h>
    srand, rand

otp.h
    otp_ctoi, otp_itoc, OTP_CHARSPACE_SIZE
```

## Complete Spec
This program creates a key file of specified length. The characters in the file generated will be any of the 27 allowed characters, generated using the standard UNIX randomization methods. Do not create spaces every five characters, as has been historically done. Note that you specifically do not have to do any fancy random number generation: we’re not looking for cryptographically secure random number generation! rand() is just fine. The last character keygen outputs should be a newline. All error text must be output to stderr, if any.

The syntax for `keygen` is as follows:

```
keygen keylength
```

Where _keylength_ is the length of the key file in characters. `keygen` outputs to stdout. Here is an example run, which redirects stdout to a key file of 256 characters called “mykey” (note that mykey is 257 characters long because of the newline):

```
$ keygen 256 > mykey
```

[Back to README](../README.md)