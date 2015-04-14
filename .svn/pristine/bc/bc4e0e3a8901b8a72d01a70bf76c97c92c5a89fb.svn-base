#include <gccore.h>
#include <ogcsys.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <ogc/machine/processor.h>
#include <wiiuse/wpad.h>
#include <vector>
#include <string>

#include "file/fileops.h"
#include "language/gettext.h"
#include "network/networkops.h"
#include "boot_homebrew/dolloader.h"
#include "filelist.h"
#include "system/sys.h"

static u8 *homebrewbuffer = (u8 *) 0x92000000;
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
    memcpy((homebrewbuffer)+pos, temp, len);

    return 1;
}

void FreeHomebrewBuffer()
{
    homebrewbuffer = (u8 *)0x92000000;
    homebrewsize = 0;
}

int LoadHomebrew(const char * filepath)
{
     u8 *buffer = NULL;
     u64 filesize = 0;
     int ret = LoadFileToMemWithProgress(tr("Loading file:"), filepath, &buffer, &filesize);
     if(ret < 0)
         return ret;

     ret = CopyHomebrewMemory(buffer, 0, filesize);
     if(buffer) {
         free(buffer);
         buffer = NULL;
     }

     return ret;
}

static int SetupARGV(struct __argv * args)
{
    if(!args)
        return -1;

    bzero(args, sizeof(struct __argv));
    args->argvMagic = ARGV_MAGIC;

    u32 stringlength = 1;

    /** Append Arguments **/
    for(u32 i = 0; i < Arguments.size(); i++)
    {
        stringlength += Arguments[i].size()+1;
    }

    args->length = stringlength;
    args->commandLine = (char*) malloc(args->length);
    if (!args->commandLine)
        return -1;

    u32 argc = 0;
    u32 position = 0;

    /** Append Arguments **/
    for(u32 i = 0; i < Arguments.size(); i++)
    {
        strcpy(&args->commandLine[position], Arguments[i].c_str());
        position += Arguments[i].size()+1;
        argc++;
    }

    args->argc = argc;

    args->commandLine[args->length - 1] = '\0';
    args->argv = &args->commandLine;
    args->endARGV = args->argv + 1;

    Arguments.clear();

    return 0;
}

int BootHomebrew()
{
    AppExit();
	
	if(homebrewsize == 0)
        return -1;

    struct __argv args;
    SetupARGV(&args);

    u32 cpu_isr;
	
	entrypoint entry = (entrypoint) load_dol(app_booter_dol, &args);

    if (!entry)
        return -1;

    SYS_ResetSystem(SYS_SHUTDOWN, 0, 0);
    _CPU_ISR_Disable (cpu_isr);
    __exception_closeall();
    entry();
    _CPU_ISR_Restore (cpu_isr);

    return 0;
}
