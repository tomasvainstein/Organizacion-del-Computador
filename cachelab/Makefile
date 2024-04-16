CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c11 -O0 -ggdb -no-pie
LIBS:=-lm

all: cachesim

cachesim: cachesim.o cacheSimulador.o contador.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

cachesim.o: cachesim.c cacheSimulador.h contador.h
	$(CC) $(CFLAGS) -c $< -o $@

cacheSimulador.o: cacheSimulador.c cacheSimulador.h
	$(CC) $(CFLAGS) -c $< -o $@

contador.o: contador.c contador.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o
	rm -f cachesim
