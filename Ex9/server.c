#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 5

// Function to handle client communication
void handle_client(int cs, struct sockaddr_in cli)
{
    char buf[BUFFER_SIZE];
    char res[BUFFER_SIZE];
    int n;

    printf("Client connected from %s:%d\n",
           inet_ntoa(cli.sin_addr),
           ntohs(cli.sin_port));

    // Send welcome message
    strcpy(res, "Welcome to the Iterative Server! Type 'quit' to exit.\n");
    send(cs, res, strlen(res), 0);

    while (1)
    {
        memset(buf, 0, BUFFER_SIZE);

        // Receive message from client
        n = recv(cs, buf, BUFFER_SIZE - 1, 0);

        if (n <= 0)
        {
            printf("Client disconnected.\n");
            break;
        }

        buf[n] = '\0';

        // Remove newline character if present
        if (buf[strlen(buf) - 1] == '\n')
        {
            buf[strlen(buf) - 1] = '\0';
        }

        printf("Received from client: %s\n", buf);

        // Check if client wants to quit
        if (strcmp(buf, "quit") == 0 || strcmp(buf, "exit") == 0)
        {
            strcpy(res, "Goodbye!\n");
            send(cs, res, strlen(res), 0);
            printf("Client requested to quit.\n");
            break;
        }

        // Process the message (simple echo with timestamp)
        snprintf(res, BUFFER_SIZE, "Server received: %s\n", buf);

        // Send response back to client
        if (send(cs, res, strlen(res), 0) == -1)
        {
            perror("Send failed");
            break;
        }

        printf("Sent response to client: %s", res);
    }

    close(cs);
    printf("Connection with client closed.\n\n");
}

int main()
{
    int port;
    printf("Enter the port number: ");
    scanf("%d", &port);

    int ss, cs;
    struct sockaddr_in ser, cli;
    socklen_t len = sizeof(cli);

    // Create socket
    ss = socket(AF_INET, SOCK_STREAM, 0);
    if (ss == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options to reuse address
    int opt = 1;
    if (setsockopt(ss, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("Setsockopt failed");
        close(ss);
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&ser, 0, sizeof(ser));
    ser.sin_family = AF_INET;
    ser.sin_addr.s_addr = INADDR_ANY;
    ser.sin_port = htons(port);

    // Bind socket
    if (bind(ss, (struct sockaddr *)&ser, sizeof(ser)) == -1)
    {
        perror("Bind failed");
        close(ss);
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(ss, MAX_CLIENTS) == -1)
    {
        perror("Listen failed");
        close(ss);
        exit(EXIT_FAILURE);
    }

    printf("=== Iterative Server Started ===\n");
    printf("Server listening on port %d...\n", port);
    printf("Waiting for clients to connect...\n\n");

    // Iterative server loop - handle one client at a time
    while (1)
    {
        printf("Waiting for next client connection...\n");

        // Accept client connection
        cs = accept(ss, (struct sockaddr *)&cli, &len);
        if (cs == -1)
        {
            perror("Accept failed");
            continue; // Continue to next iteration instead of exiting
        }

        // Handle client (this blocks until client disconnects)
        handle_client(cs, cli);

        // After client disconnects, server is ready for next client
        printf("Ready for next client...\n");
    }

    close(ss);
    return 0;
}