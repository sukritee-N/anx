CC=gcc
CFLAGS=-Wall -Wextra -pedantic
INC_FLAGS=-I extern

.build/anx: .build source/start.c
	$(CC) $(CFLAGS) $(INC_FLAGS) source/start.c -o .build/anx

.build:
	mkdir -p .build

run: .build/anx
	./.build/anx
