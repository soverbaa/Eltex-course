CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -Werror -pedantic
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    LIB_EXT = dylib
    PLUGIN_FLAGS = -dynamiclib -fPIC
    DL_LIBS =
else
    LIB_EXT = so
    PLUGIN_FLAGS = -shared -fPIC
    DL_LIBS = -ldl
endif

PLUGIN_NAMES = add sub mul div
PLUGIN_LIBS = $(addprefix plugins/,$(addsuffix .$(LIB_EXT),$(PLUGIN_NAMES)))

all: calculator $(PLUGIN_LIBS)

calculator: main.c calc.c plugin_loader.c calc.h
	$(CC) $(CFLAGS) main.c calc.c plugin_loader.c -o calculator $(DL_LIBS)

plugins/%.$(LIB_EXT): plugins/%.c plugin_api.h
	$(CC) $(CFLAGS) $(PLUGIN_FLAGS) $< -o $@

test: all
	$(CC) $(CFLAGS) tests.c calc.c plugin_loader.c -o test_calculator $(DL_LIBS)
	./test_calculator ./plugins

clean:
	rm -f calculator test_calculator plugins/*.dylib plugins/*.so

.PHONY: all test clean
