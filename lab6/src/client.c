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
#include "factorial.h"

// Структура, описывающая сервер
struct Server {
  char ip[255]; // IP-адрес сервера
  int port;     // Порт сервера
};

// Структура для передачи аргументов в поток
struct ThreadArgs{
    struct Server server_args; // Параметры сервера
    uint64_t begin;            // Начальное значение
    uint64_t end;              // Конечное значение
    uint64_t mod;              // Модуль для вычислений
};

// Функция для преобразования строки в беззнаковое 64-битное целое число
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

// Устанавливает соединение по сокету с сервером
int SetupSocketConnection(char ip[255], int port) {
    struct hostent *hostname = gethostbyname(ip);
    if (hostname == NULL) {
        fprintf(stderr, "gethostbyname failed with %s\n", ip);
        exit(1);
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = *((unsigned long *)hostname->h_addr);

    int sck = socket(AF_INET, SOCK_STREAM, 0);
    if (sck < 0) {
        fprintf(stderr, "Socket creation failed!\n");
        exit(1);
    }

    if (connect(sck, (struct sockaddr *)&server, sizeof(server)) < 0) {
        fprintf(stderr, "Connection failed\n");
        exit(1);
    }
    return sck;
}

// Генерирует задание для отправки на сервер
char* SetupTask(struct ThreadArgs args) {
    char* task = malloc(sizeof(uint64_t) * 3);
    memcpy(task, &args.begin, sizeof(uint64_t));
    memcpy(task + sizeof(uint64_t), &args.end, sizeof(uint64_t));
    memcpy(task + 2 * sizeof(uint64_t), &args.mod, sizeof(uint64_t));
    return task;
}

// Функция, выполняемая потоком для отправки данных на сервер
void* ThreadSend(void* args){
    struct ThreadArgs *thread_args = (struct ThreadArgs *)args;

    int sck = SetupSocketConnection(thread_args->server_args.ip, thread_args->server_args.port);
    char* task = SetupTask(*thread_args);
    if (send(sck, task, 24, 0) < 0) {
      fprintf(stderr, "Send failed\n");
      exit(1);
    } free(task);

    char response[sizeof(uint64_t)];
    if (recv(sck, response, sizeof(response), 0) < 0) {
      fprintf(stderr, "Receive failed\n");
      exit(1);
    }

    uint64_t answer;
    memcpy(&answer, response, sizeof(uint64_t));
    close(sck);
    return (void* )answer;
}

// Парсит серверы из файла
struct Server* ParseServersFromFile(char* filename, unsigned int *servers_counter) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "|ERROR| Can't read file: %s\n", filename);
        exit(1);
    }
    int count_str = 0;
    char txt[255+6];
    while (fgets(txt, sizeof(txt), fp) != NULL) {
        count_str++;
    }
    struct Server* servers = (struct Server*)malloc(sizeof(struct Server) * count_str);

    fseek(fp, 0, SEEK_SET);
    for (int i = 0; i < count_str; ++i) {
        fgets(txt, sizeof(txt), fp);
        int seporator_index = -1;

        for (int j = 0; j < sizeof(txt); ++j) {
            if (txt[j] == '\0') {
                fprintf(stderr, "|ParseError| Invalid address: %s\n", txt);
                fclose(fp);
                exit(1);
            }
            if (txt[j] == ':') {
                seporator_index = j;
                break;
            }
        }
        memcpy(servers[i].ip, txt, sizeof(char) * seporator_index);
        servers[i].port = atoi(&txt[seporator_index + 1]);
    } fclose(fp);
    *servers_counter = count_str;
    return servers;
}

// Основная функция программы
int main(int argc, char **argv) {
    uint64_t k = -1; // Количество вычислений
    uint64_t mod = -1; // Модуль для вычислений
    char servers[255] = {'\0'}; // Переменная для хранения имени файла с серверами
    unsigned int servers_num = 0; // Количество серверов
    struct Server *to; // Структура для хранения серверов

    // Обработка аргументов командной строки
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
                    if (k < 1) {
                        fprintf(stderr, "|ERROR| K must be positive number: %lu\n", k);
                        return 1;
                    } break;
                case 1:
                    ConvertStringToUI64(optarg, &mod);
                    if (mod < 0) {
                        fprintf(stderr, "|ERROR| Mod must be positive number: %lu\n", mod);
                        return 1;
                    }   break;
                case 2:
                    memcpy(servers, optarg, strlen(optarg));
                    to = ParseServersFromFile(servers, &servers_num);
                    if (servers_num == 0) {
                        fprintf(stderr, "|ERROR| %s must contain valid addresses: %d\n", servers, servers_num);
                        return 1;
                    } break;
                default:
                    fprintf(stderr, "|ERROR| Index %d is out of options\n", option_index);
                }
            } break;

            case '?':
                printf("|ERROR| Arguments error\n");
                break;

            default:
                fprintf(stderr, "|ERROR| getopt returned character code 0%o?\n", c);
        }
    }

    if (k == -1 || mod == -1 || !strlen(servers)) {
        fprintf(stderr, "Using: %s --k 1000 --mod 5 --servers /path/to/file\n", argv[0]);
        return 1;
    }

    pthread_t threads[servers_num]; // Потоки для общения с серверами
    struct ThreadArgs args[servers_num]; // Аргументы для передачи в поток
    uint64_t step = k / servers_num; // Шаг для разделения задания по серверам
    uint64_t answers[servers_num]; // Ответы от серверов
    
    for (int i = 0; i < servers_num; i++) {
        {

            args[i].server_args = to[i];
            args[i].mod = mod;
            args[i].begin = 1 + i * step;
            if (i == servers_num - 1) {
                args[i].end = k;
            } else {
                args[i].end = args[i].begin + step - 1;
            }
            printf("%d - %s:%d\n", i, args[i].server_args.ip, args[i].server_args.port);
        }
        if(pthread_create(&threads[i], NULL, ThreadSend, (void*)&args[i])) {
            printf("Error: pthread_create failed!\n");
            return 1;
        }
    }
    free(to);

    for (int i=0; i < servers_num; ++i) {
        pthread_join(threads[i], (void**)&answers[i]);
    }

    uint64_t result = 1;
    for (int i=0; i < servers_num; ++i) {
        printf("%lu, %lu\n", result, answers[i]);
        result = MultModulo(result, answers[i], mod);
    } printf("answer: %lu\n", result);
    return 0;
}
