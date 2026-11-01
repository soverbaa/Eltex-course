CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -Werror -pedantic

all: calculator

calculator: main.c calc.c calc.h
	$(CC) $(CFLAGS) main.c calc.c -o calculator

test: tests.c calc.c calc.h
	$(CC) $(CFLAGS) tests.c calc.c -o test_calculator
	./test_calculator

clean:
	rm -f calculator test_calculator *.o

.PHONY: all test clean
