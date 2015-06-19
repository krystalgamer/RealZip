CC = gcc
LIBS = -lz
CFLAGS = -I. -I/usr/local/include -Wall -std=gnu99 -Wall -std=gnu99
CFLAGS64 = -D_LARGEFILE64_SOURCE $(CFLAGS)

all: create

create: ioapi.o zip.o main.o
	$(CC) -o bin/realzip ioapi.o zip.o main.o $(LIBS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS64) $(LIBS)

clean:
	rm -rf *.o
