# `otp`

Common library for the OTP applications.

## Public Methods
### `int otp_ctoi(char)`
Convert char literal to int in OTP space.
Error if char is not in OTP_CHARSPACE.

### `char otp_itoc(int)`
Convert int to char literal in OTP space.
Error if int is greater or equal to OTP_CHARSPACE_SIZE.

### `void otp_strip_newline(char*)`
Replace newline with null term in-place for the given C string.

## Public Data
### `OTP_CHARSPACE_SIZE`
Size of OTP_CHARSPACE array, or modulus for OTP arithmetic operations.

## Private Data
### `char[] OTP_CHARSPACE`
Array of all char literals defined for this OTP scheme.
