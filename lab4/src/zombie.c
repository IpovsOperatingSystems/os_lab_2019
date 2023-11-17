#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>


int main() {
	pid_t child_pid = fork();
	if (child_pid > 0) {
		printf("pass parent step and no catching child before proccess exit\n");
	} else {
		exit(0);
	} return 0;
}
