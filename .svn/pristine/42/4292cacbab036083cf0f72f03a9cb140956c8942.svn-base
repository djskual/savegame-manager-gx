 /****************************************************************************
 * Copyright (C) 2010
 * by Dj_Skual
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
 *
 *
 * ShortCuts.cpp
 *
 * for SaveGame Manager GX 2010
 ***************************************************************************/

#include <gccore.h>
#include <unistd.h>

#include "prompts/ShortCuts.h"
#include "menu/MainWindow.h"
#include "main.h"

/**
 * Constructor for the Shortcuts class.
 */
Shortcuts::Shortcuts()
	: GuiWindow(424, 280)
{
	choice = -1;
	
	trigA = new SimpleGuiTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);
	trigB = new GuiTrigger;
	trigB->SetButtonOnlyTrigger(-1, WPAD_BUTTON_B | WPAD_CLASSIC_BUTTON_B, PAD_BUTTON_B);
	trigHome = new GuiTrigger;
	trigHome->SetButtonOnlyTrigger(-1, WPAD_BUTTON_HOME | WPAD_CLASSIC_BUTTON_HOME, PAD_BUTTON_START);
	
	btnSoundOver = Resources::GetSound(button_over_wav, button_over_wav_size, cfg.SFXVolume);
	btnClick = Resources::GetSound(button_click_wav, button_click_wav_size, cfg.SFXVolume);
	
	dialogBox = Resources::GetImageData(shortcut_bg_png, shortcut_bg_png_size);
	dialogBoxImg = new GuiImage(dialogBox);

	nameTxt = new GuiText(tr("Shortcuts"), 35, (GXColor){255, 255, 255, 255});
    nameTxt->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
    nameTxt->SetPosition(-5,10);
    nameTxt->SetMaxWidth(300, GuiText::SCROLL);

	close = Resources::GetImageData(close_png, close_png_size);
	closeImg = new GuiImage(close);
    closeOver = Resources::GetImageData(close_over_png, close_over_png_size);
    closeOverImg = new GuiImage(closeOver);
	
	closeBtn = new GuiButton(closeImg->GetWidth(), closeImg->GetHeight());
    closeBtn->SetImage(closeImg);
    closeBtn->SetImageOver(closeOverImg);
    closeBtn->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
    closeBtn->SetPosition(-18, 8);
    closeBtn->SetSoundOver(btnSoundOver);
    closeBtn->SetSoundClick(btnClick);
    closeBtn->SetTrigger(trigA);
    closeBtn->SetTrigger(trigB);
    closeBtn->SetTrigger(trigHome);
    closeBtn->SetEffectGrow();

    button = Resources::GetImageData(shortcut_button_png, shortcut_button_png_size);
	buttonOver = Resources::GetImageData(shortcut_button_over_png, shortcut_button_over_png_size);
    
	mii = Resources::GetImageData(shortcut_mii_png, shortcut_mii_png_size);
	miiImg = new GuiImage(mii);
    miiImg->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	buttonMiiImg = new GuiImage(button);
    buttonMiiOverImg = new GuiImage(buttonOver);
	
	miiBtn = new GuiButton(buttonMiiImg->GetWidth(), buttonMiiImg->GetHeight());
    miiBtn->SetImage(buttonMiiImg);
    miiBtn->SetImageOver(buttonMiiOverImg);
    miiBtn->SetIcon(miiImg);
    miiBtn->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
    miiBtn->SetPosition(45, 10);
    miiBtn->SetSoundOver(btnSoundOver);
    miiBtn->SetSoundClick(btnClick);
    miiBtn->SetTrigger(trigA);
    miiBtn->SetEffectGrow();

    save = Resources::GetImageData(shortcut_save_png, shortcut_save_png_size);
	saveImg = new GuiImage(save);
    saveImg->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	buttonSaveImg = new GuiImage(button);
    buttonSaveOverImg = new GuiImage(buttonOver);
	
	saveBtn = new GuiButton(buttonSaveImg->GetWidth(), buttonSaveImg->GetHeight());
    saveBtn->SetImage(buttonSaveImg);
    saveBtn->SetImageOver(buttonSaveOverImg);
    saveBtn->SetIcon(saveImg);
    saveBtn->SetAlignment(ALIGN_RIGHT, ALIGN_MIDDLE);
    saveBtn->SetPosition(-55, 10);
    saveBtn->SetSoundOver(btnSoundOver);
    saveBtn->SetSoundClick(btnClick);
    saveBtn->SetTrigger(trigA);
    saveBtn->SetEffectGrow();

	Append(dialogBoxImg);
	Append(nameTxt);
	Append(closeBtn);
	Append(miiBtn);
	Append(saveBtn);
	
	MainWindow::Instance()->SetState(STATE_DISABLED);
	this->SetState(STATE_DEFAULT);
	
	SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	SetEffect(EFFECT_SLIDE_BOTTOM | EFFECT_SLIDE_IN, 50);
}

/**
 * Destructor for the Shortcuts class.
 */
Shortcuts::~Shortcuts()
{
	MainWindow::Instance()->ResumeGui();
	SetEffect(EFFECT_SLIDE_BOTTOM | EFFECT_SLIDE_OUT, 50);
	while(this->GetEffect() > 0) usleep(100);

	MainWindow::Instance()->HaltGui();
	if(parentElement)
		((GuiWindow *) parentElement)->Remove(this);

	RemoveAll();

    /** Buttons **/
	delete closeBtn;
	delete miiBtn;
	delete saveBtn;
	
    /** Images **/
	delete dialogBoxImg;
	delete miiImg;
	delete saveImg;
	delete closeImg;
	delete closeOverImg;
	delete buttonMiiImg;
	delete buttonMiiOverImg;
	delete buttonSaveImg;
	delete buttonSaveOverImg;
	
	/** ImageDatas **/
	Resources::Remove(dialogBox);
	Resources::Remove(mii);
	Resources::Remove(save);
	Resources::Remove(close);
	Resources::Remove(closeOver);
	Resources::Remove(button);
	Resources::Remove(buttonOver);
	
	/** Sounds **/
	Resources::Remove(btnSoundOver);
	Resources::Remove(btnClick);

    /** Triggers **/
	delete trigHome;
	delete trigA;
	delete trigB;
	
    /** Texts **/
    delete nameTxt;
	
	MainWindow::Instance()->SetState(STATE_DEFAULT);
	MainWindow::Instance()->ResumeGui();
}

int Shortcuts::GetChoice()
{
	if(closeBtn->GetState() == STATE_CLICKED)
		choice = 0;
	else if(miiBtn->GetState() == STATE_CLICKED)
		choice = 1;
	else if(saveBtn->GetState() == STATE_CLICKED)
		choice = 2;
	
	return choice;
}
