#include "../include/tid.h"

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
