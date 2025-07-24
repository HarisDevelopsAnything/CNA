#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
    realloc(bin, sizeof(char) * strlen(bin));
    int par = parityReq(bin);
    printf("%d parity bits needed.\n", par);
    char *withpar = (char *)malloc(sizeof(char) * (strlen(bin) + par));
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
    printf("After adding parity bits: %s\n", withpar);
    int trl = 0;
    for (int i = 0; i < strlen(withpar); i++)
    {
        if (withpar[i] == 'p')
        {
            trl = pow(2, i);
            int cnt = 0;
            int ci = trl - 1;
            while (ci < strlen(withpar))
            {
                printf("Checking %d\n", ci);
                if (withpar[ci] == '1')
                {
                    cnt++;
                }
                trl--;
                if (trl == 0)
                {
                    ci += pow(2, i) + 1;
                    trl = pow(2, i);
                }
                else
                {
                    ci++;
                }
            }
            if (cnt % 2 == 0)
            {
                withpar[i] = '0';
            }
            else
                withpar[i] = '1';
        }
    }
    printf("The hamming code is: %s\n", withpar);
    // verify
    withpar = "1101110";
    char *parityCheck = (char *)malloc(sizeof(char) * par);
    int k = pbp - 1;
    printf("Verifying hamming code:\n");
    pbp = 0;
    for (int i = 0; i < strlen(withpar); i++)
    {
        printf("In loop\n");
        if (i == pow(2, pbp) - 1)
        {
            printf("Parity bit at pos %d\n", i);
            pbp++;
            trl = pow(2, pbp);
            int cnt = 0;
            int ci = trl - 2;
            while (ci < strlen(withpar))
            {
                printf("Checking pos %d for parity bit %d\n", ci, i);
                if (withpar[ci] == '1')
                {
                    cnt++;
                }
                trl--;
                if (trl == 0)
                {
                    ci += pow(2, i) + 1;
                    trl = pow(2, i);
                }
                else
                {
                    ci++;
                }
            }
            if (cnt % 2 == 0)
            {
                parityCheck[k] = '1';
                k--;
            }
            else
            {
                parityCheck[k] = '0';
                k--;
            }
        }
    }
    printf("Parity check: %s\n", parityCheck);
}