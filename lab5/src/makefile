CC=gcc
CFLAGS=-I.

all: factorial_app mutex_app

factorial_app : 
	$(CC) -pthread -o factorial_app factorial.c $(CFLAGS)

mutex_app : 
	$(CC) -pthread -o mutex_app mutex.c $(CFLAGS)

clean :
	rm factorial_app mutex_app 
