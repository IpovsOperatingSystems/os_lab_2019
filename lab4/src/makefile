CC=gcc
CFLAGS=-I.
all : parallel_min_max process_memory parsum


parsum: utils.o utils.h
	$(CC) -o parsum utils.o parallel_sum.c -lpthread -std=c99 $(CFLAGS)

process_memory : process_memory.c
	$(CC) -o process_memory process_memory.c -std=c99 $(CFLAGS)

parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o par.c -std=c99 $(CFLAGS)
	
utils.o : utils.h
	$(CC) -o utils.o -c utils.c -std=c99 $(CFLAGS)

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c -std=c99 $(CFLAGS)

clean :
	rm parallel_min_max process_memory 