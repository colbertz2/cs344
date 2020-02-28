CC=gcc -g -Wall
CLOG=log

COMP=script -q -c "$(CC) -c -o $@ $<" $(CLOG)/$(@F)
LINK=script -q -c "$(CC) -o $@ $^" $(CLOG)/$(@F)

all: log smallsh

smallsh: src/smallsh.c src/builtins.o src/exec.o
	$(LINK)

src/builtins.o: src/builtins.c src/builtins.h
	$(COMP)

src/exec.o: src/exec.c src/exec.h
	$(COMP)

log:
	mkdir log

clean:
	rm -f smallsh src/*.o log/*
	rm -rf *.dSYM
