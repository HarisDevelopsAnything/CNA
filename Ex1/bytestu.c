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
    printf("Enter the string to stuff: ");
    scanf("%s", str);
    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == flag || str[i] == esc)
        {
            int k = strlen(str);
            while (k > i)
            {
                str[k] = str[k - 1];
                k--;
            }
            str[i] = esc;
            i++;
        }
    }
    printf("Stuffed string: %s\n", str);
}