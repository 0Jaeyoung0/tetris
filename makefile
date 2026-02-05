CC = gcc
CFLAGS = -g -Wall

tetris: tetris.o
	$(CC) $(CFLAGS) -o tetris tetris.o

tetris.o: tetris.c
	$(CC) $(CFLAGS) -c tetris.c

clean:
ifeq ($(OS),Windows_NT)
	del tetris.exe *.o results.txt
else
	rm -f tetris *.o results.txt
endif

run:
ifeq ($(OS),Windows_NT)
	tetris.exe
else
	./tetris
endif