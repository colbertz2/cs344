/**********************************************************
 * Creates a key of the specified length.
 * 
 * AUTHOR
 *      Zach Colbert (colbertz@oregonstate.edu)
 * 
 * DATE
 *      10 March 2020
 **********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "otp.h"

int main(int argc, char* argv[]) {
    // Require at least 2 arguments (prog name and keylength)
    if (argc < 2) {
        fprintf(stderr, "Usage: keygen KEYLENGTH\n");
        return 1;
    }

    int i, keylength = atoi(argv[1]);
    char buffer[keylength + 1];

    // Initialize string
    memset(buffer, '\0', keylength + 1);

    // Random number seed
    srand(getpid());

    // Randomly fill buffer with characters from the OTP charspace
    for (i = 0; i < keylength; i++) {
        buffer[i] = otp_itoc(rand() % OTP_CHARSPACE_SIZE);
    }

    // Print buffer to stdout, terminated with newline
    printf("%s\n", buffer);

    return 0;
}
