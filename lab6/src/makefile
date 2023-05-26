.PHONY: all clean
# Это комментарий, который говорит, что переменная CC указывает компилятор, используемый для сборки
CC=gcc
#Это еще один комментарий. Он поясняет, что в переменной CFLAGS лежат флаги, которые передаются компилятору
CFLAGS= -I.

all : server client

server : server.c library.o library.h
	$(CC) -pthread -o server server.c library.o $(CFLAGS)

client : client.c library.o library.h
	$(CC) -pthread -o client client.c library.o $(CFLAGS)

library.o : library.h
	$(CC) -o library.o -c library.c $(CFLAGS)
# Цель clean. Она традиционно используется для быстрой очистки всех результатов сборки проекта.
# Очистка запускается так: make -f makefile clean
clean :
	rm server client library.o
