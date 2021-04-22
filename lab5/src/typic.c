#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <getopt.h>


pthread_mutex_t mut1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mut2 = PTHREAD_MUTEX_INITIALIZER;
int q= 0, w= 0;

void factor1(int *args){
    pthread_mutex_lock(&mut1);
    printf("fact1: lock q\n");
    sleep(2);
    q++;
    pthread_mutex_lock(&mut2);
    printf("fact1 w\n");
    w+= q;
    pthread_mutex_unlock(&mut2);
    printf("fact1 w\n");
    pthread_mutex_unlock(&mut1);
    printf("fact1 q\n");

}

void factor2(int *args){
    pthread_mutex_lock(&mut2);
    printf("fact2: lock w\n");
    sleep(2);
    q++;
    pthread_mutex_lock(&mut1);
    printf("fact2\n");
    q += w;
    pthread_mutex_unlock(&mut2);
    printf("unlock w\n");
    pthread_mutex_unlock(&mut1);
    printf("unlock q\n");
}

int main(int argc, char **argv) {
    int k=6,pnum=2,mod=3;
int stepres=1, step=1,res=1;
 
  pthread_t threadi[pnum];
    //int common;
    //printf("f");   
    /*int** mas=malloc(sizeof(int*)*4);
    mas[0]=&k;
    mas[1]=&stepres;
    mas[2]=&mod;
    mas[3]=&step;*/
    //printf("ff");
    int i;
    
        if (pthread_create(&threadi[0], NULL, (void *)factor1,
			        NULL) != 0) {
    perror("pthread_create");
    exit(1);}
        if (pthread_create(&threadi[1], NULL, (void *)factor2,
			        NULL) != 0) {
    perror("pthread_create");
    exit(1);}


    for(i=0;i<pnum;i++){
        if (pthread_join(threadi[i], NULL) != 0) {
    perror("pthread_join");
    exit(1);
    }
    }
  /*
printf("\n%d\n",*mas[1]);
    free(mas);*/
  return 0;
}

