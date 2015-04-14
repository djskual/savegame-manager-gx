#ifndef TOOLS_H_
#define TOOLS_H_

#include <gctypes.h>

#define KB_SIZE			1024.0
#define MB_SIZE			1048576.0
#define GB_SIZE			1073741824.0
#define NAND_BLOCK_SIZE	((1024*1024)/8)

#define round_up(x,n)			(-(-(x) & -(n)))
#define LIMIT(x, min, max)												\
	({																	\
		typeof(x) _x = x;												\
		typeof(min) _min = min;											\
		typeof(max) _max = max;											\
		( ((_x) < (_min)) ? (_min) : ((_x) > (_max)) ? (_max) : (_x) );	\
	})

#define ALIGN(x)	(((x) + 3) & ~3)
#define ALIGN32(x)	(((x) + 31) & ~31)
#define ALIGN64(x)	(((x) + 63) & ~63)

#define UNUSED		 __attribute__((unused))

#define MII_NAME_LENGTH		10
#define MII_CREATOR_LENGTH	10
#define MII_SIZE 			74
#define MII_MAX				100
#define MII_HEADER			4

class Tools
{	
	public:
		static void* AllocateMemory_32(u32 size);
		static void* AllocateMemory_64(u32 size);
		static bool MemIsEmpty(char * buf, int size);
};

#endif
