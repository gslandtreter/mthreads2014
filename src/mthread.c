#include "../include/mthread.h"

//Globals

TCB* MAIN;
TCB* EXEC;

ThreadList* READY_LIST;
ThreadList* BLOCKED_LIST;

ucontext_t* allocation_context;

int initialized = 0;

//Private

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
	EXEC = removeFirst(&READY_LIST);

	if(EXEC != NULL)
	{
        resetClock(EXEC);
		EXEC->state = STATE_RUNNING;
		setcontext(EXEC->context);
	}
}

//Beginning point of function called when thread ends.
void EndPoint()
{
	if(EXEC->jointThread != NULL)
	{
		EXEC->jointThread->state = STATE_READY;
		removeList(&BLOCKED_LIST, EXEC->jointThread->tid);
		insertList(&READY_LIST, EXEC->jointThread);
	}
	free(EXEC);
	escalonador();
}

// Context initializer

ucontext_t* allocator_init()
{
	allocation_context = (ucontext_t*)malloc(sizeof(ucontext_t));

	if (allocation_context == NULL)
		return NULL;

	getcontext(allocation_context);	// Criando o contexto de saída das threads
    allocation_context->uc_stack.ss_sp = (char*) malloc(STACKSIZE);
    allocation_context->uc_stack.ss_size = STACKSIZE;
    allocation_context->uc_link = NULL;
    makecontext(allocation_context, (void (*)(void))EndPoint, 0);
	return allocation_context;
}

//Cria novo tcb
TCB* createTCB(ucontext_t* context)
{
	TCB *newThread = (TCB*)malloc(sizeof(TCB));
	if(newThread != NULL)
	{
		newThread->tid = get_tid();
		newThread->context = context;
		newThread->state = STATE_READY;
		newThread->jointThread = NULL;
		newThread->executionTime = 0;
		resetClock(newThread);
		add_tid();
	}
	return newThread;
}

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
		READY_LIST = createList();
		BLOCKED_LIST = createList();

		getcontext(mainContext);

		EXEC = createTCB(mainContext);
		if(EXEC != NULL)
			EXEC->state = STATE_RUNNING;
		else
			retorno = ERROR;
	}
	return retorno;
}

//Beginning of library public functions
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
	ucontext_t* newThreadContext = (ucontext_t*) malloc(sizeof(ucontext_t));

	if(newThreadContext == NULL)
		retVal = ERROR; //Erro no malloc

	else
	{
		//Cria thread
		getcontext(newThreadContext);
		newThreadContext->uc_stack.ss_sp = (char*) malloc(STACKSIZE);
		newThreadContext->uc_stack.ss_size = STACKSIZE;
		newThreadContext->uc_link = allocation_context;
		makecontext(newThreadContext, (void (*)(void))start_routine, 1, arg);

		newThread = createTCB(newThreadContext);

		if(newThread == NULL)
			retVal = ERROR;

		else
		{
            //Insere thread na lista de aptas
			retVal = newThread->tid;
			insertList(&READY_LIST,newThread);
		}
	}
	//Ferrou
	if(retVal == ERROR)
	{
		if(newThreadContext)
            free(newThreadContext);

        if(newThread)
            free(newThread);
	}
	return retVal;
}

int myield(void)
{
    //Insere thread na lista de aptas
    calculateTime(EXEC);
	insertList(&READY_LIST,EXEC);

	EXEC->state = STATE_READY;
	getcontext(EXEC->context);

	if(EXEC->state == STATE_RUNNING)
		return OK;

    //Chama o escalonador
	escalonador();

	return OK;
}

int mjoin(int threadID)
{
    //Procura thread na lista de aptos
	TCB* threadToWait = getTCBById(READY_LIST, threadID);

    //Procura thread na lista de bloqueadas
	if (threadToWait == NULL)
		threadToWait = getTCBById(BLOCKED_LIST, threadID);

    //Nao encontrou thread
	if (threadToWait == NULL)
		return ERROR;

	if (threadToWait->jointThread == NULL)
	{
		threadToWait->jointThread = EXEC;
		calculateTime(EXEC);
		EXEC->state = STATE_BLOCKED;
		insertList(&BLOCKED_LIST,EXEC);
		getcontext(EXEC->context);

		if(EXEC->state == STATE_BLOCKED)
            escalonador();

		return OK;
	}

	//Caso a thread ja esteja joined com outra, retorna erro.
	else return ERROR;
}

int mmutex_init(mmutex_t *newMutex)
{
    //Aloca mutex, se nao existir.
    if(newMutex == NULL)
        newMutex = (mmutex_t*) malloc(sizeof(mmutex_t));

    if(newMutex == NULL)
        return ERROR;

    newMutex->status = STATUS_FREE;
    newMutex->runningThread = NULL;
    newMutex->blockedList = createList();

    myield(); //Aproveitando para escalonar alguem ;D
    return OK;
}

int mlock (mmutex_t *mutex)
{
    if(mutex == NULL)
        return ERROR;

    if(mutex->status == STATUS_FREE) //OK, bloquear mutex
    {
        mutex->runningThread = EXEC;
        mutex->status = STATUS_LOCKED;
        return OK;
    }
    else //another thread is running
    {
        if(mutex->runningThread->tid == EXEC->tid) //Duplicate call
            return ERROR;

        calculateTime(EXEC); //Bloqueia thread
		EXEC->state = STATE_BLOCKED;
		insertList(&BLOCKED_LIST,EXEC);
		insertList(&mutex->blockedList, EXEC);
        getcontext(EXEC->context);

        if(EXEC->state == STATE_BLOCKED)
            escalonador();
    }
}

int munlock (mmutex_t *mutex)
{
    if(mutex == NULL || mutex->status == STATUS_FREE || mutex->runningThread->tid != EXEC->tid)
        return ERROR;

    //Desbloqueia threads bloqueadas
    if(mutex->blockedList != NULL)
    {
        TCB *TCBToRemove;
        do
        {
            TCBToRemove = removeFirst(&mutex->blockedList);
            if(TCBToRemove != NULL)
            {
                TCBToRemove->state = STATE_READY;
                removeList(&BLOCKED_LIST, TCBToRemove->tid);
                insertList(&READY_LIST, TCBToRemove);
            }
        } while(mutex->blockedList != NULL);
    }

    //Atualiza mutex
    mutex->runningThread = NULL;
    mutex->status = STATUS_FREE;

    myield(); //Aproveitando para escalonar alguem ;D
    return OK;
}
