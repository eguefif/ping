#include "ping.h"
#define PING_RATE 1000000
#define RECV_TIMEOUT 1
#define BUFF_SIZE 1000
#define PING_SIZE 64

typedef struct {
    struct icmphdr header;
    char message[PING_SIZE - sizeof(struct icmphdr)];
} Packet;

uint16_t calculate_checksum(void *packet);
Packet init_packet(int seq);
int init_socket();
void print_buffer(char *buffer, int n);
void send_ping(int sockfd, struct sockaddr *addr, int seq);
boolean handle_response(int sockfd);
void display_ping_message(int seq);

void run_ping(Params params) {
    int sockfd = init_socket();
    int seq = 1;

    while (true) {
        usleep(PING_RATE);
        send_ping(sockfd, (struct sockaddr *)&params.addr, seq);
        if (handle_response(sockfd)) {
            display_ping_message(seq);
        } else {
            fprintf(stderr, "Error: wrong response format\n");
        }
        seq++;
    }
}

void display_ping_message(int seq) {
    printf("%d bytes from ADDR(addr): icmp_seq=%d, ttcl=64, time=%d ms\n", 64,
           seq, 0);
}

int init_socket() {
    struct timeval timeout;

    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        fprintf(stderr, "Error: socket not created\n");
        exit(EXIT_FAILURE);
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

void send_ping(int sockfd, struct sockaddr *addr, int seq) {
    Packet packet;
    packet = init_packet(seq);
    if (sendto(sockfd, &packet, sizeof(Packet), 0, addr,
               sizeof(struct sockaddr)) == -1) {
        fprintf(stderr, "Error: failed to send packet\n");
        exit(EXIT_FAILURE);
    }
}

Packet init_packet(int seq) {
    Packet packet;

    bzero(&packet, sizeof(Packet));
    packet.header.type = ICMP_ECHO;
    packet.header.un.echo.id = getpid();
    packet.header.un.echo.sequence = seq;
    packet.header.checksum = calculate_checksum(&packet);

    for (int i = 0; i < 10; i++) {
        packet.message[i] = (char)i + 33;
    }

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

boolean handle_response(int sockfd) {
    char buffer[BUFF_SIZE];

    int n = recv(sockfd, buffer, BUFF_SIZE, 0);
    if (n > 1) {
        struct iphdr *ip = (struct iphdr *)buffer;
        struct icmphdr *icmp = (struct icmphdr *)(buffer + ip->ihl * 4);
        if (icmp->type == 0 && icmp->code == 0) {
            return true;
        } else {
            return false;
        }
    } else {
        fprintf(stderr, "Ping timeout\n");
    }
    return false;
}
