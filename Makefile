build:
	gcc -o socket_tcp socket_tcp.c -lws2_32
	gcc -o socket_udp socket_udp.c -lws2_32
	