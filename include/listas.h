/*
* listas.h:
*
* NÃO MODIFIQUE ESTE ARQUIVO.
*/


#ifndef __listas__
#define __listas__ 1

#include "mdata.h"

typedef struct typeTCBList ThreadList;

struct typeTCBList
{
	TCB* thTCB;
	ThreadList* proximo;
};

/*Create a new list
	return: pointer to the first element of the list
*/
ThreadList* createList(void);


/*Print tid of all elements from the list on the screen
	ItemList *threadList: pointer to the first element of the list
	return: size of the list if successful
*/
int showList(ThreadList *threadList);


/*Inserts an element at the end of the list
	ItemList *threadList: pointer to the first element of the list
	TCB *aThread: TCB of the element to be inserted
	return: size of the list if successful, 0 if not
*/
int insertList(ThreadList **threadList, TCB *aThread);


/*Gets an element from the list with the desired tid
	ItemList *threadList: pointer to the first element of the list
	int tid: tid of the desired TCB
	return: Pointer to the desired TCB
*/
TCB* getTCBById(ThreadList *threadList, int tid);


/*Remove an element from the list
	ItemList *threadList = pointer to the first element of the list
	int tid = id of the item to be removed
	return: a pointer to the removed item
*/
TCB* removeList(ThreadList **threadList, int tid);


/*Remove the first element from the list
	ItemList *threadList = pointer to the first element of the list
	return: a pointer to the removed item
*/
TCB* removeFirst(ThreadList **threadList);


/*Destroy a list
	ItemList *threadList: pointer to the first element of the list
	return: Null if successful
*/
ThreadList* destroy(ThreadList* threadList);
#endif
