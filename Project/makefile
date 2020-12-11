CC=gcc
ARGS=-Wall -Wextra -std=c99
GTK= -pthread -pipe
CCFLAGS=$(ARGS) $(GTK)
LDFLAGS=-pthread `pkg-config --cflags --libs gtk+-3.0` -export-dynamic -lm

SRC=$(shell find -mindepth 2 -name '*.c' -not -name "main.c")
SRC += $(shell find -maxdepth 1 -name '*.c')
OBJ=$(shell find -name '*.o')
DIRS=$(shell find * -type d)
EXE=$(shell find -mindepth 2 -name '*.out')
EXE +=$(shell find -mindepth 2 -name '*.exe')

DIRNAME=$(lastword $(subst /, ,${CURDIR}))

all:
	$(MAKE) main

main:
	$(CC) $(CCFLAGS) -o ./$(DIRNAME).exe $(ADD) $(OBJ) $(SRC) $(LDFLAGS)

copy:
	for i in $(DIRS); do(cp makefile $$i);done
	
clean:
	for i in $(OBJ);do(rm $$i);done
	for i in $(EXE);do(rm $$i);done

.SILENT:all main clean copy