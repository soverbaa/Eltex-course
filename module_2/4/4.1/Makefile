CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -Werror -pedantic

TARGET = phonebook
TEST_TARGET = tests

all: $(TARGET)

$(TARGET): main.c contact.c phonebook.c contact.h phonebook.h
	$(CC) $(CFLAGS) main.c contact.c phonebook.c -o $(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): tests.c contact.c phonebook.c contact.h phonebook.h
	$(CC) $(CFLAGS) tests.c contact.c phonebook.c -o $(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET) *.o

.PHONY: all test clean
