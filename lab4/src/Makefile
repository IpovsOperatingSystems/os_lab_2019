CC=gcc
CFLAGS=-I.

up_parallel_min_max:
	./parallel_min_max --seed 2 --array_size 1000 --pnum 4 --timeout 10

utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)

parallel_min_max : utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

clean :
	rm utils.o find_min_max.o parallel_min_max

