CC=gcc -g -Wall
CLOG=compile.log

all: smallsh

smallsh: src/smallsh.c src/builtins.o src/exec.o
	script -q -c "$(CC) -o $@ $^" $(CLOG)

src/builtins.o: src/builtins.c src/builtins.h
	$(CC) -c -o $@ src/builtins.c

src/exec.o: src/exec.c src/exec.h
	$(CC) -c -o $@ src/exec.c

clean:
	rm -f smallsh $(CLOG)
	rm -f src/*.o
	rm -f *.dSYM

