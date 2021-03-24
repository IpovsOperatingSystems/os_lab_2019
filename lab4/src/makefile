CC=gcc
CFLAGS=-I.

all: zombie process_memory

process_memory: process_memory.o
	$(CC) -o process_memory process_memory.o

zombie: zombie.o
	$(CC) -o zombie zombie.o
	
process_memory.o: 
	$(CC) -o process_memory.o -c process_memory.c
	
zombie.o: 
	$(CC) -o zombie.o -c zombie.c

clean :
	rm *.o zombie process_memory
