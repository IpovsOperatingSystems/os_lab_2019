CC=gcc
CFLAGS=-I.

all: sequential_min_max parallel_min_max utils.o

parallel_sum: parallel_sum.c utils.o utils.h
	$(CC) -o parallel_sum.o utils.o  parallel_sum.c $(CFLAGS) -pthread

sum.o: sum.h
	$(CC) -o sum.o -c sum.c $(CFLAGS)

process_memory: process_memory.c
	$(CC) -o process_memory.o process_memory.c $(CFLAGS)

sequential_min_max: utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o sequential_min_max.o find_min_max.o utils.o sequential_min_max.c $(CFLAGS)

parallel_min_max: utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max.o utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)

clean :
	rm *.o 
