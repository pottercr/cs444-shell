
CFLAGS=-DUNIX -lreadline -lcurses -ansi -pedantic-errors
DEBUG=-g
#DEBUG=

all: print loop shell


shell:	shell.c parse.c parse.h
	gcc $(CFLAGS) $(DEBUG) shell.c parse.c -o shell
print:  print.c
	gcc $(CFLAGS) $(DEBUG) print.c -o print
loop:   loop.c
	gcc $(CFLAGS) $(DEBUG) loop.c -o loop
clean:
	rm -f shell loop print *~

