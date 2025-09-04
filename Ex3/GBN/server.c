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
    int error_flag; // Added to simulate errors
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

    // Allow socket reuse
    int opt = 1;
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    listen(sd, 1);
    printf("Server waiting...\n");

    t = sizeof(cli);
    nsd = accept(sd, (struct sockaddr *)&cli, &t);
    if (nsd == -1)
    {
        perror("Accept error");
        return 1;
    }

    printf("Client connected!\n");
    printf("Receiving frames using Go-Back-N protocol...\n\n");

    int expected_seq = 0;
    char received_message[MAX] = {0};
    Frame f;
    int message_complete = 0;

    while (!message_complete && read(nsd, &f, sizeof(Frame)))
    {
        printf("Received Frame: Seq = %d, Data = '%c'", f.seq_no, f.data);

        Frame ack_frame;
        ack_frame.data = 0;
        ack_frame.error_flag = 0;
        
        // Check if frame has error flag set
        if (f.error_flag == 1)
        {
            printf(" [ERROR DETECTED - CORRUPTED FRAME]\n");
            // Send NACK
            ack_frame.seq_no = f.seq_no;
            ack_frame.ack = -1; // NACK
            
            write(nsd, &ack_frame, sizeof(Frame));
            printf("Sent NACK for frame %d\n\n", f.seq_no);
            continue;
        }

        // Check if this is the expected frame
        if (f.seq_no == expected_seq)
        {
            printf(" [ACCEPTED]\n");
            received_message[expected_seq] = f.data;
            
            // Send ACK for this frame
            ack_frame.seq_no = f.seq_no;
            ack_frame.ack = 1; // ACK
            
            write(nsd, &ack_frame, sizeof(Frame));
            printf("Sent ACK for frame %d\n", f.seq_no);
            
            // Check if message is complete (ends with '*')
            if (f.data == '*')
            {
                printf("\nMessage received completely: %s\n", received_message);
                message_complete = 1;
                break;
            }
            
            expected_seq++;
        }
        else
        {
            printf(" [OUT OF ORDER - DISCARDED]\n");
            // Send ACK for the last correctly received frame
            if (expected_seq > 0)
            {
                ack_frame.seq_no = expected_seq - 1;
                ack_frame.ack = 1; // ACK
                
                write(nsd, &ack_frame, sizeof(Frame));
                printf("Sent duplicate ACK for frame %d\n", expected_seq - 1);
            }
            else
            {
                // If no frames have been accepted yet, send NACK
                ack_frame.seq_no = f.seq_no;
                ack_frame.ack = -1; // NACK
                
                write(nsd, &ack_frame, sizeof(Frame));
                printf("Sent NACK for frame %d (first frame expected is 0)\n", f.seq_no);
            }
        }
        printf("Expected next frame: %d\n\n", expected_seq);
    }

    close(nsd);
    close(sd);
    return 0;
}
