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

#include "CustomPathsSM.hpp"
#include "../../FileOperations/fileops.h"
#include "../../Prompts/PromptWindows.h"
#include "../../Language/gettext.h"
#include "../../Prompts/SelectBrowser.h"
#include "../../Themes/CTheme.h"

CustomPathsSM::CustomPathsSM()
	: SettingsMenu(tr("Custom Paths"), &GuiOptions, MENU_NONE)
{
	int Idx = 0;
	
	Options->SetName(Idx++, tr("Saves Path"));
	Options->SetName(Idx++, tr("Data.bin Path"));
	Options->SetName(Idx++, tr("Miis Path"));
	Options->SetName(Idx++, tr("Update Path"));
	Options->SetName(Idx++, tr("Homebrew Path"));
	Options->SetName(Idx++, tr("GameTDB Path"));
	Options->SetName(Idx++, tr("Screenshot Path"));
	if(!Settings.Sneek) Options->SetName(Idx++, tr("Emu Nand Path"));
	
	SetOptionValues();
}

void CustomPathsSM::SetOptionValues()
{
	int Idx = 0;
	
	//! Settings: Saves Path
	Options->SetValue(Idx++, Settings.SaveGamePath);
	
	//! Settings: Data.bin Path
	Options->SetValue(Idx++, Settings.DataBinPath);
	
	//! Settings: Miis Path
	Options->SetValue(Idx++, Settings.SaveMiisPath);
	
	//! Settings: Update Path
	Options->SetValue(Idx++, Settings.UpdatePath);
	
	//! Settings: Homebrew Path
	Options->SetValue(Idx++, Settings.HomebrewPath);
	
	//! Settings: GameTDB Path
	Options->SetValue(Idx++, Settings.GameTDBPath);
	
	//! Settings: Screenshot Path
	Options->SetValue(Idx++, Settings.ScreenshotPath);
	
	//! Settings: Emu Nand Path
	if(!Settings.Sneek)
		Options->SetValue(Idx++, Settings.EmuNandPath);
}

int CustomPathsSM::GetMenuInternal()
{
	int ret = optionBrowser->GetClickedOption();
	
	if (ret < 0)
		return MENU_NONE;
	
	int Idx = -1;
	
	//! Settings: Saves Path
	if (ret == ++Idx)
	{
		titleTxt->SetText(tr( "Save Path" ));
		showSelectBrowser(Settings.SaveGamePath, SAVEPATHBROWSER);
	}
	
	//! Settings: Saves Path
	if (ret == ++Idx)
	{
		titleTxt->SetText(tr( "Data.bin Path" ));
		showSelectBrowser(Settings.DataBinPath, DATABINPATHBROWSER);
	}
	
	//! Settings: Miis Path
	else if (ret == ++Idx)
	{
		titleTxt->SetText(tr( "Miis Path" ));
		showSelectBrowser(Settings.SaveMiisPath, MIIPATHBROWSER);
	}
	
	//! Settings: Update Path
	else if (ret == ++Idx)
	{
		titleTxt->SetText(tr( "Update Path" ));
		showSelectBrowser(Settings.UpdatePath, UPDATEPATHBROWSER);
	}
	
	//! Settings: Homebrew Path
	else if (ret == ++Idx)
	{
		titleTxt->SetText(tr( "Homebrew Path" ));
		showSelectBrowser(Settings.HomebrewPath, HOMEBREWPATHBROWSER);
	}
	
	//! Settings: GameTDB Path
	else if (ret == ++Idx)
	{
		titleTxt->SetText(tr( "GameTDB Path" ));
		showSelectBrowser(Settings.GameTDBPath, GAMETDBPATHBROWSER);
	}
	
	//! Settings: Screenshot Path
	else if (ret == ++Idx)
	{
		titleTxt->SetText(tr( "Screenshot Path" ));
		showSelectBrowser(Settings.ScreenshotPath, SCREENSHOTPATHBROWSER);
	}
	
	//! Settings: Emu Nand Path
	else if (ret == ++Idx)
	{
		titleTxt->SetText(tr( "Emu Nand Path" ));
		showSelectBrowser(Settings.EmuNandPath, EMUNANDPATHBROWSER);
	}
	
	//! Global set back of the titleTxt after a change
	titleTxt->SetText(tr( "Custom Paths" ));
	SetOptionValues();
	
	return MENU_NONE;
}

void CustomPathsSM::showSelectBrowser(const char * enterPath, short Type)
{
	GuiWindow * parent = (GuiWindow *) parentElement;
	if(parent) parent->SetState(STATE_DISABLED);
	this->Remove(optionBrowser);
	
	selectBrowser(enterPath, Type);
	
	if(parent) parent->SetState(STATE_DEFAULT);
	this->Append(optionBrowser);
}
