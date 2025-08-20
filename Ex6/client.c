#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0)
    {
        perror("inet_pton failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Send a message to the server
    printf("Enter message: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    ssize_t bytes_sent = sendto(sockfd, buffer, strlen(buffer), 0,
                                (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (bytes_sent < 0)
    {
        perror("sendto failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    // Receive the server's response (the echo)
    socklen_t addr_len = sizeof(server_addr);
    ssize_t bytes_received = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &addr_len);
    if (bytes_received < 0)
    {
        perror("recvfrom failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    buffer[bytes_received] = '\0'; // Null-terminate the received data
    printf("Received from server: %s\n", buffer);

    // Close the socket
    close(sockfd);
    return 0;
}
