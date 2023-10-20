CC=gcc
CFLAGS=-I.

all: build_bin task1

build_bin:
	$(CC) find_min_max.c parallel_min_max.c utils.c -o task1

task1: build_bin
	./task1 --seed 42 --array_size 100 --pnum 5 --timeout 1

zombie: zombie.c
	$(CC) -o zombie zombie.c

zombie_print:
	./zombie &
	ps -l

process_memory : process_memory.c
	$(CC) -o process_memory process_memory.c $(CFLAGS)

psum : Summa.o utils.o Summa.h utils.h
	$(CC) -o psum utils.o Summa.o parallel_sum.c -lpthread $(CFLAGS)

psum_print: psum
	./psum --seed 4 --array_size 1000 --threads_num 4
	
clean :
	rm task1 zoombie