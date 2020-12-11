# change application name here (executable output name)

TARGET=template_app

# compiler

CC=gcc

# debug

DEBUG=-g

# optimisation

OPT=-O0

# warnings

WARN=-Wall -Wextra -std=c99



PTHREAD=-pthread



CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe



GTKLIB=`pkg-config --cflags --libs gtk+-3.0`



# linker

LD=gcc

LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic -lm



OBJS=main.o



all: $(OBJS)

	$(LD) -o $(TARGET) ../main.c $(OBJS) $(LDFLAGS)

    

main.o: src/UIgtk.c

	$(CC) -c $(CCFLAGS) ../main.c src/UIgtk.c $(GTKLIB) -o main.o   

clean:
	rm -f *.o $(TARGET)
