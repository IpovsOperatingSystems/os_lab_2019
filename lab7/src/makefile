CC=gcc
CFLAGS=-I.
all : tcpcli tcpserv udpcli udpserv
tcp : tcpcli tcpserv
udp : udpcli udpserv

tcpcli : 
	$(CC) -o tcpcli tcpclient.c $(CFLAGS)

tcpserv : 
	$(CC) -o tcpserv tcpserver.c $(CFLAGS)
	
udpcli : 
	$(CC) -o udpcli udpclient.c $(CFLAGS)

udpserv : 
	$(CC) -o udpserv udpserver.c $(CFLAGS)

clean :
	rm -f tcpcli tcpserv udpcli udpserv