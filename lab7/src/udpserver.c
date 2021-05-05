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

//#define SERV_PORT 20001
//#define BUFSIZE 1024
#define SADDR struct sockaddr
#define SLEN sizeof(struct sockaddr_in)

int main(int argc, char **argv) {
    int SERV_PORT = -1;
      int BUFSIZE = -1;
    
      while (1) {
        int current_optind = optind ? optind : 1;
    
        static struct option options[] = {{"BUFSIZE", required_argument, 0, 0},
                                          {"SERV_PORT", required_argument, 0, 0},
                                          {0, 0, 0, 0}};
    
        int option_index = 0;
        int c = getopt_long(argc, argv, "", options, &option_index); 
        
        if (c == -1) break;
    
        switch (c) {
          case 0:
            switch (option_index) {
              case 0:
                BUFSIZE = atoi(optarg);
                if (BUFSIZE < 1) { 
                    BUFSIZE=-1;
                    return 1;
                }
                break;
              case 1:
                SERV_PORT = atoi(optarg);
                if (SERV_PORT < 0) { 
                    SERV_PORT=-1;
                    return 1;
                }
                break;
              defalut:
                printf("Index %d is out of options\n", option_index);
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
    
      if (BUFSIZE == -1 || SERV_PORT == -1) {
        printf("Usage: %s --BUFSIZE 100 --SERV_PORT 10050 \n", argv[0]);
        return 1;
      }
      
  int sockfd, n;
  char mesg[BUFSIZE], ipadr[16];
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket problem");
    exit(1);
  }

  memset(&servaddr, 0, SLEN);
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);

  if (bind(sockfd, (SADDR *)&servaddr, SLEN) < 0) {
    perror("bind problem");
    exit(1);
  }
  printf("SERVER starts...\n");

  while (1) {
    unsigned int len = SLEN;

    if ((n = recvfrom(sockfd, mesg, BUFSIZE, 0, (SADDR *)&cliaddr, &len)) < 0) {
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