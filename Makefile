CC=gcc
CFLAGS=-Wall -fPIC -Iinclude
LDFLAGS=-shared -lraylib -lX11 -lm -lpthread -ldl

PYTHON := python3
SCRIPT = scripts/test.py

SRC=$(wildcard src/*.c)
OUT=build/libengine.so

all:
	mkdir -p build
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(OUT)

clean:
	rm -rf build

run:
	$(PYTHON) $(SCRIPT)
