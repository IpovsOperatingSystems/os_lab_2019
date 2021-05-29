#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "factorial.h"

struct Server {
  char ip[255];
  int port;
};

struct ThreadArgs{
    struct Server server_args;
    int begin;
    int end;
    int mod;
};

int result = 1;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

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

void ThreadServer(void *args) {
  struct ThreadArgs *thread_args = (struct ThreadArgs *)args;
  // TODO: work continiously, rewrite to make parallel
    struct hostent *hostname = gethostbyname((*thread_args).server_args.ip);
    if (hostname == NULL) {
        fprintf(stderr, "gethostbyname failed with %s\n", (*thread_args).server_args.ip);
        exit(1);
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons((*thread_args).server_args.port);
    server.sin_addr.s_addr = *((unsigned long *)hostname->h_addr);

    int sck = socket(AF_INET, SOCK_STREAM, 0);
    if (sck < 0) {
        fprintf(stderr, "Socket creation failed!\n");
        exit(1);
    }

    printf("connecting to : ip %s , port %d\n", (*thread_args).server_args.ip, (*thread_args).server_args.port);
    if (connect(sck, (struct sockaddr *)&server, sizeof(server)) < 0) {
        fprintf(stderr, "Connection failed\n");
        fprintf(stderr, "error creating connection: %s ", strerror(errno));
        exit(1);
    }

    uint64_t begin = (*thread_args).begin;
    uint64_t end = (*thread_args).end;
    uint64_t mod = (*thread_args).mod;
    printf("sending task to server: begin %llu, end %llu, mod %llu\n", begin,end,mod);

    char task[sizeof(uint64_t) * 3];
    memcpy(task, &begin, sizeof(uint64_t));
    memcpy(task + sizeof(uint64_t), &end, sizeof(uint64_t));
    memcpy(task + 2 * sizeof(uint64_t), &mod, sizeof(uint64_t));

    if (send(sck, task, sizeof(task), 0) < 0) {
      fprintf(stderr, "Send failed\n");
      exit(1);
    }

    char response[sizeof(uint64_t)];
    if (recv(sck, response, sizeof(response), 0) < 0) {
      fprintf(stderr, "Recieve failed\n");
      exit(1);
    }

    uint64_t answer;
    memcpy(&answer, response, sizeof(uint64_t));
    printf("recieved an answer: %llu\n", answer);
    pthread_mutex_lock(&mut);
    int temp = result;
    result = (temp * answer) % mod;
    pthread_mutex_unlock(&mut);
    close(sck);
}

int main(int argc, char **argv) {
  uint64_t k = -1;
  uint64_t mod = -1;
  char servers[255] = {'\0'}; // TODO: explain why 255

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
        break;
      case 1:
        ConvertStringToUI64(optarg, &mod);
        // TODO: your code here
        break;
      case 2:
        // TODO: your code here
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

  // TODO: for one server here, rewrite with servers from file
  FILE* server_file = fopen(servers, "r");
  if (server_file == NULL) {
      perror( "cannot read server_file" );
      return 1;
  }


  char str[50];
  int servers_num = 0;
   while (1)
   {
      fgets(str,sizeof(str),server_file);

      if (feof(server_file) != 0)
      {
        break;
      }
      servers_num++;
   }
   char ips[servers_num][50];
   char ports[servers_num][50];
   struct Server *to = malloc(sizeof(struct Server) * servers_num);
   fseek(server_file,0,SEEK_SET);

   int i=0;
   while (1)
   {
      fgets(str,sizeof(str),server_file);

      if (feof(server_file) != 0)
      {
        break;
      }
      strcpy((*(to+i)).ip, strtok(str , ":"));
      (*(to+i)).port = atoi(strtok(NULL, ":"));
      i++;
   }
   fclose(server_file);

   i = 0;
   for (; i<servers_num; i++){
       printf("ip: %s / port: %d\n", (*(to+i)).ip, (*(to+i)).port);
   }

   sleep(1);
   pthread_t threads[servers_num];
   struct ThreadArgs args[servers_num];
   i = 0;
   for (; i < servers_num; ++i){
      args[i].server_args = *(to+i);
      args[i].begin = (k/servers_num)*i + 1;
      args[i].end = (i ==servers_num-1  ? k : (k/servers_num)*(i+1));
      args[i].mod = mod;
      if (pthread_create(&threads[i], NULL, (void *)ThreadServer, (void *)(args+i))) {
      printf("Error: pthread_create failed!\n");
      return 1;
      }
   }

   i = 0;
   for (; i < servers_num; ++i) {
    pthread_join(threads[i], NULL);
  }

  printf("\nResult: %d\n", result);

  free(to);
  return 0;
}