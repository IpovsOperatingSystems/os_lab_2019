#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <errno.h>
#include <getopt.h>



//#define SERV_PORT 20001
//#define BUFSIZE 1024
#define SADDR struct sockaddr
#define SLEN sizeof(struct sockaddr_in)


bool ConvertStringToUI64(const char* str, uint64_t* val) {
    char* end = NULL;
    unsigned long long i = strtoull(str, &end, 10);
    if (errno == ERANGE) {
        fprintf(stderr, "Out of uint64_t range: %s\n", str);
        return false;
    }

    if (errno != 0)
        return false;

    *val = i;
    return true;
}


int main(int argc, char** argv) {
    int sockfd, n;

    int SERV_PORT = -1;
    int BUFSIZE = -1;
    char servadr[255] = { '\0' }; 
    while (true)
    {
        int current_optind = optind ? optind : 1;

        static struct option options[] =
        {
            {"serv", required_argument, 0, 0},
            {"port", required_argument, 0, 0},
            {"bufsize", required_argument, 0, 0},
            {0, 0, 0, 0}
        };

        int option_index = 0;
        int c = getopt_long(argc, argv, "", options, &option_index);

        if (c == -1)
            break;

        switch (c)
        {
        case 0:
        {
            switch (option_index)
            {
            case 0:
                memcpy(servadr, optarg, strlen(optarg));
                if (strlen(servadr) == sizeof('\0'))
                {
                    printf("bufsize is a positive number\n");
                    return 1;
                }
                break;
            case 1:
                ConvertStringToUI64(optarg, &SERV_PORT);
                if (SERV_PORT <= 0)
                {
                    printf("port is a positive number\n");
                    return 1;
                }
                break;
            case 2:
                ConvertStringToUI64(optarg, &BUFSIZE);
                if (BUFSIZE <= 0)
                {
                    printf("bufsize is a positive number\n");
                    return 1;
                }
                break;
            default:
                printf("Index %d is out of options\n", option_index);
            }
        }
        break;

        case '?':
            printf("Arguments error\n");
            break;
        default:
            fprintf(stderr, "getopt returned character code 0%o?\n", c);
        }
    }

    if (!strlen(servadr) || SERV_PORT == -1 || BUFSIZE == -1) {
        fprintf(stderr, "Using: %s --serv 127.0.0.1 --port 20001 --bufsize 1024\n", argv[0]);
        return 1;
    }

    char sendline[BUFSIZE], recvline[BUFSIZE + 1];
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;

    if (argc < 3) {
        printf("usage: client <IPaddress of server>\n");
        exit(1);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);

    if (inet_pton(AF_INET, servadr, &servaddr.sin_addr) < 0) {
        perror("inet_pton problem");
        exit(1);
    }
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket problem");
        exit(1);
    }

    write(1, "Enter string\n", 13);

    while ((n = read(0, sendline, BUFSIZE)) > 0) {
        if (sendto(sockfd, sendline, n, 0, (SADDR*)&servaddr, SLEN) == -1) {
            perror("sendto problem");
            exit(1);
        }

        if (recvfrom(sockfd, recvline, BUFSIZE, 0, NULL, NULL) == -1) {
            perror("recvfrom problem");
            exit(1);
        }

        printf("REPLY FROM SERVER= %s\n", recvline);
    }
    close(sockfd);
}