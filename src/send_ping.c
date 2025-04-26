#include "ping.h"
#define PING_RATE 1000000
#define RECV_TIMEOUT 1
#define BUFF_SIZE 64000

typedef struct {
    struct icmphdr header;
    char message[64 - sizeof(struct icmphdr)];
} Packet;

uint16_t calculate_checksum(void *packet);
int init_socket(struct sockaddr *target);
Packet init_packet();

void send_ping(struct sockaddr_in addr) {
    Packet packet;
    char buffer[BUFF_SIZE];

    int sockfd = init_socket((struct sockaddr *)&addr);

    usleep(PING_RATE);

    packet = init_packet();
    write(1, &"BEFORE\n", 6);
    sendto(sockfd, &packet, sizeof(Packet), 0, (struct sockaddr *)&addr,
           sizeof(struct sockaddr_in));

    write(1, &"TEST\n", 5);
    int n = recv(sockfd, buffer, BUFF_SIZE, 0);
    if (n > 1) {
        struct icmphdr *hdr = (struct icmphdr *)buffer;
        printf("Packet received with ICMP type %d, code %d\n", hdr->type,
               hdr->code);
    }
}

int init_socket(struct sockaddr *target) {
    struct timeval timeout;

    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (connect(sockfd, target, sizeof(struct sockaddr)) == -1) {
        fprintf(stderr, "Error: impossible to connect socket\n");
    }

    int ttl = 64;
    if (setsockopt(sockfd, SOL_IP, IP_TTL, &ttl, sizeof(ttl)) != 0) {
        fprintf(stderr, "Error: impossible to set ttl for socket\n");
        exit(EXIT_FAILURE);
    }

    timeout.tv_sec = RECV_TIMEOUT;
    timeout.tv_usec = 0;

    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (void *)&timeout,
                   sizeof(struct timeval)) != 0) {

        fprintf(stderr, "Error: impossible to set timeout for socket\n");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}

Packet init_packet() {
    static int seqnum = 0;
    Packet packet;

    bzero(&packet, sizeof(Packet));
    packet.header.type = ICMP_ECHO;
    packet.header.un.echo.sequence = getpid();
    packet.header.un.echo.sequence = seqnum;
    packet.header.checksum = calculate_checksum(&packet);

    return packet;
}

uint16_t calculate_checksum(void *packet) {
    uint16_t *buffer = packet;
    uint16_t sum = 0;
    int max = sizeof(Packet);

    for (int i = 0; i < max; i += 2)
        sum += *buffer++;
    if (max % 2 != 0) {
        sum += *(unsigned char *)buffer;
    }

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);

    return ~sum;
}
