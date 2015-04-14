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
#include "settings/menus/CustomPathsSM.hpp"
#include "prompts/PromptWindows.h"
#include "language/gettext.h"
#include "file/fileops.h"
#include "prompts/browsers/browsers.h"
#include "menu/MainWindow.h"
#include "main.h"

extern bool boothomebrew;

CustomPathsSM::CustomPathsSM()
    : SettingsMenu(tr("Custom Paths"), &GuiOptions, MENU_NONE)
{
    int Idx = 0;
    Options->SetName(Idx++, tr("Custom Font"));
    Options->SetName(Idx++, tr("Saves Path"));
    Options->SetName(Idx++, tr("Miis Path"));
    Options->SetName(Idx++, tr("Update Path"));
    
	SetOptionValues();
}

void CustomPathsSM::SetOptionValues()
{
    int Idx = 0;

    //! Settings: Custom Font
    Options->SetValue(Idx++, cfg.CustomFont);

    //! Settings: Saves Path
    Options->SetValue(Idx++, cfg.SaveGamePath);

    //! Settings: Miis Path
    Options->SetValue(Idx++, cfg.SaveMiisPath);

    //! Settings: Update Path
    Options->SetValue(Idx++, cfg.UpdatePath);

}

int CustomPathsSM::GetMenuInternal()
{
    int ret = optionBrowser->GetClickedOption();

    if (ret < 0)
        return MENU_NONE;

    int Idx = -1;

    //! Settings: Custom Font
    if (ret == ++Idx)
    {
        titleTxt->SetText(tr( "Custom Font" ));
        if(ChangeFilePath(cfg.CustomFont, sizeof(cfg.CustomFont), "ttf") == 1)
		{
			int choice = WindowPrompt(tr("Font changed"), tr("Do you want to reboot now ?"), tr("Yes"), tr("Later"));
			if (choice == 1)
				RebootApp();
		}
    }

    //! Settings: Saves Path
    else if (ret == ++Idx)
    {
        titleTxt->SetText(tr( "Save Path" ));
        ChangePath(cfg.SaveGamePath, sizeof(cfg.SaveGamePath));
    }

    //! Settings: Miis Path
    else if (ret == ++Idx)
    {
        titleTxt->SetText(tr( "Miis Path" ));
        ChangePath(cfg.SaveMiisPath, sizeof(cfg.SaveMiisPath));
    }

    //! Settings: Update Path
    else if (ret == ++Idx)
    {
        titleTxt->SetText(tr( "Update Path" ));
        ChangePath(cfg.UpdatePath, sizeof(cfg.UpdatePath));
    }

    //! Global set back of the titleTxt after a change
    titleTxt->SetText(tr( "Custom Paths" ));
    SetOptionValues();

    return MainWindow::Instance()->GetBootHomebrew() ? MENU_EXIT : MENU_NONE;
}

int CustomPathsSM::ChangePath(char * SettingsPath, int SizeOfPath)
{
    MainWindow::Instance()->HaltGui();
    GuiWindow * parent = (GuiWindow *) parentElement;
    if(parent) parent->SetState(STATE_DISABLED);
    this->SetState(STATE_DEFAULT);
    this->Remove(optionBrowser);
    MainWindow::Instance()->ResumeGui();

    char entered[300];
    int result = BrowseCustom(SettingsPath, entered);
	
    if(parent) parent->SetState(STATE_DEFAULT);
    this->Append(optionBrowser);
	
    if (result == 1)
    {
        if ((entered[strlen(entered)-1] != '/'))
            strcat(entered, "/");

        snprintf(SettingsPath, SizeOfPath, entered);
        WindowPrompt(tr( "Path Changed" ), 0, tr( "OK" ));
    }

    return result;
}

int CustomPathsSM::ChangeFilePath(char * SettingsPath, int SizeOfPath, const char * filetype)
{
    MainWindow::Instance()->HaltGui();
    GuiWindow * parent = (GuiWindow *) parentElement;
    if(parent) parent->SetState(STATE_DISABLED);
    this->SetState(STATE_DEFAULT);
    this->Remove(optionBrowser);
    MainWindow::Instance()->ResumeGui();

    char entered[MAXPATHLEN];
    int result = BrowseCustomFile(SettingsPath, entered);
	
    if(parent) parent->SetState(STATE_DEFAULT);
    this->Append(optionBrowser);
	
    if (result == 1)
    {
        if(VerifyFileType(entered, filetype) == true)
		{
			snprintf(SettingsPath, SizeOfPath, entered);
			//WindowPrompt(tr( "Path Changed" ), 0, tr( "OK" ));
		}
		else
			result = 0;
    }

    return result;
}
