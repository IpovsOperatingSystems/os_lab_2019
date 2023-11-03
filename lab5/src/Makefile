CC=gcc
CFLAGS=-I.
LDFLAGS = -pthread

mutex: mutex.c
	$(CC) -o mutex mutex.c $(LDFLAGS)

withmutex: withmutex.c
	$(CC) -o withmutex withmutex.c $(LDFLAGS)

factorial_complile: factorial.c
	$(CC) -o factorial1 factorial.c $(LDFLAGS)

factorial: 
	./factorial1 -k 10 --pnum 4 --mod 1

deadlock: deadlock.c
	$(CC) -o deadlock deadlock.c $(LDFLAGS)
