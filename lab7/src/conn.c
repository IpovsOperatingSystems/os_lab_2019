#include "conn.h"

int SetupConnectionServer(int __type, int port) {
    int sockfd = socket(AF_INET, __type, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in address;
    memset(&address, 0, SLEN);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    if (bind(sockfd, (SADDR *)&address, SLEN) < 0) {
        perror("bind");
        exit(1);
    } return sockfd;
}

int SetupConnectionClient(struct sockaddr_in* address, int __type, char* ip, int port) {
    int sockfd = socket(AF_INET, __type, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    memset(&*address, 0, SLEN);
    address->sin_family = AF_INET;
    address->sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &address->sin_addr) < 0) {
        perror("bad address");
        exit(1);
    } return sockfd;
}
