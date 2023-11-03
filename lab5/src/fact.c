#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <getopt.h>
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
typedef struct
{
uint64_t begin;
uint64_t current;
uint64_t end;
uint64_t mod;
} Arguments;
uint64_t factorial(Arguments* arg)
{
while (arg->begin < arg->end)
{
 arg->begin++;
 arg->current *= arg->begin % arg->mod;
}
 arg->current %= arg->mod;
 printf("Res in thread: %d\n", arg->current);
return arg->current;
}
int main(int argc, char **argv)
{
int k = 0, pnum = 0, mod = 0;
while (true) {
int current_optind = optind ? optind : 1;
static struct option options[] = {{"k", required_argument, 0, 0},
 {"pnum", required_argument, 0, 0},
 {"mod",required_argument,0,0},
 {0, 0, 0, 0}};
int option_index = 0;
int c = getopt_long(argc, argv, "", options, &option_index);
if (c == -1) break;
switch (c) {
case 0:
switch (option_index) {
case 0:
 k = atoi(optarg);
 // your code here
 // error handling
 if (k <= 0) {
 printf("k is a positive number\n");
 return 1;
}
 break;
case 1:
pnum = atoi(optarg);
// your code here
// error handling
if (pnum <= 0) {
printf("pnum is a positive number\n");
return 1;
}
break;
case 2:
mod = atoi(optarg);
// your code here
// error handling
if (mod <= 0) {
printf("mod is a positive number\n");
return 1;
}
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
if (optind < argc) {
printf("Has at least one no option argument\n");
return 1;
}
if (k == -1 || mod == -1 || pnum == -1)
{
printf("Usage: %s —k \"num\" —pnum \"num\" —mod \"num\"\n", argv[0]);
return 1;
}
pthread_t threads[pnum];
uint64_t part = k/pnum;
Arguments args[pnum];
for (uint32_t i = 0; i < pnum; i++)
{
args[i].begin = i*part;
args[i].current = 1;
args[i].mod = mod;
if (i == (pnum - 1))
{
args[i].end = k;
}
else
{
args[i].end = (i+1)*part;
}
if (pthread_create(&threads[i], NULL, factorial, (void *)&args[i]))
{
printf("Thread create failed!!!\n");
return 1;
}
}
uint64_t result = 1;
for (uint32_t i = 0; i < pnum; i++)
{
uint64_t btwn_result = 1;
pthread_join(threads[i], (void **)&btwn_result);
pthread_mutex_lock(&mut);
result *= btwn_result;
result %= mod;
pthread_mutex_unlock(&mut);
}
printf("Result: %d \n",result);
return 0;
}