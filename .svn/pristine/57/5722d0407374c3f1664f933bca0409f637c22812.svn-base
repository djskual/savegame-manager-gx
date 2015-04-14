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
 * Buttons.cpp
 *
 * for SaveGame Manager GX 2010
 ***************************************************************************/
#include <gccore.h>
#include <unistd.h>

#include "manage_all/Buttons.h"
#include "manage_all/manage_all.h"
#include "menu/menu.h"
#include "main.h"

/****************************************************************************
* ManageButtons Class
*
* Displays a prompt window to user, with information, an error message, or
* presenting a user with a choice of up to 4 Buttons.
*
* Give him 1 Titel, 1 Subtitel and 4 Buttons
* If titel/subtitle or one of the buttons is not needed give him a 0 on that
* place.
***************************************************************************/
Buttons::Buttons(const char *title,
				 const char *msg,
				 const char *warning,
				 const char *btn1Label,
				 const char *btn2Label,
				 const char *btn3Label,
				 const char *btn4Label)
{
    choice = 0;
	
	btnSoundOver = Resources::GetSound(button_over_wav, button_over_wav_size, cfg.SFXVolume);
	btnClick = Resources::GetSound(button_click_wav, button_click_wav_size, cfg.SFXVolume);
	
	trigA = new SimpleGuiTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);
	
	btnOutline = Resources::GetImageData(save_manage_button_png, save_manage_button_png_size);
	btnOutlineOver = Resources::GetImageData(save_manage_button_over_png, save_manage_button_over_png_size);
	
    titleTxt = new GuiText(title, 26, (GXColor){255, 255, 255, 255});
    titleTxt->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
    titleTxt->SetPosition(0, -86);
    titleTxt->SetMaxWidth(460, GuiText::SCROLL);

    msgTxt = new GuiText(msg, 22, (GXColor){255, 255, 255, 255});
    msgTxt->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
    msgTxt->SetPosition(0, -40);
    msgTxt->SetMaxWidth(400, GuiText::WRAP);
	msgTxt->SetNumLines(4);

    warningTxt = new GuiText(warning, 22, (GXColor){255, 255, 255, 255});
    warningTxt->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
    warningTxt->SetPosition(0,40);
    warningTxt->SetMaxWidth(500, GuiText::SCROLLFULL);

	btn1Txt = NULL;
	btn1Img = NULL;
	btn1 = NULL;

    if(btn1Label)
    {
        btn1Txt = new GuiText(btn1Label, 22, (GXColor){0, 0, 0, 255});
		btn1Img = new GuiImage(btnOutline);
		btn1OverImg = new GuiImage(btnOutlineOver);
		btn1 = new GuiButton(btnOutline->GetWidth(), btnOutline->GetHeight());
		btn1->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
        btn1->SetLabel(btn1Txt);
		btn1->SetImage(btn1Img);
		btn1->SetImageOver(btn1OverImg);
		btn1->SetSoundOver(btnSoundOver);
		btn1->SetSoundClick(btnClick);
		btn1->SetTrigger(trigA);
		if(!btn2Label && !btn3Label && !btn4Label)
			btn1->SetState(STATE_SELECTED);
		btn1->SetEffectGrow();
	}
	
	btn2Txt = NULL;
	btn2Img = NULL;
	btn2 = NULL;

    if(btn2Label)
    {
        btn2Txt = new GuiText(btn2Label, 22, (GXColor){0, 0, 0, 255});
        btn2Img = new GuiImage(btnOutline);
        btn2OverImg = new GuiImage(btnOutlineOver);
		btn2 = new GuiButton(btnOutline->GetWidth(), btnOutline->GetHeight());
        btn2->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
        btn2->SetLabel(btn2Txt);
        btn2->SetImage(btn2Img);
        btn2->SetImageOver(btn2OverImg);
		btn2->SetSoundOver(btnSoundOver);
        btn2->SetSoundClick(btnClick);
        btn2->SetTrigger(trigA);
        if(!btn3Label && !btn4Label)
			btn2->SetState(STATE_SELECTED);
		btn2->SetEffectGrow();
    }

	btn3Txt = NULL;
	btn3Img = NULL;
	btn3 = NULL;

    if(btn3Label)
    {
        btn3Txt = new GuiText(btn3Label, 22, (GXColor){0, 0, 0, 255});
        btn3Img = new GuiImage(btnOutline);
        btn3OverImg = new GuiImage(btnOutlineOver);
		btn3 = new GuiButton(btnOutline->GetWidth(), btnOutline->GetHeight());
        btn3->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
        btn3->SetLabel(btn3Txt);
        btn3->SetImage(btn3Img);
        btn3->SetImageOver(btn3OverImg);
		btn3->SetSoundOver(btnSoundOver);
        btn3->SetSoundClick(btnClick);
        btn3->SetTrigger(trigA);
        if(!btn4Label)
			btn3->SetState(STATE_SELECTED);
		btn3->SetEffectGrow();
    }

	btn4Txt = NULL;
	btn4Img = NULL;
	btn4 = NULL;

    if(btn4Label)
    {
        btn4Txt = new GuiText(btn4Label, 22, (GXColor){0, 0, 0, 255});
        btn4Img = new GuiImage(btnOutline);
        btn4OverImg = new GuiImage(btnOutlineOver);
		btn4 = new GuiButton(btnOutline->GetWidth(), btnOutline->GetHeight());
        btn4->SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
        btn4->SetLabel(btn4Txt);
        btn4->SetImage(btn4Img);
        btn4->SetImageOver(btn4OverImg);
		btn4->SetSoundOver(btnSoundOver);
        btn4->SetSoundClick(btnClick);
        btn4->SetTrigger(trigA);
		btn4->SetState(STATE_SELECTED);
		btn4->SetEffectGrow();
    }

    if(!(btn1Label && btn2Label && btn3Label && btn4Label))
    {    
		if(btn1Label && btn2Label && !btn3Label && !btn4Label) { // 1 2
			btn1->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
			btn2->SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
		} else if(btn1Label && !btn2Label && btn3Label && !btn4Label) { // 1 3
			btn1->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
			btn3->SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
		} else if(btn1Label && !btn2Label && !btn3Label && btn4Label) { // 1 4
			btn1->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
			btn4->SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
		} else if(!btn1Label && btn2Label && btn3Label && !btn4Label) { // 2 3
			btn2->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
			btn3->SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
		} else if(!btn1Label && btn2Label && !btn3Label && btn4Label) { // 2 4
			btn2->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
			btn4->SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
		} else if(!btn1Label && !btn2Label && btn3Label && btn4Label) { // 3 4
			btn3->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
			btn4->SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
		} else if(btn1Label && btn2Label && btn3Label && !btn4Label) { // 1 2 3
			btn1->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
			btn2->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
			btn3->SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
		} else if(btn1Label && !btn2Label && btn3Label && btn4Label) { // 1 3 4
			btn1->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
			btn3->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
			btn4->SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
		} else if(!btn1Label && btn2Label && btn3Label && btn4Label) { // 2 3 4
			btn2->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
			btn3->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
			btn4->SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
		} else if(btn1Label && btn2Label && !btn3Label && btn4Label) { // 1 2 4
			btn1->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
			btn2->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
			btn4->SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
		} else {  // 1
			if(btn1Label)
				btn1->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
			if(btn2Label)
				btn2->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
			if(btn3Label)
				btn3->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
			if(btn4Label)
				btn4->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
		}
	}

    buttons = new GuiWindow(400, 150);
	buttons->SetParent(this);
	
	buttons->Append(titleTxt);
    buttons->Append(msgTxt);
	buttons->Append(warningTxt);

    if(btn1Label)
        buttons->Append(btn1);
    if(btn2Label)
        buttons->Append(btn2);
    if(btn3Label)
        buttons->Append(btn3);
    if(btn4Label)
        buttons->Append(btn4);
	
	SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	SetPosition(0, 20);
    
	SetEffect(EFFECT_FADE, 30);
	
	this->SetState(STATE_DEFAULT);
	
}

Buttons::~Buttons()
{
	buttons->RemoveAll();

    /** Images **/
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
	if(btn4Img){
        delete btn4Img;
        delete btn4OverImg;
	}
	
	/** ImageDatas **/
	Resources::Remove(btnOutline);
	Resources::Remove(btnOutlineOver);
	
	/** Sounds **/
	Resources::Remove(btnSoundOver);
	Resources::Remove(btnClick);

    /** Texts **/
    delete titleTxt;
    delete msgTxt;
	delete warningTxt;

    if(btn1Txt)
        delete btn1Txt;
    if(btn2Txt)
        delete btn2Txt;
    if(btn3Txt)
        delete btn3Txt;
    if(btn4Txt)
        delete btn4Txt;

    /** Buttons **/
	if(btn1)
        delete btn1;
    if(btn2)
        delete btn2;
    if(btn3)
        delete btn3;
    if(btn4)
        delete btn4;

    /** Triggers **/
	delete trigA;
	
	/** Window **/
    delete buttons;
}

void Buttons::SetTitle(const char *title)
{
    titleTxt->SetText(title);
}

void Buttons::SetMessage(const char *msg)
{
    msgTxt->SetText(msg);
}

void Buttons::SetWarning(const char *warning)
{
    warningTxt->SetText(warning);
}

void Buttons::SetAlignment(int h, int v)
{
    GuiElement::SetAlignment(h, v);
	buttons->SetAlignment(h, v);
}

int Buttons::GetChoice()
{
    return choice;
}

void Buttons::ResetState()
{
    state = STATE_DEFAULT;
	stateChan = -1;

	if(btn1)
        btn1->ResetState();
	if(btn2)
        btn2->ResetState();
	if(btn3)
        btn3->ResetState();
	if(btn4)
        btn4->ResetState();
	
	choice = 0;
	
}

/**
 * Draw the window
 */
void Buttons::Draw()
{
	if(!this->IsVisible())
		return;

	buttons->Draw();

	this->UpdateEffects();
}

void Buttons::Update(GuiTrigger * t)
{
	if(state == STATE_DISABLED || !t)
		return;
	
	if(btn1)
        btn1->Update(t);
	if(btn2)
        btn2->Update(t);
	if(btn3)
        btn3->Update(t);
	if(btn4)
        btn4->Update(t);

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
	if (btn4){
        if(btn4->GetState() == STATE_CLICKED) {
			btn4->ResetState();
			this->SetState(STATE_CLICKED);
			choice = 4; }}
	
	if(updateCB)
		updateCB(this);
}
