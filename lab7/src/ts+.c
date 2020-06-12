#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <getopt.h>

// #define SERV_PORT 10050
// #define BUFSIZE 100
#define SADDR struct sockaddr

int main(int argc, char **argv) {
    const size_t kSize = sizeof(struct sockaddr_in);

    int lfd, cfd;
    int nread;
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;

    // ------------------------- getopt -------------------------

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


    // ------------------------- getopt -------------------------

    if (buff_size < 100) {buff_size = 100;}

    char *buf = (char*)malloc(sizeof(buf) * buff_size);

    if ((lfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    memset(&servaddr, 0, kSize);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if (bind(lfd, (SADDR *)&servaddr, kSize) < 0) {
        perror("bind");
        exit(1);
    }

    if (listen(lfd, 5) < 0) {
        perror("listen");
        exit(1);
    }

    while (1) {
        unsigned int clilen = kSize;

        if ((cfd = accept(lfd, (SADDR *)&cliaddr, &clilen)) < 0) {
            perror("accept");
            exit(1);
        }
        printf("connection established\n");

        while ((nread = read(cfd, buf, buff_size)) > 0) {
            write(1, &buf, nread);
        }

        if (nread == -1) {
            perror("read");
            exit(1);
        }
        close(cfd);
    }
}
