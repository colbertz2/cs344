CC=gcc -Wall -g
CLOG=log

COMP=script -q -c "$(CC) -c -o $@ $<" $(CLOG)/$(@F)
LINK=script -q -c "$(CC) -o $@ $^" $(CLOG)/$(@F)

all: log example src/otp.o

src/otp.o: src/otp.c src/otp.h
	$(COMP)

example: ex-client ex-server

ex-client: src/start/client.c
	$(LINK)

ex-server: src/start/server.c
	$(LINK)

log:
	mkdir log

clean:
	rm -f ex-client ex-server src/*.o log/*
	rm -rf *.dSYM

