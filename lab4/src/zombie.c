#include<stdlib.h>
#include<stdio.h>
#include <sys/types.h>

int main(){
pid_t pid=fork();
if(pid>0){
printf("parent\n");
sleep(50);
}else
printf("child\n");
}