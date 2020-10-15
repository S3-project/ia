CC=gcc
ARGS=-Wall -Wextra _std=c99

SRC=$(wildcard *.c)
SUBDIR=$(wildcard */)
DIR=$(shell echo ${PWD##*/}


all: 
	for i in $(SUBDIR); do (cd $$i; $(MAKE) all); done
	@if [$(SRC) != ""]; then\
	       	$(MAKE) main;\
	fi
main:
	$(CC) $(ARGS) -c -o ../$(DIR).o $(SRC)

clean:
	rm *.o
