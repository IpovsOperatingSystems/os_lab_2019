CC=gcc
CFLAGS=-I. -Wall

all: zombie process_memory

zombie:
	$(CC) -o zombie zombie.c $(CFLAGS)

process_memory: 
	$(CC) -o process_memory process_memory.c $(CFLAGS)

clean:
	rm zombie process_memory