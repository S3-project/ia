CC=gcc
ARGS=  -Wall -Wextra -std=c99
MATH= -lm

SRC=$(shell find -mindepth 2 -name '*.c' -not -name "main.c")
SRC += $(shell find -maxdepth 1 -name '*.c')
OBJ=$(shell find -name '*.o')
DIRS=$(shell find * -type d)

DIRNAME=$(lastword $(subst /, ,${CURDIR}))

all:
	$(MAKE) main

main:
	$(CC) $(ARGS) -o ./$(DIRNAME).exe $(ADD) $(OBJ) $(SRC) $(MATH)

copy:
	for i in $(DIRS); do(cp makefile $$i);done
	
clean:
	for i in $(OBJ);do(rm $$i);done

.SILENT:all main clean copy