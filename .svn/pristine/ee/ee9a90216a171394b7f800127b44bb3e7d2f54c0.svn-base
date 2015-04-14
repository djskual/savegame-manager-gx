/***************************************************************************
 * Copyright (C) 2009
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
 * ManageAllButtons.cpp
 *
 * for SaveGame Manager GX 2010
 ***************************************************************************/
#include <gccore.h>
#include <unistd.h>

#include "manage_all/ManageAllButtons.h"
#include "menu/menu.h"
#include "main.h"

/****************************************************************************
* ManageAllButtons Class
*
* Displays a prompt window to user, with information, an error message, or
* presenting a user with a choice of up to 3 Buttons.
*
***************************************************************************/
ManageAllButtons::ManageAllButtons(const char *btn1Label,
								   const char *btn2Label,
								   int what,
								   int mii,
								   int miidelete)
{
    choice = 0;
	
	btnSoundOver = Resources::GetSound(button_over_wav, button_over_wav_size, cfg.SFXVolume);
	btnClick = Resources::GetSound(button_click_wav, button_click_wav_size, cfg.SFXVolume);
	
	trigA = new SimpleGuiTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);
	
	btnOutline = Resources::GetImageData(settings_title_png, settings_title_png_size);
	btnOutlineOver = new GuiImageData(settings_title_over_png, settings_title_over_png_size);
	
    install = Resources::GetImageData(green_left_png, green_left_png_size);
	installImg = new GuiImage(install);
    installImg->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	installImg->SetPosition(20, 0);
	
	extract = Resources::GetImageData(green_right_png, green_right_png_size);
	extractImg = new GuiImage(extract);
	extractImg->SetAlignment(ALIGN_RIGHT, ALIGN_MIDDLE);
	extractImg->SetPosition(-20, 0);
	
	deleteAll = Resources::GetImageData(delete_all_png, delete_all_png_size);
	deleteAllImg = new GuiImage(deleteAll);
	deleteAllImg->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	deleteAllImg->SetPosition(20, 0);
	
	nand = Resources::GetImageData(nand_png, nand_png_size);
	nandImg = new GuiImage(nand);
	nandImg->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	nandImg->SetPosition(20, 0);
	
	device = Resources::GetImageData(device_png, device_png_size);
	deviceImg = new GuiImage(device);
	deviceImg->SetAlignment(ALIGN_RIGHT, ALIGN_MIDDLE);
	deviceImg->SetPosition(-20, -5);
	
	btn1Txt = NULL;
	btn1Img = NULL;
	btn1 = NULL;

    if(btn1Label)
    {
        btn1Txt = new GuiText(btn1Label, 22, (GXColor){0, 0, 0, 255});
		if(what)
			btn1Txt->SetPosition(0, 0);
		else
			btn1Txt->SetPosition(30, 0);
		btn1Img = new GuiImage(btnOutline);
		btn1OverImg = new GuiImage(btnOutlineOver);
		btn1 = new GuiButton(btnOutline->GetWidth(), btnOutline->GetHeight());
		btn1->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
        if(mii)
			btn1->SetPosition(0, -30);
		btn1->SetLabel(btn1Txt);
		btn1->SetImage(btn1Img);
		btn1->SetImageOver(btn1OverImg);
		if(!what)
			btn1->SetIcon(installImg);
		if(miidelete)
			btn1->SetIcon(nandImg);
		btn1->SetSoundOver(btnSoundOver);
		btn1->SetSoundClick(btnClick);
		btn1->SetTrigger(trigA);
		btn1->SetEffectGrow();
	}
	
	btn2Txt = NULL;
	btn2Img = NULL;
	btn2 = NULL;

    if(btn2Label)
    {
        btn2Txt = new GuiText(btn2Label, 22, (GXColor){0, 0, 0, 255});
        if(what)
			btn2Txt->SetPosition(0, 0);
		else
			btn2Txt->SetPosition(-30, 0);
		btn2Img = new GuiImage(btnOutline);
        btn2OverImg = new GuiImage(btnOutlineOver);
		btn2 = new GuiButton(btnOutline->GetWidth(), btnOutline->GetHeight());
        btn2->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
        if(mii)
			btn2->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
		btn2->SetLabel(btn2Txt);
        btn2->SetImage(btn2Img);
        btn2->SetImageOver(btn2OverImg);
		if(!what)
			btn2->SetIcon(extractImg);
		if(miidelete)
			btn2->SetIcon(deviceImg);
		btn2->SetSoundOver(btnSoundOver);
        btn2->SetSoundClick(btnClick);
        btn2->SetTrigger(trigA);
        btn2->SetEffectGrow();
    }

    btn3Txt = NULL;
	btn3Img = NULL;
	btn3 = NULL;
	
    if(mii)
    {
		btn3Txt = new GuiText(tr("Delete All"), 22, (GXColor){0, 0, 0, 255});
        btn3Txt->SetPosition(30, 0);
		btn3Img = new GuiImage(btnOutline);
        btn3OverImg = new GuiImage(btnOutlineOver);
		btn3 = new GuiButton(btnOutline->GetWidth(), btnOutline->GetHeight());
        btn3->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
        btn3->SetPosition(0, 30);
		btn3->SetLabel(btn3Txt);
        btn3->SetImage(btn3Img);
        btn3->SetImageOver(btn3OverImg);
		btn3->SetIcon(deleteAllImg);
		btn3->SetSoundOver(btnSoundOver);
        btn3->SetSoundClick(btnClick);
        btn3->SetTrigger(trigA);
        btn3->SetEffectGrow();
	}
	
    buttons = new GuiWindow(400, 180);
	buttons->SetParent(this);
	
	if(btn1Label)
        buttons->Append(btn1);
    if(btn2Label)
        buttons->Append(btn2);
    if(mii)
        buttons->Append(btn3);
    
	SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	SetPosition(0, 0);
    
	this->SetState(STATE_DEFAULT);
	
}

ManageAllButtons::~ManageAllButtons()
{
	buttons->RemoveAll();

    // Images //
	if(btn1Img){
        delete btn1Img;
		delete btn1OverImg;
    }
	if(btn2Img){
        delete btn2Img;
        delete btn2OverImg;
    }
	if(btn3Img){
        delete btn3Img;
        delete btn3OverImg;
    }
	delete installImg;
    delete extractImg;
	delete deleteAllImg;
	
	// ImageDatas //
	Resources::Remove(btnOutline);
	Resources::Remove(btnOutlineOver);
	Resources::Remove(extract);
	Resources::Remove(install);
	Resources::Remove(deleteAll);
	
	// Sounds //
	Resources::Remove(btnSoundOver);
	Resources::Remove(btnClick);

    // Texts //
    if(btn1Txt)
        delete btn1Txt;
    if(btn2Txt)
        delete btn2Txt;
    if(btn3Txt)
        delete btn3Txt;
    
    // Buttons //
	if(btn1)
        delete btn1;
    if(btn2)
        delete btn2;
    if(btn3)
        delete btn3;
    
    // Triggers //
	delete trigA;
	
	// Window //
    delete buttons;
}

void ManageAllButtons::SetAlignment(int h, int v)
{
    GuiElement::SetAlignment(h, v);
	buttons->SetAlignment(h, v);
}

int ManageAllButtons::GetChoice()
{
    return choice;
}

void ManageAllButtons::ResetState()
{
    state = STATE_DEFAULT;
	stateChan = -1;

	if(btn1)
        btn1->ResetState();
	if(btn2)
        btn2->ResetState();
	if(btn3)
        btn3->ResetState();
	
	choice = 0;
	
}

/**
 * Draw the window
 */
void ManageAllButtons::Draw()
{
	if(!this->IsVisible())
		return;

	buttons->Draw();

	this->UpdateEffects();
}

void ManageAllButtons::Update(GuiTrigger * t)
{
	if(state == STATE_DISABLED || !t)
		return;
	
	if(btn1)
        btn1->Update(t);
	if(btn2)
        btn2->Update(t);
	if(btn3)
        btn3->Update(t);
	
    if (btn1){
        if(btn1->GetState() == STATE_CLICKED) {
			
			btn1->ResetState();
			this->SetState(STATE_CLICKED);
			choice = 1; }}
	if (btn2){
        if(btn2->GetState() == STATE_CLICKED) {
			
			btn2->ResetState();
			this->SetState(STATE_CLICKED);
			choice = 2; }}
	if (btn3){
        if(btn3->GetState() == STATE_CLICKED) {
			
			btn3->ResetState();
			this->SetState(STATE_CLICKED);
			choice = 3; }}
	
	if(updateCB)
		updateCB(this);
}

