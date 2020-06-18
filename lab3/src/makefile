CC=gcc
CFLAGS=-I. -Wall

all: parallel_min_max sequential_min_max ex5

sequential_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o sequential_min_max find_min_max.o utils.o sequential_min_max.c $(CFLAGS)

parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

ex5: ex5.o
	$(CC) -o ex5 ex5.o $(CFLAGS)

utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)

ex5.o:
	$(CC) -o ex5.o -c ex5.c $(CFLAGS)	

clean :
	rm utils.o find_min_max.o ex5.o sequential_min_max parallel_min_max ex5
