CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -Werror -pedantic

all: priority_queue

priority_queue: main.c queue.c queue.h
	$(CC) $(CFLAGS) main.c queue.c -o priority_queue

test: tests.c queue.c queue.h
	$(CC) $(CFLAGS) tests.c queue.c -o test_queue
	./test_queue

clean:
	rm -f priority_queue test_queue *.o

.PHONY: all test clean
