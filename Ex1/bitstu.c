#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main()
{
    char *bs = (char *)malloc(sizeof(char) * 100);
    printf("Enter the bit string: ");
    scanf("%s", bs);
    char *trf = (char *)malloc(sizeof(char) * 100);
    char *flag = "01111110";
    int k = 0;
    for (int i = 0; i < 8; i++)
    {
        trf[i] = flag[i];
    }
    for (int i = 8; i < strlen(bs) + 8; i++)
    {
        trf[i] = bs[i - 8];
    }
    for (int i = strlen(bs) + 8; i < strlen(bs) + 16; i++)
    {
        trf[i] = flag[i - strlen(bs) - 8];
    }
    printf("%s", trf);
    // stuffing
    int match = 0;
    for (int i = 8; i < strlen(trf) - 8; i++)
    {
        if ('1' == trf[i])
        {
            ++match;
            if (match == 5)
            {
                int l = strlen(trf);
                while (l > i + 2)
                {
                    trf[l + 1] = trf[l];
                    l--;
                }
                trf[i + 2] = '0';
                match = 0;
            }
        }
    }
    printf("Stuffed string: %s\n", trf);
}