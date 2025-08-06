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
    char *iptable[] = {
        "192.168.0.1",
        "192.168.0.2",
        "192.168.0.3",
        "192.168.0.4",
        "192.168.0.5",
        "192.168.0.6",
        "192.168.0.7",
        "192.168.0.8",
        "192.168.0.9",
        "192.168.1.0",
        "192.168.1.1",
        "192.168.1.2",
        "192.168.1.3",
        "192.168.1.4",
        "192.168.1.5",
        "192.168.1.6",
        "192.168.1.7",
        "192.168.1.8",
        "192.168.1.9",
        "192.168.2.0",
    };
    char *mactable[] = {
        "A1:12:AB:FB:5C:9B",
        "B2:23:BC:FC:6D:AC",
        "C3:34:CD:FD:7E:BD",
        "D4:45:DE:FE:8F:CE",
        "E5:56:EF:AF:90:DF",
        "F6:67:F0:B0:A1:E0",
        "A7:78:A1:C1:B2:F1",
        "B8:89:B2:D2:C3:A2",
        "C9:9A:C3:E3:D4:B3",
        "DA:AB:D4:F4:E5:C4",
        "EB:BC:E5:A5:F6:D5",
        "FC:CD:F6:B6:A7:E6",
        "AD:DE:A7:C7:B8:F7",
        "BE:EF:B8:D8:C9:A8",
        "CF:F0:C9:E9:DA:B9",
        "D0:A1:DA:FA:EB:CA",
        "E1:B2:EB:AB:FC:DB",
        "F2:C3:FC:BC:AD:EC",
        "A3:D4:AD:CD:BE:FD",
        "B4:E5:BE:DE:CF:AE"};
    printf("Enter port no.: ");
    scanf("%d", &port);
    unsigned int t;
    char *c = (char *)malloc(sizeof(char) * 16);
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
    while (read(nsd, c, sizeof(char) * 18))
    {

        c[strcspn(c, "\r\n")] = 0;
        printf("Received MAC: %s\n", c);
        int index;
        for (index = 0; index < 20; index++)
        {
            if (strcmp(mactable[index], c) == 0)
            {
                break;
            }
        }
        if (index < 20)
        {
            write(nsd, iptable[index], strlen(iptable[index]) + 1);
            printf("Sent IP: %s\n", iptable[index]);
        }
        else
        {
            char not_found[] = "IP_NOT_FOUND";
            write(nsd, not_found, strlen(not_found) + 1);
            printf("IP address not found for MAC: %s\n", c);
        }
    }
    close(nsd);
    close(sd);
    return 0;
}