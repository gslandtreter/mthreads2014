all: teste1 teste2

teste1: teste1.c
	gcc -lrt -o ../bin/teste1 teste1.c -L../lib/  -lmthread -w
teste2: teste2.c
	gcc -lrt -o ../bin/teste2 teste2.c -L../lib/  -lmthread -w

clean:
	rm -rf ../bin/teste*
