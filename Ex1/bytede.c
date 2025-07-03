#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char flag;
    char esc;
    printf("Enter the flag character: ");
    scanf("%c", &flag);
    fgetc(stdin);
    printf("Enter the escape character: ");
    scanf("%c", &esc);
    fgetc(stdin);
    char *str = (char *)malloc(sizeof(char) * 100);
    printf("Enter the string to destuff: ");
    scanf("%s", str);
    for (int i = 1; i < strlen(str); i++)
    {
        if (str[i] == flag || str[i] == esc)
        {
            if (str[i - 1] == esc)
            {
                int k = i - 1;
                while (k < strlen(str))
                {
                    str[k] = str[k + 1];
                    k++;
                }
            }
        }
    }
    printf("Destuffed string: %s\n", str);
}