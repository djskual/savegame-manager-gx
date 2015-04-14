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
 * ManageAllWindow.cpp
 *
 * for SaveGame Manager GX 2010
 ***************************************************************************/

#include <gccore.h>
#include <unistd.h>

#include "manage_all/ManageAllWindow.h"
#include "menu/menu.h"
#include "main.h"

/**
 * Constructor for the ManageAllWindow class.
 */
ManageAllWindow::ManageAllWindow(const char *title)
{
	trigA = new SimpleGuiTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);
	trigB = new GuiTrigger;
	trigB->SetButtonOnlyTrigger(-1, WPAD_BUTTON_B | WPAD_CLASSIC_BUTTON_B, PAD_BUTTON_B);
	trigHome = new GuiTrigger;
	trigHome->SetButtonOnlyTrigger(-1, WPAD_BUTTON_HOME | WPAD_CLASSIC_BUTTON_HOME, PAD_BUTTON_START);
	
	btnSoundOver = Resources::GetSound(button_over_wav, button_over_wav_size, cfg.SFXVolume);
	btnClick = Resources::GetSound(button_click_wav, button_click_wav_size, cfg.SFXVolume);
	
	dialogBox = Resources::GetImageData(save_manage_bg_png, save_manage_bg_png_size);
	dialogBoxImg = new GuiImage(dialogBox);

	titleTxt = new GuiText(title, 35, (GXColor){255, 255, 255, 255});
    titleTxt->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
    titleTxt->SetPosition(-6,10);
    titleTxt->SetMaxWidth(400, GuiText::SCROLL);

	close = Resources::GetImageData(close_png, close_png_size);
	closeImg = new GuiImage(close);
    closeOver = Resources::GetImageData(close_over_png, close_over_png_size);
    closeOverImg = new GuiImage(closeOver);
	
	closeBtn = new GuiButton(closeImg->GetWidth(), closeImg->GetHeight());
    closeBtn->SetImage(closeImg);
    closeBtn->SetImageOver(closeOverImg);
    closeBtn->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
    closeBtn->SetPosition(-22, 10);
    closeBtn->SetSoundOver(btnSoundOver);
    closeBtn->SetSoundClick(btnClick);
    closeBtn->SetTrigger(trigA);
    closeBtn->SetTrigger(trigB);
    closeBtn->SetTrigger(trigHome);
    closeBtn->SetEffectGrow();

    window = new GuiWindow(dialogBoxImg->GetWidth(), dialogBoxImg->GetHeight());
	window->SetParent(this);
	
	window->Append(dialogBoxImg);
	window->Append(titleTxt);
	window->Append(closeBtn);
	
	SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	SetPosition(6, -15);
	
	SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_IN, 50);
	
	this->SetState(STATE_DEFAULT);
}

/**
 * Destructor for the ManageAllWindow class.
 */
ManageAllWindow::~ManageAllWindow()
{
	window->RemoveAll();

    /** Buttons **/
	delete closeBtn;

    /** Images **/
	delete dialogBoxImg;
	delete closeImg;
	delete closeOverImg;
	
	/** ImageDatas **/
	Resources::Remove(dialogBox);
	Resources::Remove(close);
	Resources::Remove(closeOver);
	
    /** Sounds **/
	Resources::Remove(btnSoundOver);
	Resources::Remove(btnClick);

    /** Triggers **/
	delete trigHome;
	delete trigA;
	delete trigB;
	
    /** Texts **/
    delete titleTxt;
	
	/** Window **/
    delete window;
	
}

void ManageAllWindow::SetAlignment(int h, int v)
{
    GuiElement::SetAlignment(h, v);
	window->SetAlignment(h, v);
}

void ManageAllWindow::SetTitle(const char *title)
{
    titleTxt->SetText(title);
}

void ManageAllWindow::ResetState()
{
    state = STATE_DEFAULT;
	stateChan = -1;

	closeBtn->ResetState();
}

void ManageAllWindow::Disabled()
{
	closeBtn->SetState(STATE_DISABLED);
}

void ManageAllWindow::Enabled()
{
	closeBtn->SetState(STATE_DEFAULT);
}

/**
 * Draw the window
 */
void ManageAllWindow::Draw()
{
	if(!this->IsVisible())
		return;

	window->Draw();

	this->UpdateEffects();
}

void ManageAllWindow::Update(GuiTrigger * t)
{
	if(state == STATE_DISABLED || !t)
		return;

	closeBtn->Update(t);
	
    if(closeBtn->GetState() == STATE_CLICKED)
	{
		closeBtn->ResetState();
		this->SetState(STATE_CLICKED);
	}
	
	if(updateCB)
		updateCB(this);
}
