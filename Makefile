
CFLAGS=-DUNIX -lreadline -lcurses -ansi -pedantic-errors
DEBUG=-g
#DEBUG=

all: shell


shell:	shell.c parse.c parse.h
	gcc $(CFLAGS) $(DEBUG) shell.c parse.c -o shell
clean:
	rm -f shell *~

