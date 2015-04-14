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
#include "settings/menus/WiiTDBSettingsMenu.hpp"
#include "settings/menus/SettingsMenu.hpp"
#include "prompts/browsers/browsers.h"
#include "prompts/PromptWindows.h"
#include "prompts/ProgressWindow.h"
#include "language/gettext.h"
#include "network/update.h"
#include "menu/MainWindow.h"
#include "main.h"

WiiTDBSettingsMenu::WiiTDBSettingsMenu()
    : SettingsMenu(tr("WiiTDB Settings"), &GuiOptions, MENU_NONE)
{
    int Idx = 0;
    
	GuiOptions.SetName(Idx++, "%s", tr( "WiiTDB Titles" ));
    GuiOptions.SetName(Idx++, "%s", tr( "WiiTDB Path" ));
    GuiOptions.SetName(Idx++, "%s", tr( "WiiTDB Langue" ));
    GuiOptions.SetName(Idx++, "%s", tr( "JAP to EN" ));
    GuiOptions.SetName(Idx++, "%s", tr( "Updates" ));
    
    SetOptionValues();
}

WiiTDBSettingsMenu::~WiiTDBSettingsMenu()
{
    
}

void WiiTDBSettingsMenu::SetOptionValues()
{
    int Idx = 0;
	
	if (cfg.WiiTdbTitles == 0)
        GuiOptions.SetValue(Idx++, "%s", tr("OFF"));
    else
        GuiOptions.SetValue(Idx++, "%s", tr("ON"));

    GuiOptions.SetValue(Idx++, "%s", cfg.WiiTdbPath);
	
	char langname[50];
	GetLanguageToLangCode(cfg.WiiTdbLanguage, cfg.WiiTdbLanguageCode);
	GetLanguageOptionDisplayname(cfg.WiiTdbLanguage, langname);
	GuiOptions.SetValue(Idx++, "%s", langname);
	
	if (cfg.WiiTdb_JPtoEN == 0)
        GuiOptions.SetValue(Idx++, "%s", tr("OFF"));
    else
        GuiOptions.SetValue(Idx++, "%s", tr("ON"));

    GuiOptions.SetValue(Idx++, "%s", tr(" "));
}

int WiiTDBSettingsMenu::GetMenuInternal()
{
    int ret = optionBrowser->GetClickedOption();

    if (ret < 0)
        return MENU_NONE;

    int Idx = -1;

    //! Settings: WiiTDB Titles
    if (ret == ++Idx)
    {
        if (++cfg.WiiTdbTitles > 1) cfg.WiiTdbTitles = 0;
    }

    //! Settings: WiiTDB Path
    else if (ret == ++Idx)
    {
        titleTxt->SetText(tr( "WiiTDB Path" ));
        char entered[300];
		snprintf(entered, sizeof(entered), cfg.WiiTdbPath);

		MainWindow::Instance()->HaltGui();
		GuiWindow * parent = (GuiWindow *) parentElement;
		if(parent) parent->SetState(STATE_DISABLED);
		this->SetState(STATE_DEFAULT);
		this->Remove(optionBrowser);
		MainWindow::Instance()->ResumeGui();

		int result = BrowseCustom(cfg.WiiTdbPath, entered);

		if(parent) parent->SetState(STATE_DEFAULT);
		this->Append(optionBrowser);

		if (result == 1)
		{
			if (entered[strlen(entered)-1] != '/')
				strcat(entered, "/");

			snprintf(cfg.WiiTdbPath, sizeof(cfg.WiiTdbPath), entered);
			WindowPrompt(tr( "Path Changed" ), 0, tr( "OK" ));
		}

		titleTxt->SetText(tr( "WiiTDB Settings" ));
    }

    //! Settings: WiiTDB Langue
    else if (ret == ++Idx)
    {
        if (++cfg.WiiTdbLanguage > 9) cfg.WiiTdbLanguage = 0;
    }

    //! Settings: JAP to EN
    else if (ret == ++Idx)
    {
        if (++cfg.WiiTdb_JPtoEN > 1) cfg.WiiTdb_JPtoEN = 0;
    }

    //! Button: Update
    else if (ret == ++Idx)
    {
        int choice = WindowPrompt(tr("WiiTDB Update"), tr("Do you want to update now ?"), tr("Yes"), tr("Cancel"));
		if(choice)
		{
			if(NetworkInitPrompt())
			{
				StartProgress(tr("Updating WiiTDB"), THROBBER, true);
				ShowProgress(0, 1);
				
				int ret = UpdateWiiTdb();
				StopProgress();
				if(ret) {
					WindowPrompt(tr("WiiTDB Update"), tr("Update successfully finished"), tr("OK"));
				}
			}
		}
    }

    SetOptionValues();

    return MENU_NONE;
}
