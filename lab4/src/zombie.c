#include <stdio.h>
#include <stdlib.h>


int main()
{
    int id;
    id = fork();
    if (id > 0)
    {   
        printf("Parent process, id: %d\n", id);
        sleep(60);
    }
    else
    {
         printf("Child process, id: %d\n", id);
    }
}