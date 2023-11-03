CC=gcc
CFLAGS=-I.

task1: 
	./parallel_min_max --seed 42 --array_size 100 --pnum 5 --timeout 20

zombie: zombie.c
	$(CC) -o zombie zombie.c

zombie_print:
	./zombie &
	ps -l

cleanzombie:
	rm zombie

process_memory : process_memory.c
	$(CC) -o process_memory process_memory.c $(CFLAGS)

psum : Summa.o utils.o Summa.h utils.h
	$(CC) -o psum utils.o Summa.o parallel_sum.c -lpthread $(CFLAGS)

psum_print: psum
	./psum --seed 4 --array_size 1000 --threads_num 4

up_parallel_min_max:
	./parallel_min_max --seed 2 --array_size 1000 --pnum 4 --timeout 10

utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

Summa.o : Summa.h
	$(CC) -o Summa.o -c Summa.c $(CFLAGS)

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)

parallel_min_max : utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

clean :
	rm utils.o find_min_max.o parallel_min_max

