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
#include <unistd.h>
#include "ParentalControlSM.hpp"
#include "../../Prompts/PromptWindows.h"
#include "../../Language/gettext.h"
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
	: SettingsMenu(tr("Parental Control"), &GuiOptions, MENU_NONE)
{
	int Idx = 0;
	Options->SetName(Idx++, tr( "Console" ));
	Options->SetName(Idx++, tr( "Password" ));
	Options->SetName(Idx++, tr( "Block Global Settings" ));
	Options->SetName(Idx++, tr( "Block Gui Settings" ));
	Options->SetName(Idx++, tr( "Block Audio Settings" ));
	Options->SetName(Idx++, tr( "Block Custom Paths" ));
	Options->SetName(Idx++, tr( "Block Theme Settings" ));
	Options->SetName(Idx++, tr( "Block Controls Settings" ));
	Options->SetName(Idx++, tr( "Block Parental Settings" ));
	Options->SetName(Idx++, tr( "Block Features Menu" ));
	Options->SetName(Idx++, tr( "Block Updates" ));
	Options->SetName(Idx++, tr( "Block Reset Settings" ));
	Options->SetName(Idx++, tr( "Block Manage Emu Nand" ));
	Options->SetName(Idx++, tr( "Block Manage DataBin" ));
	Options->SetName(Idx++, tr( "Block ManageAll" ));
	Options->SetName(Idx++, tr( "Block Manage" ));
	Options->SetName(Idx++, tr( "Block HBC Menu" ));
	Options->SetName(Idx++, tr( "Block Title Launcher" ));
	Options->SetName(Idx++, tr( "Block SD Reload Button" ));
	Options->SetName(Idx++, tr( "Block Saves Download" ));
	
	SetOptionValues();
}

void ParentalControlSM::SetOptionValues()
{
	int Idx = 0;

	//! Settings: Console
	Options->SetValue(Idx++, tr(LockModeText[Settings.GodMode]));

	if(!Settings.GodMode)
	{
		for(int i = Idx; i < Options->GetLength(); ++i)
			Options->SetValue(i, "********");
		
		return;
	}
	
	//! Settings: Password
	if (strcmp(Settings.unlockCode, "") == 0)
		Options->SetValue(Idx++, tr( "not set" ));
	else
		Options->SetValue(Idx++, Settings.unlockCode);
	
	//! Settings: Block Global Settings
	Options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_GLOBAL_SETTINGS) != 0)]));
	
	//! Settings: Block Gui Settings
	Options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_GUI_SETTINGS) != 0)]));

	//! Settings: Block Audio Settings
	Options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_AUDIO_SETTINGS) != 0)]));

	//! Settings: Block Custom Paths
	Options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_CUSTOMPATH_SETTINGS) != 0)]));

	//! Settings: Block Theme Settings
	Options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_THEME_SETTINGS) != 0)]));

	//! Settings: Block Controls Settings
	Options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_CONTROLS_SETTINGS) != 0)]));

	//! Settings: Block Parental Settings
	Options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_PARENTAL_SETTINGS) != 0)]));

	//! Settings: Block Features Settings
	Options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_FEATURES_MENU) != 0)]));
	
	//! Settings: Block Updates
	Options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_UPDATES) != 0)]));

	//! Settings: Block Reset Settings
	Options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_RESET_SETTINGS) != 0)]));

	//! Settings: Block Manage Emu Nand
	Options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_MANAGE_EMUNAND) != 0)]));
	
	//! Settings: Block Manage DataBin
	Options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_MANAGE_DATABIN) != 0)]));

	//! Settings: Block ManageAll
	Options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_MANAGE_ALL) != 0)]));

	//! Settings: Block Manage
	Options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_MANAGE) != 0)]));

	//! Settings: Block HBC Menu
	Options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_HBC_MENU) != 0)]));

	//! Settings: Block Title Launcher
	Options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_TITLE_LAUNCHER) != 0)]));
	
	//! Settings: Block SD Reload Button
	Options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_SD_RELOAD_BUTTON) != 0)]));
	
	//! Settings: Block Saves Download
	Options->SetValue(Idx++, tr(OnOffText[((Settings.ParentalBlocks & BLOCK_WIISAVE_DOWNLOAD) != 0)]));
}

int ParentalControlSM::GetMenuInternal()
{
	int ret = optionBrowser->GetClickedOption();

	if (ret < 0)
		return MENU_NONE;

	int Idx = -1;

	//! Settings: Console
	if (ret == ++Idx)
	{
		if (!Settings.GodMode)
		{
			//!password check to unlock
			SetState(STATE_DISABLED);
			int result = PasswordCheck(Settings.unlockCode);
			SetState(STATE_DEFAULT);
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
	}

	//! General permission check for all following
	else if(!Settings.GodMode)
	{
		WindowPrompt(tr( "Permission denied." ), tr( "Console must be unlocked for this option." ), tr( "OK" ));
	}

	//! Settings: Password
	else if (ret == ++Idx)
	{
		char entered[100];
		SetState(STATE_DISABLED);
		snprintf(entered, sizeof(entered), Settings.unlockCode);
		int result = OnScreenNumpad(entered, 9);
		SetState(STATE_DEFAULT);
		if (result == 1)
		{
			snprintf(Settings.unlockCode, sizeof(Settings.unlockCode), entered);
			WindowPrompt(tr( "Password:" ), tr( "Password has been changed" ), tr( "OK" ));
		}
	}
	
	//! Settings: Block Global Settings
	else if (ret == ++Idx)
	{
		Settings.ParentalBlocks ^= BLOCK_GLOBAL_SETTINGS;
	}

	//! Settings: Block Gui Settings
	else if (ret == ++Idx)
	{
		Settings.ParentalBlocks ^= BLOCK_GUI_SETTINGS;
	}

	//! Settings: Block Audio Settings
	else if (ret == ++Idx)
	{
		Settings.ParentalBlocks ^= BLOCK_AUDIO_SETTINGS;
	}

	//! Settings: Block Custom Paths
	else if (ret == ++Idx)
	{
		Settings.ParentalBlocks ^= BLOCK_CUSTOMPATH_SETTINGS;
	}

	//! Settings: Block Theme Settings
	else if (ret == ++Idx)
	{
		Settings.ParentalBlocks ^= BLOCK_THEME_SETTINGS;
	}

	//! Settings: Block Controls Settings
	else if (ret == ++Idx)
	{
		Settings.ParentalBlocks ^= BLOCK_CONTROLS_SETTINGS;
	}

	//! Settings: Block Parental Settings
	else if (ret == ++Idx)
	{
		Settings.ParentalBlocks ^= BLOCK_PARENTAL_SETTINGS;
	}

	//! Settings: Block Features Settings
	else if (ret == ++Idx)
	{
		Settings.ParentalBlocks ^= BLOCK_FEATURES_MENU;
	}

	//! Settings: Block Updates
	else if (ret == ++Idx)
	{
		Settings.ParentalBlocks ^= BLOCK_UPDATES;
	}

	//! Settings: Block Reset Settings
	else if (ret == ++Idx)
	{
		Settings.ParentalBlocks ^= BLOCK_RESET_SETTINGS;
	}

	//! Settings: Block Manage Emu Nand
	else if (ret == ++Idx)
	{
		Settings.ParentalBlocks ^= BLOCK_MANAGE_EMUNAND;
	}

	//! Settings: Block Manage DataBin
	else if (ret == ++Idx)
	{
		Settings.ParentalBlocks ^= BLOCK_MANAGE_DATABIN;
	}

	//! Settings: Block ManageAll
	else if (ret == ++Idx)
	{
		Settings.ParentalBlocks ^= BLOCK_MANAGE_ALL;
	}

	//! Settings: Block Manage
	else if (ret == ++Idx)
	{
		Settings.ParentalBlocks ^= BLOCK_MANAGE;
	}

	//! Settings: Block HBC Menu
	else if (ret == ++Idx)
	{
		Settings.ParentalBlocks ^= BLOCK_HBC_MENU;
	}

	//! Settings: Block Title Launcher
	else if (ret == ++Idx)
	{
		Settings.ParentalBlocks ^= BLOCK_TITLE_LAUNCHER;
	}
	
	//! Settings: Block SD Reload Button
	else if (ret == ++Idx)
	{
		Settings.ParentalBlocks ^= BLOCK_SD_RELOAD_BUTTON;
	}

	//! Settings: Block Saves Download
	else if (ret == ++Idx)
	{
		Settings.ParentalBlocks ^= BLOCK_WIISAVE_DOWNLOAD;
	}

	SetOptionValues();

	return MENU_NONE;
}
