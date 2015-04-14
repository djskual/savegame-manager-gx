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
#include "ParentalControlSM.hpp"
#include "SettingsMenu.h"
#include "../../Prompts/PromptWindows.h"
#include "../../Tools/PasswordCheck.h"

static const char * OnOffText[MAX_ON_OFF] =
{
	trNOOP( "OFF" ),
	trNOOP( "ON" )
};

static const char * LockModeText[MAX_ON_OFF] =
{
	trNOOP( "Locked" ),
	trNOOP( "Unlocked" )
};

ParentalControlSM::ParentalControlSM()
	: SettingsBrowser()
{
	SettingsMenu::Instance()->SetTitle(tr( "Parental Control" ));
	
	SetupOptionName();
	show();
}

void ParentalControlSM::SetupOptionName()
{
	int Idx = 0;
	options->SetName(Idx++, tr( "Console" ));
	options->SetName(Idx++, tr( "Password" ));
	options->SetName(Idx++, tr( "Remember Unlock" ));
	options->SetName(Idx++, tr( "Block Global Settings" ));
	options->SetName(Idx++, tr( "Block Gui Settings" ));
	options->SetName(Idx++, tr( "Block Audio Settings" ));
	options->SetName(Idx++, tr( "Block Custom Paths" ));
	options->SetName(Idx++, tr( "Block Theme Settings" ));
	options->SetName(Idx++, tr( "Block Controls Settings" ));
	options->SetName(Idx++, tr( "Block Parental Settings" ));
	options->SetName(Idx++, tr( "Block Features Menu" ));
	options->SetName(Idx++, tr( "Block Updates" ));
	options->SetName(Idx++, tr( "Block Reset Settings" ));
	options->SetName(Idx++, tr( "Block Manage Emu Nand" ));
	options->SetName(Idx++, tr( "Block Manage DataBin" ));
	options->SetName(Idx++, tr( "Block ManageAll" ));
	options->SetName(Idx++, tr( "Block Manage" ));
	options->SetName(Idx++, tr( "Block HBC Menu" ));
	options->SetName(Idx++, tr( "Block Title Launcher" ));
	options->SetName(Idx++, tr( "Block SD Reload Button" ));
	options->SetName(Idx++, tr( "Block Saves Download" ));
	
	SetOptionValues();
}

void ParentalControlSM::SetOptionValues()
{
	int Idx = 0;

	//! Settings: Console
	options->SetValue(Idx++, tr(LockModeText[Settings.GodMode]));

	if(!Settings.GodMode)
	{
		for(int i = Idx; i < options->GetLength(); ++i)
			options->SetValue(i, "********");
		
		return;
	}
	
	//! Settings: Password
	if (strcmp(Settings.unlockCode, "") == 0)
		options->SetValue(Idx++, tr( "not set" ));
	else
		options->SetValue(Idx++, Settings.unlockCode);
	
	//! Settings: Remember Unlock
	options->SetValue(Idx++, tr(OnOffText[Settings.RememberUnlock]));

	//! Settings: Block Global Settings
	options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_GLOBAL_SETTINGS) != 0)]));
	
	//! Settings: Block Gui Settings
	options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_GUI_SETTINGS) != 0)]));

	//! Settings: Block Audio Settings
	options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_AUDIO_SETTINGS) != 0)]));

	//! Settings: Block Custom Paths
	options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_CUSTOMPATH_SETTINGS) != 0)]));

	//! Settings: Block Theme Settings
	options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_THEME_SETTINGS) != 0)]));

	//! Settings: Block Controls Settings
	options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_CONTROLS_SETTINGS) != 0)]));

	//! Settings: Block Parental Settings
	options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_PARENTAL_SETTINGS) != 0)]));

	//! Settings: Block Features Settings
	options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_FEATURES_MENU) != 0)]));
	
	//! Settings: Block Updates
	options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_UPDATES) != 0)]));

	//! Settings: Block Reset Settings
	options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_RESET_SETTINGS) != 0)]));

	//! Settings: Block Manage Emu Nand
	options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_MANAGE_EMUNAND) != 0)]));
	
	//! Settings: Block Manage DataBin
	options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_MANAGE_DATABIN) != 0)]));

	//! Settings: Block ManageAll
	options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_MANAGE_ALL) != 0)]));

	//! Settings: Block Manage
	options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_MANAGE) != 0)]));

	//! Settings: Block HBC Menu
	options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_HBC_MENU) != 0)]));

	//! Settings: Block Title Launcher
	options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_TITLE_LAUNCHER) != 0)]));
	
	//! Settings: Block SD Reload Button
	options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_SD_RELOAD_BUTTON) != 0)]));
	
	//! Settings: Block Saves Download
	options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_WIISAVE_DOWNLOAD) != 0)]));
}

void ParentalControlSM::OnBrowserClick(GuiOptionBrowser * browser UNUSED, int option)
{
	//! General permission check for all following
	if(option > 0 && !Settings.GodMode)
	{
		WindowPrompt(tr( "Permission denied." ), tr( "Console must be unlocked for this option." ), tr( "OK" ));
		return;
	}
	
	switch(option)
	{
		case 0:
			if (!Settings.GodMode)
			{
				//!password check to unlock
				int result = PasswordCheck(Settings.unlockCode);
				if (result > 0)
				{
					if(result == 1)
						WindowPrompt( tr( "Correct Password" ), tr( "All the features of SaveGame Manager GX are unlocked." ), tr( "OK" ));
					Settings.GodMode = 1;
				}
				else if(result < 0)
					WindowPrompt(tr( "Wrong Password" ), tr( "SaveGame Manager GX is protected" ), tr( "OK" ));
			}
			else
			{
				int choice = WindowPrompt(tr( "Lock Console" ), tr( "Are you sure?" ), tr( "Yes" ), tr( "No" ));
				if (choice == 1)
				{
					WindowPrompt(tr( "Console Locked" ), tr( "SaveGame Manager GX is protected" ), tr( "OK" ));
					Settings.GodMode = 0;
				}
			}
			break;
		case 1:
		{
			char entered[100];
			snprintf(entered, sizeof(entered), Settings.unlockCode);
			int result = OnScreenNumpad(entered, 9, true);
			if (result)
			{
				snprintf(Settings.unlockCode, sizeof(Settings.unlockCode), entered);
				WindowPrompt(tr( "Password:" ), tr( "Password has been changed" ), tr( "OK" ));
			}
			break;
		}
		case 2:
			if (++Settings.RememberUnlock >= MAX_ON_OFF) Settings.RememberUnlock = OFF;
			break;
		case 3:
			Settings.ParentalBlocks ^= BLOCK_GLOBAL_SETTINGS;
			break;
		case 4:
			Settings.ParentalBlocks ^= BLOCK_GUI_SETTINGS;
			break;
		case 5:
			Settings.ParentalBlocks ^= BLOCK_AUDIO_SETTINGS;
			break;
		case 6:
			Settings.ParentalBlocks ^= BLOCK_CUSTOMPATH_SETTINGS;
			break;
		case 7:
			Settings.ParentalBlocks ^= BLOCK_THEME_SETTINGS;
			break;
		case 8:
			Settings.ParentalBlocks ^= BLOCK_CONTROLS_SETTINGS;
			break;
		case 9:
			Settings.ParentalBlocks ^= BLOCK_PARENTAL_SETTINGS;
			break;
		case 10:
			Settings.ParentalBlocks ^= BLOCK_FEATURES_MENU;
			break;
		case 11:
			Settings.ParentalBlocks ^= BLOCK_UPDATES;
			break;
		case 12:
			Settings.ParentalBlocks ^= BLOCK_RESET_SETTINGS;
			break;
		case 13:
			Settings.ParentalBlocks ^= BLOCK_MANAGE_EMUNAND;
			break;
		case 14:
			Settings.ParentalBlocks ^= BLOCK_MANAGE_DATABIN;
			break;
		case 15:
			Settings.ParentalBlocks ^= BLOCK_MANAGE_ALL;
			break;
		case 16:
			Settings.ParentalBlocks ^= BLOCK_MANAGE;
			break;
		case 17:
			Settings.ParentalBlocks ^= BLOCK_HBC_MENU;
			break;
		case 18:
			Settings.ParentalBlocks ^= BLOCK_TITLE_LAUNCHER;
			break;
		case 19:
			Settings.ParentalBlocks ^= BLOCK_SD_RELOAD_BUTTON;
			break;
		case 20:
			Settings.ParentalBlocks ^= BLOCK_WIISAVE_DOWNLOAD;
			break;
		default:
			break;
	}

	SetOptionValues();
}
