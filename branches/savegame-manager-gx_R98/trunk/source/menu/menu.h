/****************************************************************************
 * Modified by Dj_Skual
 * for SaveGame Manager GX 2011
 *
 * menu.h
 ***************************************************************************/

#ifndef _MENU_H_
#define _MENU_H_

#include <ogcsys.h>

int SaveGameBrowser();

enum
{
	MENU_EXIT = -1,
	MENU_NONE,
	MENU_SAVEGAME,
	MENU_SETTINGS
};

#endif
