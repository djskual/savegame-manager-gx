/***************************************************************************
 * Copyright (C) 2011
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
 * AllButtons.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include "AllButtons.hpp"
#include "../Settings/CSettings.h"
#include "../Themes/CTheme.h"
#include "../Menu/menus.h"

/****************************************************************************
* AllButtons Class
*
* Displays a prompt window to user, with information, an error message, or
* presenting a user with a choice of up to 3 Buttons.
*
* Give him 1 Title, 1 Message and 3 Buttons
* If title/message or one of the buttons is not needed give him a 0 on that
* place.
***************************************************************************/
AllButtons::AllButtons(const char *title,
					   const char *msg,
					   const char *btn1Label,
					   const char *btn2Label,
					   const char *btn3Label)
{
	choice = 0;
	
	trigA = new SimpleGuiTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	
	btnOutline = Resources::GetImageData("settings_title.png");
	btnOutlineOver = Resources::GetImageData("settings_title_over.png");
	
	this->SetSize(400, 212);
	
	titleTxt = new GuiText(title, 26, thColor("r=255 g=255 b=255 a=255 - manage windows text color"));
	titleTxt->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	titleTxt->SetPosition(0, -46);
	titleTxt->SetMaxWidth(460, SCROLL_HORIZONTAL);

	int labelCount = 0;
	if(btn1Label) labelCount++;
	if(btn2Label) labelCount++;
	if(btn3Label) labelCount++;
	
	msgTxt = new GuiText(msg, 22, thColor("r=255 g=255 b=255 a=255 - manage windows text color"));
	msgTxt->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	msgTxt->SetPosition(0, -15);
	msgTxt->SetMaxWidth(400, (labelCount > 1) ? SCROLL_HORIZONTAL : WRAP);
	msgTxt->SetLinesToDraw((labelCount > 1) ? 1 : 3);
	
	btn1Txt = NULL;
	btn1Img = NULL;
	btn1 = NULL;

	if(btn1Label)
	{
		btn1Txt = new GuiText(btn1Label, 22, thColor("r=0 g=0 b=0 a=255 - manage buttons text color"));
		btn1Img = new GuiImage(btnOutline);
		btn1OverImg = new GuiImage(btnOutlineOver);
		btn1 = new GuiButton(btn1Img, btn1OverImg,
							 ALIGN_CENTRE, ALIGN_TOP,
							 0, 0,
							 trigA,
							 btnSoundOver, btnSoundClick,
							 1);
		btn1->SetLabel(btn1Txt);
		if(!btn2Label && !btn3Label)
			btn1->SetState(STATE_SELECTED);
	}
	
	btn2Txt = NULL;
	btn2Img = NULL;
	btn2 = NULL;

	if(btn2Label)
	{
		btn2Txt = new GuiText(btn2Label, 22, thColor("r=0 g=0 b=0 a=255 - manage buttons text color"));
		btn2Img = new GuiImage(btnOutline);
		btn2OverImg = new GuiImage(btnOutlineOver);
		btn2 = new GuiButton(btn2Img, btn2OverImg,
							 ALIGN_CENTRE, ALIGN_MIDDLE,
							 0, 0,
							 trigA,
							 btnSoundOver, btnSoundClick,
							 1);
		btn2->SetLabel(btn2Txt);
		if(!btn3Label)
			btn2->SetState(STATE_SELECTED);
	}

	btn3Txt = NULL;
	btn3Img = NULL;
	btn3 = NULL;

	if(btn3Label)
	{
		btn3Txt = new GuiText(btn3Label, 22, thColor("r=0 g=0 b=0 a=255 - manage buttons text color"));
		btn3Img = new GuiImage(btnOutline);
		btn3OverImg = new GuiImage(btnOutlineOver);
		btn3 = new GuiButton(btn3Img, btn3OverImg,
							 ALIGN_CENTRE, ALIGN_BOTTOM,
							 0, 0,
							 trigA,
							 btnSoundOver, btnSoundClick,
							 1);
		btn3->SetLabel(btn3Txt);
		btn3->SetState(STATE_SELECTED);
	}

	if(!(btn1Label && btn2Label && btn3Label))
	{    
		if(btn1Label && btn2Label && !btn3Label) // 1 2
		{
			btn1->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
			btn1->SetPosition(0, -50);
			btn2->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
			btn2->SetPosition(0, 50);
		}
		else if(btn1Label && !btn2Label && btn3Label) // 1 3
		{
			btn1->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
			btn1->SetPosition(0, -50);
			btn3->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
			btn3->SetPosition(0, 50);
		}
		else if(!btn1Label && btn2Label && btn3Label) // 2 3
		{
			btn2->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
			btn2->SetPosition(0, -50);
			btn3->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
			btn3->SetPosition(0, 50);
		}
		else // 1
		{
			if(btn1Label)
				btn1->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
			if(btn2Label)
				btn2->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
			if(btn3Label)
				btn3->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
		}
	}

	buttons = new GuiWindow(400, 212);
	buttons->SetParent(this);
	
	buttons->Append(titleTxt);
	buttons->Append(msgTxt);

	if(btn1Label)
		buttons->Append(btn1);
	if(btn2Label)
		buttons->Append(btn2);
	if(btn3Label)
		buttons->Append(btn3);
	
	this->SetState(STATE_DEFAULT);
}

AllButtons::~AllButtons()
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
	
	/** ImageDatas **/
	delete btnOutline;
	delete btnOutlineOver;
	
	/** Texts **/
	delete titleTxt;
	delete msgTxt;

	if(btn1Txt)
		delete btn1Txt;
	if(btn2Txt)
		delete btn2Txt;
	if(btn3Txt)
		delete btn3Txt;

	/** Buttons **/
	if(btn1)
		delete btn1;
	if(btn2)
		delete btn2;
	if(btn3)
		delete btn3;

	/** Triggers **/
	delete trigA;
	
	/** Window **/
	delete buttons;
}

void AllButtons::SetTitle(const char *title)
{
	titleTxt->SetText(title);
}

void AllButtons::SetMessage(const char *msg)
{
	msgTxt->SetText(msg);
}

int AllButtons::GetChoice()
{
	return choice;
}

void AllButtons::ResetState()
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

void AllButtons::Draw()
{
	if(!this->IsVisible())
		return;

	buttons->Draw();

	this->UpdateEffects();
}

void AllButtons::Update(GuiTrigger * t)
{
	if(state == STATE_DISABLED || !t)
		return;
	
	if(btn1)
		btn1->Update(t);
	if(btn2)
		btn2->Update(t);
	if(btn3)
		btn3->Update(t);

	if(btn1 && (btn1->GetState() == STATE_CLICKED))
	{
		this->SetState(STATE_CLICKED);
		choice = 1;
	}
	if(btn2 && (btn2->GetState() == STATE_CLICKED))
	{
		this->SetState(STATE_CLICKED);
		choice = 2;
	}
	if(btn3 && (btn3->GetState() == STATE_CLICKED))
	{
		this->SetState(STATE_CLICKED);
		choice = 3;
	}
	
	if(updateCB)
		updateCB(this);
}
