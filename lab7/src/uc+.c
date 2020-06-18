#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>
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
    char adress[12];

    while (1) {
        static struct option options[] = {{"bfsize", required_argument, 0, 0},
        {"port", required_argument, 0, 0},
        {"adr", required_argument, 0, 0},
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
                case 2:
                memcpy(adress, optarg, sizeof(char) * 12);
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
        printf("Usage: %s --bfsize \"num\" --port \"num\" --adr 127.0.0.1 \n",
        argv[0]);
        return 1;
    }

    char *buf = (char*)malloc(sizeof(buf) * buff_size);
    char *sendline = (char*)malloc(sizeof(sendline) * buff_size);
    char *recvline = (char*)malloc(sizeof(recvline) * (buff_size + 1));

    // if (argc != 2) {
    //     printf("usage: client <IPaddress of server>\n");
    //     exit(1);
    // }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);

    if (inet_pton(AF_INET, adress, &servaddr.sin_addr) < 0) {
        perror("inet_pton problem");
        exit(1);
    }
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket problem");
        exit(1);
    }

    write(1, "Enter string\n", 13);

    while ((n = read(0, sendline, buff_size)) > 0) {
        if (sendto(sockfd, sendline, n, 0, (SADDR *)&servaddr, SLEN) == -1) {
            perror("sendto problem");
            exit(1);
        }

        if (recvfrom(sockfd, recvline, buff_size, 0, NULL, NULL) == -1) {
            perror("recvfrom problem");
            exit(1);
        }

        printf("REPLY FROM SERVER= %s\n", recvline);
    }
    close(sockfd);
}
