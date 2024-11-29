#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Server address setup
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // Any incoming interface
    server_addr.sin_port = htons(PORT);

    // Bind socket to the port
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    addr_len = sizeof(client_addr);

    printf("UDP server is running on port %d...\n", PORT);

    while (1) {
        // Receive message
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, 
                        (struct sockaddr *)&client_addr, &addr_len);
        buffer[n] = '\0'; // Null-terminate received data
        printf("Client: %s\n", buffer);

        // Send response
        char *message = "Message received";
        sendto(sockfd, message, strlen(message), 0, 
               (struct sockaddr *)&client_addr, addr_len);
    }

    close(sockfd);
    return 0;
}
