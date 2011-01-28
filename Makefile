
CFLAGS=-DUNIX -lreadline -lcurses -ansi -pedantic-errors
DEBUG=-g
#DEBUG=

all: print shell


shell:	shell.c parse.c parse.h
	gcc $(CFLAGS) $(DEBUG) shell.c parse.c -o shell
print:  print.c
	gcc $(CFLAGS) $(DEBUG) print.c -o print
clean:
	rm -f shell print *~

