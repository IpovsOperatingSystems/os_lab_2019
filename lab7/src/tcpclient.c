#include <stdbool.h>
#include <unistd.h>
#include <getopt.h>

#include "conn.h"

#define BUFSIZE 100

int main(int argc, char *argv[]) {
    char ip[255] = {'\0'};
    int port = -1;
    int buf_size = -1;

    while (true) {
        int current_optind = optind ? optind : 1;

        static struct option options[] = {{"ip", required_argument, 0, 0},
                                        {"port", required_argument, 0, 0},
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
                        memcpy(ip, optarg, strlen(optarg));
                        break;
                    case 1:
                        port = atoi(optarg);
                        if (port < 0 || port > 65532) {
                            fprintf(stderr, "Invalid port: %d\n", port);
                            return 1;
                        } break;
                    case 2:
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

    if (!strlen(ip) || port == -1 || buf_size == -1) {
        fprintf(stderr, "Using: %s --ip '127.0.0.1' --port 20001 --buf_size 1024\n", argv[0]);
        return 1;
    }

    struct sockaddr_in servaddr;
    int sockfd = SetupConnectionClient(&servaddr, SOCK_STREAM, ip, port);

    if (connect(sockfd, (SADDR *)&servaddr, SLEN) < 0) {
        perror("connect");
        exit(1);
    }
    write(1, "Input message to send\n", 22);

    int nread;
    char buf[buf_size];
    while ((nread = read(0, buf, buf_size)) > 0) {
        if (write(sockfd, buf, nread) < 0) {
            perror("write");
            exit(1);
        }
    } close(sockfd);
    return 0;
}
