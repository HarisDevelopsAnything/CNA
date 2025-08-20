#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    printf("Enter the port no.: ");
    int port;
    scanf("%d", &port);
    int sd, nsd;
    sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd == -1)
    {
        perror("Error creating socket!\n");
        exit(0);
    }
    struct sockaddr_in ser, cli;
    memset(&ser, 0, sizeof(ser));
    ser.sin_family = AF_INET;
    ser.sin_port = htons(port);
    ser.sin_addr.s_addr = INADDR_ANY;
    if (bind(sd, (struct sockaddr *)&ser, sizeof(ser)) == -1)
    {
        perror("Binding problem (port busy)\n");
        exit(0);
    }
    listen(sd, 1);
    socklen_t cl = sizeof(cli);
    nsd = accept(sd, (struct sockaddr *)&cli, &cl);
    if (nsd == -1)
    {
        perror("Error accepting client!\n");
        exit(0);
    }
    printf("Client accepted!\n");
    char *fname = (char *)malloc(sizeof(char) * 100);
    read(nsd, fname, sizeof(char) * 100);
    FILE *fp;
    int rep = 1;
    if ((fp = fopen(fname, "r")) == NULL)
    {
        perror("Error opening file!");
        rep = 0;
        write(nsd, &rep, sizeof(int));
        close(sd);
        close(nsd);
        exit(0);
    }
    write(nsd, &rep, sizeof(int));
    printf("File %s sending...\n", fname);
    char c;
    while ((c = fgetc(fp)) != EOF)
    {
        printf("writing %c\n", c);
        write(nsd, &c, sizeof(c));
    }
    write(nsd, &c, 1);
    printf("Sent a file!\n");
    fclose(fp);
    close(sd);
    close(nsd);
}