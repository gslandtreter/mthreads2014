#include <stdio.h>
#include "../include/listas.h"
#define NULL 0


ThreadList* createList(void)
{
	return NULL;
}


int showList(ThreadList *threadList)
{
    int length = 0;
    ThreadList *ptaux = threadList; //ponteiro auxiliar para percorrer a lista
    //printf("ptaux: %d\n", ptaux);

    if (ptaux == NULL)
    {
        return length;
    }
    else
    {
        length++;
        while (ptaux->proximo != NULL)      	//procura o fim da lista
        {
             ptaux = ptaux->proximo;
             length++;
        }
    }
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
	{
		*threadList = novo;
	}
    else
	{
        ThreadList *ptaux;
        ptaux = *threadList; //ponteiro auxiliar para percorrer a lista

		while (ptaux->proximo != NULL)      	//procura o fim da lista
		{
			ptaux = ptaux->proximo;
			length++;
 		}
		ptaux->proximo = novo;
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
	{
		return NULL;
	}
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
