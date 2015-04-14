#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <ogcsys.h>
#include <malloc.h>

#define BLOCKSIZE 2048
#define KB_SIZE	1024
#define MB_SIZE (1024*1024)
#define GB_SIZE (1024*1024*1024)

#include "utils.h"

bool supportedFormat(const char * path)
{
	FILE *fp = NULL;
	char filepath[128];
	
	sprintf(filepath, "%s/index.sgmgx", path);
	
	fp = fopen(filepath, "rb");
	if (!fp)
		return false;

	fclose(fp);

	return true;
}

u64 StrToHex64(const char *str)
{
	u64 val = 0;
	u32 cnt, len;

	/* String length */
	len = strlen(str);

	for (cnt = 0; cnt < len; cnt++) {
		u32  idx = len - (cnt + 1);
		char c   = toupper((int)str[idx]);

		u64 n = (isdigit((int)c)) ? c - '0' : (c - 'A') + 0xA;
		u64 m = 1;

		for (idx = 0; idx < cnt; idx++)
			m *= 16;

		/* Convert to hex */
		val += n * m;
	}

	return val;
} 

int is_empty(char * buf, int size) {
 int i;
 for(i = 0; i < size; i++) {
  if(buf[i] != 0) return 0;
 }
 return 1;
}

void *allocate_memory(u32 size)
{
	return memalign(32, (size+63)&(~63) );
}

u32 read_isfs(char *path, u8 **out)
{

	fstats *status;

	s32 ret;
	u32 size;
	s32 fd;
	fd = ISFS_Open(path, ISFS_OPEN_READ);
	//gprintf("ISFS_Open returned %d\n", fd);
	status = allocate_memory(sizeof(fstats) );
	if(status == NULL)
	{
		//gprintf("Error allocating memory for status\n");
		return 0;
	}
	ret = ISFS_GetFileStats(fd, status);
	if (ret < 0)
	{
		//gprintf("\nISFS_GetFileStats(fd) returned %d\n", ret);
		free(status);
		return 0;
	}
	u32 fullsize = status->file_length;
	u8 *out2 = allocate_memory(fullsize);
	if(out2 == NULL)
	{
		//gprintf("Error allocating memory for out2\n");
		free(status);
		return 0;
	}
	u32 restsize = status->file_length;
	u32 writeindex = 0;
	while (restsize > 0)
	{
		if (restsize >= BLOCKSIZE)
		{
			size = BLOCKSIZE;
		} else
		{
			size = restsize;
		}
		ret = ISFS_Read(fd, &(out2[writeindex]), size);
		if (ret < 0)
		{
			//gprintf("\nISFS_Read(%d, %d) returned %d\n", fd, size, ret);
			free(status);
			return 0;
		}
		writeindex = writeindex + size;
		restsize -= size;
	}
	free(status);
	ISFS_Close(fd);
	*out = out2;
	return fullsize;
}

s32 get_nand_size_file(char *path)
{

	fstats *status;

	s32 ret;
//	u32 size;
	s32 fd;
	fd = ISFS_Open(path, ISFS_OPEN_READ);
	if (fd<0)
	{
	    //gprintf("\nISFS_Open returned %d\t(%s)", fd,path);
	    return fd;
	}

	status = allocate_memory(sizeof(fstats) );
	if(status == NULL)
	{
		//gprintf("Error allocating memory for status\n");
		return 0;
	}
	ret = ISFS_GetFileStats(fd, status);
	if (ret < 0)
	{
		//gprintf("\nISFS_GetFileStats(fd) returned %d\n", ret);
		free(status);
		return 0;
	}
	u32 fullsize = status->file_length;

	free(status);
	ISFS_Close(fd);
	return fullsize;
}

s32 get_nand_size_directory(char *path)
{
	s32 res, ret=0;
	u32 num = 0;
	char pbuf[ISFS_MAXPATH + 1];

	int i, j, k;

	res = ISFS_ReadDir(path, NULL, &num);
	if(res != ISFS_OK)
	{
		//gprintf("\nError: could not get dir entry count! (result: %d)", res);
		return -1;
	}

	char *nbuf = (char *)allocate_memory((ISFS_MAXPATH + 1) * num);
	char ebuf[ISFS_MAXPATH + 1];

	if(nbuf == NULL)
	{
		//gprintf("\nError: could not allocate buffer for name list!");
		return -1;
	}

	res = ISFS_ReadDir(path, nbuf, &num);
	if(res != ISFS_OK)
	{
		//gprintf("\nError: could not get name list! (result: %d)", res);
		return -1;
	}

	for(i = 0, k=0; i < (int)num; i++)
	{
	    for(j = 0; nbuf[k] != 0; j++, k++)
		    ebuf[j] = nbuf[k];
	    k++;
	    ebuf[j] = 0;
	    snprintf(pbuf, sizeof(pbuf),"%s/%s",path,ebuf);
	    //gprintf("\nchecking %s",pbuf);

	    //this will spit out some error in the gecko for saves contianing folders
	    // but it is doing a recursive search, so the error is fine.
	    res = get_nand_size_file(pbuf);
	    if(res<=0)
	    {
		res = get_nand_size_directory(pbuf);
		if(res<=0)
		{
		    ret = -1;
		    //gprintf("\nError opening %s",pbuf);
		    break;
		}

	    }
	    if(res>0)
	    {
		ret+=res;
		//gprintf(" + %d",res);
	    }
	}

	free(nbuf);
	return ret;
}

