#makefile
CC = g++ -Wall
LDFLAGS = -pthread
all: server

server : server.o
				$(CC) $(LDFLAGS) -o server server.o

server_main.o: server_main.c
				$(CC) $(LDFLAGS) -c server.cpp

clean:
				rm -f *.o core.* server client
