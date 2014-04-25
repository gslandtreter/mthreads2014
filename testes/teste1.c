#include "../include/mthread.h"

mmutex_t mutex = { STATUS_FREE, NULL, NULL };

void func1()
{
	int i = 0, x = 0;

	//Doing loops takes a long time
	for (i = 0; i < 1000000; i++)
		x = i + 2;

	mmutex_init(&mutex);
	mlock(&mutex);

	myield();

	printf("Thread 1\n");
	munlock(&mutex);
	return;
}

void func2()
{
	//Mesmo dando yield, deve voltar para thread2, pois thread1 eh mais demorada
	myield();
	printf("Thread 2\n");
	return;
}

int main()
{
	int a = mcreate(func1,NULL);
	int b = mcreate(func2,NULL);

	mjoin(a);
	printf("Create - %d %d\n",a, b);
	myield();
	printf("MaiThread\n");
	mjoin(b);
	return 0;
}
