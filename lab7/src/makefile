CC=gcc
CFLAGS=-I.
all : tcp_c tcp_s udp_c udp_s
tcp : tcp_c tcp_s
udp : udp_c udp_s

tcp_c : 
	$(CC) -o tcp_c tcpclient.c $(CFLAGS)

tcp_s : 
	$(CC) -o tcp_s tcpserver.c $(CFLAGS)
	
udp_c : 
	$(CC) -o udp_c udpclient.c $(CFLAGS)

udp_s : 
	$(CC) -o udp_s udpserver.c $(CFLAGS)

clean :
	rm -f tcp_c tcp_s udp_c udp_s