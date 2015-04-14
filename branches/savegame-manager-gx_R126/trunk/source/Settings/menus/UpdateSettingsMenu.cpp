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

#include "UpdateSettingsMenu.hpp"
#include "../../Prompts/PromptWindows.h"
#include "../../Prompts/ProgressWindow.h"
#include "../../Network/update.h"
#include "../../Network/ChangeLog.h"
#include "../../Language/gettext.h"

static const char * OnOffText[MAX_ON_OFF] =
{
	trNOOP( "OFF" ),
	trNOOP( "ON" )
};

static const char *LanguageText[LANG_UPDATE_MAX] =
{
	trNOOP( "OFF" ),
	trNOOP( "All" ),
	trNOOP( "Installed" )
};

UpdateSettingsMenu::UpdateSettingsMenu()
	: SettingsMenu(tr("Update Settings"), &GuiOptions, MENU_NONE)
{
	int Idx = 0;
	Options->SetName(Idx++, tr( "Auto Connect" ));
	Options->SetName(Idx++, tr( "Meta Update" ));
	Options->SetName(Idx++, tr( "Icon Update" ));
	Options->SetName(Idx++, tr( "Language Update" ));
	Options->SetName(Idx++, tr( "GameTDB Update" ));
	Options->SetName(Idx++, tr( "Wiiload Receive" ));
	Options->SetName(Idx++, tr( "Update" ));
	
	SetOptionValues();
}

void UpdateSettingsMenu::SetOptionValues()
{
	int Idx = 0;

	//! Settings: Auto Update
	Options->SetValue(Idx++, tr(OnOffText[Settings.AutoConnect]));
	
	//! Settings: Meta Update
	Options->SetValue(Idx++, tr(OnOffText[Settings.UpdateMeta]));
	
	//! Settings: Icon Update
	if (Settings.UpdateIcon == 0)
		Options->SetValue(Idx++, tr("OFF"));
	else if (Settings.UpdateIcon == 1)
		Options->SetValue(Idx++, tr("Standard"));
	else
		Options->SetValue(Idx++, "Black.Pearl");
	
	//! Settings: Language Update
	Options->SetValue(Idx++, tr(LanguageText[Settings.UpdateLanguage]));
	
	//! Settings: GameTDB Update
	Options->SetValue(Idx++, tr(OnOffText[Settings.UpdateGameTDB]));
	
	//! Settings: Wiiload Receive
	Options->SetValue(Idx++, tr(OnOffText[Settings.Wiiload]));
	
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
		if (++Settings.AutoConnect > 1) Settings.AutoConnect = 0;
	}

	//! Settings: Meta Update
	else if (ret == ++Idx)
	{
		if (++Settings.UpdateMeta > 1) Settings.UpdateMeta = 0;
	}

	//! Settings: Icon Update
	else if (ret == ++Idx)
	{
		if (++Settings.UpdateIcon > 2) Settings.UpdateIcon = 0;
		
		
		if(Settings.UpdateIcon > 0)
		{
			char text[100];
			if(Settings.UpdateIcon == 1)
				strcpy(text, tr("Standard Icon"));
			else if(Settings.UpdateIcon == 2)
				strcpy(text, tr("Black.Pearl Icon"));
			
			if(WindowPrompt(text, tr("Do you want to download now?"), tr("Yes"), tr("Cancel")) == 1)
			{
				int ret = 0;
				
				if(!NetworkInitPrompt())
					goto out;
				
				ShowProgress(tr("Downloading..."), text, 0, 0, 1, false, false, UPDATE);
				ret = UpdateIconPNG();
				usleep(1000000);
				ProgressStop();
				
			  out:
				if(ret > 0) 
					WindowPrompt(tr("Icon successfully downloaded."), 0, tr("OK"));
			}
		}
	}

	//! Settings: Language Update
	else if (ret == ++Idx)
	{
		if (++Settings.UpdateLanguage > 2) Settings.UpdateLanguage = 0;
	}
	
	//! Settings: GameTDB Update
	else if (ret == ++Idx)
	{
		if (++Settings.UpdateGameTDB > 1) Settings.UpdateGameTDB = 0;
	}

	//! Settings: Wiiload Receive
	else if (ret == ++Idx)
	{
		if (++Settings.Wiiload > 1) Settings.Wiiload = 0;
	}

	//! Button: Update
	else if (ret == ++Idx)
	{
		HaltGui();
		GuiWindow * parent = (GuiWindow *) parentElement;
		if(parent) parent->SetState(STATE_DISABLED);
		this->SetState(STATE_DEFAULT);
		this->Remove(optionBrowser);
		ResumeGui();
		
		if(!CheckForUpdate())
		{
			int choice = WindowPrompt(tr("No new updates available"), 0, tr("Update files"), tr("Show Changelog"), tr("Cancel"));
			if(choice == 2)
			{
				ChangeLog Changelog;
				if(!Changelog.Show())
					WindowPrompt(tr("Error:"), tr("Failed to get the Changelog."), tr("OK"));
			}
			else if(choice == 1)
			{
				if(UpdateFiles())
					WindowPrompt(tr("Update successfully finished"), 0, tr("OK"));
			}
		}
		
		if(parent) parent->SetState(STATE_DEFAULT);
		this->Append(optionBrowser);
	}

	SetOptionValues();

	return MENU_NONE;
}
