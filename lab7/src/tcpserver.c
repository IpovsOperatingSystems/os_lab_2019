#include <stdbool.h>
#include <unistd.h>
#include <getopt.h>

#include "conn.h"


int main(int argc, char* argv[]) {
    int port = -1;
    int buf_size = -1;

    while (true) {
        int current_optind = optind ? optind : 1;

        static struct option options[] = {{"port", required_argument, 0, 0},
                                        {"buf_size", required_argument, 0, 0},
                                        {0, 0, 0, 0}};

        int option_index = 0;
        int c = getopt_long(argc, argv, "", options, &option_index);

        if (c == -1)
        break;

        switch (c) {
            case 0: {
                switch (option_index) {
                    case 0:
                    port = atoi(optarg);
                    if (port < 0 || port > 65532) {
                        fprintf(stderr, "Invalid port: %d\n", port);
                        return 1;
                    } break;
                case 1:
                    buf_size = atoi(optarg);
                    if (buf_size < 32) {
                        fprintf(stderr, "buf_size must be more than 32: %d\n", buf_size);
                        return 1;
                    } break;
                default:
                    fprintf(stderr, "Index %d is out of options\n", option_index);
                }
            } break;

            case '?':
                printf("Arguments error\n");
                break;
                
            default:
                fprintf(stderr, "getopt returned character code 0%o?\n", c);
        }
    }

    if (port == -1 || buf_size == -1) {
        fprintf(stderr, "Using: %s --port 20001 --buf_size 1024\n", argv[0]);
        return 1;
    }

    int sockfd = SetupConnectionServer(SOCK_STREAM, port);
    if (listen(sockfd, 5) < 0) {
        perror("listen");
        exit(1);
    }    
    
    int cfd;
    int nread;
    char buf[buf_size];
    struct sockaddr_in cliaddr;
    while (true) {
        unsigned int clilen = SLEN;

        if ((cfd = accept(sockfd, (SADDR *)&cliaddr, &clilen)) < 0) {
            perror("accept");
            exit(1);
        }
        printf("connection established\n");

        while ((nread = read(cfd, buf, buf_size)) > 0) {
            write(1, &buf, nread);
        }

        if (nread == -1) {
            perror("read");
            exit(1);
        } close(cfd);
    }
}
