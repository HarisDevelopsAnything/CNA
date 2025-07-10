#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char *bin = (char *)malloc(sizeof(char) * 100);
    int ch;
    printf("Enter the binary data: ");
    scanf("%s", bin);
    printf("1) Parity check\n2) Parity generator\nEnter your choice: ");
    scanf("%d", &ch);
    for (int i = 0; i < strlen(bin); i++)
    {
        if (bin[i] != '0' && bin[i] != '1')
        {
            printf("Invalid binary string.\n");
            exit(0);
        }
    }
    int n = 0;
    for (int i = 0; i < strlen(bin); i++)
    {
        n = bin[i] == '1' ? n + 1 : n;
    }
    switch (ch)
    {
    case 1:
        printf("Parity: %s\n", n % 2 == 0 ? "Even" : "Odd");
        break;
    case 2:
        int ch1;
        printf("1) Even parity 2) Odd parity\nEnter your choice: ");
        scanf("%d", &ch1);
        switch (ch1)
        {
        case 1:
            if (n % 2 == 0)
            {
                strncat(bin, "0", 1);
            }
            else
            {
                strncat(bin, "1", 1);
            }
            break;
        case 2:
            if (n % 2 == 0)
            {
                strncat(bin, "1", 1);
            }
            else
            {
                strncat(bin, "0", 1);
            }
            break;
        }
        printf("Generated string: %s\n", bin);
    }
}