#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <netpacket/packet.h>
#include <sys/ioctl.h>

// ARP Header Structure
struct arp_header
{
    uint16_t hardware_type; // Hardware type (Ethernet = 1)
    uint16_t protocol_type; // Protocol type (IP = 0x0800)
    uint8_t hardware_len;   // Hardware address length (6 for MAC)
    uint8_t protocol_len;   // Protocol address length (4 for IP)
    uint16_t operation;     // Operation (request = 1, reply = 2)
    uint8_t sender_mac[6];  // Sender MAC address
    uint8_t sender_ip[4];   // Sender IP address
    uint8_t target_mac[6];  // Target MAC address
    uint8_t target_ip[4];   // Target IP address
};

// ARP Table Entry Structure
struct arp_entry
{
    char ip_addr[16];       // IP address string
    char mac_addr[18];      // MAC address string
    struct arp_entry *next; // Pointer to next entry
};

// Global ARP table
struct arp_entry *arp_table = NULL;

// Function to convert MAC address to string
void mac_to_string(uint8_t *mac, char *str)
{
    sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

// Function to convert string to MAC address
void string_to_mac(char *str, uint8_t *mac)
{
    sscanf(str, "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",
           &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
}

// Function to convert IP address to string
void ip_to_string(uint8_t *ip, char *str)
{
    sprintf(str, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
}

// Function to convert string to IP address
void string_to_ip(char *str, uint8_t *ip)
{
    sscanf(str, "%hhu.%hhu.%hhu.%hhu", &ip[0], &ip[1], &ip[2], &ip[3]);
}

// Add entry to ARP table
void add_arp_entry(char *ip, char *mac)
{
    struct arp_entry *new_entry = malloc(sizeof(struct arp_entry));
    strcpy(new_entry->ip_addr, ip);
    strcpy(new_entry->mac_addr, mac);
    new_entry->next = arp_table;
    arp_table = new_entry;
}

// Find MAC address for given IP (ARP lookup)
char *arp_lookup(char *ip)
{
    struct arp_entry *current = arp_table;
    while (current != NULL)
    {
        if (strcmp(current->ip_addr, ip) == 0)
        {
            return current->mac_addr;
        }
        current = current->next;
    }
    return NULL;
}

// Find IP address for given MAC (RARP lookup)
char *rarp_lookup(char *mac)
{
    struct arp_entry *current = arp_table;
    while (current != NULL)
    {
        if (strcmp(current->mac_addr, mac) == 0)
        {
            return current->ip_addr;
        }
        current = current->next;
    }
    return NULL;
}

// Display ARP table
void display_arp_table()
{
    struct arp_entry *current = arp_table;
    printf("\n=== ARP TABLE ===\n");
    printf("IP Address\t\tMAC Address\n");
    printf("----------------------------------------\n");

    while (current != NULL)
    {
        printf("%-15s\t%s\n", current->ip_addr, current->mac_addr);
        current = current->next;
    }
    printf("\n");
}

// Create ARP request packet
void create_arp_request(struct arp_header *arp, char *sender_ip,
                        char *sender_mac, char *target_ip)
{
    arp->hardware_type = htons(1);      // Ethernet
    arp->protocol_type = htons(0x0800); // IP
    arp->hardware_len = 6;              // MAC address length
    arp->protocol_len = 4;              // IP address length
    arp->operation = htons(1);          // ARP request

    // Set sender addresses
    string_to_mac(sender_mac, arp->sender_mac);
    string_to_ip(sender_ip, arp->sender_ip);

    // Set target IP (MAC unknown for request)
    string_to_ip(target_ip, arp->target_ip);
    memset(arp->target_mac, 0, 6); // Unknown MAC
}

// Create ARP reply packet
void create_arp_reply(struct arp_header *arp, char *sender_ip,
                      char *sender_mac, char *target_ip, char *target_mac)
{
    arp->hardware_type = htons(1);      // Ethernet
    arp->protocol_type = htons(0x0800); // IP
    arp->hardware_len = 6;              // MAC address length
    arp->protocol_len = 4;              // IP address length
    arp->operation = htons(2);          // ARP reply

    // Set sender addresses
    string_to_mac(sender_mac, arp->sender_mac);
    string_to_ip(sender_ip, arp->sender_ip);

    // Set target addresses
    string_to_mac(target_mac, arp->target_mac);
    string_to_ip(target_ip, arp->target_ip);
}

// Process ARP request
void process_arp_request(struct arp_header *request, char *our_ip, char *our_mac)
{
    char sender_ip[16], target_ip[16];
    char sender_mac[18];

    ip_to_string(request->sender_ip, sender_ip);
    ip_to_string(request->target_ip, target_ip);
    mac_to_string(request->sender_mac, sender_mac);

    printf("ARP Request received:\n");
    printf("  Who has %s? Tell %s (%s)\n", target_ip, sender_ip, sender_mac);

    // Check if the request is for our IP
    if (strcmp(target_ip, our_ip) == 0)
    {
        printf("  This is for us! Sending ARP reply...\n");

        struct arp_header reply;
        create_arp_reply(&reply, our_ip, our_mac, sender_ip, sender_mac);

        printf("ARP Reply sent:\n");
        printf("  %s is at %s\n", our_ip, our_mac);

        // Add sender to ARP table
        add_arp_entry(sender_ip, sender_mac);
    }
}

// Send ARP request
void send_arp_request(char *our_ip, char *our_mac, char *target_ip)
{
    struct arp_header request;
    create_arp_request(&request, our_ip, our_mac, target_ip);

    printf("Sending ARP request:\n");
    printf("  Who has %s? Tell %s (%s)\n", target_ip, our_ip, our_mac);

    // In a real implementation, this would be sent over the network
    // For simulation, we'll just display the packet structure
    printf("ARP Request packet created successfully\n");
}

// RARP Server functionality
void rarp_server_process(char *client_mac)
{
    printf("RARP Request received from MAC: %s\n", client_mac);

    char *ip = rarp_lookup(client_mac);
    if (ip != NULL)
    {
        printf("RARP Reply: MAC %s has IP %s\n", client_mac, ip);
    }
    else
    {
        printf("RARP: No IP found for MAC %s\n", client_mac);
    }
}

// RARP Client functionality
void rarp_client_request(char *our_mac)
{
    printf("RARP Client: Requesting IP for MAC %s\n", our_mac);

    // In real implementation, this would broadcast a RARP request
    // For simulation, we'll call the server directly
    rarp_server_process(our_mac);
}

// Initialize sample ARP table
void init_sample_data()
{
    add_arp_entry("192.168.1.1", "aa:bb:cc:dd:ee:01");
    add_arp_entry("192.168.1.2", "aa:bb:cc:dd:ee:02");
    add_arp_entry("192.168.1.3", "aa:bb:cc:dd:ee:03");
    add_arp_entry("192.168.1.100", "aa:bb:cc:dd:ee:64");
}

// Main function with menu
int main()
{
    int choice;
    char ip[16], mac[18], target_ip[16];
    char our_ip[] = "192.168.1.100";
    char our_mac[] = "aa:bb:cc:dd:ee:64";

    // Initialize with sample data
    init_sample_data();

    printf("ARP/RARP Simulation Program\n");
    printf("===========================\n");

    while (1)
    {
        printf("\nMenu:\n");
        printf("1. Display ARP Table\n");
        printf("2. ARP Lookup (IP to MAC)\n");
        printf("3. RARP Lookup (MAC to IP)\n");
        printf("4. Send ARP Request\n");
        printf("5. Process ARP Request (Simulation)\n");
        printf("6. RARP Client Request\n");
        printf("7. Add ARP Entry\n");
        printf("8. Exit\n");
        printf("Enter choice: ");

        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            display_arp_table();
            break;

        case 2:
            printf("Enter IP address: ");
            scanf("%s", ip);
            char *found_mac = arp_lookup(ip);
            if (found_mac)
            {
                printf("MAC address for %s: %s\n", ip, found_mac);
            }
            else
            {
                printf("MAC address not found for %s\n", ip);
            }
            break;

        case 3:
            printf("Enter MAC address: ");
            scanf("%s", mac);
            char *found_ip = rarp_lookup(mac);
            if (found_ip)
            {
                printf("IP address for %s: %s\n", mac, found_ip);
            }
            else
            {
                printf("IP address not found for %s\n", mac);
            }
            break;

        case 4:
            printf("Enter target IP address: ");
            scanf("%s", target_ip);
            send_arp_request(our_ip, our_mac, target_ip);
            break;

        case 5:
            printf("Simulating ARP request for our IP (%s)...\n", our_ip);
            struct arp_header sim_request;
            create_arp_request(&sim_request, "192.168.1.1", "aa:bb:cc:dd:ee:01", our_ip);
            process_arp_request(&sim_request, our_ip, our_mac);
            break;

        case 6:
            printf("Enter MAC address for RARP request: ");
            scanf("%s", mac);
            rarp_client_request(mac);
            break;

        case 7:
            printf("Enter IP address: ");
            scanf("%s", ip);
            printf("Enter MAC address: ");
            scanf("%s", mac);
            add_arp_entry(ip, mac);
            printf("Entry added successfully\n");
            break;

        case 8:
            printf("Exiting...\n");
            exit(0);

        default:
            printf("Invalid choice!\n");
        }
    }

    return 0;
}