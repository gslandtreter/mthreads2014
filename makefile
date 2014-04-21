all: tid.o listas.o mthread.o
	ar crs lib/libmthread.a bin/mthread.o bin/listas.o bin/tid.o

tid.o: src/tid.c
	gcc -o bin/tid.o -c src/tid.c

mthread.o: src/mthread.c
	gcc -ltr -o bin/mthread.o -c src/mthread.c

listas.o: src/listas.c
	gcc -o bin/listas.o -c src/listas.c

clean:
	rm  lib/*.a bin/*.o
