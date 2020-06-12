CC=gcc
CFLAGS=-I.

all : client servers

client :
	$(CC) -w -o client client.c $(CFLAGS) -pthread -std=c99

servers :
	mkdir servers
	$(CC) -w server.c MultModulo.c -o server1 $(CFLAGS) -pthread -std=c99
	cp server1 server2

libMod :
	gcc -c -fPIC MultModulo.c -o mod.o
	gcc -shared mod.o -o libMod.so
	export LD_LIBRARY_PATH=/projects/ITu-Dev-os_lab_2019/lab6/src/servers
	
clean :
	rm server1 server2
	rm -r servers
	rm client