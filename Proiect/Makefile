CC=gcc
CFLAGS=-Wall -g

all: liblockers.a test

liblockers.o: liblockers.c lockers.h
	$(CC) $(CFLAGS) -c liblockers.c

liblockers.a: liblockers.o
	ar rcs liblockers.a liblockers.o

test: test.c
	$(CC) $(CFLAGS) -o $@ $^ -L. -llockers -pthread
