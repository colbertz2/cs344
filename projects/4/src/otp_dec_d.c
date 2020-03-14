#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include "otp.h"

void error(const char *msg) { perror(msg); exit(1); } // Error function used for reporting issues

char* _dec_otp(char* ptxt, char* key);  // OTP decryption method

int main(int argc, char *argv[])
{
    int listenSocketFD, establishedConnectionFD, portNumber, charsRead, retval;
    socklen_t sizeOfClientInfo;
    char buffer[256];
    struct sockaddr_in serverAddress, clientAddress;
    pid_t spawnpid = -5;
    char *ptxt, *key, *ctxt;
    int sizeof_pk = 256;

    if (argc < 2) { fprintf(stderr,"USAGE: %s port\n", argv[0]); exit(1); } // Check usage & args

    // Initialize buffers on heap to store plaintext and key
    ptxt = calloc(sizeof_pk, sizeof(char));
    key = calloc(sizeof_pk, sizeof(char));
    memset(ptxt, '\0', sizeof_pk);
    memset(key, '\0', sizeof_pk);

    // Set up the address struct for this process (the server)
    memset((char *)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
    portNumber = atoi(argv[1]); // Get the port number, convert to an integer from a string
    serverAddress.sin_family = AF_INET; // Create a network-capable socket
    serverAddress.sin_port = htons(portNumber); // Store the port number
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Any address is allowed for connection to this process

    // Set up the socket
    listenSocketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
    if (listenSocketFD < 0) error("otp_dec_d: ERROR opening socket");

    // Enable the socket to begin listening
    if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to port
        error("otp_dec_d: ERROR on binding");
    listen(listenSocketFD, 5); // Flip the socket on - it can now receive up to 5 connections

    // Continue listening until terminated
    while (1) {
        // Clean up terminated child processes until there are none
        do {
            spawnpid = waitpid(-1, &retval, WNOHANG);
        } while (spawnpid > 0);

        // Accept a connection, blocking if one is not available until one connects
        sizeOfClientInfo = sizeof(clientAddress); // Get the size of the address for the client that will connect
        establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); // Accept
        if (establishedConnectionFD < 0) error("otp_dec_d: ERROR on accept");

        // Create new child process to handle the connection
        spawnpid = fork();
        switch(spawnpid) {
            case -1:
                error("otp_dec_d: Hull breach!");
                break;

            case 0:     // Child process
                // Receive name of connected program, verify we're talking to otp_dec
                memset(buffer, '\0', 256);
                charsRead = recv(establishedConnectionFD, buffer, 255, 0);
                if (charsRead < 0) error("otp_dec_d: ERROR reading from socket");
                // printf("SERVER: New connection from %s\n", buffer);     // @DEV
                if (strcmp(buffer, "otp_dec") != 0) {
                    // Reject connection from the client, sending it non-zero response
                    strcpy(buffer, "1");
                    charsRead = send(establishedConnectionFD, buffer, 1, 0);
                    if (charsRead < 0) error("otp_dec_d: ERROR reading from socket");
                    exit(1);
                } else {
                    // Accept connection, sending client zero response
                    strcpy(buffer, "0");
                    charsRead = send(establishedConnectionFD, buffer, 1, 0);
                    if (charsRead < 0) error("otp_dec_d: ERROR reading from socket");
                }

                // Receive plaintext
                do {
                    // Make sure ptxt and key strings are large enough to hold new data
                    if (sizeof_pk - strlen(ptxt) < 256) {
                        ptxt = realloc(ptxt, 2 * sizeof_pk);    // double allocated memory
                        key = realloc(key, 2 * sizeof_pk);
                        if (ptxt == NULL) error("otp_dec_d: ERROR realloc ptxt");
                        if (key == NULL) error("otp_dec_d: ERROR realloc key");
                        memset(ptxt + sizeof_pk, '\0', sizeof_pk);  // initialize new memory
                        memset(key + sizeof_pk, '\0', sizeof_pk);
                        sizeof_pk *= 2;
                    }

                    // Read data from socket
                    memset(buffer, '\0', 256);
                    charsRead = recv(establishedConnectionFD, buffer, 255, 0);
                    if (charsRead < 0) error("otp_dec_d: ERROR reading from socket");
                    if (charsRead == 0) {
                        fprintf(stderr, "otp_dec_d: Socket closed unexpectedly\n"); exit(1);
                    }

                    // fprintf(stderr, "otp_dec_d: received ptxt from client: \"%s\"\n", buffer);   // @DEV

                    // Concat new data onto end of plaintext string
                    strcat(ptxt, buffer);

                } while (strstr(buffer, "@") == NULL);  // Loop until @ terminator is received

                // Strip "@" terminator from plaintext
                ptxt[strcspn(ptxt, "@")] = '\0';

                // Send client cue to start transmitting key
                charsRead = send(establishedConnectionFD, "0", 1, 0);
                if (charsRead < 0) error("otp_dec_d: ERROR writing to socket");

                // Receive key
                do {
                    // Read data from socket
                    memset(buffer, '\0', 256);
                    charsRead = recv(establishedConnectionFD, buffer, 255, 0);
                    if (charsRead < 0) error("otp_dec_d: ERROR reading from socket");
                    if (charsRead == 0) {
                        fprintf(stderr, "otp_dec_d: Socket closed unexpectedly\n"); exit(1);
                    }

                    // fprintf(stderr, "otp_dec_d: received key from client: \"%s\"\n", buffer);    // @DEV

                    // Concat new data onto end of key string
                    strcat(key, buffer);

                } while (strlen(key) < strlen(ptxt));   // Loop until key is at least as long as ptxt

                fprintf(stderr, "otp_dec_d: PTXT: %s\n", ptxt); // @DEV
                fprintf(stderr, "otp_dec_d: KEY:  %s\n", key);  // @DEV

                // Encrypt plaintext with key
                ctxt = _dec_otp(ptxt, key);

                // Send ciphertext to client
                charsRead = send(establishedConnectionFD, ctxt, strlen(ctxt), 0); // Send success back
                if (charsRead < 0) error("otp_dec_d: ERROR writing to socket");
                close(establishedConnectionFD); // Close the existing socket which is connected to the client
                free(ptxt);
                free(key);
                free(ctxt);
                exit(0);
                break;

            default:    // Parent process
                // fprintf(stderr, "Spawned child: %d\n", spawnpid);    // @DEV
                spawnpid = -5;
                break;
        }
    }

    close(listenSocketFD); // Close the listening socket
    return 0; 
}

// OTP decryption method (basically modular addition)
char* _dec_otp(char* ptxt, char* key) {
    int i, p, c, k;
    char *ctxt;

    ctxt = calloc(strlen(ptxt) + 1, sizeof(char));
    memset(ctxt, '\0', strlen(ptxt) + 1);

    for (i = 0; i < strlen(ptxt); i++) {
        // Convert chars to int
        p = otp_ctoi(ptxt[i]);
        k = otp_ctoi(key[i]);

        // Add p and k mod number of available vals
        c = (p + k) % OTP_CHARSPACE_SIZE;

        // Convert back to char
        ctxt[i] = otp_itoc(c);
    }

    return ctxt;
}