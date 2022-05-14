all: tcpclient tcpserver udpclient udpserver

tcpserver : tcpserver.o 
	gcc -o tcpserver tcpserver.o 

tcpserver.o : tcpserver.c
	gcc -c tcpserver.c

tcpclient : tcpclient.o 
	gcc -o tcpclient tcpclient.o 

tcpclient.o : tcpclient.c
	gcc -c tcpclient.c 

udpserver : udpserver.o 
	gcc -o udpserver udpserver.o 

udpserver.o : udpserver.c
	gcc -c udpserver.c

udpclient : udpclient.o 
	gcc -o udpclient udpclient.o 

tcpclient.o : tcpclient.c
	gcc -c tcpclient.c 

clean :
	rm tcpclient.o tcpserver.o udpclient.o udpserver.o tcpclient tcpserver udpclient udpserver