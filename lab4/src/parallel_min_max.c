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

void sighandler(int signum) {
   printf("Caught signal %d, coming out...\n", signum);
   kill(0, SIGKILL);//exit immediately
}

int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  int timeout = -1;
  bool with_files = false;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
				      {"by_files", no_argument, 0, 'f'},
				      {"timeout", required_argument, 0, 0},//add argument timeout
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
	    //your code here
	    //error handling
            if (seed <=0)
	    {
		    printf("Invalid arguments (seed)!\n");
		    exit(EXIT_FAILURE);
	    }
            break;
          case 1:
            array_size = atoi(optarg);
	    //your code here
            if (array_size <= 0)
	    {
		printf("Invalid arguments (array_size)!\n");
		exit(EXIT_FAILURE);
	    }
            break;
          case 2:
            pnum = atoi(optarg);
	    //your code here
            	if  (pnum <= 0)
		{
			printf("Invalid arguments (pnum)!\n");
			exit(EXIT_FAILURE);
		}
            break;
          case 3:
            with_files = true;
            break;
	  case 4:
            timeout = atoi(optarg);
            if (signal(SIGALRM, sighandler) == SIG_ERR)//Register signal handler fail
                printf("\ncan't catch SIGINT\n");
            alarm(timeout);//Scheduled alarm after value timeout's  seconds

            if (timeout <= 0)
            {
                printf("Invalid arguments (timeout)!\n");
                exit(EXIT_FAILURE);
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
  //print array
  for (int i =0; i < array_size ; i++)
	  printf("%d ",array[i]);
  printf("\n");

  //Declare variables
  int array_part = array_size / pnum;
  int pipefd1[2];
  int pipefd2[2];
  char min_array[24];
  char max_array[24];

  //error handling pipe
  if (!with_files)
  {
	if (pipe(pipefd1) == -1)
	{
		perror("pipe1");
		exit(EXIT_FAILURE);
	}
	if (pipe(pipefd2) == -1)
	{
		perror("pipe2");
		exit(EXIT_FAILURE);
	}
  }

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  for (int i = 0; i < pnum; i++) {
    pid_t child_pid = fork();
    if (child_pid >= 0) {
      // successful fork
      active_child_processes += 1;
      if (child_pid == 0) {
        // child process
	struct MinMax min_max;
        // parallel somehow
	if (i != pnum - 1)
		min_max = GetMinMax(array, i*(array_part) , (i+1)*(array_part));
	 // c++ discards a fractional part of the quotient after dividing operation
	else
		min_max = GetMinMax(array, i*(array_part) , array_size);


        if (with_files) {
          // use files here
	    char filename[8];
            snprintf(filename, 8, "file%d", i);//write "filei" to filename

            FILE* fp;
            fp = fopen(filename, "w+");//overwrite
            fprintf(fp, "%d,%d", min_max.min, min_max.max);//write value min max to file i
            fclose(fp);
	    
	    //print min and max of part i
	    printf("file %d---", i);
            printf("min: %d, max: %d",min_max.min , min_max.max);
            printf("\n");
        } else {
          // use pipe here
	    snprintf(min_array, 24, "%d", min_max.min);//write value min of part i to min_array
            snprintf(max_array, 24, "%d", min_max.max);//write value max of part i to max_array

            close(pipefd1[0]); // Close unused read end
            close(pipefd2[0]); // Close unused read end

            write(pipefd1[1], min_array, 24);//write min_array to pipefd1
            write(pipefd2[1], max_array, 24);//write max_array to piprfd2

            close(pipefd1[1]); // Reader will see EOF (end of file)
            close(pipefd2[1]); // Reader will see EOF (end of file)
	    
	    //print min and max of part i
       	    printf("pipe %d---", i);
            printf("min: %d, max: %d",min_max.min , min_max.max);
            printf("\n");
        }
	//Create infinite loop in child
	if (timeout != -1)
        {
            while(true) {}
        }

        return 0;
      }

    } else {
      printf("Fork failed!\n");
      return 1;
    }
  }

  while (active_child_processes > 0) {
    // your code here
    if (timeout == -1) {
            wait(NULL); // blocks parent process until any of its children has finished
        }
        else {
            waitpid(0, NULL, WNOHANG);
	    //The set of child processes that you want to get status information for:
	    //0 — any child process whose process group ID is equal to that of the calling process
	    //WNOHANG — return immediately if there are no children to wait for.
        }
    
    active_child_processes -= 1;
  }

  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;
  char buffer[24];

  for (int i = 0; i < pnum; i++) {
    int min = INT_MAX;
    int max = INT_MIN;

    if (with_files) {
      // read from files
        char filename[8];
        snprintf(filename, 8, "file%d", i);

        FILE* fp;
        fp = fopen(filename, "r");
        fscanf(fp, "%d,%d", &min, &max);
        fclose(fp);
        remove(filename);
    } else {
      // read from pipes
      close(pipefd1[1]);          /* Close unused write end */
      close(pipefd2[1]);          /* Close unused write end */

      read(pipefd1[0], buffer, 24);
      min = atoi(buffer);

      read(pipefd2[0], buffer, 24);
      max = atoi(buffer);
    }

    if (min < min_max.min) min_max.min = min;
    if (max > min_max.max) min_max.max = max;
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);
  printf("Result\n");
  printf("Min: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  
  //Create infinite loop in parents
  if (timeout != -1)
  {
    while(true) {}
  }

  return 0;
}

