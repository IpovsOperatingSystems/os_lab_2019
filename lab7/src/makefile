.PHONY: all clean
# Это комментарий, который говорит, что переменная CC указывает компилятор, используемый для сборки
CC=gcc

all : tcpclient tcpserver udpclient udpserver

tcpclient : tcpclient.c
	$(CC) -o tcpclient tcpclient.c

tcpserver : tcpserver.c
	$(CC) -o tcpserver tcpserver.c

udpclient : udpclient.c
	$(CC) -o udpclient udpclient.c

udpserver : udpserver.c
	$(CC) -o udpserver udpserver.c

# Цель clean. Она традиционно используется для быстрой очистки всех результатов сборки проекта.
# Очистка запускается так: make -f makefile clean
clean :
	rm tcpclient tcpserver udpclient udpserver
