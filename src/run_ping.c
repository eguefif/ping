#include "ping.h"
#define PING_RATE 1000000
#define RECV_TIMEOUT 1
#define BUFF_SIZE 1000

boolean running = true;

Packet init_packet(int seq);
int init_socket();
void print_buffer(char *buffer, int n);
void send_ping(int sockfd, struct sockaddr *addr, int seq);
boolean handle_response(int sockfd, int seq);
void signalHandler();

void run_ping(Params params) {
    int seq = 1;
    Stats stats;
    struct timeval before, after;
    unsigned long elapsed = 0;
    boolean success = true;
    int sockfd = init_socket();

    bzero(&stats, sizeof(Stats));
    signal(SIGINT, signalHandler);

    if (gettimeofday(&stats.start, NULL) != 0) {
        fprintf(stderr, "Error: impossible to get time\n");
        exit(EXIT_FAILURE);
    }
    while (running) {
        usleep(PING_RATE);
        if (gettimeofday(&before, NULL) != 0) {
            fprintf(stderr, "Error: impossible to get time\n");
            exit(EXIT_FAILURE);
        }
        send_ping(sockfd, (struct sockaddr *)&params.addr, seq);
        if (handle_response(sockfd, seq)) {
            if (gettimeofday(&after, NULL) != 0) {
                fprintf(stderr, "Error: impossible to get time: %s\n",
                        strerror(errno));
                exit(EXIT_FAILURE);
            }
            elapsed = subtract_time(after, before);
            display_ping_message(seq, &params, elapsed);
            success = true;
        } else {
            success = false;
            display_unreachable(seq, &params);
        }
        gather_statistics(&stats, elapsed, success);
        seq++;
    }
    if (gettimeofday(&stats.end, NULL) != 0) {
        fprintf(stderr, "Error: impossible to get time\n");
        exit(EXIT_FAILURE);
    }
    display_stat(&params, stats);
}

void signalHandler() { running = false; }

int init_socket() {
    struct timeval timeout;

    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        fprintf(stderr, "Error: socket not created\n");
        exit(EXIT_FAILURE);
    }

    int ttl = TTL;
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
               sizeof(struct sockaddr_in)) == -1) {
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

    for (int i = 0; i < 10; i++) {
        packet.message[i] = (char)i + 33;
    }
    packet.header.checksum = calculate_checksum(&packet);

    return packet;
}

boolean handle_response(int sockfd, int seq) {
    char buffer[BUFF_SIZE];

    int n = recv(sockfd, buffer, BUFF_SIZE, 0);
    if (n > 1) {
        return check_response(buffer, seq) ? true : false;
    } else {
        fprintf(stderr, "Ping timeout\n");
    }
    return false;
}
