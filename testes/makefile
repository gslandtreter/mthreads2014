all: teste1 teste2

teste1: teste1.c
	gcc -o teste1 teste1.c -L../lib/  -lmthread -w
teste2: teste2.c
	gcc -o teste2 teste2.c -L../lib/  -lmthread -w

clean:
	rm -rf teste1 teste2
