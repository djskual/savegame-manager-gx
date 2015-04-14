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
#include "settings/menus/GlobalSettings.hpp"
#include "settings/menus/GUISettingsMenu.hpp"
#include "settings/menus/SoundSettingsMenu.hpp"
#include "settings/menus/CustomPathsSM.hpp"
#include "settings/menus/WiiTDBSettingsMenu.hpp"
#include "settings/menus/UpdateSettingsMenu.hpp"
#include "prompts/PromptWindows.h"
#include "language/gettext.h"
#include "main.h"

GlobalSettings::GlobalSettings()
    : FlyingButtonsMenu(tr("Settings"))
{
    creditsImgData = Resources::GetImageData(credits_button_png, credits_button_png_size);
    creditsOverImgData = Resources::GetImageData(credits_button_over_png, credits_button_over_png_size);
}

GlobalSettings::~GlobalSettings()
{
    Settings Settings;
	
	Settings.Save();
	
    Resources::Remove(creditsImgData);
    Resources::Remove(creditsOverImgData);
}

void GlobalSettings::SetupMainButtons()
{
    int pos = 0;

    SetMainButton(pos++, tr("GUI Settings"), MainButtonImgData, MainButtonImgOverData);
    SetMainButton(pos++, tr("Audio Settings"), MainButtonImgData, MainButtonImgOverData);
    SetMainButton(pos++, tr("Custom Paths"), MainButtonImgData, MainButtonImgOverData);
    SetMainButton(pos++, tr("WiiTDB"), MainButtonImgData, MainButtonImgOverData);
    SetMainButton(pos++, tr("Updates"), MainButtonImgData, MainButtonImgOverData);
    SetMainButton(pos++, tr("Default Settings"), MainButtonImgData, MainButtonImgOverData);
    SetMainButton(pos++, tr("Credits"), creditsImgData, creditsOverImgData);
	
}

void GlobalSettings::CreateSettingsMenu(int menuNr)
{
    Settings Settings;
	
	if(CurrentMenu)
        return;

    int Idx = 0;

    //! GUI Settings
    if(menuNr == Idx++)
    {
        HideMenu();
        MainWindow::Instance()->ResumeGui();
        CurrentMenu = new GuiSettingsMenu();
        Append(CurrentMenu);
    }
    //! Sound
    else if(menuNr == Idx++)
    {
        HideMenu();
        MainWindow::Instance()->ResumeGui();
        CurrentMenu = new SoundSettingsMenu();
        Append(CurrentMenu);
    }
    //! Custom Paths
    else if(menuNr == Idx++)
    {
        HideMenu();
		MainWindow::Instance()->ResumeGui();
		CurrentMenu = new CustomPathsSM();
		Append(CurrentMenu);
    }
    //! WiiTDB
    if(menuNr == Idx++)
    {
        HideMenu();
        MainWindow::Instance()->ResumeGui();
        CurrentMenu = new WiiTDBSettingsMenu();
        Append(CurrentMenu);
    }
    //! Update
    else if(menuNr == Idx++)
    {
        HideMenu();
		MainWindow::Instance()->ResumeGui();
        CurrentMenu = new UpdateSettingsMenu();
        Append(CurrentMenu);
    }
    //! Default Settings
    else if(menuNr == Idx++)
    {
        int choice = WindowPrompt(tr( "Reset Settings" ), tr("Are you sure ?"), tr( "Yes" ), tr( "Cancel" ));
        if (choice == 1)
        {
			MainWindow::Instance()->HaltGui();
			Settings.SetDefault();
			bgMusic->Load(cfg.BgMusicPath);
			bgMusic->SetLoop(cfg.BgMusicLoop);
			bgMusic->SetVolume(cfg.MusicVolume);
			bgMusic->Play();
			Settings.LoadLanguage(NULL, APP_DEFAULT);
			returnMenu = MENU_SETTINGS;
			MainWindow::Instance()->ResumeGui();
		}
	}
    //! Credits
    else if(menuNr == Idx++)
    {
        HideMenu();
        Remove(backBtn);
        MainWindow::Instance()->ResumeGui();
        ShowCredits();
        Append(backBtn);
        ShowMenu();
    }
}

void GlobalSettings::DeleteSettingsMenu()
{
   if(!CurrentMenu)
        return;

    int type = CurrentMenu->GetType();

    switch(type)
    {
        case CGUISettingsMenu:
            delete ((GuiSettingsMenu *) CurrentMenu);
            break;
        case CSoundSettingsMenu:
            delete ((SoundSettingsMenu *) CurrentMenu);
            break;
        case CCustomPathsSM:
            delete ((CustomPathsSM *) CurrentMenu);
            break;
        case CWiiTDBSettingsMenu:
            delete ((WiiTDBSettingsMenu *) CurrentMenu);
            break;
        case CUpdateSettingsMenu:
            delete ((UpdateSettingsMenu *) CurrentMenu);
            break;
        case CSettingsMenu:
        default:
            delete CurrentMenu;
            break;
    }

    CurrentMenu = NULL;
}
