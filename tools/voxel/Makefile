CC     = gcc
CFLAGS = -Wall -I. -I/usr/local/include
LDFLAGS= -L/usr/local/lib -lraylib -lX11 -lm -lpthread -ldl

SRC  = main.c voxel_grid.c camera.c picking.c
OUT  = build/voxel_tool

all: build $(OUT)

build:
	mkdir -p build

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(OUT)

clean:
	rm -rf build

run: all
	./$(OUT)
