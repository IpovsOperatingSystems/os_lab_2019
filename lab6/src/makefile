CC = gcc
CFLAG = -I.

all : server client

server :
	$(CC)  server.c -lpthread -L../multmodulo -lmultmodulo -o ./server $(CFLAG)

client : 
	$(CC)  client.c -lpthread -L../multmodulo -lmultmodulo -o ./client $(CFLAG)

server.o :
	$(CC) server.c -o server.o $(CFLAG)

client.o : 
	$(CC) client.c -o client.o $(CFLAG)

clean :
	rm server client
