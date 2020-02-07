CC=gcc -g -Wall

all: buildrooms adventure

buildrooms: src/colbertz.buildrooms.c
	$(CC) -o $@ $^

adventure: src/colbertz.adventure.c
	$(CC) -o $@ $^

clean:
	rm -f buildrooms adventure
	rm -rf *.dSYM
	rm -rf colbertz.rooms.*
