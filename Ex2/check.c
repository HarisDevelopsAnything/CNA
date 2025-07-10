#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to calculate XOR checksum of a byte array
unsigned char xor_checksum(const unsigned char *data, size_t length)
{
    unsigned char checksum = 0;
    for (size_t i = 0; i < length; i++)
    {
        checksum ^= data[i];
    }
    return checksum;
}

// Function to calculate XOR checksum of a string
unsigned char xor_checksum_string(const char *str)
{
    return xor_checksum((const unsigned char *)str, strlen(str));
}

// Function to calculate XOR checksum from a file
unsigned char xor_checksum_file(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }

    unsigned char checksum = 0;
    int byte;

    while ((byte = fgetc(file)) != EOF)
    {
        checksum ^= (unsigned char)byte;
    }

    fclose(file);
    return checksum;
}

// Function to verify data integrity using XOR checksum
int verify_checksum(const unsigned char *data, size_t length, unsigned char expected_checksum)
{
    unsigned char calculated = xor_checksum(data, length);
    return calculated == expected_checksum;
}

int main()
{
    // Example 1: Calculate checksum for a string
    const char *message = "Hello, World!";
    unsigned char checksum1 = xor_checksum_string(message);
    printf("String: \"%s\"\n", message);
    printf("XOR Checksum: 0x%02X (%d)\n\n", checksum1, checksum1);

    // Example 2: Calculate checksum for byte array
    unsigned char data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    size_t data_len = sizeof(data);
    unsigned char checksum2 = xor_checksum(data, data_len);

    printf("Byte array: ");
    for (size_t i = 0; i < data_len; i++)
    {
        printf("0x%02X ", data[i]);
    }
    printf("\nXOR Checksum: 0x%02X (%d)\n\n", checksum2, checksum2);

    // Example 3: Verify data integrity
    unsigned char test_data[] = {0xAA, 0xBB, 0xCC, 0xDD};
    unsigned char test_checksum = xor_checksum(test_data, sizeof(test_data));

    printf("Test data: ");
    for (size_t i = 0; i < sizeof(test_data); i++)
    {
        printf("0x%02X ", test_data[i]);
    }
    printf("\nCalculated checksum: 0x%02X\n", test_checksum);

    // Verify with correct checksum
    if (verify_checksum(test_data, sizeof(test_data), test_checksum))
    {
        printf("Checksum verification: PASSED\n");
    }
    else
    {
        printf("Checksum verification: FAILED\n");
    }

    // Verify with incorrect checksum
    if (verify_checksum(test_data, sizeof(test_data), 0xFF))
    {
        printf("Wrong checksum test: PASSED (unexpected)\n");
    }
    else
    {
        printf("Wrong checksum test: FAILED (expected)\n");
    }

    // Example 4: Interactive mode
    printf("\n--- Interactive Mode ---\n");
    printf("Enter a string to calculate its XOR checksum: ");

    char input[256];
    if (fgets(input, sizeof(input), stdin))
    {
        // Remove newline if present
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n')
        {
            input[len - 1] = '\0';
        }

        unsigned char user_checksum = xor_checksum_string(input);
        printf("Your string: \"%s\"\n", input);
        printf("XOR Checksum: 0x%02X (%d)\n", user_checksum, user_checksum);
    }

    return 0;
}