/*
 * mdata.h: arquivo de inclus�o onde os grupos devem colocar as defini��es
 *          de suas estruturas de dados
 *
 * VERS�O 1 - 20/03/2014
 */
 
#ifndef __mdata__
#define __mdata__ 1

/*
 * Exemplo de estrutura de dados "mutex"
 * Os grupos devem alterar essa estrutura de acordo com sua necessidade
 
*/
typedef struct mutex {
	int flag;
    //TCB *next;
} mmutex_t;



#include <ucontext.h>

/*DEFINES*/
#define STACKSIZE 10485760 //Stacksize for contexts
#define OK 0				//Ok return of the functions
#define ERROR -1			//Flag of error

/*TYPES*/
typedef enum
{
	st_READY,
	st_RUNNING,
	st_BLOCKED
} State;

typedef struct st_TCB TCB;
struct st_TCB
{
	int tid;
	ucontext_t* context;
	State state;
	TCB *waiting_for_me;
	long executionTime;
};

#endif
