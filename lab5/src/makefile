CC=gcc
CFLAGS=-I.

all: factorial_app mutex_app deadlock_app

deadlock_app :
	$(CC) -pthread -o deadlock_app deadlock.c $(CFLAGS)

factorial_app : 
	$(CC) -pthread -o factorial_app factorial.c $(CFLAGS)

mutex_app : 
	$(CC) -pthread -o mutex_app mutex.c $(CFLAGS)

clean :
	rm factorial_app mutex_app 
