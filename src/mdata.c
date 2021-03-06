#include <stdio.h>
#include "../include/mdata.h"


ThreadList* createList(void)
{
	return NULL;
}

int showList(ThreadList *threadList)
{
    int length = 0;
    ThreadList *ptaux = threadList; //ponteiro auxiliar para percorrer a lista

    if (ptaux == NULL)
        return 0;

    else
    {
        printf("Lista, elementos: ");
        length++;
        while (ptaux != NULL)      	//procura o fim da lista
        {
            printf("%d ", ptaux->thTCB->tid);
            ptaux = ptaux->proximo;
            length++;
        }
    }
    printf("\n");
    return length;
}


int insertList(ThreadList **threadList, TCB *aThread)
{
	ThreadList *novo; //novo elemento
	int length = 1;

    novo = (ThreadList*) malloc(sizeof(ThreadList));
	novo->thTCB = aThread;
	novo->proximo = NULL;

	if (*threadList == NULL)
		*threadList = novo;

    else
	{
        ThreadList *ptAux;
        ptAux = *threadList; //ponteiro auxiliar para percorrer a lista

        if (aThread->executionTime < ptAux->thTCB->executionTime)
        {
            novo->proximo = ptAux;
            *threadList = novo;
            return length;
        }

        //Operador de comparacao eh maior ou igual para garantir FIFO
		while (ptAux->proximo != NULL /* && ptAux->proximo->thTCB->tid != 0 */ && aThread->executionTime >= ptAux->proximo->thTCB->executionTime)      	//procura o fim da lista
		{
			ptAux = ptAux->proximo;
			length++;
 		}

 		novo->proximo = ptAux->proximo;
		ptAux->proximo = novo;
	}
	return length;
}


TCB* getTCBById(ThreadList *threadList, int tid)
{
	ThreadList *ptaux;
    	ptaux = threadList; //ponteiro auxiliar para percorrer a lista

	while (ptaux != NULL)      	//procura o elemento na lista
	{
		if(ptaux->thTCB->tid == tid)
			return ptaux->thTCB;

		ptaux = ptaux->proximo;
	}
	return NULL;
}


TCB* removeList(ThreadList **threadList, int tid)
{
	ThreadList *ant = NULL; //ponteiro auxiliar para a posição anterior
	ThreadList *ptaux;
    ptaux = *threadList; //ponteiro auxiliar para percorrer a lista


    while (ptaux != NULL && ptaux->thTCB->tid != tid)      	//procura o elemento na lista
    {
		ant = ptaux;
		ptaux = ptaux->proximo;
 	}

    if (ptaux == NULL)  //se nao achou
		return NULL;

    if (ant == NULL)
	{
		*threadList = ptaux->proximo;  //Remove o primeiro elemento
		return ptaux->thTCB;
	}
	else
	{
		ant->proximo = ptaux->proximo;  //Remove o elemento desejado
    	return ptaux->thTCB;
	}
}


TCB* removeFirst(ThreadList **threadList)
{
	ThreadList *ptaux; //ponteiro auxiliar para salvar o endereco do primeiro
    ptaux = *threadList;

    if (*threadList == NULL)  //se nao achou
		return NULL;

    else
	{
		*threadList = (*threadList)->proximo;  //Remove o primeiro elemento
		return ptaux->thTCB;
	}
}


ThreadList* destroy(ThreadList *threadList)
{
    ThreadList *ptaux; //ponteiro auxiliar para percorrer a lista

    while(threadList != NULL)
    {
         ptaux = threadList;
         threadList = threadList->proximo;
         free(ptaux);
    }
    free(threadList);
    return NULL;

}

int* get_tid_object()
{
	static int tid = 0;
	return &tid;
}

int get_tid()
{
	return *(get_tid_object());
}

void set_tid(int value)
{
	*(get_tid_object()) = value;
}

int add_tid()
{
	return ++*(get_tid_object());
}
