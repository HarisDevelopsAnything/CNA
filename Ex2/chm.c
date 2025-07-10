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
char *decToBin(int dec)
{
    char *binrev = (char *)malloc(sizeof(char) * 100);
    char *bin = (char *)malloc(sizeof(char) * 100);
    int i = 0;
    while (dec != 0)
    {
        int rem = dec % 2;
        binrev[i] = rem + '0';
        i++;
        dec /= 2;
    }
    for (int j = 0; j < i; j++)
    {
        bin[j] = binrev[i - j - 1];
    }
    return bin;
}
char *onesComp(char *bin)
{
    char *onc = (char *)malloc(sizeof(char) * strlen(bin));
    for (int i = 0; i < strlen(bin); i++)
    {
        onc[i] = bin[i] == '1' ? '0' : '1';
    }
    return onc;
}
char *binSum(char *bin1, char *bin2)
{
    int dec1 = binToDec(bin1);
    int dec2 = binToDec(bin2);
    int sum = dec1 + dec2;
    return decToBin(sum);
}
char *checksumGen(char *bin, int width)
{
    char *carrybin = (char *)malloc(sizeof(char) * (strlen(bin) - width));
    for (int i = 0; i < strlen(bin) - width; i++)
    {
        carrybin[i] = bin[i];
    }
    char *nocbin = (char *)malloc(sizeof(char) * width);
    for (int i = 0; i < width; i++)
    {
        nocbin[i] = carrybin[i + strlen(carrybin)];
    }
    return onesComp(binSum(carrybin, nocbin));
}
int main()
{
    char *inp = (char *)malloc(sizeof(char) * 100);
    printf("Enter the bit string: ");
    scanf("%s", inp);
    int rem = strlen(inp) % 8;
    if (rem != 0)
    {
        int pad = 8 - rem;
        char padding[pad];
        for (int i = 0; i < pad; i++)
        {
            padding[i] = '0';
        }
        strcat(inp, padding);
    }
    char **segments = (char **)malloc(sizeof(char *) * (strlen(inp) / 8));
    int k = 0;
    for (int i = 0; i < strlen(inp) / 8; i++)
    {
        segments[i] = (char *)malloc(sizeof(char) * 8);
        for (int j = 0; j < 8; j++)
        {
            segments[i][j] = inp[k];
            k++;
        }
        printf("%s\n", segments[i]);
    }
    int sum = 0;
    for (int i = 0; i < strlen(inp) / 8; i++)
    {
        int dec = binToDec(segments[i]);
        printf("Decimal of %s = %d\n", segments[i], dec);
        sum += dec;
    }
    printf("After sum of %d\n", sum);
    char *rawsum = (char *)malloc(sizeof(char) * 100);
    char *checksum = (char *)malloc(sizeof(char) * 8);
    printf("after checksum init\n");
    rawsum = decToBin(sum);
    checksum = checksumGen(rawsum, 8);
    printf("%s\n", checksum);
}