 /****************************************************************************
 * Copyright (C) 2010
 * by Dimok
 * Modified by Dj_Skual
 * for SaveGame Manager GX
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
#include <unistd.h>
#include "settings/menus/GUISettingsMenu.hpp"
#include "settings/menus/SettingsMenu.hpp"
#include "settings/SettingsPrompts.h"
#include "prompts/PromptWindows.h"
#include "language/langbrowser.h"
#include "language/gettext.h"
#include "saves/WiiTDB.hpp"
#include "menu/menu_tools.h"
#include "menu/MainWindow.h"
#include "main.h"

GuiSettingsMenu::GuiSettingsMenu()
    : SettingsMenu(tr("GUI Settings"), &GuiOptions, MENU_NONE)
{
    int Idx = 0;
    
	GuiOptions.SetName(Idx++, "%s", tr( "Language" ));
    GuiOptions.SetName(Idx++, "%s", tr( "BootMethod" ));
    GuiOptions.SetName(Idx++, "%s", tr( "Clock" ));
    GuiOptions.SetName(Idx++, "%s", tr( "Tooltips" ));
    GuiOptions.SetName(Idx++, "%s", tr( "InfoWindow" ));
    GuiOptions.SetName(Idx++, "%s", tr( "Screensaver" ));
    GuiOptions.SetName(Idx++, "%s", tr( "Screen Size" ));
    GuiOptions.SetName(Idx++, "%s", tr( "Rumble" ));
    
    SetOptionValues();
}

GuiSettingsMenu::~GuiSettingsMenu()
{
    
}

void GuiSettingsMenu::SetOptionValues()
{
    int Idx = 0;
	
	if (strcmp(cfg.LanguageFile, "Default") == 0)
        GuiOptions.SetValue(Idx++, "%s", tr("Default"));
    else
        GuiOptions.SetValue(Idx++, "%s", cfg.LanguageFile);

    if (cfg.BootMethod == 0)
		GuiOptions.SetValue(Idx++, "%s", tr("SD/USB"));
	else if (cfg.BootMethod == 1)
		GuiOptions.SetValue(Idx++, "%s", tr("Wii"));
	else 
		GuiOptions.SetValue(Idx++, "%s", tr("Mii"));
	
	if (cfg.Clock == 0)
        GuiOptions.SetValue(Idx++, "%s", tr("12 Hours"));
    else if (cfg.Clock == 1)
        GuiOptions.SetValue(Idx++, "%s", tr("24 Hours"));
    else if (cfg.Clock == 2)
        GuiOptions.SetValue(Idx++, "%s", tr("OFF"));

    if (cfg.Tooltips == 0)
		GuiOptions.SetValue(Idx++, "%s", tr("OFF"));
	else 
		GuiOptions.SetValue(Idx++, "%s", tr("ON"));
	
	if (cfg.InfoWindow == 0)
		GuiOptions.SetValue(Idx++, "%s", tr("OFF"));
	else 
		GuiOptions.SetValue(Idx++, "%s", tr("ON"));
	
	if (cfg.Screensaver == 0)
		GuiOptions.SetValue(Idx++, "%s", tr("OFF"));
	else if (cfg.Screensaver == 1)
		GuiOptions.SetValue(Idx++, "%s", tr("1 min"));
	else if (cfg.Screensaver == 2)
		GuiOptions.SetValue(Idx++, "%s", tr("3 mins"));
	else if (cfg.Screensaver == 3)
		GuiOptions.SetValue(Idx++, "%s", tr("5 mins"));
	else if (cfg.Screensaver == 4)
		GuiOptions.SetValue(Idx++, "%s", tr("10 mins"));
	else if (cfg.Screensaver == 5)
		GuiOptions.SetValue(Idx++, "%s", tr("20 mins"));
	else if (cfg.Screensaver == 6)
		GuiOptions.SetValue(Idx++, "%s", tr("30 mins"));
	else
		GuiOptions.SetValue(Idx++, "%s", tr("1 hour"));
	
	if (cfg.Widescreen == 0)
		GuiOptions.SetValue(Idx++, "%s", tr("4/3"));
	else 
		GuiOptions.SetValue(Idx++, "%s", tr("16/9"));
	
	if (cfg.Rumble == 0)
		GuiOptions.SetValue(Idx++, "%s", tr("OFF"));
	else 
		GuiOptions.SetValue(Idx++, "%s", tr("ON"));
}

int GuiSettingsMenu::GetMenuInternal()
{
    int ret = optionBrowser->GetClickedOption();

    if (ret < 0)
        return MENU_NONE;

    int Idx = -1;

    //! Settings: App Language
    if (ret == ++Idx)
    {
        MainWindow::Instance()->HaltGui();
        if(parentElement)
			((GuiWindow *) parentElement)->SetState(STATE_DISABLED);
        MainWindow::Instance()->ResumeGui();

        int returnhere = 1;
        while (returnhere == 1)
            returnhere = BrowseLangDevice();
		
		if (returnhere == 2)
        {
            //! Language changed. Reload game titles with new lang code.
			WiiTDBLanguageReload();
			LoadTitlesFromWiiTDB(cfg.WiiTdbPath, WiiTDB::WII);
			return MENU_SETTINGS;
        }

        MainWindow::Instance()->HaltGui();
        if(parentElement)
			((GuiWindow *) parentElement)->SetState(STATE_DEFAULT);
        MainWindow::Instance()->ResumeGui();
    }

    //! Settings: BootMethod
    else if (ret == ++Idx)
    {
        if (++cfg.BootMethod > 2) cfg.BootMethod = 0;
    }

    //! Settings: Clock
    else if (ret == ++Idx)
    {
        if (++cfg.Clock > 2) cfg.Clock = 0; //CLOCK
    }

    //! Settings: Tooltips
    else if (ret == ++Idx)
    {
        if (++cfg.Tooltips > 1) cfg.Tooltips = 0;
    }

    //! Settings: InfoWindow
    else if (ret == ++Idx)
    {
        if (++cfg.InfoWindow > 1) cfg.InfoWindow = 0;
    }

    //! Settings: Screensaver
    else if (ret == ++Idx)
    {
        if (++cfg.Screensaver > 7) cfg.Screensaver = 0;
    }

    //! Settings: Widescreen
    else if (ret == ++Idx)
    {
        if (++cfg.Widescreen > 1) cfg.Widescreen = 0;
    }

    //! Settings: Rumble
    else if (ret == ++Idx)
    {
        if (++cfg.Rumble > 1) cfg.Rumble = 0;
    }

    SetOptionValues();

    return MENU_NONE;
}
