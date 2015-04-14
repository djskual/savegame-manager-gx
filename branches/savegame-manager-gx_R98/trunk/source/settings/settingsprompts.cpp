#include <string.h>
#include <unistd.h>

#include "settings/menus/GlobalSettings.hpp"
#include "prompts/promptwindows.h"
#include "prompts/browsers/browsers.h"
#include "menu/MainWindow.h"
#include "main.h"

/****************************************************************************
 * MenuSettings
 ***************************************************************************/
int MenuSettings()
{
    GlobalSettings * Menu = new GlobalSettings();
    MainWindow::Instance()->Append(Menu);

    Menu->ShowMenu();

    int returnMenu = MENU_NONE;

    while((returnMenu = Menu->MainLoop()) == MENU_NONE);

    delete Menu;

    return returnMenu;
}

/****************************************************************************
 * MenuBackgroundMusic
 ***************************************************************************/
int MenuBackgroundMusic(char * returnPath)
{
    
	char enter[1024];
    
	snprintf(enter, sizeof(enter), "%s", cfg.BgMusicPath);

    if (strcmp(enter, "") == 0 )
    {
        strcpy( enter, (device.ConfigDevice == 2) ? "usb:/" : "sd:/");
    }
    else
    {
        char * pathptr = strrchr(enter, '/');
        if (pathptr)
        {
            pathptr++;
            int choice = WindowPrompt(tr("Playing Music:"), pathptr, tr("Play Previous"), tr("Play Next"), tr("Change Play Path"), tr("Cancel"), false);
            if (choice == 1)
            {
                return 2;
            }
            else if (choice == 2)
            {
                return 3;
            }
            else if (choice == 3)
            {
                strcpy(enter, pathptr);
            }
            else
                return 0;
        }
        else
            strcpy(enter, (device.ConfigDevice == 2) ? "usb:/" : "sd:/");
    }

    return BrowseCustomFile(enter, returnPath);
}

