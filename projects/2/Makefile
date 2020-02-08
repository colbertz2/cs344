CC=gcc -g -Wall

all: buildrooms adventure

buildrooms: src/colbertz.buildrooms.c
	$(CC) -o $@ $^

adventure: src/colbertz.adventure.c
	$(CC) -o $@ $^ -lpthread

clean:
	rm -f buildrooms adventure currentTime.txt
	rm -rf *.dSYM
	rm -rf colbertz.rooms.*
