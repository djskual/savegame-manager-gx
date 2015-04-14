#ifndef __MEMTOOLS_H_
#define __MEMTOOLS_H_

#include <gctypes.h>
//#include "../Tools/tools.h"

#define ALIGN(x)	(((x) + 3) & ~3)
#define ALIGN32(x)	(((x) + 31) & ~31)
#define ALIGN64(x)	(((x) + 63) & ~63)

class MemTools
{	
	public:
		static void* AllocateMemory_32(u32 size);
		static void* AllocateMemory_64(u32 size);
		static bool MemIsEmpty(char * buf, int size);
};

#endif
