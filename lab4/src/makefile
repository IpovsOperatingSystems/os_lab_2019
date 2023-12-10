CC=gcc
CFLAGS=-I.

all: zombie process_memory parallel_sum

parallel_sum: utils.o
	$(CC) -pthread -o parallel_sum utils.o parallel_sum.c $(CFLAGS)

process_memory:
	$(CC) -o process_memory process_memory.c $(CFLAGS)

zombie:
	$(CC) -o zombie zombie.c $(CFLAGS)

parallel_min_max : utils.o find_min_max.o
	$(CC) -o parallel_min_max utils.o find_min_max.o task1/parallel_min_max.c $(CFLAGS)

find_min_max.o : task1/find_min_max.h
	$(CC) -o find_min_max.o -c task1/find_min_max.c $(CFLAGS)

utils.o: task1/utils.h
	$(CC) -o utils.o -c task1/utils.c $(CFLAGS)

clean :
	rm *.o *.a zombie process_memory parallel_sum