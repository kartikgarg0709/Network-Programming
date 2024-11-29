#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BROADCAST_IP "255.255.255.255"
#define MESSAGE "Broadcast message from server"
#define INTERVAL 5 // Seconds between broadcasts

int main() {
    int sockfd;
    struct sockaddr_in broadcast_addr;
    int broadcast_enable = 1;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Enable broadcasting
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable)) < 0) {
        perror("Failed to set broadcast option");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Configure broadcast address
    memset(&broadcast_addr, 0, sizeof(broadcast_addr));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(PORT);
    broadcast_addr.sin_addr.s_addr = inet_addr(BROADCAST_IP);

    printf("Broadcast server running on port %d...\n", PORT);

    while (1) {
        // Send broadcast message
        if (sendto(sockfd, MESSAGE, strlen(MESSAGE), 0, (struct sockaddr *)&broadcast_addr, sizeof(broadcast_addr)) < 0) {
            perror("Failed to send broadcast message");
            break;
        }
        printf("Broadcast message sent: %s\n", MESSAGE);

        sleep(INTERVAL); // Wait before next broadcast
    }

    close(sockfd);
    return 0;
}
