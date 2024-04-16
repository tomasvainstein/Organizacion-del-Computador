CC=c99
CFLAGS=-Wall -Wextra -pedantic -Wno-unused-variable -Wno-unused-parameter -Wfloat-conversion -O0 -ggdb -no-pie -lm

all: main tester game

main: main.c lib_c.o lib_asm.o
	$(CC) $(CFLAGS) $^ -o $@

tester: tester.c lib_c.o lib_asm.o
	$(CC) $(CFLAGS) $^ -o $@

game: game.c lib_c.o lib_asm.o
	$(CC) $(CFLAGS) $^ -o $@

lib_c.o: lib.c
	$(CC) $(CFLAGS) -c $< -o $@

lib_asm.o: lib.s
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o
	rm -f main tester game
	rm -f salida.propios.caso*
	rm -f gameResult*

