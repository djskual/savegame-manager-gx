/****************************************************************************
 * Copyright (C) 2010
 * by Dimok
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
#include "GlobalSettings.hpp"
#include "../../Prompts/PromptWindows.h"
#include "../../Language/gettext.h"
#include "GUISettingsMenu.hpp"
#include "ParentalControlSM.hpp"
#include "ControlsSettingsMenu.hpp"
#include "SoundSettingsMenu.hpp"
#include "CustomPathsSM.hpp"
#include "FeaturesSettingsMenu.hpp"
#include "ThemeSettingsMenu.hpp"
#include "UpdateSettingsMenu.hpp"
#include "../../Themes/CTheme.h"

GlobalSettings::GlobalSettings()
	: FlyingButtonsMenu(tr("Global Settings"))
{
	creditsImgData = Resources::GetImageData("settings_credits_title.png");
	creditsImgOverData = Resources::GetImageData("settings_credits_title_over.png");
}

GlobalSettings::~GlobalSettings()
{
	Settings.Save();

	delete creditsImgData;
	delete creditsImgOverData;
}

void GlobalSettings::SetupMainButtons()
{
	int pos = 0;

	SetMainButton(pos++, tr( "GUI Settings" ), MainButtonImgData, MainButtonImgOverData);
	SetMainButton(pos++, tr( "Audio Settings" ), MainButtonImgData, MainButtonImgOverData);
	SetMainButton(pos++, tr( "Custom Paths" ), MainButtonImgData, MainButtonImgOverData);
	SetMainButton(pos++, tr( "Theme Settings" ), MainButtonImgData, MainButtonImgOverData);
	SetMainButton(pos++, tr( "Controller Settings" ), MainButtonImgData, MainButtonImgOverData);
	SetMainButton(pos++, tr( "Parental Control" ), MainButtonImgData, MainButtonImgOverData);
	SetMainButton(pos++, tr( "Features Menu" ), MainButtonImgData, MainButtonImgOverData);
	SetMainButton(pos++, tr( "Updates" ), MainButtonImgData, MainButtonImgOverData);
	SetMainButton(pos++, tr( "Default Settings" ), MainButtonImgData, MainButtonImgOverData);
	SetMainButton(pos++, tr( "Credits" ), creditsImgData, creditsImgOverData);
}

void GlobalSettings::CreateSettingsMenu(int menuNr)
{
	if(CurrentMenu)
		return;

	int Idx = 0;

	//! GUI Settings
	if(menuNr == Idx++)
	{
		if(Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_GUI_SETTINGS))
		{
			HideMenu();
			ResumeGui();
			CurrentMenu = new GuiSettingsMenu();
			Append(CurrentMenu);
		}
		else
			WindowPrompt(tr( "Permission denied." ), tr( "Console must be unlocked for this option." ), tr( "OK" ));
	}
	//! Audio Settings
	else if(menuNr == Idx++)
	{
		if(Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_AUDIO_SETTINGS))
		{
			HideMenu();
			ResumeGui();
			CurrentMenu = new SoundSettingsMenu();
			Append(CurrentMenu);
		}
		else
			WindowPrompt(tr( "Permission denied." ), tr( "Console must be unlocked for this option." ), tr( "OK" ));
	}
	//! Custom Paths
	else if(menuNr == Idx++)
	{
		if(Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_CUSTOMPATH_SETTINGS))
		{
			HideMenu();
			ResumeGui();
			CurrentMenu = new CustomPathsSM();
			Append(CurrentMenu);
		}
		else
			WindowPrompt(tr( "Permission denied." ), tr( "Console must be unlocked for this option." ), tr( "OK" ));
	}
	//! Theme Settings
	else if(menuNr == Idx++)
	{
		if(Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_THEME_SETTINGS))
		{
			HideMenu();
			ResumeGui();
			CurrentMenu = new ThemeSettingsMenu();
			Append(CurrentMenu);
		}
		else
			WindowPrompt(tr( "Permission denied." ), tr( "Console must be unlocked for this option." ), tr( "OK" ));
	}
	//! Controls Settings
	else if(menuNr == Idx++)
	{
		if(Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_CONTROLS_SETTINGS))
		{
			HideMenu();
			ResumeGui();
			CurrentMenu = new ControlsSettingsMenu();
			Append(CurrentMenu);
		}
		else
			WindowPrompt(tr( "Permission denied." ), tr( "Console must be unlocked for this option." ), tr( "OK" ));
	}
	//! Parental Control
	else if(menuNr == Idx++)
	{
		if(Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_PARENTAL_SETTINGS))
		{
			HideMenu();
			ResumeGui();
			CurrentMenu = new ParentalControlSM();
			Append(CurrentMenu);
		}
		else
			WindowPrompt(tr( "Permission denied." ), tr( "Console must be unlocked for this option." ), tr( "OK" ));
	}
	//! Features Menu
	else if(menuNr == Idx++)
	{
		if(Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_FEATURES_MENU))
		{
			HideMenu();
			ResumeGui();
			CurrentMenu = new FeaturesSettingsMenu();
			Append(CurrentMenu);
		}
		else
			WindowPrompt(tr( "Permission denied." ), tr( "Console must be unlocked for this option." ), tr( "OK" ));
	}
	//! Update
	else if(menuNr == Idx++)
	{
		if (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_UPDATES))
		{
			HideMenu();
			ResumeGui();
			CurrentMenu = new UpdateSettingsMenu();
			Append(CurrentMenu);
		}
		else
			WindowPrompt(tr( "Permission denied." ), tr( "Console must be unlocked for this option." ), tr( "OK" ));
	}
	//! Default Settings
	else if(menuNr == Idx++)
	{
		if (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_RESET_SETTINGS))
		{
			int choice = WindowPrompt(tr( "Are you sure you want to reset?" ), 0, tr( "Yes" ), tr( "Cancel" ));
			if (choice == 1)
			{
				HaltGui();
				gettextCleanUp();
				Settings.Reset();
				returnMenu = MENU_SETTINGS;
				ResumeGui();
			}
		}
		else
			WindowPrompt(tr( "Permission denied." ), tr( "Console must be unlocked for this option." ), tr( "OK" ));
	}
	//! Credits
	else if(menuNr == Idx++)
	{
		HideMenu();
		Remove(backBtn);
		ResumeGui();
		ShowCredits();
		Append(backBtn);
		ShowMenu();
	}
}

void GlobalSettings::DeleteSettingsMenu()
{
	delete CurrentMenu;

	CurrentMenu = NULL;
}
