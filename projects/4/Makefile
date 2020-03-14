CC=gcc -Wall -g
CLOG=log

COMP=script -q -c "$(CC) -c -o $@ $<" $(CLOG)/$(@F)
LINK=script -q -c "$(CC) -o $@ $^" $(CLOG)/$(@F)

all: log keygen otp_enc_d otp_enc otp_dec_d otp_dec

otp_enc: src/otp_enc.c src/otp.o
	$(LINK)

otp_enc_d: src/otp_enc_d.c src/otp.o
	$(LINK)

otp_dec: src/otp_dec.c src/otp.o
	$(LINK)

otp_dec_d: src/otp_dec_d.c src/otp.o
	$(LINK)

keygen: src/keygen.c src/otp.o
	$(LINK)

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
	rm -f keygen otp_enc_d otp_enc otp_dec_d otp_dec
	rm -rf *.dSYM

