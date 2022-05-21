#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <getopt.h>
#include "find_min_max.h"
#include "utils.h"
int pnum; 

void kill_all(int sig)
{
  int *PID_id = malloc(sizeof(int) * pnum);
  for (int i=0;i<pnum;i++)
    kill(PID_id[i],SIGKILL);
  printf("TIMEOUT\n");
}
int main(int argc, char **argv) {
 int seed = -1;
 int array_size = -1;
 int pnum = -1;
 int i = 0;
 bool with_files = false;
 int timeout=-100;
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
case 4:
   timeout=atoi(optarg);
   printf("Get %d",timeout);
     if (timeout<=0)
   {
     printf("timeout is a positive number\n");
     return 1;
   }
   break;
 case 3:
 with_files = true;
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
 for (i = 0; i < array_size; i++)
 {
 printf("%d ", array[i]);
 }
 printf("\n");
 int active_child_processes = 0;
 int sub_array_size = array_size / pnum;
 struct timeval start_time;
 gettimeofday(&start_time, NULL);
 int *array_fd_read = malloc(sizeof(int) * pnum);
 for (i = 0; i < pnum; i++) {
 int pipefd[2];
 if (!with_files) {
 pipe(pipefd);
 }
 pid_t child_pid = fork();
 if (child_pid >= 0) {
 // successful fork
 active_child_processes += 1;
 if (child_pid == 0) {
 // child process
 struct MinMax min_max;
 // parallel somehow
 if (i!=pnum-1)
 {
 min_max = GetMinMax(array, i * sub_array_size, (i+1) * sub_array_size);
 }
 else min_max = GetMinMax(array, i * sub_array_size, array_size);
 printf("Prc[%03d]:\t%d\t%d\n", active_child_processes, min_max.min,
min_max.max);
 if (with_files) {
 // use files here
 char* str = (char*)malloc(15*sizeof(char));
 sprintf(str, "data_%d.txt", i);
 FILE * fp = fopen (str, "a");
 if (fp==0)
 {
 printf( "Could not open file\n" );
 return -1;
 }
 else
 {
 fwrite(&min_max, sizeof(struct MinMax), 1, fp);
 }
 }
 else {
 // use pipe here
 write(pipefd[1],&min_max,sizeof(struct MinMax));
 //close(pipefd[1]);
 }
 return 0;
 }
 else {
 //parent process
 if (!with_files) {
 //add pipe reading fds to array
 array_fd_read[i] = pipefd[0];
 }
 }
 } else {
 printf("Fork failed!\n");
 return 1;
 }
 }
  printf("TIMEOUT NOW = %d\n",timeout);
    if (timeout > 0)
  {
    int *PID_id = malloc(sizeof(int) * pnum);
    PID_id=array_fd_read;
    alarm(timeout);
    signal(SIGALRM,kill_all);
    sleep(1);
  }
 while (active_child_processes > 0) {
 // your code here
 wait(NULL);
 active_child_processes -= 1;
 }
 struct MinMax min_max;
 min_max.min = INT_MAX;
 min_max.max = INT_MIN;
 for (i = 0; i < pnum; i++) {
 struct MinMax cur;
 cur.min = INT_MAX;
 cur.max = INT_MIN;
 if (with_files) {
 // read from files
 char* str = (char*)malloc(15*sizeof(char));
 sprintf(str, "data_%d.txt", i);
 FILE * fp = fopen (str, "r");
 if (fp==0){
 printf( "Could not open file\n" );
 return 1;
 }
 else
 {
 fread(&cur, sizeof(struct MinMax), 1, fp);
 }
 fclose(fp);
 } else {
 // read from pipes
 read(array_fd_read[i], &cur, sizeof(struct MinMax));
 //close(array_fd_read[i]);
 }
// printf("Res[%03d]:\t%d\t%d\n", i, cur.min, cur.max);
 if (cur.min < min_max.min) min_max.min = cur.min;
 if (cur.max > min_max.max) min_max.max = cur.max;

 }
 struct timeval finish_time;
 gettimeofday(&finish_time, NULL);
 double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
 elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;
 free(array);
 printf("Min: %d\n", min_max.min);
 printf("Max: %d\n", min_max.max);
 printf("Elapsed time: %fms\n", elapsed_time);
 fflush(NULL);
 return 0;
}