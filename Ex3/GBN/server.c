#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX 10

typedef struct
{
    int seq_no;
    char data;
    int ack;
} Frame;

int main()
{
    int sd, nsd, port;
    socklen_t t;
    struct sockaddr_in ser, cli;

    printf("Enter port no.: ");
    scanf("%d", &port);

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket error");
        return 1;
    }

    memset(&ser, 0, sizeof(ser));
    ser.sin_family = AF_INET;
    ser.sin_port = htons(port);
    ser.sin_addr.s_addr = INADDR_ANY;

    if (bind(sd, (struct sockaddr *)&ser, sizeof(ser)) == -1)
    {
        perror("Bind error");
        return 1;
    }

    listen(sd, 1);
    printf("Server waiting...\n");

    t = sizeof(cli);
    nsd = accept(sd, (struct sockaddr *)&cli, &t);
    if (nsd == -1)
    {
        perror("Accept error");
        return 1;
    }

    int expected_seq = 0;
    Frame f;

    while (read(nsd, &f, sizeof(Frame)))
    {
        if (f.seq_no == expected_seq)
        {
            printf("Received Frame: Seq = %d, Data = %c\n", f.seq_no, f.data);
            expected_seq++;
        }
        else
        {
            printf("Out of order frame. Expected %d but got %d. Discarded.\n", expected_seq, f.seq_no);
        }

        // Send ACK (always ack the last correct frame)
        Frame ack_frame;
        ack_frame.ack = 1;
        ack_frame.seq_no = expected_seq - 1; // last accepted
        write(nsd, &ack_frame, sizeof(Frame));
    }

    close(nsd);
    close(sd);
    return 0;
}
