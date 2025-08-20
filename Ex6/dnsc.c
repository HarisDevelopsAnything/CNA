#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main()
{
    int port;
    printf("Enter the port no.: ");
    scanf("%d", &port);
    getchar(); // Consume the newline left by scanf
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
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0)
    {
        perror("inet_pton failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Send a message to the server
    while (1)
    {
        printf("Enter domain name: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Remove the newline character from the input
        buffer[strcspn(buffer, "\n")] = '\0';

        ssize_t bytes_sent = sendto(sockfd, buffer, strlen(buffer), 0,
                                    (struct sockaddr *)&server_addr, sizeof(server_addr));

        if (bytes_sent < 0)
        {
            perror("sendto failed");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        if (strcmp(buffer, "exit") == 0)
        {
            printf("Exiting client...\n");
            break;
        }
        // Receive the server's response (the IP address)
        socklen_t addr_len = sizeof(server_addr);
        ssize_t bytes_received = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &addr_len);
        if (bytes_received < 0)
        {
            perror("recvfrom failed");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        printf("IP Address: %s\n", buffer);
    }
    // Close the socket
    close(sockfd);
    return 0;
}
