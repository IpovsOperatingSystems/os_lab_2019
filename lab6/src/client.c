#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "pthread.h"

uint64_t MultModulo(uint64_t a, uint64_t b, uint64_t mod) {
  uint64_t result = 0;
  a = a % mod;
  while (b > 0) {
    if (b % 2 == 1)
      result = (result + a) % mod;
    a = (a * 2) % mod;
    b /= 2;
  }

  return result % mod;
}

struct Server {
    char ip[255];
    int port;
};

struct ServerPack {
    struct Server to;
    int index;
    int step;
    uint64_t number;
    uint64_t mod;
    unsigned int servers_num;
};

uint64_t send_to_server( struct ServerPack pack ) {

    struct hostent *hostname = gethostbyname(pack.to.ip);
    if (hostname == NULL) {
        fprintf(stderr, "gethostbyname failed with %s\n", pack.to.ip);
        exit(1);
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(pack.to.port);
    server.sin_addr.s_addr = *((unsigned long *)hostname->h_addr_list[0]);

    int sck = socket(AF_INET, SOCK_STREAM, 0);
    if (sck < 0) {
        fprintf(stderr, "Socket creation failed!\n");
        exit(1);
    }

    if (connect(sck, (struct sockaddr *)&server, sizeof(server)) < 0) {
        fprintf(stderr, "Connection failed\n");
        exit(1);
    }

    int index = pack.index;
    int step = pack.step;
    // Separating for parallel calculation
    uint64_t begin = step * index;
    uint64_t end = (index + 1) * step;
    // if last then end equals k (for not round numbers)
    if (index == pack.servers_num - 1) { end = pack.number; }

    // Making data packs for sending
    char task[sizeof(uint64_t) * 3];
    memcpy(task, &begin, sizeof(uint64_t));
    memcpy(task + sizeof(uint64_t), &end, sizeof(uint64_t));
    memcpy(task + 2 * sizeof(uint64_t), &pack.mod, sizeof(uint64_t));

    if (send(sck, task, sizeof(task), 0) < 0) {
        fprintf(stderr, "Send failed\n");
        exit(1);
    }

    char response[sizeof(uint64_t)];
    if (recv(sck, response, sizeof(response), 0) < 0) {
        fprintf(stderr, "Recieve failed\n");
        exit(1);
    }

    uint64_t server_answer = 0;
    memcpy(&server_answer, response, sizeof(uint64_t));
    printf("server %d factorial answer: %lu\n", index, server_answer);
    printf("server %d answer: %lu\n", index, server_answer % pack.mod);
    close(sck);
    return server_answer;
}

void *ThreadSend(void *args) {
    struct ServerPack *pack = (struct ServerPack *)args;
    return (void *)(uint64_t *)send_to_server(*pack);
}

bool ConvertStringToUI64(const char *str, uint64_t *val) {
    char *end = NULL;
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

int main(int argc, char **argv) {
    uint64_t k = -1;
    uint64_t mod = -1;
    char servers[255] = {'\0'}; // TODO: explain why 255

    system("clear");

    while (true) {
        int current_optind = optind ? optind : 1;

        static struct option options[] = {{"k", required_argument, 0, 0},
        {"mod", required_argument, 0, 0},
        {"servers", required_argument, 0, 0},
        {0, 0, 0, 0}};

        int option_index = 0;
        int c = getopt_long(argc, argv, "", options, &option_index);

        if (c == -1)
        break;

        switch (c) {
            case 0: {
                switch (option_index) {
                    case 0:
                    ConvertStringToUI64(optarg, &k);
                    // Factorial number
                    if ( k < 0 || k > 2000 ) {
                        printf("Unacceptable number 0 >= k < 2000 \n");
                        return 1;
                    }

                    break;
                    case 1:
                    ConvertStringToUI64(optarg, &mod);
                    // Mod number
                    if ( mod < 0 ) {
                        printf("Modul must be positive\n");
                        return 1;
                    }
                    break;
                    case 2:
                    memcpy(servers, optarg, strlen(optarg));
                    break;
                    default:
                    printf("Index %d is out of options\n", option_index);
                }
            } break;

            case '?':
            printf("Arguments error\n");
            break;
            default:
            fprintf(stderr, "getopt returned character code 0%o?\n", c);
        }
    }

    if (k == -1 || mod == -1 || !strlen(servers)) {
        fprintf(stderr, "Using: %s --k 1000 --mod 5 --servers /path/to/file\n",
        argv[0]);
        return 1;
    }

    unsigned int servers_num = 2;
    struct Server *to = malloc(sizeof(struct Server) * servers_num);
    struct ServerPack *pack = malloc(sizeof(struct ServerPack) * servers_num);;
    // seting threads for background waiting servers results
    pthread_t threads[servers_num];
    int  i_returns[servers_num];

    // installation of servers routes
    for (size_t i = 0; i < servers_num; i++) {
        to[i].port = 20001 + i;
        memcpy(to[i].ip, "127.0.0.1", sizeof("127.0.0.1"));
    }

    // Step for differentiating
    int step = k > servers_num ? (k / servers_num) : 1;
    // Variable for accumulating results
    uint64_t answer = 1;

    // Sending data to servers by background
    for (int i = 0; i < servers_num; i++) {
        pack[i].to = to[i];
        pack[i].index = i;
        pack[i].step = step;
        pack[i].number = k;
        pack[i].mod = mod;
        pack[i].servers_num = servers_num;

        if (pthread_create(&threads[i], NULL, ThreadSend,
            (void*)&(pack[i]))) {
                printf("Error: pthread_create failed!\n");
                return 1;
        }
    }

    // Receiving results from background processes who takes it from servers
    for (size_t i = 0; i < servers_num; i++) {
        uint64_t result = 1;
        pthread_join(threads[i], (void **)&result);
        answer *= result;
    }

    printf("----------------------------------------\n");
    printf("Final factorial answer: %lu\n", answer);
    printf("Final answer: %lu\n", answer % mod);
    printf("Work was paralleled between %d servers\n", servers_num);
    printf("----------------------------------------\n");
    printf("=========================================\n");

    free(to);
    return 0;
}