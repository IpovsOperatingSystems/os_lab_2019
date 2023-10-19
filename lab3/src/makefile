CC=gcc
CFLAGS=-I.

sequential_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o sequential_min_max find_min_max.o utils.o sequential_min_max.c $(CFLAGS)

parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)

clean :
	rm utils.o find_min_max.o sequential_min_max parallel_min_max task3 task4

task4: sequential_min_max
	$(CC) -o task4 task4.c
	./task4


# Запуск 2-3 задания
build_bin:
	$(CC) find_min_max.c parallel_min_max.c utils.c -o task3 

up_by_files: build_bin
	./task3 --seed 2 --array_size 1000 --pnum 4 --by_files

up_by_pipes: build_bin
	./task3 --seed 2 --array_size 1000 --pnum 4

all: build_bin up_by_files up_by_pipes
