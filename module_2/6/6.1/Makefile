CC = cc
CFLAGS = -std=c17 -Wall -Wextra -Werror -pedantic
AR = ar
ARFLAGS = rcs

LIB = libphonebook.a
TARGET = phonebook
TEST_TARGET = tests

all: $(TARGET)

$(LIB): phonebook.o contact.o
	$(AR) $(ARFLAGS) $@ $^

phonebook.o: phonebook.c phonebook.h contact.h
	$(CC) $(CFLAGS) -c phonebook.c -o $@

contact.o: contact.c contact.h
	$(CC) $(CFLAGS) -c contact.c -o $@

$(TARGET): main.c phonebook.h contact.h $(LIB)
	$(CC) $(CFLAGS) main.c -L. -lphonebook -o $@

$(TEST_TARGET): tests.c phonebook.h contact.h $(LIB)
	$(CC) $(CFLAGS) tests.c -L. -lphonebook -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET) $(LIB) *.o

.PHONY: all test clean
