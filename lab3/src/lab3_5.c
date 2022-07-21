#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(void) {
int link[2];
pipe(link);
char foo[4096];
pid_t child_pid = fork();
char *argv[3] = {"1","2","10"};
 if (child_pid >= 0) {
 if (child_pid == 0) {
 dup2(link[1],STDOUT_FILENO);
 execv("sequential_min_max",argv);
 }
 else {
 close(link[1]);
 int nbytes = read(link[0], foo, sizeof(foo));
 printf("Output:\n%.*s\n", nbytes, foo);
 wait(NULL);
 }
 }
 return 0;
}