#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define SADDR struct sockaddr
#define SLEN sizeof(struct sockaddr_in)

int SetupConnectionServer(int, int);
int SetupConnectionClient(struct sockaddr_in*, int, char*, int);
