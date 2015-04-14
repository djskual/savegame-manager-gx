#include <string.h>
#include <unistd.h>

#include "menus/GlobalSettings.hpp"
#include "../FileOperations/fileops.h"
#include "../FileOperations/DirList.h"
#include "../Prompts/PromptWindows.h"
#include "../Prompts/SelectBrowser.h"
#include "../Language/gettext.h"
#include "../menu.h"

/****************************************************************************
 * MenuSettings
 ***************************************************************************/
int MenuSettings()
{
	GlobalSettings * Menu = new GlobalSettings();
	mainWindow->Append(Menu);

	Menu->ShowMenu();

	int returnMenu = MENU_NONE;

	while((returnMenu = Menu->MainLoop()) == MENU_NONE);

	delete Menu;

	return returnMenu;
}

/****************************************************************************
 * MenuBackgroundMusic
 ***************************************************************************/
bool MenuBackgroundMusic()
{
	
	bool ret = false;
	char enter[1024];
	int result = -1;
	snprintf(enter, sizeof(enter), "%s", Settings.BgMusicPath);

	if (strcmp(enter, "") == 0 )
	{
		sprintf(enter, "%s/", Settings.BootDevice);
	}
	else
	{
		char * pathptr = strrchr(enter, '/');
		if (pathptr)
		{
			pathptr++;
			int choice = WindowPrompt(tr("Playing Music:"), pathptr, tr("Play Previous"), tr("Play Next"), tr("Change Play Path"), tr("Cancel"));
			if (choice == 1)
			{
				return bgMusic->PlayPrevious();
			}
			else if (choice == 2)
			{
				return bgMusic->PlayNext();
			}
			else if (choice == 3)
			{
				pathptr[0] = 0;
			}
			else
				return true;
		}
		else
			sprintf( enter, "%s/", Settings.BootDevice );
	}

	result = selectBrowser(enter, SOUNDBROWSER);

	if ( result == 5 )
	{
		if ( !bgMusic->Load( Settings.BgMusicPath ) )
		{
			WindowPrompt( tr( "Not supported format!" ), tr( "Loading standard music." ), tr( "OK" ) );
		}
		else
			ret = true;
		bgMusic->Play();
		bgMusic->SetVolume( Settings.MusicVolume );
	}

	return ret;
}

