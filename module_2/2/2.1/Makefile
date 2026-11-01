CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -Werror -pedantic

all: phonebook

phonebook: main.c contact.c phonebook.c contact.h phonebook.h
	$(CC) $(CFLAGS) main.c contact.c phonebook.c -o phonebook

test: tests.c contact.c phonebook.c contact.h phonebook.h
	$(CC) $(CFLAGS) tests.c contact.c phonebook.c -o test_phonebook
	./test_phonebook

clean:
	rm -f phonebook test_phonebook *.o

.PHONY: all test clean
