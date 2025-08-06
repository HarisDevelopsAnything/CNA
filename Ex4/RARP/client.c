#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main()
{
    int sd, nsd, i = 0, port;
    printf("Enter port no.: ");
    scanf("%d", &port);
    char c, s;
    char content[30] = "\0";
    struct sockaddr_in ser;
    if ((sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    {
        printf("Error: Socket cannot be created (client)\n");
        return 0;
    }
    bzero((char *)&ser, sizeof(ser));
    printf("The port addr. is: %d\n", port);
    ser.sin_family = AF_INET;
    ser.sin_port = htons(port);
    ser.sin_addr.s_addr = INADDR_ANY;
    if (connect(sd, (struct sockaddr *)&ser, sizeof(ser)) == -1)
    {
        printf("Error: Connecting client problem (port busy)\n");
        return 0;
    }
    printf("This is client\n");
    printf("Enter the IP: ");
    char *ip = (char *)malloc(sizeof(char) * 16);
    char *mac = (char *)malloc(sizeof(char) * 18);
    while (strcmp(fgets(mac, sizeof(char) * 18, stdin), "exit") != 0)
    {
        write(sd, mac, sizeof(char) * 18);
        read(sd, ip, sizeof(char) * 16);
        printf("Received IP: ");
        puts(mac);
    }
    close(sd);
    return 0;
}