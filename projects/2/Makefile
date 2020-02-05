CC=gcc -g

all: buildrooms

buildrooms: src/colbertz.buildrooms.c
	$(CC) -o $@ $^

clean:
	rm -f buildrooms
	rm -rf *.dSYM
	rm -rf colbertz.rooms.*