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
#include "FeaturesSettingsMenu.hpp"
#include "SettingsMenu.h"

/*static const char * OnOffText[MAX_ON_OFF] =
{
	trNOOP( "OFF" ),
	trNOOP( "ON" )
};*/

static const char *ScreenshotTypeText[SCREENSHOT_MAX] =
{
	"PNG",
	"JPEG",
	"GIF",
	"TIFF",
	"BMP",
	"GD",
	"GD2"
};

FeaturesSettingsMenu::FeaturesSettingsMenu()
	: SettingsBrowser()
{
	SettingsMenu::Instance()->SetTitle(tr( "Features Menu" ));
	
	SetupOptionName();
	show();
}

void FeaturesSettingsMenu::SetupOptionName()
{
	int Idx = 0;
	
	//options->SetName(Idx++, tr( "GameTDB Titles" ));
	options->SetName(Idx++, tr( "Screenshot Format" ));
	//if(!Settings.Sneek)
	//	options->SetName(Idx++, tr("Emu Nand browser"));
	
	SetOptionValues();
}

void FeaturesSettingsMenu::SetOptionValues()
{
	int Idx = 0;
	
	//! Settings: GameTDB Titles
	//Options->SetValue(Idx++, tr(OnOffText[Settings.GameTDBTitles]));
	
	//! Settings: Screenshot Format
	options->SetValue(Idx++, ScreenshotTypeText[Settings.ScreenshotFormat]);
	
	//! Settings: Emu Nand browse
	//if(!Settings.Sneek) Options->SetValue(Idx++, tr(OnOffText[Settings.EmuNand]));
}

void FeaturesSettingsMenu::OnBrowserClick(GuiOptionBrowser * browser UNUSED, int option)
{
	switch (option)
	{
		/*case 0:
			if (++Settings.GameTDBTitles >= MAX_ON_OFF) Settings.GameTDBTitles = 0;
		
			if (Settings.GameTDBTitles == 1)
			{
				std::string xmlPath= Settings.GameTDBPath;
				xmlPath += "/wiitdb.xml";
				
				if (!CheckFile(xmlPath.c_str()))
				{
					int choice = WindowPrompt(tr("GameTDB not found."), tr("Do you want to update now ?"), tr("Yes"), tr("Cancel"));
					if(choice)
					{
						if(NetworkInitPrompt())
						{
							ShowProgress(tr("Updating GameTDB..."), "wiitdb.zip", 0, 0, 1, false, false, UPDATE);
							
							int ret = UpdateGameTDB();
							ProgressStop();
							if(ret) {
								WindowPrompt(tr("GameTDB Update"), tr("Update successfully finished"), tr("OK"));
							}
						}
					}
				}
			}
			break;*/
		case 0:
			if (++Settings.ScreenshotFormat >= SCREENSHOT_MAX) Settings.ScreenshotFormat = SCREENSHOT_PNG;
			break;
		/*case 2:
			if (++Settings.EmuNand >= MAX_ON_OFF) Settings.EmuNand = OFF;
			break;*/
		default:
			break;
	}
	
	SetOptionValues();
}
