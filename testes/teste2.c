/*
 * Programa exemplo de teste da biblioteca microthreads
 *
 * Versão 1 - 20/03/2014
 */

#include "../include/mthread.h" /*considera que este fonte está no diretório "testes" (vide seção 8)*/

mmutex_t mutex = { STATUS_FREE, NULL, NULL };

void func0(void *arg) {

     /*corpo da função func0 */
 	 int i = 0, x = 0;

 	 for (i = 0; i < 1000000; i++)
 		 x = i + 2;
 	 mmutex_init(&mutex);
 	 mlock(&mutex);
 	 myield();
 	 printf("Thread 1\n");
 	 munlock(&mutex);
	 
     return;    /*termina a execução da thread via comando C */
}

void func1(void *arg) {

     /*corpo da função func1 */
 	 myield();
 	 //mlock(&mutex); //Deve bloquear a thread pq o mutex ta rodando.
 	 //Ou dar segmentation fault por um motivo desconhecido
 	 //Acredito por o mutex ter sido setado no contexto 1.
 	 printf("Thread 2\n");
	 
}    /* termina a execução da thread ao chegar ao final da função naturalmente */

int main(int argc, char *argv[]) {
    int	id0, id1;
    int 	i;
    
	/* Código de execução */
	
    id0 = mcreate(func0, (void *)&i);
    id1 = mcreate(func1, (void *)&i);
	
	/* Código de execução */
	
    mjoin(id0);
	printf("Create retornou %d %d\n",id0, id1);
    mjoin(id1);
	printf("Create retornou %d %d\n",id0, id1);
	printf("Main :D\n");
}
