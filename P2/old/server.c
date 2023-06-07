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

void saveGrayscalePNG(const char *filename, const unsigned char *image_data, int width, int height) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        die("Failed to open file for writing");
    }

    // PNG file header
    const unsigned char png_header[] = {
        0x89, 'P', 'N', 'G', '\r', '\n', 0x1A, '\n'
    };
    fwrite(png_header, sizeof(unsigned char), sizeof(png_header), file);

    // IHDR chunk
    unsigned char ihdr_chunk[] = {
        0x00, 0x00, 0x00, 0x0D, 'I', 'H', 'D', 'R', 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    ihdr_chunk[11] = (unsigned char)(width >> 8);    // Image width
    ihdr_chunk[12] = (unsigned char)(width & 0xFF);
    ihdr_chunk[15] = (unsigned char)(height >> 8);   // Image height
    ihdr_chunk[16] = (unsigned char)(height & 0xFF);
    fwrite(ihdr_chunk, sizeof(unsigned char), sizeof(ihdr_chunk), file);

    // IDAT chunk
    unsigned char idat_chunk_header[] = {
        0x00, 0x00, 0x00, 0x05, 'I', 'D', 'A', 'T'
    };
    fwrite(idat_chunk_header, sizeof(unsigned char), sizeof(idat_chunk_header), file);

    // Compressed image data
    unsigned long compressed_data_size = (unsigned long)(width + 1) * height;
    unsigned char *compressed_data = (unsigned char *)malloc(compressed_data_size);
    if (!compressed_data) {
        fclose(file);
        die("Memory allocation failed");
    }

    unsigned long compressed_data_pos = 0;
    for (int y = 0; y < height; y++) {
        compressed_data[compressed_data_pos++] = 0x00; // Filter type: None
        for (int x = 0; x < width; x++) {
            compressed_data[compressed_data_pos++] = image_data[y * width + x];
        }
    }
    fwrite(compressed_data, sizeof(unsigned char), compressed_data_pos, file);
    free(compressed_data);

    // IEND chunk
    const unsigned char iend_chunk[] = {
        0x00, 0x00, 0x00, 0x00, 'I', 'E', 'N', 'D', 0xAE, 0x42, 0x60, 0x82
    };
    fwrite(iend_chunk, sizeof(unsigned char), sizeof(iend_chunk), file);

    fclose(file);
}

int main() {
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[MAX_BUFFER_SIZE];
    socklen_t addrLen = sizeof(clientAddr);

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        die("Failed to create socket");
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        die("Binding failed");
    }

    printf("Server is listening on port 12345...\n");

    while (1) {
        // Receive data from the client
        int bytesReceived = recvfrom(sockfd, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&clientAddr, &addrLen);
        if (bytesReceived < 0) {
            die("Error in recvfrom()");
        }

        // Process the received data (assuming it is a PNG image)
        // Add your PNG processing code here

        // Save the grayscale PNG image
        saveGrayscalePNG("man.png", (unsigned char *)buffer, bytesReceived, 1);

        printf("Received a grayscale PNG image from client: %d bytes\n", bytesReceived);
    }

    close(sockfd);
    return 0;
}
