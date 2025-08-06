#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX 100
#define WINDOW_SIZE 4

typedef struct
{
    int seq_no;
    char data;
    int ack;
} Frame;

int main()
{
    int sd, port;
    struct sockaddr_in ser;
    printf("Enter port no.: ");
    scanf("%d", &port);
    getchar(); // consume newline

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket error");
        return 1;
    }

    memset(&ser, 0, sizeof(ser));
    ser.sin_family = AF_INET;
    ser.sin_port = htons(port);
    ser.sin_addr.s_addr = INADDR_ANY;

    if (connect(sd, (struct sockaddr *)&ser, sizeof(ser)) == -1)
    {
        perror("Connect error");
        return 1;
    }

    printf("Enter message (end with *): ");
    char buffer[MAX];
    fgets(buffer, MAX, stdin);

    int base = 0, next_seq = 0;
    int len = strlen(buffer);
    if (buffer[len - 1] == '\n')
        buffer[len - 1] = '\0';

    while (base < strlen(buffer))
    {
        // Send up to WINDOW_SIZE frames
        while (next_seq < base + WINDOW_SIZE && next_seq < strlen(buffer))
        {
            Frame f = {next_seq, buffer[next_seq], 0};
            write(sd, &f, sizeof(Frame));
            printf("Sent Frame: Seq = %d, Data = %c\n", f.seq_no, f.data);
            next_seq++;
        }

        // Wait for ACK
        Frame ack_frame;
        read(sd, &ack_frame, sizeof(Frame));
        printf("Received ACK for %d\n", ack_frame.seq_no);

        if (ack_frame.seq_no >= base)
        {
            base = ack_frame.seq_no + 1;
        }
        else
        {
            // Go-Back-N: reset next_seq to base
            next_seq = base;
        }
    }

    close(sd);
    return 0;
}
