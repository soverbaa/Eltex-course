CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -Werror -pedantic

TARGET = lab3_1
TEST_TARGET = test_permissions

SOURCES = main.c permissions.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

main.o: main.c permissions.h
	$(CC) $(CFLAGS) -c main.c -o main.o

permissions.o: permissions.c permissions.h
	$(CC) $(CFLAGS) -c permissions.c -o permissions.o

$(TEST_TARGET): test_permissions.c permissions.c permissions.h
	$(CC) $(CFLAGS) test_permissions.c permissions.c -o $(TEST_TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET) $(TEST_TARGET)

.PHONY: all test clean
