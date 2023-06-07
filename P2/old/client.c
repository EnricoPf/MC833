#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 65507

void die(const char *message) {
    perror(message);
    exit(1);
}

int main() {
    int sockfd;
    struct sockaddr_in serverAddr;
    socklen_t addrLen = sizeof(serverAddr);

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        die("Failed to create socket");
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address

    // Open the grayscale PNG image file
    FILE *file = fopen("grayscale.png", "rb");
    if (file == NULL) {
        die("Failed to open file");
    }

    // Read the file data and send it to the server
    char buffer[MAX_BUFFER_SIZE];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, MAX_BUFFER_SIZE, file)) > 0) {
        int bytesSent = sendto(sockfd, buffer, bytesRead, 0, (struct sockaddr *)&serverAddr, addrLen);
        if (bytesSent < 0) {
            die("Error in sendto()");
        }
    }

    printf("Grayscale PNG image sent to the server\n");

    fclose(file);
    close(sockfd);
    return 0;
}
