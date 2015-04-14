/****************************************************************************
 * USB Loader GX Team
 *
 * Main loadup of the application
 *
 * libwiigui
 * Tantric 2009
 ***************************************************************************/
#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dirent.h>
#include <ogcsys.h>
#include <unistd.h>
#include <locale.h>
#include <wiiuse/wpad.h>
#include <sys/iosupport.h>

#include "Memory/mem2.h"
#include "System/video.h"
#include "System/ESPatch.h"
#include "Tools/tools.h"
#include "Tools/gecko.h"
#include "Tools/StartUpProcess.h"
#include "Settings/CSettings.h"
#include "menu.h"

bool startError = false;

extern "C"
{
	extern s32 MagicPatches(s32);
	void __exception_setreload(int t);
}

int main(int argc, char *argv[])
{
	__exception_setreload(20);
	InitVideo();
	MEM2_init(48);
	InitGecko();
	USBGeckoOutput();
	
	Settings.EntryIOS = IOS_GetVersion();
	
	//! Enable AHBPROT on title launch
    if (ESPatch::AHBPROT())
	{
        //! Reload current IOS, typically IOS58
        IOS_ReloadIOS(IOS_GetVersion());
	}
    //! else, lets see if app ios will allow full use!!!
	
	MagicPatches(1);
	setlocale(LC_ALL, "en.UTF-8");
	
	if(!StartUpProcess::Run(argc, argv))
		startError = true;
	
	MainMenu(MENU_SAVEGAME);
	
	return 0;
}
