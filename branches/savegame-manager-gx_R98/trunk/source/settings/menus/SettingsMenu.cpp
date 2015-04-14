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
#include "SettingsMenu.hpp"
#include "language/gettext.h"
#include "settings/settings.h"
#include "system/Resources.h"
#include "menu/MainWindow.h"

SettingsMenu::SettingsMenu(const char * title, CustomOptionList * opts, int returnTo)
    : GuiWindow(screenwidth, screenheight)
{
    Options = opts;
    returnToMenu = returnTo;
    backBtn = NULL;
    trigA = NULL;
    trigB = NULL;
    backBtnTxt = NULL;
    backBtnImg = NULL;
    backBtn = NULL;
    btnOutline = NULL;

    //! Skipping back button if there is no menu defined to go back to
    if(returnToMenu != MENU_NONE)
    {
        btnOutline = Resources::GetImageData(settings_menu_button_png, settings_menu_button_png_size);

        trigA = new SimpleGuiTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

        trigB = new GuiTrigger();
        trigB->SetButtonOnlyTrigger(-1, WPAD_BUTTON_B | WPAD_CLASSIC_BUTTON_B, PAD_BUTTON_B);

        backBtnTxt = new GuiText(tr("Go Back"), 22, (GXColor){0, 0, 0, 255});
        backBtnImg = new GuiImage(btnOutline);
        backBtn = new GuiButton(backBtnImg->GetWidth(), backBtnImg->GetWidth());
		backBtn->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
		backBtn->SetPosition(-180, 400);
		backBtn->SetImage(backBtnImg);
		backBtn->SetImageOver(backBtnImg);
		backBtn->SetLabel(backBtnTxt);
        backBtn->SetTrigger(trigA);
        backBtn->SetTrigger(trigB);
        Append(backBtn);
    }

	optionBrowser = new GuiCustomOptionBrowser(396, 280, Options);
    optionBrowser->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	optionBrowser->SetPosition(0, 90);
    
	titleTxt = new GuiText(title, 28, (GXColor) {0, 0, 0, 255});
    titleTxt->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	titleTxt->SetPosition(170,38);
	
	Append(optionBrowser);
	Append(titleTxt);

    SetEffect(EFFECT_FADE, 50);
}

SettingsMenu::~SettingsMenu()
{
	MainWindow::Instance()->ResumeGui();

    SetEffect(EFFECT_FADE, -50);
    while(this->GetEffect() > 0)
        usleep(100);

    MainWindow::Instance()->HaltGui();
    if(parentElement)
        ((GuiWindow *) parentElement)->Remove(this);

    RemoveAll();

    if(btnOutline)
        Resources::Remove(btnOutline);
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
	
	MainWindow::Instance()->ResumeGui();
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
