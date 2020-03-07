# Project 4

A set of client-daemon applications to implement basic OTP encryption for capital letters and the space character.

## Programs
### [`otp_enc_d`](doc/otp_enc_d.md)
Daemon receives plaintext and key from `otp_enc` via socket connection, and returns the resulting OTP ciphertext.

### [`otp_enc`](doc/otp_enc.md)
Client takes plaintext and key from files, and connects to `otp_enc_d` via socket connection to request encryption.

### [`otp_dec_d`](doc/otp_dec_d.md)
Daemon receives ciphertext and key from `otp_dec` via socket connection, and returns the resulting OTP plaintext.

### [`otp_dec`](doc/otp_dec.md)
Client takes ciphertext and key from files, and connect to `otp_dec_d` via socket connection to request encryption.

### [`keygen`](doc/keygen.md)
Creates a key file of the specified length.

## Links
[Course Home](https://oregonstate.instructure.com/courses/1780106)
[Block 4](https://oregonstate.instructure.com/courses/1780106/pages/block-4)
[Assignment Description](https://oregonstate.instructure.com/courses/1780106/assignments/7770261)
