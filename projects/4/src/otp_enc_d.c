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

int main(int argc, char *argv[])
{
    int listenSocketFD, establishedConnectionFD, portNumber, charsRead, retval;
    socklen_t sizeOfClientInfo;
    char buffer[256];
    struct sockaddr_in serverAddress, clientAddress;
    pid_t spawnpid = -5;

    if (argc < 2) { fprintf(stderr,"USAGE: %s port\n", argv[0]); exit(1); } // Check usage & args

    // Set up the address struct for this process (the server)
    memset((char *)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
    portNumber = atoi(argv[1]); // Get the port number, convert to an integer from a string
    serverAddress.sin_family = AF_INET; // Create a network-capable socket
    serverAddress.sin_port = htons(portNumber); // Store the port number
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Any address is allowed for connection to this process

    // Set up the socket
    listenSocketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
    if (listenSocketFD < 0) error("otp_enc_d: ERROR opening socket");

    // Enable the socket to begin listening
    if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to port
        error("otp_enc_d: ERROR on binding");
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
        if (establishedConnectionFD < 0) error("otp_enc_d: ERROR on accept");

        // Create new child process to handle the connection
        spawnpid = fork();
        switch(spawnpid) {
            case -1:
                error("otp_enc_d: Hull breach!");
                break;

            case 0:     // Child process
                // Receive name of connected program, verify we're talking to otp_enc
                memset(buffer, '\0', 256);
                charsRead = recv(establishedConnectionFD, buffer, 255, 0);
                if (charsRead < 0) error("otp_enc_d: ERROR reading from socket");
                // printf("SERVER: New connection from %s\n", buffer);     // @ DEV
                if (strcmp(buffer, "otp_enc") != 0) {
                    // Reject connection from the client, sending it non-zero response
                    strcpy(buffer, "1");
                    charsRead = send(establishedConnectionFD, buffer, 1, 0);
                    if (charsRead < 0) error("otp_enc_d: ERROR reading from socket");
                    exit(1);
                } else {
                    // Accept connection, sending client zero response
                    strcpy(buffer, "0");
                    charsRead = send(establishedConnectionFD, buffer, 1, 0);
                    if (charsRead < 0) error("otp_enc_d: ERROR reading from socket");
                }

                // Get the message from the client and display it
                memset(buffer, '\0', 256);
                charsRead = recv(establishedConnectionFD, buffer, 255, 0); // Read the client's message from the socket
                if (charsRead < 0) error("otp_enc_d: ERROR reading from socket");
                printf("SERVER: I received this from the client: \"%s\"\n", buffer);

                // Send a Success message back to the client
                charsRead = send(establishedConnectionFD, "I am the server, and I got your message", 39, 0); // Send success back
                if (charsRead < 0) error("otp_enc_d: ERROR writing to socket");
                close(establishedConnectionFD); // Close the existing socket which is connected to the client
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
