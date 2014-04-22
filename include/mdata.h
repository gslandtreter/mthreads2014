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

/*DEFINES*/
#define BILLION 1E9
#define STACKSIZE 10485760 //Stacksize for contexts
#define OK 0				//Ok return of the functions
#define ERROR -1			//Flag of error
#define STATUS_FREE 0
#define STATUS_LOCKED 1

/*TYPES*/
typedef enum
{
	STATE_READY,
	STATE_RUNNING,
	STATE_BLOCKED
} State;

typedef struct st_TCB TCB;
struct st_TCB
{
	int tid;
	ucontext_t* context;
	State state;
	TCB *waiting_for_me;
	struct timespec baseClock;
	long executionTime;
};

typedef struct typeTCBList ThreadList;

struct typeTCBList
{
	TCB* thTCB;
	ThreadList* proximo;
};

/*
 * Exemplo de estrutura de dados "mutex"
 * Os grupos devem alterar essa estrutura de acordo com sua necessidade

*/
typedef struct mutex {
	int status;
    TCB *runningThread;
    ThreadList *blockedList;
} mmutex_t;

#endif
