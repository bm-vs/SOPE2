CC = gcc
CFLAGS = -D_REENTRANT -lpthread -Wall -pthread

all: clean parque gerador

parque: parque.c
	$(CC) Parque.c -o bin/Parque $(CFLAGS)

gerador: gerador.c
	$(CC) Gerador.c -o bin/Gerador $(CFLAGS)

clean:
	rm Gerador
	rm Parque
