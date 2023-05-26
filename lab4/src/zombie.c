#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	pid_t p;
	p = fork();
	if (p==0)
	{
		printf("Child--PID %d\n",getpid());
		exit(0);
	}
	else
	{
		sleep(10);
		printf("Parents--PID %d\n",getpid());
		system("ps -eo pid,ppid,stat,cmd");  // prints a list of processes in your terminal
		printf("/n");
	}

	return 0;
}

