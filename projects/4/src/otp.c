/**********************************************************
 * Common library for the OTP applications.
 * 
 * AUTHOR
 *      Zach Colbert (colbertz@oregonstate.edu)
 * 
 * DATE
 *      7 March 2020
 **********************************************************/

#include <stdio.h>
#include <string.h>
#include "otp.h"

// Array of all char literals defined for this OTP scheme
char OTP_CHARSPACE[] = {
    ' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 0
};

// Convert char literal to int in OTP space.
int otp_ctoi(char c) {
    int i = strcspn(OTP_CHARSPACE, &c);

    if (i >= OTP_CHARSPACE_SIZE) {
        fprintf(stderr, "otp_ctoi: No such character: %c\n", c);
        return -1;
    }

    return i;
}

// Convert int to char literal in OTP space.
char otp_itoc(int i) {
    if (i >= OTP_CHARSPACE_SIZE) {
        fprintf(stderr, "otp_itoc: Invalid index: %d\n", i);
        return '\0';
    }

    return OTP_CHARSPACE[i];
}

// Replace newline with null term in-place for the given C string.
void otp_strip_newline(char* buffer) {
    buffer[strcspn(buffer, "\n")] = '\0';
}
