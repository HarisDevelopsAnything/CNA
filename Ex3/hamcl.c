#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int binToDec(char *bin)
{
    int pv = strlen(bin) - 1;
    int dec = 0;
    printf("pv= %d\n", pv);
    for (int i = 0; i < strlen(bin); i++)
    {
        if (bin[i] == '1')
        {
            dec += pow(2, pv);
        }
        pv--;
    }
    return dec;
}
int parityReq(char *str)
{
    int len = strlen(str);
    for (int i = 1; i < len; i++)
    {
        if (pow(2, i) >= len + i + 1)
        {
            return i;
        }
    }
    return 0;
}

int main()
{
    char *bin = (char *)malloc(sizeof(char) * 100);
    printf("Enter the binary string: ");
    scanf("%s", bin);
    bin = realloc(bin, sizeof(char) * (strlen(bin) + 1));

    int par = parityReq(bin);
    printf("%d parity bits needed.\n", par);

    char *withpar = (char *)malloc(sizeof(char) * (strlen(bin) + par + 1));
    int pbp = 0;
    int j = 0;

    for (int i = 0; i < strlen(bin) + par; i++)
    {
        if (i == pow(2, pbp) - 1)
        {
            withpar[i] = 'p';
            pbp++;
        }
        else
        {
            withpar[i] = bin[j];
            j++;
        }
    }
    withpar[strlen(bin) + par] = '\0';
    printf("After adding parity bits: %s\n", withpar);
    pbp = 0;
    for (int i = 0; i < strlen(withpar); i++)
    {
        if (withpar[i] == 'p')
        {
            int cnt = 0;
            int step = pow(2, pbp);
            for (int ci = i; ci < strlen(withpar); ci += 2 * step)
            {
                for (int k = 0; k < step && ci + k < strlen(withpar); k++)
                {
                    if (withpar[ci + k] == '1')
                    {
                        cnt++;
                    }
                }
            }

            if (cnt % 2 == 0)
            {
                withpar[i] = '0';
            }
            else
            {
                withpar[i] = '1';
            }
            pbp++;
        }
    }
    printf("The hamming code is: %s\n", withpar);

    strcpy(withpar, "1101110");
    char *parityCheck = (char *)malloc(sizeof(char) * (par + 1));
    parityCheck[par] = '\0';

    printf("Verifying hamming code:\n");
    pbp = 0;
    int pin = par - 1;
    for (int i = 0; i < strlen(withpar); i++)
    {
        if (i == pow(2, pbp) - 1)
        {
            printf("Parity bit at pos %d\n", i);
            int cnt = 0;
            int step = pow(2, pbp);

            for (int ci = i; ci < strlen(withpar); ci += 2 * step)
            {
                for (int k = 0; k < step && ci + k < strlen(withpar); k++)
                {
                    if (withpar[ci + k] == '1')
                    {
                        cnt++;
                    }
                }
            }

            if (cnt % 2 == 0)
            {
                parityCheck[pin] = '0';
            }
            else
            {
                parityCheck[pin] = '1';
            }
            pin--;
            pbp++;
        }
    }
    printf("Parity check: %s\n", parityCheck);
    int pos = binToDec(parityCheck);
    printf("Position of error: %dth bit\n", pos);
    withpar[pos - 1] = withpar[pos - 1] == '0' ? '1' : '0';
    printf("Correct code: %s\n", withpar);
    return 0;
}