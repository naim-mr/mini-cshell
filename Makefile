EXEC=shell
SOURCES=main.c shell.c
OBJECTS=$(SOURCES:.c=.o)
CC=gcc
CFLAGS=-W -Wall -std=gnu99 -g
 
.PHONY: default clean
 
default: $(EXEC)

shell.o: shell.c shell.h
main.o: main.c shell.h

 
%.o: %.c
		$(CC) -o $@ -c $< $(CFLAGS)

$(EXEC): $(OBJECTS)
		$(CC) -o $@ $^

clean:
	rm -rf $(EXEC) $(OBJECTS) $(SOURCES:.c=.c~) $(SOURCES:.c=.h~) Makefile~