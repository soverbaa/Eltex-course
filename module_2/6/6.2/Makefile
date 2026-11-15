CC = cc
CFLAGS = -std=c17 -Wall -Wextra -Werror -pedantic
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    LIB = libphonebook.dylib
    SHARED_FLAGS = -dynamiclib -fPIC -Wl,-install_name,@rpath/libphonebook.dylib
    RPATH_FLAGS = -Wl,-rpath,@loader_path
else
    LIB = libphonebook.so
    SHARED_FLAGS = -shared -fPIC -Wl,-soname,libphonebook.so
    RPATH_FLAGS = -Wl,-rpath,'$$ORIGIN'
endif

TARGET = phonebook
TEST_TARGET = tests

all: $(TARGET)

$(LIB): phonebook.c phonebook.h contact.c contact.h
	$(CC) $(CFLAGS) $(SHARED_FLAGS) phonebook.c contact.c -o $@

$(TARGET): main.c phonebook.h contact.h $(LIB)
	$(CC) $(CFLAGS) main.c -L. -lphonebook $(RPATH_FLAGS) -o $@

$(TEST_TARGET): tests.c phonebook.h contact.h $(LIB)
	$(CC) $(CFLAGS) tests.c -L. -lphonebook $(RPATH_FLAGS) -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET) libphonebook.a libphonebook.so libphonebook.dylib *.o

.PHONY: all test clean
