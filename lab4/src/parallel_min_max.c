#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

pid_t child_pid;  // Идентификатор дочернего процесса
volatile bool child_exited = false;  // Флаг, указывающий на завершение дочернего процесса

// Обработчик сигнала SIGALRM (вызывается по истечению таймаута)
void alarm_handler(int signum) {
    printf("Таймаут истек. Отправляем SIGKILL дочернему процессу...\n");
    kill(child_pid, SIGKILL);  // Отправляем сигнал SIGKILL дочернему процессу
}

int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  bool with_files = false;
  int timeout = -1;  // Инициализируем значение таймаута

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {"timeout", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            // your code here
            // error handling
            if (seed <= 0) {
              printf("seed is a positive number\n");
              return 1;
            }
            break;
          case 1:
            array_size = atoi(optarg);
            // your code here
            // error handling
            if (array_size <= 0) {
              printf("array_size is a positive number\n");
              return 1;
            }
            break;
          case 2:
            pnum = atoi(optarg);
            // your code here
            // error handling
            if (pnum <= 0) {
              printf("pnum is a positive number\n");
              return 1;
            }
            break;
          case 3:
            with_files = true;
            break;
          case 4:
            timeout = atoi(optarg);  // Устанавливаем таймаут 
            if (timeout <= 0) {
              printf("Timeout must be a positive number.\n");
              return 1;
            }
            break;
          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 'f':
        with_files = true;
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

  if (seed == -1 || array_size == -1 || pnum == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;

  
  

  struct timeval start_time;
  gettimeofday(&start_time, NULL);
// Создаем каналы связи(pipes) синхронизации процессов
  int **pipes = malloc(pnum * sizeof(int *)); 
  for (int i = 0; i < pnum; i++) {
      pipes[i] = malloc(2 * sizeof(int));
  }
  
  for (int i = 0; i < pnum; i++) {
      if (pipe(pipes[i]) == -1) {
           perror("pipe");
          return 1;
      }
  }

  if (timeout > 0) {  // Если таймаут задан, устанавливаем таймаут с использованием функции alarm
    signal(SIGALRM, alarm_handler);  // Устанавливаем обработчик сигнала SIGALRM
    alarm(timeout);  // Устанавливаем таймаут (в секундах)
  }

  for (int i = 0; i < pnum; i++) {
    pid_t child_pid = fork();
    if (child_pid >= 0) {
      // successful fork
      active_child_processes += 1;
      if (child_pid == 0) {
        // child process
        int subarray_size = array_size / pnum;

        int begin = i * subarray_size;
        int end;
        if (i == pnum - 1) {
            end = array_size - 1;
        } else {
            end = (i + 1) * subarray_size;
        }
        // parallel somehow


        struct MinMax subarray_minmax = GetMinMax(array, begin, end);
        // Создание файлов и запись в них результатов каждого дочернего процесса
        if (with_files) {
          // use files here
          char filename[256];
          snprintf(filename, sizeof(filename), "minmax_%d.txt", i);

          FILE *file = fopen(filename, "w");
          if (file == NULL) {
              perror("fopen");
              return 1; 
          }

          fprintf(file, "%d %d\n", subarray_minmax.min, subarray_minmax.max);

          fclose(file);
        } else {
          // use pipe here
              close(pipes[i][0]); // Закрыть чтение из pipe
              write(pipes[i][1], &subarray_minmax, sizeof(struct MinMax));
              close(pipes[i][1]); // Закрыть запись в pipe
        }
        return 0;
      }

    } else {
      printf("Fork failed!\n");
      return 1;
    }
  }
// Ждем пока все потоки закончат свое выполнение
  while (active_child_processes > 0) {
    // your code here
    int status;
    pid_t finished_pid = wait(&status);

    if (finished_pid > 0) {
        active_child_processes -= 1;

        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
        }
    } else {
        perror("wait");
        return 1; 
    }
    // active_child_processes -= 1;
  }

  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  for (int i = 0; i < pnum; i++) {
    int min = INT_MAX;
    int max = INT_MIN;
  // 
    if (with_files) {
      // read from files
      char filename[256];
      snprintf(filename, sizeof(filename), "minmax_%d.txt", i);

      // Открытие файла для чтения
      FILE *file = fopen(filename, "r");
      if (file == NULL) {
          perror("fopen");
          return 1; // Обработка ошибки открытия файла
      }

      // Чтение минимума и максимума из файла
      int min, max;
      fscanf(file, "%d %d", &min, &max);

      // Закрытие файла
      fclose(file);

      // Сравнение с глобальным минимумом и максимумом
      if (min < min_max.min) min_max.min = min;
      if (max > min_max.max) min_max.max = max;
    } else {
      // read from pipes
      close(pipes[i][1]); // Закрыть запись в pipe
      struct MinMax subarray_minmax;
      read(pipes[i][0], &subarray_minmax, sizeof(struct MinMax));
      close(pipes[i][0]); // Закрыть чтение из pipe

      if (subarray_minmax.min < min_max.min) min_max.min = subarray_minmax.min;
      if (subarray_minmax.max > min_max.max) min_max.max = subarray_minmax.max;
        
    }


  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  for (int i = 0; i < pnum; i++) {
      free(pipes[i]);
  }
  free(pipes);
  free(array);

  printf("Min: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}

