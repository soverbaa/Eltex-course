
CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -Werror -pedantic

TARGET = lab3_2
TEST_TARGET = ipv4_tests

all: $(TARGET)

$(TARGET): main.c ipv4.c ipv4.h
	$(CC) $(CFLAGS) main.c ipv4.c -o $(TARGET)

$(TEST_TARGET): ipv4_tests.c ipv4.c ipv4.h
	$(CC) $(CFLAGS) ipv4_tests.c ipv4.c -o $(TEST_TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET)

.PHONY: all test clean
