CC=gcc
ARGS=-Wall -Wextra -std=c99

SRC=$(shell find . -name '*.c')
OBJ=$(shell find . -name '*.o')
DIRS=$(shell find * -type d)

DIRNAME=$(lastword $(subst /, ,${CURDIR}))

all:
	for i in $(DIRS); do(cp makefile $$i);done
	$(MAKE) main
	$(MAKE) clean

main:
	$(CC) $(ARGS) -o ./$(DIRNAME).exe $(ADD) $(OBJ) $(SRC) 

clean:
	for i in $(OBJ);do(rm $$i);done

.SILENT:all main clean