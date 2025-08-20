#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

int main()
{
    printf("Enter port no.: ");
    int port;
    scanf("%d", &port);
    int sd;
    sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd == -1)
    {
        perror("Socket error!\n");
        exit(0);
    }
    // getchar();
    struct sockaddr_in ser;
    memset(&ser, 0, sizeof(ser));
    ser.sin_family = AF_INET;
    ser.sin_port = htons(port);
    ser.sin_addr.s_addr = INADDR_ANY;
    if (connect(sd, (struct sockaddr *)&ser, sizeof(ser)) == -1)
    {
        perror("Connection failed!\n");
        exit(0);
    }
    printf("Connected to server.\n");

    printf("Enter the filename: ");
    char *fname = (char *)malloc(sizeof(char) * 100);
    scanf("%s", fname);
    fname[strlen(fname)] = '\0';
    printf("Requesting file %s...", fname);
    write(sd, fname, sizeof(char) * strlen(fname));
    printf("Request sent!\n");
    printf("Waiting for reply...\n");
    int reply;
    read(sd, &reply, 1);
    if (reply == 0)
    {
        printf("File not found!\n");
        close(sd);
        exit(0);
    }
    printf("File found!\n");
    printf("Enter the file name to store: ");
    char *fcpname = (char *)malloc(sizeof(char) * 100);
    scanf("%s", fcpname);
    FILE *fp = fopen(fcpname, "w");
    char cur;
    while (1)
    {
        read(sd, &cur, 1);
        printf("%c", cur);
        if (cur == EOF)
        {
            printf("File received!\n");
            break;
        }
        fwrite(&cur, sizeof(char), 1, fp);
    }
    fclose(fp);
    close(sd);
    exit(0);
}