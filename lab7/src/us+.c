#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <getopt.h>

// #define SERV_PORT 20001
// #define BUFSIZE 1024
#define SADDR struct sockaddr
#define SLEN sizeof(struct sockaddr_in)

int main(int argc, char **argv) {
    int sockfd, n;
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;



    int buff_size = -1;
    int port = -1;

    while (1) {
        static struct option options[] = {{"bfsize", required_argument, 0, 0},
        {"port", required_argument, 0, 0},
        {0, 0, 0, 0}};

        int option_index = 0;
        int c = getopt_long(argc, argv, "?", options, &option_index);

        if (c == -1) break;

        switch (c) {
            case 0:
            switch (option_index) {
                case 0:
                buff_size = atoi(optarg);
                if ( buff_size < 0 ) {
                    printf("Wrong number\n");
                    return 1;
                }
                break;
                case 1:
                port = atoi(optarg);
                if ( port < 0 ) {
                    printf("Wrong port number\n");
                    return 1;
                }
                break;
            }
            break;

            case '?':
            break;

            default:
            printf("getopt returned character code 0%o?\n", c);
        }
    }

    if (optind < argc) {
        printf("Has at least one no option argument\n");
        return 1;
    }

    if ( buff_size == -1 || port == -1 ) {
        printf("Usage: %s --bfsize \"num\" --port \"num\"\n",
        argv[0]);
        return 1;
    }



    char *mesg = (char*)malloc(sizeof(mesg) * buff_size);
    char ipadr[16];


    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket problem");
        exit(1);
    }

    memset(&servaddr, 0, SLEN);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if (bind(sockfd, (SADDR *)&servaddr, SLEN) < 0) {
        perror("bind problem");
        exit(1);
    }
    printf("SERVER starts...\n");

    while (1) {
        unsigned int len = SLEN;

        if ((n = recvfrom(sockfd, mesg, buff_size, 0, (SADDR *)&cliaddr, &len)) < 0) {
            perror("recvfrom");
            exit(1);
        }
        mesg[n] = 0;

        printf("REQUEST %s      FROM %s : %d\n", mesg,
        inet_ntop(AF_INET, (void *)&cliaddr.sin_addr.s_addr, ipadr, 16),
        ntohs(cliaddr.sin_port));

        if (sendto(sockfd, mesg, n, 0, (SADDR *)&cliaddr, len) < 0) {
            perror("sendto");
            exit(1);
        }
    }
}
