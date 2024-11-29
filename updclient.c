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
    struct sockaddr_in server_addr;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Server address setup
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    socklen_t addr_len = sizeof(server_addr);

    while (1) {
        // Input message
        printf("Enter message to server: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Remove trailing newline
        buffer[strcspn(buffer, "\n")] = '\0';

        // Send message
        sendto(sockfd, buffer, strlen(buffer), 0, 
               (const struct sockaddr *)&server_addr, addr_len);

        // Receive response
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, 
                         (struct sockaddr *)&server_addr, &addr_len);
        buffer[n] = '\0'; // Null-terminate received data
        printf("Server: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
