#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  pid_t pid;

  pid=fork();

  if(pid==0){
      printf("child process\n");
      execv("sequential_min_max\n", argv);
  }

  printf("parent process is waiting");
  
  return 0;
}