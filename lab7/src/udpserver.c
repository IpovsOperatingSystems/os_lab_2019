#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <getopt.h>

#include "conn.h"

int main(int argc, char* argv[]) {
    //char ip[255] = {'\0'};
    int port = -1;
    int buf_size = -1;

    while (true) {
        int current_optind = optind ? optind : 1;

        static struct option options[] = {//{"ip", _argument, 0, 0},
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

    int sockfd = SetupConnectionServer(SOCK_DGRAM, port);
    printf("SERVER starts...\n");

    int n;
    char mesg[buf_size], ipadr[16];
    struct sockaddr_in cliaddr;
    while (true) {
        unsigned int len = SLEN;
        if ((n = recvfrom(sockfd, mesg, buf_size, 0, (SADDR *)&cliaddr, &len)) < 0) {
            perror("recvfrom");
            exit(1);
        }

        mesg[n] = '\0';
        const char* recv_ip = inet_ntop(AF_INET, (void *)&cliaddr.sin_addr.s_addr, ipadr, 16);
        printf("REQUEST %s      FROM %s:%d\n", mesg, recv_ip, cliaddr.sin_port);

        if (sendto(sockfd, mesg, n, 0, (SADDR *)&cliaddr, len) < 0) {
            perror("sendto");
            exit(1);
        }
    }
}
