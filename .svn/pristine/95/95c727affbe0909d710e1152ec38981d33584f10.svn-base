#ifndef _UTILS_H_
#define _UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <gctypes.h>

/* Macros */
#define round_up(x,n)	(-(-(x) & -(n)))
#define NAND_BLOCK_SIZE	((1024*1024)/8)
#define UNUSED		 __attribute__((unused))

/* Prototypes */
bool supportedFormat(const char * path);
u64 StrToHex64(const char *);
int is_empty(char * buf, int size);
void *allocate_memory(u32 size);
u32 read_isfs(char *path, u8 **out);
u32 get_size_nand_file(char *path);
s32 get_nand_size_directory(char *path);

#ifdef __cplusplus
}
#endif

#endif
