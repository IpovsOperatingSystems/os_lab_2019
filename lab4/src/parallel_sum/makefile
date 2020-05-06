CC=gcc
CFLAGS=-I. -Wall

all: parallel_sum

parallel_sum: utils.o
	$(CC) -o parallel_sum  -std=c99 -lpthread parallel_sum.c utils.o  $(CFLAGS)

utils.o: utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

clean: 
	rm parallel_sum utils.o		