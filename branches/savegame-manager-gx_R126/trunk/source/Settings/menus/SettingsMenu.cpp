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

#include "../../LibWiiGui/gui.h"
#include "../../Prompts/ProgressWindow.h"
#include "../../EmuNand/CEmuMiiList.hpp"
#include "../../EmuNand/CEmuList.hpp"
#include "../../Saves/CSaveList.hpp"
#include "../../Miis/MiiInstaller.h"
#include "../../Language/gettext.h"
#include "../../Themes/CTheme.h"
#include "SettingsMenu.hpp"

SettingsMenu::SettingsMenu(const char * title, OptionList * opts, int returnTo)
	: GuiWindow(screenwidth, screenheight)
{
	Options = opts;
	returnToMenu = returnTo;
	oldGameTDBSetting = Settings.GameTDBTitles;
	oldEmuNandSetting = Settings.EmuNand;
	oldEmuNandPath = Settings.EmuNandPath;
	trigA = NULL;
	trigB = NULL;
	backBtnTxt = NULL;
	backBtnImg = NULL;
	backBtn = NULL;
	btnOutline = NULL;

	//! Skipping back button if there is no menu defined to go back to
	if(returnToMenu != MENU_NONE)
	{
		btnOutline = Resources::GetImageData("button_dialogue_box.png");
		
		trigA = new SimpleGuiTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
		
		trigB = new GuiTrigger();
		trigB->SetButtonOnlyTrigger(-1, WiiControls.BackButton | ClassicControls.BackButton << 16, GCControls.BackButton);
		
		backBtnTxt = new GuiText(tr("Back"), 22, thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
		backBtnImg = new GuiImage(btnOutline);
		backBtn = new GuiButton(backBtnImg, backBtnImg, 2, 3, -180, 400, trigA, btnSoundOver, btnSoundClick, 1);
		backBtn->SetLabel(backBtnTxt);
		backBtn->SetTrigger(trigB);
		Append(backBtn);
	}

	optionBrowser = new GuiOptionBrowser(Options);
	optionBrowser->SetPosition(0, 90);
	optionBrowser->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);

	titleTxt = new GuiText(title, 28, thColor("r=0 g=0 b=0 a=255 - flyingbutton menu title text color"));
	titleTxt->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	titleTxt->SetPosition(170, 38);
	titleTxt->SetMaxWidth(310, SCROLL_HORIZONTAL);

	Append(optionBrowser);
	Append(titleTxt);

	SetEffect(EFFECT_FADE, 50);
}

SettingsMenu::~SettingsMenu()
{
	ResumeGui();

	SetEffect(EFFECT_FADE, -50);
	while(this->GetEffect() > 0)
		usleep(100);

	HaltGui();
	if(parentElement)
		((GuiWindow *) parentElement)->Remove(this);

	RemoveAll();

	if(btnOutline)
		delete btnOutline;

	if(backBtnTxt)
		delete backBtnTxt;
	if(backBtnImg)
		delete backBtnImg;
	if(backBtn)
		delete backBtn;

	if(trigA)
		delete trigA;
	if(trigB)
		delete trigB;

	delete titleTxt;

	delete optionBrowser;

	ResumeGui();
	
	bool emu_list_ok = false;
	if (Settings.EmuNand && (oldEmuNandSetting != Settings.EmuNand || oldEmuNandPath.compare(Settings.EmuNandPath)))
	{
		Mii::Fix_EmuDB();
		ShowProgress(tr( "Getting EmuNand Lists..." ), NULL, NULL, 0, 1, false, false, THROBBER);
		EmuList.Get();
		EmuMiiList.Get();
		emu_list_ok = true;
		ProgressStop();
	}
	
	if (oldGameTDBSetting != Settings.GameTDBTitles)
	{
		ShowProgress(tr( "Getting Saves List..." ), NULL, NULL, 0, 1, false, false, THROBBER);
		
		SaveList.Get();
		
		if(Settings.EmuNand && !emu_list_ok)
			EmuList.Get();
		
		ProgressStop();
	}
}

int SettingsMenu::GetClickedOption()
{
	if(!optionBrowser)
		return -1;

	return optionBrowser->GetClickedOption();
}

int SettingsMenu::GetMenu()
{
	if(backBtn && backBtn->GetState() == STATE_CLICKED)
		return returnToMenu;
	
	return GetMenuInternal();
}
