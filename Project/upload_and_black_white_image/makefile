CC=gcc
ARGS=-Wall -Wextra -std=c99
SRC=$(wildcard *.c)
SRC2=$(wildcard *.o)
SUBDIR=$(wildcard */)
DIRNAME=$(lastword $(subst /, ,${CURDIR}))
MAINFUNCT=yes

all:
	for i in $(SUBDIR); do ($(MAKE) -C $$i); done

ifeq ($(strip $(MAINFUNCT)),yes)
	$(MAKE) master
else
	$(MAKE) main
endif	
	$(MAKE) clean

master:
ifneq ($(strip $(SRC)),)
	$(CC) $(ARGS) -o main.exe $(SRC) $(SRC2)
endif

main:
ifneq ($(strip $(SRC)),)
	$(CC) $(ARGS) -c -o ../$(DIRNAME).o $(SRC) $(SRC2)
endif

clean:
ifneq ($(strip $(SRC2)),)
	rm *.o
endif
