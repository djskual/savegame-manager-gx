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
#include "settings/menus/UpdateSettingsMenu.hpp"
#include "prompts/PromptWindows.h"
#include "network/update.h"
#include "network/ChangeLog.h"
#include "language/gettext.h"
#include "menu/MainWindow.h"
#include "main.h"

UpdateSettingsMenu::UpdateSettingsMenu()
    : SettingsMenu(tr("Update Settings"), &GuiOptions, MENU_NONE)
{
    int Idx = 0;
    Options->SetName(Idx++, "%s", tr( "Auto Update" ));
    Options->SetName(Idx++, "%s", tr( "Meta Update" ));
    Options->SetName(Idx++, "%s", tr( "Icon Update" ));
    Options->SetName(Idx++, "%s", tr( "WiiTDB Update" ));
    Options->SetName(Idx++, "%s", tr( "Update" ));
    
	SetOptionValues();
}

void UpdateSettingsMenu::SetOptionValues()
{
    int Idx = 0;

	//! Settings: Auto Update
    if (cfg.AutoConnect == 0)
        Options->SetValue(Idx++, tr( "OFF" ));
    else
        Options->SetValue(Idx++, tr( "ON" ));

    //! Settings: Meta Update
    if (cfg.UpdateMeta == 0)
        Options->SetValue(Idx++, tr( "OFF" ));
    else
        Options->SetValue(Idx++, tr( "ON" ));
	
	//! Settings: Icon Update
    if (cfg.UpdateIcon == 0)
        Options->SetValue(Idx++, tr( "OFF" ));
    else if (cfg.UpdateIcon == 1)
        Options->SetValue(Idx++, tr( "Standard" ));
	else
        Options->SetValue(Idx++, tr( "Black.Pearl" ));
	
	//! Settings: WiiTDB Update
    if (cfg.UpdateWiiTdb == 0)
        Options->SetValue(Idx++, tr( "OFF" ));
    else
        Options->SetValue(Idx++, tr( "ON" ));
	
	//! Button: Update
    Options->SetValue(Idx++, " ");
}

int UpdateSettingsMenu::GetMenuInternal()
{
    int ret = optionBrowser->GetClickedOption();

    if (ret < 0)
        return MENU_NONE;

    int Idx = -1;

    //! Settings: Auto Update
    if (ret == ++Idx)
    {
        if (++cfg.AutoConnect > 1) cfg.AutoConnect = 0;
    }

    //! Settings: Meta Update
    else if (ret == ++Idx)
    {
        if (++cfg.UpdateMeta > 1) cfg.UpdateMeta = 0;
    }

    //! Settings: Icon Update
    else if (ret == ++Idx)
    {
        if (++cfg.UpdateIcon > 2) cfg.UpdateIcon = 0;
		
		if(cfg.UpdateIcon > 0)
		{
			char text[20];
			if(cfg.UpdateIcon == 1)
				strcpy(text, tr("Standard Icon"));
			else if(cfg.UpdateIcon == 2)
				strcpy(text, tr("Black.Pearl Icon"));
			
			MainWindow::Instance()->SetState(STATE_DISABLED);
			if(WindowPrompt(text, tr("Do you want to download now?"), tr("Yes"), tr("Cancel"), 0, 0, false) == 1)
			{
				UpdateIconPNG();
				WindowPrompt(tr("Icon successfully downloaded."), 0, tr("OK"), 0, 0, 0, false);
			}
			MainWindow::Instance()->SetState(STATE_DEFAULT);
		}
    }

    //! Settings: WiiTDB Update
    else if (ret == ++Idx)
    {
        if (++cfg.UpdateWiiTdb > 1) cfg.UpdateWiiTdb = 0;
    }

    //! Button: Update
    else if (ret == ++Idx)
    {
        int res = CheckForUpdate();
		if(res == 0)
		{
			MainWindow::Instance()->SetState(STATE_DISABLED);
			
			int choice = WindowPrompt(tr("No new updates available"), 0, tr("OK"), tr("Show Changelog"), 0, 0, false);
			if(choice == 0)
			{
				ChangeLog Changelog;
				if(!Changelog.Show())
					WindowPrompt(tr("Failed to get the Changelog."), 0, tr("OK"), 0, 0, 0, false);
			}
			
			MainWindow::Instance()->SetState(STATE_DEFAULT);
		}
    }

    SetOptionValues();

    return MainWindow::Instance()->GetBootHomebrew() ? MENU_EXIT : MENU_NONE;
}
