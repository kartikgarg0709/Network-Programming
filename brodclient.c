#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in listen_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(listen_addr);

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Allow socket to reuse address
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Failed to set socket option");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Configure address to listen on
    memset(&listen_addr, 0, sizeof(listen_addr));
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_port = htons(PORT);
    listen_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(sockfd, (struct sockaddr *)&listen_addr, sizeof(listen_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Broadcast client listening on port %d...\n", PORT);

    while (1) {
        // Receive broadcast message
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&listen_addr, &addr_len);
        if (n < 0) {
            perror("Failed to receive message");
            break;
        }

        buffer[n] = '\0'; // Null-terminate received message
        printf("Received broadcast message: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
