CC=gcc
CFLAGS=-I.
LDFLAGS = -lpthread

all: mutex_without_mutex mutex_with_mutex

mutex_without_mutex: mutex.c
	$(CC) mutex.c -o mutex_without_mutex $(LDFLAGS)

mutex_with_mutex: mutex.c
	$(CC) mutex.c -o mutex_with_mutex $(LDFLAGS)

clean:
	rm -f mutex_without_mutex mutex_with_mutex