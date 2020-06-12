#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <getopt.h>

// #define BUFSIZE 100
#define SADDR struct sockaddr
#define SIZE sizeof(struct sockaddr_in)

int main(int argc, char *argv[]) {
    int fd;
    int nread;
    struct sockaddr_in servaddr;

    // ------------------------- getopt -------------------------

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


    // ------------------------- getopt -------------------------

    char *buf = (char*)malloc(sizeof(buf) * buff_size);


    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creating");
        exit(1);
    }

    memset(&servaddr, 0, SIZE);
    servaddr.sin_family = AF_INET;

    if (inet_pton(AF_INET, adress, &servaddr.sin_addr) <= 0) {
        perror("bad address");
        exit(1);
    }

    servaddr.sin_port = htons(port);

    if (connect(fd, (SADDR *)&servaddr, SIZE) < 0) {
        perror("connect");
        exit(1);
    }

    write(1, "Input message to send\n", 22);
    while ((nread = read(0, buf, buff_size)) > 0) {
        if (write(fd, buf, nread) < 0) {
            perror("write");
            exit(1);
        }
    }

    close(fd);
    exit(0);
}
