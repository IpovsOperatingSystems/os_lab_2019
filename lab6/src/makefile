CC=gcc
CFLAGS=-I.
all : client server
client : factorial.o factorial.h
	$(CC) -o client factorial.o client.c -lpthread $(CFLAGS)

server : factorial.o factorial.h
	$(CC) -o server factorial.o server.c -lpthread $(CFLAGS)
	
factorial.o : factorial.h
	$(CC) -o factorial.o -c factorial.c $(CFLAGS)

clean :
	rm -f client server factorial.o