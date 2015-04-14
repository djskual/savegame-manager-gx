#include <malloc.h>

#include "MemTools.h"

void* MemTools::AllocateMemory_32(u32 size)
{
	return memalign(32, ALIGN32(size));
}

void* MemTools::AllocateMemory_64(u32 size)
{
	return memalign(64, ALIGN64(size));
}

bool MemTools::MemIsEmpty(char * buf, int size)
{
	int i;
	
	for(i = 0; i < size; i++)
	{
		if(buf[i] != 0)
			return false;
	}
	
	return true;
}