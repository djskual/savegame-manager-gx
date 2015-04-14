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
#include "UpdateSettingsMenu.hpp"
#include "SettingsMenu.h"
#include "../../Controls/Application.h"
//#include "../../Prompts/PromptWindows.h"
#include "../../Prompts/UpdateWindow.h"
#include "../../Network/update.h"
#include "../../Network/ChangeLog.h"

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
	: SettingsBrowser()
{
	SettingsMenu::Instance()->SetTitle(tr( "Update Settings" ));
	
	SetupOptionName();
	show();
}

void UpdateSettingsMenu::SetupOptionName()
{
	int Idx = 0;
	options->SetName(Idx++, tr( "Auto Connect" ));
	options->SetName(Idx++, tr( "Icon Update" ));
	options->SetName(Idx++, tr( "Language Update" ));
	options->SetName(Idx++, tr( "GameTDB Update" ));
	options->SetName(Idx++, tr( "Wiiload Receive" ));
	options->SetName(Idx++, tr( "Update" ));
	
	SetOptionValues();
}

void UpdateSettingsMenu::SetOptionValues()
{
	int Idx = 0;

	//! Settings: Auto Update
	options->SetValue(Idx++, tr(OnOffText[Settings.AutoConnect]));
	
	//! Settings: Icon Update
	if (Settings.UpdateIcon == 0)
		options->SetValue(Idx++, tr("OFF"));
	else if (Settings.UpdateIcon == 1)
		options->SetValue(Idx++, tr("Standard"));
	else
		options->SetValue(Idx++, "Black.Pearl");
	
	//! Settings: Language Update
	options->SetValue(Idx++, tr(LanguageText[Settings.UpdateLanguage]));
	
	//! Settings: GameTDB Update
	options->SetValue(Idx++, tr(OnOffText[Settings.UpdateGameTDB]));
	
	//! Settings: Wiiload Receive
	options->SetValue(Idx++, tr(OnOffText[Settings.Wiiload]));
	
	//! Button: Update
	options->SetValue(Idx++, " ");
}

void UpdateSettingsMenu::OnBrowserClick(GuiOptionBrowser * browser, int option)
{
	UpdateWindow * updateWindow = NULL;
	
	switch (option)
	{
		case 0:
			if (++Settings.AutoConnect >= MAX_ON_OFF) Settings.AutoConnect = OFF;
			break;
		case 1:
			if (++Settings.UpdateIcon > 2) Settings.UpdateIcon = OFF;
			
			//! download new icon
			if(Settings.UpdateIcon)
			{
				char text[100];
				if(Settings.UpdateIcon == 1)
					strcpy(text, tr("Standard Icon"));
				else if(Settings.UpdateIcon == 2)
					strcpy(text, tr("Black.Pearl Icon"));
				
				updateWindow = new UpdateWindow();
				if(updateWindow)
				{
					int choice = updateWindow->Choose(text, tr("Do you want to download now?"), tr("Yes"), tr("Cancel"));
					if(choice == 1)
					{
						if(!updateWindow->NetInitPrompt())
							goto out;
						
						updateWindow->ShowThrobber(tr("Downloading..."), text);
						
						int ret = UpdateIconPNG();
						if(ret)
							updateWindow->ShowMessage(tr("Icon successfully downloaded."));
						else
							updateWindow->ShowError(tr("Download failed."));
					}
					
				  out:
					Application::Instance()->PushForDelete(updateWindow);
					Application::Instance()->updateEvents();
				}
			}
			break;
		case 2:
			if (++Settings.UpdateLanguage >= LANG_UPDATE_MAX)
				Settings.UpdateLanguage = LANG_UPDATE_OFF;
			break;
		case 3:
			if (++Settings.UpdateGameTDB >= MAX_ON_OFF) Settings.UpdateGameTDB = OFF;
			break;
		case 4:
			if (++Settings.Wiiload >= MAX_ON_OFF) Settings.Wiiload = OFF;
			break;
		case 5:
			updateWindow = new UpdateWindow();
			if(updateWindow)
			{
				if(!CheckForUpdate(updateWindow))
				{
					int choice = 0;
					do
					{
						choice = updateWindow->Choose(tr("No new updates available"), "", (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_UPDATES)) ? tr("Update files") : "", tr("Show Changelog"), tr("Cancel"));
						if(choice == 1)
						{
							if(UpdateFiles(false, updateWindow))
								updateWindow->ShowMessage(tr("Update successfully finished"));
							
							break;
						}
						else if(choice == 2)
						{
							ChangeLog Changelog(updateWindow);
							if(!Changelog.Show())
								updateWindow->ShowError(tr("Failed to get the Changelog."));
						}
						
					} while(choice != 3);
				}
				
				Application::Instance()->PushForDelete(updateWindow);
				Application::Instance()->updateEvents();
			}
			break;
		default:
			break;
	}

	SetOptionValues();
}
