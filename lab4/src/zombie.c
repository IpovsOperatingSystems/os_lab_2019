
   
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(){

    pid_t zombie;
    for (int i=0;i<5;i++)
    {

        sleep(1);
        zombie = fork();
        printf("\nPID: %d\t",zombie);
        if(zombie == 0) {
            sleep(2);
            printf("\n zombie %d",i);    //Мы в ветке потомка
            exit(0);
        }
        if(zombie > 0){
            printf("\n родитель %d",i);    //Мы в ветке родителя
        }
        if(zombie == -1){
            printf("\nОшибка %d", i);
        }
    }

    return 0;
}