CC=gcc
CFLAGS=-I.

all: client server
	clear

client : libfactorial
	$(CC) -c client.c -o client.o $(CFLAGS)
	$(CC) -o client client.o -L$(shell pwd) -lfactorial -lpthread $(CFLAGS)

server : libfactorial
	$(CC) -c server.c -o server.o $(CFLAGS)
	$(CC) -o server server.o -L$(shell pwd) -lfactorial -pthread $(CFLAGS)

libfactorial :
	$(CC) -o factorial.o -c factorial.c $(CFLAGS)
	ar rcs libfactorial.a factorial.o
	
clean :
	rm libfactorial.a factorial.o server.o client.o
	
clean_app: clean
	rm server client
