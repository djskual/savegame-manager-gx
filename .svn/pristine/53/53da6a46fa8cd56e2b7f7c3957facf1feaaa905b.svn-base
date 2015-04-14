/****************************************************************************
 * libwiigui Template
 * Tantric 2009
 *
 * menu.h
 * Menu flow routines - handles all menu logic
 ***************************************************************************/

#ifndef _MENU_H_
#define _MENU_H_

#include <ogcsys.h>

#include "HomebrewBoot/HomebrewReceiver.hpp"
#include "DeviceControls/SdPort.hpp"
#include "LibWiiGui/gui.h"
#include "Controls/WiiPointer.h"
#include "main.h"

void InitGUIThreads(void);
void ExitGUIThreads(void);

int MainMenu(int menuitem);

enum
{
	MENU_EXIT = -1,
	MENU_NONE,
	MENU_SETTINGS,
	MENU_SAVEGAME,
	MENU_THEMEDOWNLOADER,
	MENU_THEMEMENU,
	MENU_HOMEBREWBROWSE,
};

void ResumeGui();
void HaltGui();

extern WiiPointer * pointer[4];
extern GuiImageData * background;
extern GuiImage * bgImg;
extern GuiWindow * mainWindow;
extern FreeTypeGX * fontSystem;
extern HomebrewReceiver * Receiver;
extern SdPort * SD_Port;

#endif
