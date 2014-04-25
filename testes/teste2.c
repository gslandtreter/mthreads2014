
#include "../include/mthread.h" 

// Fastest job first.
// Como a thread 1 eh mais lenta, a thread 2 eh escalonada antes

void func0(int counter)
{
	int i = 0, n = 0;
	for(i = 0; i < counter; i++)
	{
		for(n = 0; n < counter * 10000; n++)
		{
			int x = 2 + 1;
		}

		printf("Thread %d\n", i);
		myield();	
	}
} 

int main(int argc, char *argv[]) 
{
	int loopTime = 30;
	int thread1 = mcreate(func0, loopTime);
	int thread2 = mcreate(func0, 2 * loopTime);

	mjoin(thread1);
	mjoin(thread2);

	printf("Done!\n");
}
