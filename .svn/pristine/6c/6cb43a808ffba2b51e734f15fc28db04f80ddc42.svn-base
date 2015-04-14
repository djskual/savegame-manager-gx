#include <malloc.h>

#include "tools.h"

void* Tools::AllocateMemory_32(u32 size)
{
	return memalign(32, ALIGN32(size));
}

void* Tools::AllocateMemory_64(u32 size)
{
	return memalign(64, ALIGN64(size));
}

bool Tools::MemIsEmpty(char * buf, int size)
{
	int i;
	
	for(i = 0; i < size; i++)
	{
		if(buf[i] != 0)
			return false;
	}
	
	return true;
}
