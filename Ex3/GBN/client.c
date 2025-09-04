#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define MAX 100
#define WINDOW_SIZE 4
#define TIMEOUT_SEC 5

typedef struct
{
    int seq_no;
    char data;
    int ack;
    int error_flag; // Added to simulate errors
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

    // Get error frame specification from user
    int error_frame = -1;
    printf("Enter frame number to simulate error (-1 for no error): ");
    scanf("%d", &error_frame);

    int base = 0, next_seq = 0;
    int len = strlen(buffer);
    if (buffer[len - 1] == '\n')
        buffer[len - 1] = '\0';

    printf("\n=== Starting Go-Back-N transmission ===\n");
    printf("Window Size: %d\n", WINDOW_SIZE);
    printf("Message length: %lu\n", strlen(buffer));
    if (error_frame >= 0)
        printf("Error will be simulated at frame %d\n\n", error_frame);
    else
        printf("No error simulation\n\n");

    int retransmission_count = 0;
    int error_simulated = 0; // Flag to track if error was already simulated
    int max_attempts = 100; // Prevent infinite loops
    int attempts = 0;

    while (base < strlen(buffer) && attempts < max_attempts)
    {
        attempts++;
        printf("=== Attempt %d - Window [%d - %d] ===\n", attempts, base, base + WINDOW_SIZE - 1);
        
        // Send up to WINDOW_SIZE frames
        while (next_seq < base + WINDOW_SIZE && next_seq < strlen(buffer))
        {
            Frame f = {next_seq, buffer[next_seq], 0, 0};
            
            // Check if this frame should have an error (only simulate once)
            if (next_seq == error_frame && !error_simulated)
            {
                f.error_flag = 1;
                error_simulated = 1;
                printf("SIMULATING ERROR: Frame %d will be corrupted\n", next_seq);
            }
            
            write(sd, &f, sizeof(Frame));
            printf("Sent Frame: Seq = %d, Data = '%c'%s\n", 
                   f.seq_no, f.data, f.error_flag ? " [ERROR SIMULATED]" : "");
            next_seq++;
        }

        // Wait for ACK
        Frame ack_frame;
        int bytes_read = read(sd, &ack_frame, sizeof(Frame));
        
        if (bytes_read <= 0)
        {
            printf("Connection closed or error occurred\n");
            break;
        }

        if (ack_frame.ack == -1) // NACK received
        {
            printf("NACK received for frame %d - Error detected!\n", ack_frame.seq_no);
            printf("Go-Back-N: Retransmitting from frame %d\n", base);
            next_seq = base; // Reset to retransmit from base
            retransmission_count++;
            
            // Prevent too many retransmissions
            if (retransmission_count > 10) {
                printf("Too many retransmissions. Aborting.\n");
                break;
            }
        }
        else
        {
            printf("ACK received for frame %d\n", ack_frame.seq_no);
            if (ack_frame.seq_no >= base)
            {
                base = ack_frame.seq_no + 1;
                printf("Window slides to base = %d\n", base);
            }
            else
            {
                // Duplicate ACK - continue without retransmitting
                printf("Duplicate ACK received for frame %d\n", ack_frame.seq_no);
            }
        }
        printf("\n");
    }

    printf("Transmission completed!\n");
    printf("Total retransmissions: %d\n", retransmission_count);

    close(sd);
    return 0;
}
