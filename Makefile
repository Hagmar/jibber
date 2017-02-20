CC=gcc
CFLAGS=-Wall

all: jibi.c
	$(CC) $(CFLAGS) -o jibi jibi.c
