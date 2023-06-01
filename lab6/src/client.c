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

#include "library.h"

struct Server {
  char ip[255];
  int port;
};

bool is_file_exist(const char *fileName)
{
    FILE *file;
    if (file = fopen(fileName, "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }   
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
  char servers[255] = {'\0'}; // max length of UTF-8 filename in UNIX

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
        // TODO: your code here
        if (k <= 0)
          {
              printf("Invalid arguments (k)!\n");
              exit(EXIT_FAILURE);
          }
        break;
      case 1:
        ConvertStringToUI64(optarg, &mod);
        // TODO: your code here
        if (mod <= 0)
          {
              printf("Invalid arguments (mod)!\n");
              exit(EXIT_FAILURE);
          }
        break;
      case 2:
        // TODO: your code here
        if (is_file_exist(optarg))
        {
            memcpy(servers, optarg, strlen(optarg));
        }
        else
        {
            printf("Invalid arguments (servers)!\n");
            exit(EXIT_FAILURE);
        }        
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

  // TODO: for one server here, rewrite with servers from file
  unsigned int servers_num = 0;
  FILE* fp;
  fp = fopen(servers, "r");
  while (!feof(fp))
  {
    char test1[255];
    char test2[255];
    fscanf(fp, "%s : %s\n", test1, test2);
    servers_num++;
  }
  struct Server *to = malloc(sizeof(struct Server) * servers_num);
  fseek(fp, 0L, SEEK_SET);

  int index = 0;
  while (!feof(fp))
  {
    fscanf(fp, "%s : %d\n", to[index].ip, &to[index].port);
    printf("ip: %s, port: %d\n", to[index].ip, to[index].port);
    index++;
  }
  fclose(fp);

  int factorial_part = k / servers_num;
  uint64_t result = 1;
  int* sck = malloc(sizeof(int) * servers_num);
  // TODO: work continiously, rewrite to make parallel
  for (int i = 0; i < servers_num; i++) {
    struct hostent *hostname = gethostbyname(to[i].ip);
    if (hostname == NULL) {
      fprintf(stderr, "gethostbyname failed with %s\n", to[i].ip);
      exit(1);
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(to[i].port);
    server.sin_addr.s_addr = *((unsigned long *)hostname->h_addr);

    sck[i] = socket(AF_INET, SOCK_STREAM, 0);
    if (sck[i] < 0) {
      fprintf(stderr, "Socket creation failed!\n");
      exit(1);
    }

    if (connect(sck[i], (struct sockaddr *)&server, sizeof(server)) < 0) {
      fprintf(stderr, "Connection failed\n");
      exit(1);
    }

    // TODO: for one server
    // parallel between servers
    
    uint64_t begin = (i*factorial_part) + 1;
    uint64_t end;
    if (i != servers_num - 1) {
        end = (i + 1)*factorial_part;
    }
    else {
        end = k;
    }

    char task[sizeof(uint64_t) * 3];
    memcpy(task, &begin, sizeof(uint64_t));
    memcpy(task + sizeof(uint64_t), &end, sizeof(uint64_t));
    memcpy(task + 2 * sizeof(uint64_t), &mod, sizeof(uint64_t));

    if (send(sck[i], task, sizeof(task), 0) < 0) {
      fprintf(stderr, "Send failed\n");
      exit(1);
    }
  }

  for (int i = 0; i < servers_num; i++) {
    char response[sizeof(uint64_t)];
    if (recv(sck[i], response, sizeof(response), 0) < 0) {
      fprintf(stderr, "Recieve failed\n");
      exit(1);
    }

    // TODO: from one server
    // unite results
    uint64_t answer = 0;
    memcpy(&answer, response, sizeof(uint64_t));
    result = MultModulo(result, answer, mod);
    //result *= answer;

    close(sck[i]);
  }
  printf("answer: %lu\n", result);
  
  free(to);

  return 0;
}
