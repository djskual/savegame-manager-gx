#include <gccore.h>
#include <ogcsys.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ogc/machine/processor.h>
#include <wiiuse/wpad.h>
#include <vector>
#include <string>

#include "../DeviceControls/DeviceHandler.hpp"
#include "../Settings/CSettings.h"
#include "../Tools/lstub.h"
#include "../System/sys.h"
#include "../System/ESPatch.h"

#define EXECUTE_ADDR	((u8 *) 0x92000000)
#define BOOTER_ADDR		((u8 *) 0x93000000)
#define ARGS_ADDR		((u8 *) 0x93200000)

extern const u8 app_booter_bin[];
extern const u32 app_booter_bin_size;

typedef void (*entrypoint) (void);
extern "C" { void __exception_closeall(); }

static u8 *homebrewbuffer = EXECUTE_ADDR;
static u32 homebrewsize = 0;
static std::vector<std::string> Arguments;

void AddBootArgument(const char * argv)
{
	std::string arg(argv);
	Arguments.push_back(arg);
}

int CopyHomebrewMemory(u8 *temp, u32 pos, u32 len)
{
	homebrewsize += len;
	memcpy((homebrewbuffer) + pos, temp, len);
	
	return 1;
}

void FreeHomebrewBuffer()
{
	homebrewbuffer = EXECUTE_ADDR;
	homebrewsize = 0;
	
	Arguments.clear();
}

static inline bool IsDollZ (const u8 *buf)
{
	return (buf[0x100] == 0x3C);
}

static int SetupARGV(struct __argv * args)
{
	if (!args)
		return -1;

	bzero(args, sizeof(struct __argv));
	args->argvMagic = ARGV_MAGIC;
	
	u32 argc = 0;
	u32 position = 0;
	u32 stringlength = 1;
	
	/** Append Arguments **/
	for (u32 i = 0; i < Arguments.size(); i++)
	{
		stringlength += Arguments[i].size() + 1;
	}
	
	args->length = stringlength;
	//! Put the argument into mem2 too, to avoid overwriting it
	args->commandLine = (char *) ARGS_ADDR + sizeof(struct __argv);
	
	/** Append Arguments **/
	for (u32 i = 0; i < Arguments.size(); i++)
	{
		strcpy(&args->commandLine[position], Arguments[i].c_str());
		position += Arguments[i].size() + 1;
		argc++;
	}
	
	args->argc = argc;
	
	args->commandLine[args->length - 1] = '\0';
	args->argv = &args->commandLine;
	args->endARGV = args->argv + 1;
	
	Arguments.clear();
	
	return 0;
}

static int RunAppbooter()
{
	if (homebrewsize == 0)
		return -1;
	
	ExitApp();
	
	if(Settings.EntryIOS != IOS_GetVersion())
	{
		ESPatch::AHBPROT();
		IOS_ReloadIOS(Settings.EntryIOS);
	}
	
	struct __argv args;
	SetupARGV(&args);
	
	u32 cpu_isr;
	
	DCFlushRange(homebrewbuffer, homebrewsize);
	
	memcpy(BOOTER_ADDR, app_booter_bin, app_booter_bin_size);
	DCFlushRange(BOOTER_ADDR, app_booter_bin_size);
	ICInvalidateRange(BOOTER_ADDR, app_booter_bin_size);
	
	entrypoint entry = (entrypoint) BOOTER_ADDR;
	
	if (!IsDollZ(homebrewbuffer))
		memmove(ARGS_ADDR, &args, sizeof(struct __argv));
	else
		memset(ARGS_ADDR, 0, sizeof(struct __argv));
	
	DCFlushRange(ARGS_ADDR, sizeof(struct __argv) + args.length);
	
	u64 currentStub = getStubDest();
	loadStub();
	
	if (Set_Stub_Split(0x00010001, "SMGX") < 0)
	{
		if (!currentStub) currentStub = 0x100000002ULL;
		
		Set_Stub(currentStub);
	}
	
	SYS_ResetSystem(SYS_SHUTDOWN, 0, 0);
	_CPU_ISR_Disable( cpu_isr );
	__exception_closeall();
	entry();
	_CPU_ISR_Restore( cpu_isr );
	
	return 0;
}

int BootHomebrew(const char * filepath)
{
	void *buffer = NULL;
	u32 filesize = 0;
	
	FILE *file = fopen(filepath, "rb");
	
	if (!file) Sys_BackToLoader();
	
	fseek(file, 0, SEEK_END);
	filesize = ftell(file);
	rewind(file);
	
	buffer = malloc(filesize);
	
	if (fread(buffer, 1, filesize, file) != filesize)
	{
		fclose(file);
		free(buffer);
		DeviceHandler::DestroyInstance();
		Sys_BackToLoader();
	}

	fclose(file);
	
	CopyHomebrewMemory((u8*) buffer, 0, filesize);
	
	if (buffer)
	{
		free(buffer);
		buffer = NULL;
	}
	
	AddBootArgument(filepath);
	return RunAppbooter();
}

int BootHomebrewFromMem()
{
	return RunAppbooter();
}
