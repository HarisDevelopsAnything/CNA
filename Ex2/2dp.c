#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parityCheck(char *arr, int l, int eve)
{
    int even = 0;
    int n = 0;
    for (int i = 0; i < l; i++)
    {
        if (arr[i] == '1')
        {
            n++;
        }
    }
    if (n % 2 == 0)

        even = 1;
    return (eve ? even : !even);
}

void printMatrix(char **mat, char *rowParity, char *colParity, int rows, int cols, int cornerParity)
{
    printf("\n2D Parity Check Matrix:\n");
    printf("Data bits:\t\tRow Parity\n");

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%c ", mat[i][j]);
        }
        printf("\t\t%c\n", rowParity[i] + '0');
    }

    printf("Column Parity:\t\t");
    for (int i = 0; i < cols; i++)
    {
        printf("%c ", colParity[i] + '0');
    }
    printf("\t\t%c (Corner)\n", cornerParity + '0');
}

int detectAndCorrectError(char **mat, char *rowParity, char *colParity, int rows, int cols, int cornerParity)
{
    int errorRow = -1, errorCol = -1;

    // Check row parity
    for (int i = 0; i < rows; i++)
    {
        if (parityCheck(mat[i], cols, 1) != rowParity[i])
        {
            errorRow = i;
            printf("Row %d parity error detected!\n", i);
        }
    }

    // Check column parity
    char *currentCol = (char *)malloc(sizeof(char) * rows);
    for (int j = 0; j < cols; j++)
    {
        for (int i = 0; i < rows; i++)
        {
            currentCol[i] = mat[i][j];
        }
        if (parityCheck(currentCol, rows, 1) != colParity[j])
        {
            errorCol = j;
            printf("Column %d parity error detected!\n", j);
        }
    }
    free(currentCol);

    // Error correction
    if (errorRow != -1 && errorCol != -1)
    {
        printf("Single bit error at position (%d, %d)\n", errorRow, errorCol);
        printf("Original bit: %c\n", mat[errorRow][errorCol]);

        // Correct the error
        mat[errorRow][errorCol] = (mat[errorRow][errorCol] == '0') ? '1' : '0';

        printf("Corrected bit: %c\n", mat[errorRow][errorCol]);
        return 1; // Error corrected
    }
    else if (errorRow != -1 || errorCol != -1)
    {
        printf("Error detected but cannot be corrected (parity bit error)\n");
        return -1; // Error detected but not correctable
    }

    printf("No errors detected\n");
    return 0; // No errors
}

int main()
{
    char *bin = (char *)malloc(sizeof(char) * 100);
    printf("Enter the binary data: ");
    scanf("%s", bin);

    // Validate input
    for (int i = 0; i < strlen(bin); i++)
    {
        if (bin[i] != '0' && bin[i] != '1')
        {
            printf("Invalid binary string.\n");
            exit(0);
        }
    }

    printf("Original data: %s (%zu bits)\n", bin, strlen(bin));

    // Padding to make length divisible by 7
    int rem = strlen(bin) % 7;
    if (rem != 0)
    {
        char *padding = (char *)malloc(sizeof(char) * (7 - rem + 1));
        for (int i = 0; i < 7 - rem; i++)
        {
            padding[i] = '0';
        }
        padding[7 - rem] = '\0';
        strcat(bin, padding);
        printf("After padding: %s\n", bin);
        free(padding);
    }

    int rows = strlen(bin) / 7;
    int cols = 7;

    // Create matrix for 7-bit segments
    char **mat = (char **)malloc(sizeof(char *) * rows);
    int k = 0;
    for (int i = 0; i < rows; i++)
    {
        mat[i] = (char *)malloc(sizeof(char) * (cols + 1));
        for (int j = 0; j < cols; j++)
        {
            mat[i][j] = bin[k];
            k++;
        }
        mat[i][cols] = '\0'; // Null terminate each row
    }

    printf("\nDivided into %d segments of 7 bits each:\n", rows);
    for (int i = 0; i < rows; i++)
    {
        printf("Segment %d: %s\n", i + 1, mat[i]);
    }

    // Calculate row parity
    char *rowParity = (char *)malloc(sizeof(char) * rows);
    printf("\nRow Parity Calculation:\n");
    for (int i = 0; i < rows; i++)
    {
        rowParity[i] = parityCheck(mat[i], cols, 1);
        printf("Row %d: %s -> Parity: %d\n", i, mat[i], rowParity[i]);
    }

    // Calculate column parity
    char *colParity = (char *)malloc(sizeof(char) * cols);
    char *currentCol = (char *)malloc(sizeof(char) * rows);
    printf("\nColumn Parity Calculation:\n");
    for (int i = 0; i < cols; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            currentCol[j] = mat[j][i];
        }
        colParity[i] = parityCheck(currentCol, rows, 1);
        printf("Column %d: ", i);
        for (int j = 0; j < rows; j++)
        {
            printf("%c", currentCol[j]);
        }
        printf(" -> Parity: %d\n", colParity[i]);
    }

    // Calculate corner parity (parity of all parity bits)
    char *allParityBits = (char *)malloc(sizeof(char) * (rows + cols));
    for (int i = 0; i < rows; i++)
    {
        allParityBits[i] = rowParity[i] + '0';
    }
    for (int i = 0; i < cols; i++)
    {
        allParityBits[rows + i] = colParity[i] + '0';
    }
    int cornerParity = parityCheck(allParityBits, rows + cols, 1);

    printf("\nCorner Parity: %d\n", cornerParity);

    // Display the complete 2D parity matrix
    printMatrix(mat, rowParity, colParity, rows, cols, cornerParity);

    // Test error detection and correction
    printf("\n=== Error Detection and Correction Test ===\n");
    printf("Current matrix status:\n");
    int errorStatus = detectAndCorrectError(mat, rowParity, colParity, rows, cols, cornerParity);

    // Demonstrate error injection and correction
    if (rows > 0 && cols > 0)
    {
        printf("\n=== Demonstrating Error Injection ===\n");
        printf("Injecting error at position (0, 1)...\n");
        printf("Original bit at (0,1): %c\n", mat[0][1]);

        // Flip the bit
        mat[0][1] = (mat[0][1] == '0') ? '1' : '0';
        printf("After injecting error: %c\n", mat[0][1]);

        // Display matrix with error
        printMatrix(mat, rowParity, colParity, rows, cols, cornerParity);

        // Detect and correct the error
        printf("\nDetecting and correcting error:\n");
        detectAndCorrectError(mat, rowParity, colParity, rows, cols, cornerParity);

        // Display corrected matrix
        printf("\nMatrix after correction:\n");
        printMatrix(mat, rowParity, colParity, rows, cols, cornerParity);
    }

    // Free allocated memory
    for (int i = 0; i < rows; i++)
    {
        free(mat[i]);
    }
    free(mat);
    free(bin);
    free(rowParity);
    free(colParity);
    free(currentCol);
    free(allParityBits);

    return 0;
}