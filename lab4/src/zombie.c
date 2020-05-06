#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>

  

int main()

{

    int i;

    int pid = fork();

  

    if (pid == 0)

    {

        for (i=0; i<5; i++)

            printf("Child\n");

    }

    else

    {

        printf("Parent\n");

        while(1);

    }

}