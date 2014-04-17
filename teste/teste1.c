#include "../include/mthread.h"

/*
	Criação de thread simples
*/

void func1()
{
	printf("Thread 1\n");
	return;
}

int main()
{
	int a = mcreate(func1,NULL);
	printf("Create retornou %d \n",a);
	myield();
	printf("Main :D\n");
	return 0;
}
