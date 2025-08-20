#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main()
{
    int port;
    printf("Enter the port no.: ");
    scanf("%d", &port);
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(client_addr);

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
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the address and port
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP Echo Server listening on port %d...\n", port);

    // Receive data from the client
    while (1)
    {
        ssize_t bytes_received = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                                          (struct sockaddr *)&client_addr, &addr_len);
        if (bytes_received < 0)
        {
            perror("recvfrom failed");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        buffer[bytes_received] = '\0'; // Null-terminate the received data
        printf("Received from client: %s\n", buffer);

        // Send the received data back to the client (echo)
        ssize_t bytes_sent = sendto(sockfd, buffer, bytes_received, 0,
                                    (struct sockaddr *)&client_addr, addr_len);
        if (bytes_sent < 0)
        {
            perror("sendto failed");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        printf("Echoed message back to client.\n");
    }

    // Close the socket (this won't be reached in the infinite loop above)
    close(sockfd);
    return 0;
}