/**********************************************************
 * Common library for the OTP applications.
 * 
 * AUTHOR
 *      Zach Colbert (colbertz@oregonstate.edu)
 * 
 * DATE
 *      7 March 2020
 **********************************************************/

// Modulus for OTP arithmetic operations
#define OTP_CHARSPACE_SIZE 27

// Convert char literal to int in OTP space.
int otp_ctoi(char);

// Convert int to char literal in OTP space.
char otp_itoc(int);

// Replace newline with null term in-place for the given C string.
void otp_strip_newline(char*);

// Return 1 if any characters in the string are not part of the OTP charspace
int otp_validate(char*);