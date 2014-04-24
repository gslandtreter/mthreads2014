/*
 * mdata.h: arquivo de inclusão onde os grupos devem colocar as definições
 *          de suas estruturas de dados
 *
 * VERSÃO 1 - 20/03/2014
 */

#ifndef __mdata__
#define __mdata__ 1

#include <ucontext.h>
#include "time.h"
#include <stdlib.h>

#define BILLION 1E9
#define STACKSIZE 10485760 //Stacksize for contexts
#define OK 0				//Ok return of the functions
#define ERROR -1			//Flag of error
#define STATUS_FREE 0
#define STATUS_LOCKED 1

//Mutex State
typedef enum
{
	STATE_READY,
	STATE_RUNNING,
	STATE_BLOCKED
} State;

//Thread Control Block
typedef struct structTCB TCB;
struct structTCB
{
	int tid;
	State state;
	ucontext_t* context;
	TCB* jointThread;
	struct timespec baseClock;
	long executionTime;
};

//Thread List
typedef struct TCBList ThreadList;
struct TCBList
{
	TCB* thTCB;
	ThreadList* proximo;
};

//Mutex
typedef struct mutex {
	int status;
    TCB *runningThread;
    ThreadList *blockedList;
} mmutex_t;



/***** FUNCTIONS *****/

//Creates new list.
ThreadList* createList(void);

//Prints list elements.
int showList(ThreadList *threadList);

//Inserts an element at the list, ordering it by executionTime,
//This way, always the shortest job is scheduled.
int insertList(ThreadList **threadList, TCB *aThread);

//Gets thread by ID
TCB* getTCBById(ThreadList *threadList, int tid);

//Removes thread from a list by ID
TCB* removeList(ThreadList **threadList, int tid);

//Removes the first element of a list
TCB* removeFirst(ThreadList **threadList);

//Destroys a list
ThreadList* destroy(ThreadList* threadList);

// Get current TID
int get_tid();

//Set current TID
void set_tid(int value);

// TID = TID + 1
int add_tid();

#endif
