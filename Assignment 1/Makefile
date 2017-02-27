CCFLAGS = gcc -Wall -g -o

all: memgrind

memgrind: memgrind.c mymalloc.c mymalloc.h
	$(CCFLAGS) memgrind memgrind.c mymalloc.c

clean:
	rm memgrind