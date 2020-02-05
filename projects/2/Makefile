CC=gcc -g

all: buildrooms

buildrooms: src/buildrooms.c
	$(CC) -o $@ $^

clean:
	rm -f buildrooms
	rm -rf *.dSYM