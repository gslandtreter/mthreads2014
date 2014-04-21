#include "../include/mthread.h"

/*
	Criação de thread simples
*/

void func1()
{
	int i = 0, x = 0;

	for (i = 0; i < 1000000; i++)
		x = i + 2;

	myield();
	printf("Thread 1\n");
	return;
}

void func2()
{
	myield();
	printf("Thread 2\n");
	return;
}

int main()
{
	int a = mcreate(func1,NULL);
	int b = mcreate(func2,NULL);

	printf("Create retornou %d %d\n",a, b);
	myield();
	printf("Main :D\n");
	return 0;
}
