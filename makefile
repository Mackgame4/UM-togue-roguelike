CFLAGS=-Wall -Wextra -pedantic -O2
LIBS=-lm -lcurses
#LIBS=-lm -lncurses
#CC=gcc

jogo: main.o
	$(CC) $(CFLAGS) -o build/$@ $< $(LIBS)

clean:
	rm build/jogo *.o

rebuild: clean jogo