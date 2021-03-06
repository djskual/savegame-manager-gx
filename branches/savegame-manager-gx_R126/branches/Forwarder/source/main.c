/****************************************************************************
 * Copyright 2009 The Lemon Man and thanks to luccax, Wiipower, Aurelio and crediar
 * Copyright 2010 Dimok
 * Copyright 2010 Dj_Skual
 *
 * Original forwarder source by
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any
 * purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you
 * must not claim that you wrote the original software. If you use
 * this software in a product, an acknowledgment in the product
 * documentation would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and
 * must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 ***************************************************************************/
#include <gccore.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ogc/machine/processor.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "video.h"
#include "background_image.h"
#include "filelist.h"
#include "devicemounter.h"

#define EXECUTE_ADDR  ((u8 *) 0x92000000)
#define BOOTER_ADDR    ((u8 *) 0x93000000)
#define ARGS_ADDR    ((u8 *) 0x93200000)

static bool compile_dark = false; //! change this to set the color of the background

typedef void (*entrypoint) (void);
extern void __exception_setreload(int t);
extern void __exception_closeall();
extern const u8 app_booter_bin[];
extern const u32 app_booter_bin_size;

static int GetXMLArguments(const char *path, struct __argv *args)
{
	char xmlpath[200];
	snprintf(xmlpath, sizeof(xmlpath), "%s", path);
	
	char *ptr = strrchr(xmlpath, '/');
	if(ptr)
		ptr[1] = '\0';
	else
		return -1; //! only take full path
	
	strncat(xmlpath, "meta.xml", sizeof(xmlpath));
	
	FILE *fp = fopen(xmlpath, "rb");
	if(!fp)
		return -1;
	
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	rewind(fp);
	
	char *fileBuf = (char *) malloc(size+1);
	if(!fileBuf)
	{
		fclose(fp);
		return -1;
	}
	
	fread(fileBuf, 1, size, fp);
	fclose(fp);
	
	fileBuf[size] = '\0';
	
	const int max_len = 1024;
	char *entry = (char *) malloc(max_len);
	ptr = fileBuf;
	
	while(ptr)
	{
		char *comment = strstr(ptr, "<!--");
		ptr = strstr(ptr, "<arg>");
		if(!ptr)
			break;
		
		if(comment && comment < ptr)
		{
			ptr = strstr(ptr, "-->");
			continue;
		}
		
		ptr += strlen("<arg>");
		int len;
		for(len = 0; *ptr != '\0' && !(ptr[0] == '<' && ptr[1]  == '/') && len < max_len-1; ptr++, len++)
			entry[len] = *ptr;
		
		entry[len] = '\0';
		
		strcpy(args->commandLine + args->length - 1, entry);
		args->length += len + 1;
	}

	free(entry);
	free(fileBuf);

	return 0;
}

static FILE * open_file(const char * dev, char * filepath)
{
	sprintf(filepath, "%s:/apps/SaveGame_Manager_GX/boot.dol", dev);
	
	FILE * exeFile = fopen(filepath ,"rb");
	if (exeFile == NULL)
	{
		sprintf(filepath, "%s:/apps/SaveGame_Manager_GX/boot.elf", dev);
		exeFile = fopen(filepath ,"rb");
	}
	if (exeFile == NULL)
	{
		sprintf(filepath, "%s:/apps/SaveGameManagerGX/boot.dol", dev);
		exeFile = fopen(filepath ,"rb");
	}
	if (exeFile == NULL)
	{
		sprintf(filepath, "%s:/apps/SaveGameManagerGX/boot.elf", dev);
		exeFile = fopen(filepath ,"rb");
	}
	if (exeFile == NULL)
	{
		sprintf(filepath, "%s:/apps/savegame_gx/boot.dol", dev);
		exeFile = fopen(filepath ,"rb");
	}
	if (exeFile == NULL)
	{
		sprintf(filepath, "%s:/apps/savegame_gx/boot.elf", dev);
		exeFile = fopen(filepath ,"rb");
	}
	
	
	return exeFile;
}

int main(int argc, char **argv)
{
	u32 cookie;
	FILE * exeFile = NULL;
	void * exeBuffer = (void *)EXECUTE_ADDR;
	u32 exeSize = 0;
	entrypoint exeEntryPoint;
	__exception_setreload(0);
	
	//! int videomod
	InitVideo();
	//! get imagedata
	u8 * imgdata = GetImageData(compile_dark);
	fadein(imgdata);
	
	char filepath[200];

	//! try SD Card First
	SDCard_Init();
	
	exeFile = open_file(DeviceName[SD], filepath);
	
	//! if app not found on SD Card try USB
	if (!exeFile)
	{
		USBDevice_Init();
		int dev;
		for(dev = USB1; (dev < MAXDEVICES) && (!exeFile); ++dev)
			exeFile = open_file(DeviceName[dev], filepath);
	}
	
	//! if nothing found exiting
	if (!exeFile)
	{
		fadeout(imgdata);
		fclose (exeFile);
		SDCard_deInit();
		USBDevice_deInit();
		StopGX();
		free(imgdata);
		SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);
	}
	
	fseek (exeFile, 0, SEEK_END);
	exeSize = ftell(exeFile);
	rewind (exeFile);

	if(fread (exeBuffer, 1, exeSize, exeFile) != exeSize)
	{
		fadeout(imgdata);
		fclose (exeFile);
		SDCard_deInit();
		USBDevice_deInit();
		StopGX();
		free(imgdata);
		SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);
	}
	fclose (exeFile);
	
	//! load entry point
	struct __argv args;
	bzero(&args, sizeof(args));
	args.argvMagic = ARGV_MAGIC;
	args.length = strlen(filepath) + 2;
	args.commandLine = (char *) ARGS_ADDR + sizeof(args);
	strcpy(args.commandLine, filepath);
	GetXMLArguments(filepath, &args);
	args.commandLine[args.length - 1] = '\0';
	args.argc = 1;
	args.argv = &args.commandLine;
	args.endARGV = args.argv + 1;
	
	memcpy(BOOTER_ADDR, app_booter_bin, app_booter_bin_size);
	DCFlushRange(BOOTER_ADDR, app_booter_bin_size);
	
	memcpy(ARGS_ADDR, &args, sizeof(args));
	DCFlushRange(ARGS_ADDR, sizeof(args) + args.length);
	
	fadeout(imgdata);
	SDCard_deInit();
	USBDevice_deInit();
	StopGX();
	free(imgdata);
	
	//! Reset HBC stub so we can leave correct from SaveGame GX to System Menu
	memset((char *) 0x80001804, 0, 8);
	DCFlushRange((char *) 0x80001804, 8);
	
	exeEntryPoint = (entrypoint) BOOTER_ADDR;
	
	//! cleaning up and load dol
	SYS_ResetSystem(SYS_SHUTDOWN, 0, 0);
	_CPU_ISR_Disable (cookie);
	__exception_closeall ();
	exeEntryPoint ();
	_CPU_ISR_Restore (cookie);
	return 0;
}
