CC=arm-linux-gnueabihf-gcc
LDFLAGS=-L ./lib -lshbf -lshbfev


all: server.c
	$(CC) server.c $(LDFLAGS) -o server

clean:
	$(RM) server
