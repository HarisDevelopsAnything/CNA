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

    char *dn[10] = {"www.google.com", "www.youtube.com", "mail.google.com", "erp.mepcoeng.ac.in", "www.mepcoeng.ac.in", "www.facebook.com", "www.twitter.com", "www.linkedin.com", "www.github.com", "www.stackoverflow.com"};
    char *ip[10] = {"142.250.190.14", "142.250.190.206", "64.233.177.188", "172.16.16.", "103.21.244.0", "157.240.22.35", "104.244.42.1", "108.177.125.101", "140.82.121.3", "151.101.65.69"};
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

        // Remove any trailing newline or carriage return characters
        buffer[strcspn(buffer, "\r\n")] = '\0';

        printf("Received domain: '%s' (length: %zu)\n", buffer, strlen(buffer));
        // Check if the received domain is in the list
        if (strcmp(buffer, "exit") == 0)
        {
            printf("Exiting server...\n");
            break;
        }
        printf("Looking up IP address for domain: %s\n", buffer);
        // Check if the domain is in the predefined list
        if (strlen(buffer) == 0)
        {
            printf("No domain provided.\n");
            continue;
        }
        // If the domain is not found, send an error message
        int found = 0;
        // Look up the IP address for the received domain
        char *ip_address = NULL;
        for (int i = 0; i < 10; i++)
        {
            printf("Comparing '%s' with '%s'\n", buffer, dn[i]);
            if (strcmp(buffer, dn[i]) == 0)
            {
                ip_address = ip[i];
                found = 1;
                printf("Found IP address: %s\n", ip_address);
                break;
            }
        }

        // Send the IP address back to the client
        if (found)
        {
            ssize_t bytes_sent = sendto(sockfd, ip_address, strlen(ip_address), 0,
                                        (struct sockaddr *)&client_addr, addr_len);
            if (bytes_sent < 0)
            {
                perror("sendto failed");
                close(sockfd);
                exit(EXIT_FAILURE);
            }
            printf("Sent IP address: %s to client.\n", ip_address);
        }
        else
        {
            const char *error_message = "Domain not found";
            ssize_t bytes_sent = sendto(sockfd, error_message, strlen(error_message), 0,
                                        (struct sockaddr *)&client_addr, addr_len);
            if (bytes_sent < 0)
            {
                perror("sendto failed");
                close(sockfd);
                exit(EXIT_FAILURE);
            }
            printf("Sent error message to client: %s\n", error_message);
        }
    }

    // Close the socket (this won't be reached in the infinite loop above)
    close(sockfd);
    return 0;
}