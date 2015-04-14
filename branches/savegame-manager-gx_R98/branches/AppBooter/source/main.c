#include <gccore.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ogc/machine/processor.h>

#include "dolloader.h"
#include "elfloader.h"

int main(int argc, char *argv[])
{
	VIDEO_Init();
	VIDEO_SetBlack(TRUE);
	VIDEO_Flush ();

	u32 cookie;
	void *exeBuffer          = (void *) EXECUTABLE_MEM_ADDR;
	u32 exeEntryPointAddress = 0;
	entrypoint exeEntryPoint;

	struct __argv args;
	bzero(&args, sizeof(args));
    args.argvMagic = ARGV_MAGIC;
    args.length = 0;
    args.commandLine = NULL;
    args.argc = argc;
    args.argv = &args.commandLine;
    args.endARGV = args.argv + 1;

	if(argc > 0)
	{
        u32 i = 0;

        for(i = 0; i < argc; i++)
        {
            if(argv[i])
                args.length += strlen(argv[i])+1;
        }
        args.length += 1;
        args.commandLine = (char*) malloc(args.length);
        if (!args.commandLine)
            SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);

        u32 pos = 0;

        for(i = 0; i < argc; i++)
        {
            if(argv[i])
            {
                strcpy(&args.commandLine[pos], argv[i]);
                pos += strlen(argv[i])+1;
            }
        }

        args.commandLine[args.length - 1] = '\0';
        args.argc = argc;
        args.argv = &args.commandLine;
        args.endARGV = args.argv + 1;
	}

	if (valid_elf_image(exeBuffer) == 1)
		exeEntryPointAddress = load_elf_image(exeBuffer);
	else
		exeEntryPointAddress = load_dol_image(exeBuffer, &args);

	if (exeEntryPointAddress == 0)
	{
		SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);
	}

	exeEntryPoint = (entrypoint) exeEntryPointAddress;
	/* cleaning up and load dol */
	SYS_ResetSystem(SYS_SHUTDOWN, 0, 0);
	_CPU_ISR_Disable (cookie);
	__exception_closeall ();
	exeEntryPoint ();
	_CPU_ISR_Restore (cookie);
	return 0;
}
