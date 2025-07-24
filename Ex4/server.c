#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main()
{
    int sd, nsd, i = 0, port;
    printf("Enter port no.: ");
    scanf("%d", &port);
    unsigned int t;
    char c;
    char content[30];
    struct sockaddr_in ser, cli;
    if ((sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    {
        printf("Error: Socket cannot be created (server)\n");
        return 0;
    }
    bzero((char *)&ser, sizeof(ser));
    printf("The port addr. is: %d\n", port);
    ser.sin_family = AF_INET;
    ser.sin_port = htons(port);
    ser.sin_addr.s_addr = INADDR_ANY;
    if (bind(sd, (struct sockaddr *)&ser, sizeof(ser)) == -1)
    {
        printf("Error: Binding problem (port busy)\n");
        return 0;
    }
    listen(sd, 1);
    printf("\t\tServer\n");
    t = sizeof(cli);
    nsd = accept(sd, (struct sockaddr *)&cli, &t);
    if (nsd == -1)
    {
        printf("Error in accepting client\n");
        return 0;
    }
    printf("Client accepted!\n");
    while (read(nsd, &c, 1))
    {
        write(nsd, &c, 1);
    }
    close(nsd);
    close(sd);
    return 0;
}