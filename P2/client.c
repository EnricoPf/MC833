#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void die(const char *message) {
    perror(message);
    exit(1);
}

void sendImage(const char *filename, const char *serverIP, int serverPort) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        die("Failed to open image file");
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char *)malloc(fileSize);
    if (!buffer) {
        fclose(file);
        die("Memory allocation failed");
    }

    size_t bytesRead = fread(buffer, 1, fileSize, file);
    fclose(file);

    int sockfd;
    struct sockaddr_in serverAddr;

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        free(buffer);
        die("Failed to create socket");
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);

    if (inet_pton(AF_INET, serverIP, &(serverAddr.sin_addr)) <= 0) {
        free(buffer);
        die("Invalid server IP address");
    }

    // Send image data to the server
    ssize_t bytesSent = sendto(sockfd, buffer, bytesRead, 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (bytesSent < 0) {
        free(buffer);
        die("Error in sendto()");
    }

    printf("Sent a grayscale PNG image to the server: %ld bytes\n", bytesRead);

    free(buffer);
    close(sockfd);
}

int main() {
    const char *imageFile = "grayscale.png";
    const char *serverIP = "127.0.0.1";  // Server IP address
    int serverPort = 12345;              // Server port

    sendImage(imageFile, serverIP, serverPort);

    return 0;
}
