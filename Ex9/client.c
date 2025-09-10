#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"

int main()
{
    int port;
    printf("Enter the server port number: ");
    scanf("%d", &port);
    getchar(); // Consume newline left by scanf

    int cs;
    struct sockaddr_in ser;
    char buf[BUFFER_SIZE];
    char msg[BUFFER_SIZE];
    int n;

    // Create socket
    cs = socket(AF_INET, SOCK_STREAM, 0);
    if (cs == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&ser, 0, sizeof(ser));
    ser.sin_family = AF_INET;
    ser.sin_port = htons(port);

    if (inet_pton(AF_INET, SERVER_IP, &ser.sin_addr) <= 0)
    {
        perror("Invalid server address");
        close(cs);
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(cs, (struct sockaddr *)&ser, sizeof(ser)) == -1)
    {
        perror("Connection failed");
        close(cs);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server at %s:%d\n", SERVER_IP, port);

    // Receive welcome message
    n = recv(cs, buf, BUFFER_SIZE - 1, 0);
    if (n > 0)
    {
        buf[n] = '\0';
        printf("%s", buf);
    }

    // Client communication loop
    while (1)
    {
        printf("Enter message (type 'quit' to exit): ");

        if (fgets(msg, BUFFER_SIZE, stdin) == NULL)
        {
            break;
        }

        // Remove newline character if present
        if (msg[strlen(msg) - 1] == '\n')
        {
            msg[strlen(msg) - 1] = '\0';
        }

        // Send message to server
        if (send(cs, msg, strlen(msg), 0) == -1)
        {
            perror("Send failed");
            break;
        }

        // Check if user wants to quit
        if (strcmp(msg, "quit") == 0 || strcmp(msg, "exit") == 0)
        {
            // Receive goodbye message
            n = recv(cs, buf, BUFFER_SIZE - 1, 0);
            if (n > 0)
            {
                buf[n] = '\0';
                printf("Server: %s", buf);
            }
            break;
        }

        // Receive response from server
        memset(buf, 0, BUFFER_SIZE);
        n = recv(cs, buf, BUFFER_SIZE - 1, 0);

        if (n <= 0)
        {
            printf("Server disconnected.\n");
            break;
        }

        buf[n] = '\0';
        printf("Server: %s", buf);
    }

    close(cs);
    printf("Disconnected from server.\n");
    return 0;
}
