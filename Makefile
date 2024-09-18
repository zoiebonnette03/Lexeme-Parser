CC = gcc
CFLAGS = -Wall

all: dcooke_parser

dcooke_parser: front.c parser.c
	$(CC) $(CFLAGS) -o dcooke_parser front.c parser.c

clean:
	rm -f dcooke_parser