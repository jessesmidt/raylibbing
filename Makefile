CC     = gcc
CFLAGS = -Wall -Iinclude -I/usr/include/lua5.4
LDFLAGS= -lraylib -lX11 -lm -lpthread -ldl -llua5.4

SRC = $(wildcard src/*.c)
OUT = build/dieding

all:
	mkdir -p build
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(OUT)

clean:
	rm -rf build

run:
	./build/dieding
