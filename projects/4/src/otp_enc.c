#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg) { perror(msg); exit(0); } // Error function used for reporting issues

int main(int argc, char *argv[])
{
    int socketFD, portNumber, charsWritten, charsRead, len_ptxt;
    struct sockaddr_in serverAddress;
    struct hostent* serverHostInfo;
    char buffer[256];
    
    if (argc < 3) { fprintf(stderr,"USAGE: %s hostname port\n", argv[0]); exit(0); } // Check usage & args

    // Set up the server address struct
    memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
    portNumber = atoi(argv[2]); // Get the port number, convert to an integer from a string
    serverAddress.sin_family = AF_INET; // Create a network-capable socket
    serverAddress.sin_port = htons(portNumber); // Store the port number
    serverHostInfo = gethostbyname(argv[1]); // Convert the machine name into a special form of address
    if (serverHostInfo == NULL) { fprintf(stderr, "CLIENT: ERROR, no such host\n"); exit(0); }
    memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address

    // Set up the socket
    socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
    if (socketFD < 0) error("CLIENT: ERROR opening socket");
    
    // Connect to server
    if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to address
        error("CLIENT: ERROR connecting");

    // Send server my name
    memset(buffer, '\0', sizeof(buffer));
    strcpy(buffer, "otp_enc");
    charsWritten = send(socketFD, buffer, strlen(buffer), 0);
    if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
    if (charsWritten < strlen(buffer)) printf("CLIENT: WARNING: Not all data written to socket!\n");

    // Wait for server to accept connection
    memset(buffer, '\0', sizeof(buffer));
    charsRead = recv(socketFD, buffer, 1, 0);	// Read single char response
    if (charsRead < 0) error("CLIENT: ERROR reading from socket");
    if (atoi(buffer) != 0) {
        fprintf(stderr, "CLIENT: Unable to connect to otp_enc_d on port %d\n", portNumber);
        exit(2);    // Return value required by specs!
    }

    // Get input message from user
    printf("CLIENT: Enter text to send to the server, and then hit enter: ");
    memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer array
    fgets(buffer, sizeof(buffer) - 1, stdin); // Get input from the user, trunc to buffer - 1 chars, leaving \0
    buffer[strcspn(buffer, "\n")] = '\0'; // Remove the trailing \n that fgets adds
    len_ptxt = strlen(buffer);      // @DEV hang on to ptxt length

    // Send plaintext to server
    charsWritten = send(socketFD, buffer, strlen(buffer), 0); // Write to the server
    if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
    if (charsWritten < strlen(buffer)) printf("CLIENT: WARNING: Not all data written to socket!\n");

    // Send plaintext terminator @ to server
    charsWritten = send(socketFD, "@", 1, 0);

    // Wait for server cue to send key
    memset(buffer, '\0', sizeof(buffer));
    charsRead = recv(socketFD, buffer, 1, 0);	// Read single char response
    if (charsRead < 0) error("CLIENT: ERROR reading from socket");

    // Send key to server
    memset(buffer, ' ', len_ptxt);    // @ DEV
    charsWritten = send(socketFD, buffer, strlen(buffer), 0);
    if (charsWritten < 0) error ("CLIENT: ERROR writing to socket");
    if (charsWritten < strlen(buffer)) printf("CLIENT: WARNING: Not all data written to socket!\n");

    // Get ciphertext back from server
    charsRead = 0;
    do {
        memset(buffer, '\0', sizeof(buffer));
        charsRead += recv(socketFD, buffer, sizeof(buffer) - 1, 0);
        if (charsRead < 0) error("CLIENT: ERROR reading from socket");
        printf("%s", buffer);
    } while (charsRead < len_ptxt);
    printf("\n");

    close(socketFD); // Close the socket
    return 0;
}
