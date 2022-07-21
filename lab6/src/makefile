CC=gcc
CFLAGS=-I.

all: server client

server: server.c
$(CC)   -lpthread -L../multmodulo   -lmultmodulo -o ./server  $(CFLAGS)
	

client: client.c
$(CC)    -lpthread -L../multmodulo -lmultmodulo -o ./client  $(CFLAGS)

server.o : 
$(CC)  server.c -o server.o $(CFLAGS)

client.o : 
$(CC)  client.c -o client.o $(CFLAGS)

clean : 
	rm client server 