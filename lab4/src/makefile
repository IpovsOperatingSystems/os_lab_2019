# Это комментарий, который говорит, что переменная CC указывает компилятор, используемый для сборки
CC=gcc
#Это еще один комментарий. Он поясняет, что в переменной CFLAGS лежат флаги, которые передаются компилятору
CFLAGS=-I.

all : parallel_min_max zombie process_memory

zombie : zombie.c
	$(CC) -o zombie zombie.c

process_memory : process_memory.c
	$(CC) -o process_memory process_memory.c

parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

utils.o : utils.h
	$(CC) -o utils.o -c utils.c

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)

parallel_sum.o : parallel_sum.c
	$(CC) -o parallel_sum.o -c parallel_sum.c  $(CFLAGS)
	
parallel_sum.out : parallel_sum.o utils.o utils.h
	$(CC) -o parallel_sum.out parallel_sum.o utils.o -pthread $(CFLAGS)

# Цель clean. Она традиционно используется для быстрой очистки всех результатов сборки проекта.
# Очистка запускается так: make -f makefile clean
clean :
	rm zombie process_memory parallel_min_max utils.o find_min_max.o
