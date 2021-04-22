#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <getopt.h>

struct Args{
    int step1;
    int step2;
};

int factorial=1;

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

void factor1(struct Args* mas){
   int  step1=-1,step2=-1;
   
       step1=mas->step1;
       step2=mas->step2;
//3 step1 4 step2
    
int i;
//printf("p%d %d\n",step1,step2);
for(i=step1;i<=step2;i++){
    pthread_mutex_lock(&mut);
    factorial*=i;
    pthread_mutex_unlock(&mut);
}

}

/*void factor(int** mas){

    pthread_mutex_lock(&mut);
    while(*mas[0]>*mas[3]){
        *mas[1]=(*mas[3]%*mas[2])*(*mas[1]);
    (*mas[3])++;
    }
    pthread_mutex_unlock(&mut);
  

}*/

int main(int argc, char **argv) {
    int k=6,pnum=2,mod=3;
int stepres=1, step=1,res=1;
  while (true) {
    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {0, 0, 0, 0}};
 
    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);
    if (c == -1) break;
    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            k = atoi(optarg)+1;
            break;
          case 1:
            pnum = atoi(optarg);
            break;
          case 2:
            mod = atoi(optarg);
            
            break;
          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case '?':
        break;
 
      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }
int step1,step2;
  pthread_t threadi[pnum];  
    int** mas=malloc(sizeof(int*)*4);
    mas[0]=&k;
    mas[1]=&stepres;
    mas[2]=&mod;
    mas[3]=&step;

    int** mas1=malloc(sizeof(int*)*5);
    mas1[0]=&k;
    mas1[1]=&stepres;
    mas1[2]=&mod;
    mas1[3]=&step1;
    mas1[4]=&step2;

    struct Args mas2[pnum];
    
    //printf("ff");
    int i;
    for(i=0;i<pnum;i++){
        if(i==pnum-1){
            step1=i*(k/pnum)+1;
            step2=k-1;
        }
        else{
        step1=i*(k/pnum)+1;
        step2=(i+1)*(k/pnum);
        }
       // printf("%d %d\n",step1,step2);
        mas2[i].step1=step1;
        mas2[i].step2=step2;
    //sleep(1);
        if (pthread_create(&threadi[i], NULL, (void *)factor1,
			        &mas2[i]) != 0) {
    perror("pthread_create");
    exit(1);}
    }
    for(i=0;i<pnum;i++){
        if (pthread_join(threadi[i], NULL) != 0) {
    perror("pthread_join");
    exit(1);
    }
    }

    
  
printf("\n%d\n",factorial%mod);
    free(mas);
  return 0;
}

