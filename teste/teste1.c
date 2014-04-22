#include "../include/mthread.h"

mmutex_t mutex = { STATUS_FREE, NULL, NULL };

void func1()
{
	int i = 0, x = 0;

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
	myield();
	//mlock(&mutex); //Deve bloquear a thread pq o mutex ta rodando.
	//Ou dar segmentation fault por um motivo desconhecido
	//Acredito por o mutex ter sido setado no contexto 1.
	printf("Thread 2\n");
	return;
}

int main()
{
	int a = mcreate(func1,NULL);
	int b = mcreate(func2,NULL);

	mjoin(a);
	printf("Create retornou %d %d\n",a, b);
	myield();
	printf("Main :D\n");
	return 0;
}
