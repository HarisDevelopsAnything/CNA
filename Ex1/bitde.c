#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main()
{
    char *bs = (char *)malloc(sizeof(char) * 100);
    printf("Enter the stuffed string: ");
    scanf("%s", bs);
    // check start and end flag
    char *flag = "01111110";
    for (int i = 0; i < 8; i++)
    {
        if (flag[i] != bs[i])
        {
            printf("Start flag not found!\n");
            exit(0);
        }
    }
    for (int i = strlen(bs) - 8; i < strlen(bs); i++)
    {
        if (flag[i - strlen(bs) + 8] != bs[i])
        {
            printf("End flag not found!\n");
            exit(0);
        }
    }
    // destuff
    int match = 0;
    for (int i = 8; i < strlen(bs) - 8; i++)
    {
        if (bs[i] == '1')
        {
            match++;
            if (match == 5)
            {
                if (bs[i + 1] == '0')
                {
                    int k = strlen(bs);
                    while (k > i + 1)
                    {
                        bs[k - 1] = bs[k];
                    }
                    match = 0;
                }
            }
        }
    }
    printf("Destuffed string: %s\n", bs);
}