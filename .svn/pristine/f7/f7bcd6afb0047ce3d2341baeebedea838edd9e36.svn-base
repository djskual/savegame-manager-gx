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

#include "FeaturesSettingsMenu.hpp"
#include "../../FileOperations/fileops.h"
#include "../../Prompts/PromptWindows.h"
#include "../../Prompts/ProgressWindow.h"
#include "../../XML/GameTDB.hpp"
#include "../../Language/gettext.h"
#include "../../Network/update.h"

static const char * OnOffText[MAX_ON_OFF] =
{
	trNOOP( "OFF" ),
	trNOOP( "ON" )
};

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
	: SettingsMenu(tr("Features Menu"), &GuiOptions, MENU_NONE)
{
	int Idx = 0;
	
	Options->SetName(Idx++, tr( "GameTDB Titles" ));
	Options->SetName(Idx++, tr( "Use IOS58" ));
	Options->SetName(Idx++, tr( "Screenshot Format" ));
	if(!Settings.Sneek)
		Options->SetName(Idx++, tr("Emu Nand browser"));
	
	SetOptionValues();
}

void FeaturesSettingsMenu::SetOptionValues()
{
	int Idx = 0;
	
	//! Settings: GameTDB Titles
	Options->SetValue(Idx++, tr(OnOffText[Settings.GameTDBTitles]));
	
	//! Settings: Use IOS58
	Options->SetValue(Idx++, tr(OnOffText[Settings.UseIOS58]));
	
	//! Settings: Screenshot Format
	Options->SetValue(Idx++, ScreenshotTypeText[Settings.ScreenshotFormat]);
	
	//! Settings: Emu Nand browse
	if(!Settings.Sneek) Options->SetValue(Idx++, tr(OnOffText[Settings.EmuNand]));
}

int FeaturesSettingsMenu::GetMenuInternal()
{
	int ret = optionBrowser->GetClickedOption();

	if (ret < 0)
		return MENU_NONE;

	int Idx = -1;

	//! Settings: GameTDB Titles
	if (ret == ++Idx)
	{
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
	}

	//! Settings: Use IOS58
	else if (ret == ++Idx)
	{
		if (++Settings.UseIOS58 >= MAX_ON_OFF) Settings.UseIOS58 = 0;
	}
	
	//! Settings: Screenshot Format
	else if (ret == ++Idx)
	{
		if (++Settings.ScreenshotFormat >= SCREENSHOT_MAX) Settings.ScreenshotFormat = 0;
	}
	
	//! Settings: Emu Nand browse
	else if (ret == ++Idx)
	{
		if (++Settings.EmuNand >= MAX_ON_OFF) Settings.EmuNand = 0;
	}
	
	SetOptionValues();

	return MENU_NONE;
}
