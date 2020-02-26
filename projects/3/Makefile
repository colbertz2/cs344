CC=gcc -g -Wall
CLOG=compile.log

all: smallsh

smallsh: src/smallsh.c
	script -q -c "$(CC) -o $@ $^" $(CLOG)

clean:
	rm -f smallsh $(CLOG)
	rm -f *.dSYM

