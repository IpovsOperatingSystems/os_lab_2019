CC=gcc
CFLAGS=-I.
LDFLAGS = -lpthread

mutex: mutex.c
	$(CC) -o mutex mutex.c $(LDFLAGS)

withmutex: withmutex.c
	$(CC) -o withmutex withmutex.c $(LDFLAGS)

factorial_complile: factorial.c
	$(CC) -o factorial factorial.c $(LDFLAGS)

factorial:
	./factorial --k 10 --pnum 4 --mod 10
