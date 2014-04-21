#include "../include/mthread.h"

//Globals

ucontext_t* allocation_context;
ThreadList* READY;
ThreadList* BLOCKED;
TCB* EXEC;
TCB* MAIN;
int initialized = 0;

int calculateTime(TCB *threadItem)
{
    if(threadItem == NULL)
        return ERROR;

    struct timespec clock;
    clock_gettime(CLOCK_REALTIME, &clock);
    long elapsedTime = clock.tv_nsec - threadItem->baseClock.tv_nsec;
    threadItem->executionTime = elapsedTime;

    return OK;
}

int resetClock(TCB *threadItem)
{
    if(threadItem == NULL)
        return ERROR;

    clock_gettime(CLOCK_REALTIME, &threadItem->baseClock);
    return OK;
}

void escalonador()
{
	EXEC = removeFirst(&READY);

	if(EXEC != NULL)
	{
        resetClock(EXEC);
		EXEC->state = st_RUNNING;
		setcontext(EXEC->context);
	}
}

//Beginning point of function called when thread ends.
void EndPoint()
{
	if(EXEC->waiting_for_me != NULL)
	{
		EXEC->waiting_for_me->state = st_READY;
		removeList(&BLOCKED, EXEC->waiting_for_me->tid);
		insertList(&READY, EXEC->waiting_for_me);
	}
	free(EXEC);
	escalonador();
}

/* Context initializer
*/

ucontext_t* allocator_init()
{
	allocation_context = (ucontext_t*)malloc(sizeof(ucontext_t));

	if (allocation_context == NULL)
		return NULL;

	getcontext(allocation_context);	// Criando o contexto de saída das threads
		allocation_context->uc_stack.ss_sp = mmap(NULL,STACKSIZE,PROT_WRITE|PROT_READ,MAP_PRIVATE|MAP_GROWSDOWN|MAP_ANONYMOUS,-1,0);
		allocation_context->uc_stack.ss_size = STACKSIZE;
		allocation_context->uc_link = NULL;
		makecontext(allocation_context, (void (*)(void))EndPoint, 0);
	return allocation_context;
}

/* Creates a new tcb object, incrementing the tid
		ucontext_t* context = the context that will be executed in the thread
*/
TCB* create_tcb(ucontext_t* context)
{
	TCB *newThread = (TCB*)malloc(sizeof(TCB));
	if(newThread != NULL)
	{
		newThread->tid = get_tid();
		newThread->context = context;
		newThread->state = st_READY;
		newThread->waiting_for_me = NULL;
		newThread->executionTime = 0;
		resetClock(newThread);
		add_tid();
	}
	return newThread;
}

/*PUBLIC*/

int init()
{
	int retorno = OK;
	ucontext_t* mainContext = (ucontext_t*)malloc(sizeof(ucontext_t));

	set_tid(0);
	if (allocator_init() == NULL)
		retorno = ERROR;

	if (retorno == OK)
	{
		EXEC = NULL;
		READY = createList();
		BLOCKED = createList();

		getcontext(mainContext);

		EXEC = create_tcb(mainContext);
		if(EXEC != NULL)
			EXEC->state = st_RUNNING;
		else
			retorno = ERROR;
	}
	return retorno;
}

int mcreate(void (*start_routine)(void*), void * arg)
{
	if(!initialized)
	{
		init();
		initialized = 1;
	}

	int retVal = OK;
	TCB* newThread;

	//Aloca contexto
	ucontext_t* thread_context = (ucontext_t*)malloc(sizeof(ucontext_t));

	if(thread_context == NULL)
		retVal = ERROR; //Erro no malloc
	else
	{
		//Cria thread
		getcontext(thread_context);
		thread_context->uc_stack.ss_sp = mmap(NULL,STACKSIZE,PROT_WRITE|PROT_READ,MAP_PRIVATE|MAP_GROWSDOWN|MAP_ANONYMOUS,-1,0);
		thread_context->uc_stack.ss_size =STACKSIZE;
		thread_context->uc_link = allocation_context;
		makecontext(thread_context, (void (*)(void))start_routine, 1, arg);

		newThread = create_tcb(thread_context);

		if(newThread == NULL)
			retVal = ERROR;
		else
		{
			retVal = newThread->tid;
			insertList(&READY,newThread);
		}
	}
	if(retVal == ERROR)
	{
		//Ferrou
		free(thread_context);
		free(newThread);
	}
	return retVal;
}

int myield(void)
{
    calculateTime(EXEC);
	insertList(&READY,EXEC);

	EXEC->state = st_READY;

	getcontext(EXEC->context);
	if(EXEC->state == st_RUNNING)
		return OK;

	escalonador();

	return OK;
}

int mjoin(int thr)
{
    //Procura thread na lista de aptos
	TCB* threadToWait = getTCBById(READY, thr);

    //Procura thread na lista de bloqueadas
	if (threadToWait == NULL)
		threadToWait = getTCBById(BLOCKED, thr);

    //Nao encontrou thread
	if (threadToWait == NULL)
		return ERROR;

	if (threadToWait->waiting_for_me == NULL)
	{
		threadToWait->waiting_for_me = EXEC;
		calculateTime(EXEC);
		EXEC->state = st_BLOCKED;
		insertList(&BLOCKED,EXEC);
		escalonador();
		return OK;
	}
	else return ERROR;
}

int mmutex_init(mmutex_t *newMutex)
{
    if(newMutex == NULL)
        newMutex = (mmutex_t*) malloc(sizeof(mmutex_t));

    if(newMutex == NULL)
        return ERROR;

    newMutex->status = STATUS_FREE;
    newMutex->runningThread = NULL;
    newMutex->blockedList = createList();

    return OK;
}

int my_mlock (mmutex_t *mutex)
{
    if(mutex == NULL)
        return ERROR;

    if(mutex->status == STATUS_FREE) //OK, update critical section.
    {
        mutex->runningThread = EXEC;
        mutex->status = STATUS_LOCKED;
        return OK;
    }
    else //another thread is running
    {
        if(mutex->runningThread->tid == EXEC->tid) //Duplicate call
            return ERROR;

        calculateTime(EXEC);
		EXEC->state = st_BLOCKED;
		insertList(&BLOCKED,EXEC);
		insertList(&mutex->blockedList, EXEC);

		escalonador();
    }
}

int my_munlock (mmutex_t *mutex)
{
    if(mutex == NULL || mutex->status == STATUS_FREE || mutex->runningThread->tid != EXEC->tid)
        return ERROR;

    if(mutex->blockedList != NULL)
    {
        TCB *TCBToRemove;
        do
        {
            TCBToRemove = removeFirst(&mutex->blockedList);
            if(TCBToRemove != NULL)
            {
                TCBToRemove->state = st_READY;
                removeList(&BLOCKED, TCBToRemove->tid);
                insertList(&READY, TCBToRemove);
            }
        } while(mutex->blockedList != NULL);
    }

    mutex->runningThread = NULL;
    mutex->status = STATUS_FREE;

    return OK;
}
