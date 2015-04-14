 /************************************************************************************************************************************************
 * \mainpage SavegeGame Manager GX
 *
 * \section sec_intro Introduction
 *
 * SavegeGame Manager GX is a multi-device Savegame/Mii extractor and installer for the Wii made with a customized libwiigui as base for the GUI.
 * <p>
 * It was created and is developed by dj_skual and is based on WiiXplorer and USB Loader GX sources.
 * Some features of the project were contributed/developed by Dimok, giantpune, R-win and Dude.
 * <p>
 * The graphical design and the required artwork for that were/are made by NeoRame, Cyrex, dj_skual.
 * <p>
 * SaveGame Manager GX is written mainly in C++ and makes use of sevaral open-source C/C++ libraries.
 * <p>
 * \section sec_special_thanks Special thanks:
 * All translators<br>
 * Tantric for his tool libwiigui<br>
 * Armin Tamzarian for FreeTypeGX<br>
 * The libogc/devkitPro Team<br>
 *
 * \section sec_license License
 *
 * The SaveGame Manager GX source code is distributed under the GNU General Public License v3.
 *
 * \section sec_contact Contact
 *
 * If you have any suggestions, questions, or comments regarding the source code or the application feel free to e-mail me at dj_skual@savegamegx.net.
 *************************************************************************************************************************************************/
#define GDB_DEBUG

#include <debug.h>

#include "Controls/Application.h"
#include "Tools/StartUpProcess.h"
#include "System/video.h"
#include "System/sys.h"

int main(int argc, char *argv[])
{
	__exception_setreload(20);
	
#ifdef GDB_DEBUG
	DEBUG_Init(GDBSTUB_DEVICE_USB, 1);
	_break();
#endif
	
	InitVideo();
	
	if(StartUpProcess::Run(argc, argv))
	{
		Application::Instance()->init();
		Application::Instance()->show();
		Application::Instance()->exec();
	}
	
	//! Return to the Wii system menu if not from HBC
	if(!IsFromHBC())
		SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);

	return 0;
}
