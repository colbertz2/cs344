#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include "otp.h"

void error(const char *msg) { perror(msg); exit(5); } // Error function used for reporting issues

// Shift a string n characters to the left
// (Trunc chars at the start of the string in-place)
void strshift(char* buffer, int n) {
    int i;
    if (n < 0) fprintf(stderr, "strshift: Invalid input\n");
    if (n == 0) return;

    // shift chars to the left
    for (i = 0; i < n; i++) { 
        if (i > strlen(buffer)) break;
        
        buffer[i] = buffer[n + i];
    }

    // null chars following the shifted string
    memset(buffer + n, '\0', strlen(buffer) - n);
}

int main(int argc, char *argv[])
{
    int socketFD, portNumber, charsWritten, charsRead, len_ptxt;
    int ptxtFD, keyFD;
    struct sockaddr_in serverAddress;
    struct hostent* serverHostInfo;
    char buffer[256];
    
    if (argc < 4) { fprintf(stderr,"USAGE: %s plaintext key port\n", argv[0]); exit(0); } // Check usage & args

    /***************************
     * INPUT VALIDATION
     ***************************/
    
    // Open input files
    ptxtFD = open(argv[1], O_RDONLY);
    if (ptxtFD == -1) { perror("otp_dec: Unable to open plaintext file"); exit(1); }

    keyFD = open(argv[2], O_RDONLY);
    if (keyFD == -1) { perror("otp_dec: Unable to open key file"); exit(1); }

    // Check plaintext is valid
    memset(buffer, '\0', sizeof(buffer));
    len_ptxt = 0;
    while (read(ptxtFD, buffer, sizeof(buffer) - 1) > 0) {
        otp_strip_newline(buffer);
        len_ptxt += strlen(buffer);
        charsWritten = otp_validate(buffer);

        if (charsWritten == 1) {
            fprintf(stderr, "otp_dec: plaintext \"%s\" contains bad characters\n", argv[1]);
            exit(1);
        }

        memset(buffer, '\0', sizeof(buffer));
    }

    // Check key is valid
    charsRead = 0;
    while (read(keyFD, buffer, sizeof(buffer) - 1) > 0) {
        otp_strip_newline(buffer);
        charsRead += strlen(buffer);        // Temp track length of key
        charsWritten = otp_validate(buffer);

        if (charsWritten == 1) {
            fprintf(stderr, "otp_dec: key \"%s\" contains bad characters\n", argv[2]);
            exit(1);
        }

        memset(buffer, '\0', sizeof(buffer));
    }

    // Check key length is sufficient for plaintext
    if (charsRead < len_ptxt) {
        fprintf(stderr, "otp_dec: key \"%s\" is too short\n", argv[2]);
        exit(1);
    }

    // Seek back to start of files for reading later
    lseek(ptxtFD, 0, SEEK_SET);
    lseek(keyFD, 0, SEEK_SET);


    /*************************
     * SERVER CONNECTION
     *************************/

    // Set up the server address struct
    memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
    portNumber = atoi(argv[3]); // Get the port number, convert to an integer from a string
    serverAddress.sin_family = AF_INET; // Create a network-capable socket
    serverAddress.sin_port = htons(portNumber); // Store the port number
    serverHostInfo = gethostbyname("localhost"); // Convert the machine name into a special form of address
    if (serverHostInfo == NULL) { fprintf(stderr, "otp_dec: No such host: localhost\n"); exit(2); }
    memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address

    // Set up the socket
    socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
    if (socketFD < 0) {
        fprintf(stderr, "otp_dec: Error opening socket\n");
        exit(2);
    }
    
    // Connect to server
    if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        fprintf(stderr, "otp_dec: Unable to connect to otp_dec_d on port %d\n", portNumber);
        exit(2);
    }

    // printf("Connected to server\n");    // @DEV

    /**************************
     * SERVER ROUTINE
     **************************/

    // Send server my name
    memset(buffer, '\0', sizeof(buffer));
    strcpy(buffer, "otp_dec");
    charsWritten = send(socketFD, buffer, strlen(buffer), 0);
    if (charsWritten < 0) error("otp_dec: Error writing to socket");
    while (charsWritten < strlen(buffer)) {
        strshift(buffer, strlen(buffer) - charsWritten);
        charsWritten = send(socketFD, buffer, strlen(buffer), 0);
        if (charsWritten < 0) error("otp_dec: Error writing to socket");
    }

    // printf("Sent identification\n");    // @DEV

    // Wait for server to accept connection
    memset(buffer, '\0', sizeof(buffer));
    charsRead = recv(socketFD, buffer, 1, 0);   // Read single char response
    if (charsRead < 0) error("otp_dec: Error reading from socket");
    if (atoi(buffer) != 0) {
        fprintf(stderr, "otp_dec: Unable to connect to otp_dec_d on port %d\n", portNumber);
        exit(2);    // Return value required by specs!
    }

    // printf("Server accepted connection\n");     // @DEV

    // Send plaintext to server
    memset(buffer, '\0', sizeof(buffer));
    len_ptxt = 0;
    while (read(ptxtFD, buffer, sizeof(buffer) - 1) > 0) { // Read plaintext 255 bytes at a time
        otp_strip_newline(buffer);  // Strip newline from buffer if it is there
        
        len_ptxt += strlen(buffer);     // Keep track of total plaintext length

        charsWritten = send(socketFD, buffer, strlen(buffer), 0);   // Attempt to send whole buffer
        if (charsWritten < 0) error("otp_dec: Error writing to socket");

        while (charsWritten < strlen(buffer)) {     // Continue attempts until whole buffer has been sent
            strshift(buffer, strlen(buffer) - charsWritten);    // Only send bytes that haven't sent already
            charsWritten = send(socketFD, buffer, strlen(buffer), 0);
            if (charsWritten < 0) error("otp_dec: Error writing to socket");
        }

        memset(buffer, '\0', sizeof(buffer));   // Reset buffer before next read
    }

    // Send plaintext terminator @ to server
    do {
        charsWritten = send(socketFD, "@", 1, 0);
        if (charsWritten < 0) error("otp_dec: Error writing to socket");
    } while (charsWritten < 1);

    // printf("Finished sending plaintext\n");     // @DEV

    // Wait for server cue to send key
    memset(buffer, '\0', sizeof(buffer));
    charsRead = recv(socketFD, buffer, 1, 0);   // Read single char response
    if (charsRead < 0) error("otp_dec: Error reading from socket");

    // Send key to server
    memset(buffer, '\0', sizeof(buffer));
    charsRead = 0;
    while (read(keyFD, buffer, sizeof(buffer) - 1) > 0 && charsRead < len_ptxt) {
        otp_strip_newline(buffer);      // Strip newline from buffer if it's there

        charsRead += strlen(buffer);    // Temp track total length of key read from file

        charsWritten = send(socketFD, buffer, strlen(buffer), 0);   // Attempt to send whole buffer
        if (charsWritten < 0) error("otp_dec: Error writing to socket");

        while (charsWritten < strlen(buffer)) {     // Continue sending bytes that fail
            strshift(buffer, strlen(buffer) - charsWritten);    // Only send bytes that haven't sent already
            charsWritten = send(socketFD, buffer, strlen(buffer), 0);
            if (charsWritten < 0) error("otp_dec: Error writing to socket");
        }

        memset(buffer, '\0', sizeof(buffer));   // Reset buffer before next read
    }

    // printf("Finished sending key\n");       // @DEV

    // Server will automatically stop reading key when it's length meets ptxt length

    // Get ciphertext back from server
    charsRead = 0;
    do {
        memset(buffer, '\0', sizeof(buffer));
        charsRead += recv(socketFD, buffer, sizeof(buffer) - 1, 0);
        if (charsRead < 0) error("otp_dec: ERROR reading from socket");
        printf("%s", buffer);
    } while (charsRead < len_ptxt);
    printf("\n");

    close(socketFD); // Close the socket
    close(ptxtFD);    // Close input files
    close(keyFD);
    return 0;
}
