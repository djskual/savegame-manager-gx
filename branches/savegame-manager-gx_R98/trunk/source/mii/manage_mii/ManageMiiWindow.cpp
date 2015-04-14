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
 * ManageMiiWindow.cpp
 *
 * for SaveGame Manager GX 2010
 ***************************************************************************/

#include <gccore.h>
#include <unistd.h>

#include "saves/manage/ManageWindow.h"
#include "saves/manage/ManageButtons.h"
#include "mii/manage_mii/ManageMiiWindow.h"
#include "prompts/browsers/browsers.h"
#include "menu/menu.h"
#include "main.h"

static char monthTxt[20];
static int red = 0;
static int green = 0;
static int blue = 0;

/**
 * Constructor for the ManageMiiWindow class.
 */
ManageMiiWindow::ManageMiiWindow (const char *name,
								  const char *creator,
								  int day,
								  int month,
								  int female,
								  int favColor)
{
	choice = 0;
	move = windowinfo.move;
	
	if(creator) {
		if(creator[0] == '\0')
			creator = NULL;
	}
	
	trigA = new SimpleGuiTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);
	trigB = new GuiTrigger;
	trigB->SetButtonOnlyTrigger(-1, WPAD_BUTTON_B | WPAD_CLASSIC_BUTTON_B, PAD_BUTTON_B);
	trigHome = new GuiTrigger;
	trigHome->SetButtonOnlyTrigger(-1, WPAD_BUTTON_HOME | WPAD_CLASSIC_BUTTON_HOME, PAD_BUTTON_START);
	trigMinus = new GuiTrigger;
	trigMinus->SetButtonOnlyTrigger(-1, WPAD_BUTTON_MINUS | WPAD_CLASSIC_BUTTON_MINUS, 0);
	trigPlus = new GuiTrigger;
	trigPlus->SetButtonOnlyTrigger(-1, WPAD_BUTTON_PLUS | WPAD_CLASSIC_BUTTON_PLUS, 0);
	trigLeft = new GuiTrigger;
	trigLeft->SetButtonOnlyTrigger(-1, WPAD_BUTTON_LEFT | WPAD_CLASSIC_BUTTON_LEFT, PAD_BUTTON_LEFT);
	trigRight = new GuiTrigger;
	trigRight->SetButtonOnlyTrigger(-1, WPAD_BUTTON_RIGHT | WPAD_CLASSIC_BUTTON_RIGHT, PAD_BUTTON_RIGHT);
	
	btnSoundOver = Resources::GetSound(button_over_wav, button_over_wav_size, cfg.SFXVolume);
	btnClick = Resources::GetSound(button_click_wav, button_click_wav_size, cfg.SFXVolume);
	
	dialogBox = Resources::GetImageData(save_manage_bg_png, save_manage_bg_png_size);
	dialogBoxImg = new GuiImage(dialogBox);

	nameTxt = new GuiText(name, 35, (GXColor){255, 255, 255, 255});
    nameTxt->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
    nameTxt->SetPosition(0,10);
    nameTxt->SetMaxWidth(350, GuiText::SCROLL);

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

    left = Resources::GetImageData(arrow_left_png, arrow_left_png_size);
	leftImg = new GuiImage(left);
    
	leftBtn = new GuiButton(leftImg->GetWidth(), leftImg->GetHeight());
    leftBtn->SetImage(leftImg);
    leftBtn->SetImageOver(leftImg);
    leftBtn->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
    leftBtn->SetPosition(-23, 0);
    leftBtn->SetSoundOver(btnSoundOver);
    leftBtn->SetSoundClick(btnClick);
    leftBtn->SetTrigger(trigA);
    leftBtn->SetTrigger(trigLeft);
    leftBtn->SetTrigger(trigMinus);
    leftBtn->SetEffectGrow();

    right = Resources::GetImageData(arrow_right_png, arrow_right_png_size);
	rightImg = new GuiImage(right);
    
	rightBtn = new GuiButton(rightImg->GetWidth(), rightImg->GetHeight());
    rightBtn->SetImage(rightImg);
    rightBtn->SetImageOver(rightImg);
    rightBtn->SetAlignment(ALIGN_RIGHT, ALIGN_MIDDLE);
    rightBtn->SetPosition(10, 0);
    rightBtn->SetSoundOver(btnSoundOver);
    rightBtn->SetSoundClick(btnClick);
    rightBtn->SetTrigger(trigA);
    rightBtn->SetTrigger(trigRight);
    rightBtn->SetTrigger(trigPlus);
    rightBtn->SetEffectGrow();

	char creatorText[50];
	if(!creator)
		snprintf(creatorText, sizeof(creatorText), "...");
	else
		snprintf(creatorText, sizeof(creatorText), "%s", creator);
	creatorTxt = new GuiText(creatorText, 22, (GXColor){255, 255, 255, 255});
    creatorTxt->SetAlignment(ALIGN_RIGHT, ALIGN_MIDDLE);
    creatorTxt->SetMaxWidth(350, GuiText::SCROLL);
	
	creatorIcon = Resources::GetImageData(icon_creator_png, icon_creator_png_size);
	creatorIconImg = new GuiImage(creatorIcon);
	creatorIconImg->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	
	creatorBtn = new GuiButton(creatorTxt->GetTextWidth()+creatorIconImg->GetWidth()+15, creatorIconImg->GetHeight());
    creatorBtn->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
    creatorBtn->SetLabel(creatorTxt);
    creatorBtn->SetIcon(creatorIconImg);
    creatorBtn->SetState(STATE_DISABLED);
    
	char dateText[50];
	if(day && month) {
		GetMonth(month);
		snprintf(dateText, sizeof(dateText), "%i %s", day, monthTxt); }
	else {
		snprintf(dateText, sizeof(dateText), "../..");}
	dateTxt = new GuiText(dateText, 22, (GXColor){255, 255, 255, 255});
    dateTxt->SetAlignment(ALIGN_RIGHT, ALIGN_MIDDLE);
    dateTxt->SetMaxWidth(350, GuiText::SCROLL);
	
	dateIcon = Resources::GetImageData(icon_date_png, icon_date_png_size);
	dateIconImg = new GuiImage(dateIcon);
	dateIconImg->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	
	dateBtn = new GuiButton(dateTxt->GetTextWidth()+dateIconImg->GetWidth()+15, dateIconImg->GetHeight());
    dateBtn->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
    dateBtn->SetLabel(dateTxt);
    dateBtn->SetIcon(dateIconImg);
    dateBtn->SetState(STATE_DISABLED);
    
	gender = Resources::GetImageData(female ? icon_female_png : icon_male_png,
									 female ? icon_female_png_size : icon_male_png_size);
	genderImg = new GuiImage(gender);
	genderImg->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
    
	if(favColor>=0 && favColor<12)
		GetFavColor(favColor);
	
	favColorImg = new GuiImage(24, 24, (GXColor){red, green, blue, 255});
	favColorImg->SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
    
	int creatorSize = creatorTxt->GetTextWidth()+creatorIconImg->GetWidth()+15;
	int dateSize = dateTxt->GetTextWidth()+dateIconImg->GetWidth()+15;
	
	int x = creatorSize+50+24;
	int y = dateIconImg->GetHeight()+creatorIconImg->GetHeight()+10;
	
	if(creatorSize<dateSize)
		x = dateSize+50+24;
	
	infos = new GuiWindow(x, y);
	infos->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
    infos->SetPosition(-6, 60);
	
	infos->Append(creatorBtn);
	infos->Append(dateBtn);
	infos->Append(genderImg);
	if(favColor>=0 && favColor<12)
		infos->Append(favColorImg);
    
	window = new GuiWindow(dialogBoxImg->GetWidth(), dialogBoxImg->GetHeight());
	window->SetParent(this);
	
	window->Append(dialogBoxImg);
	window->Append(nameTxt);
	window->Append(closeBtn);
	window->Append(leftBtn);
	window->Append(rightBtn);
	window->Append(infos);
	
	SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	SetPosition(6, 0);
	
	SetWindowEffect();
	
	this->SetState(STATE_DEFAULT);
}

/**
 * Destructor for the ManageMiiWindow class.
 */
ManageMiiWindow::~ManageMiiWindow()
{
	window->RemoveAll();

    /** Buttons **/
	delete closeBtn;
	delete leftBtn;
	delete rightBtn;
	delete creatorBtn;
	delete dateBtn;

    /** Images **/
	delete dialogBoxImg;
	delete closeImg;
	delete closeOverImg;
	delete leftImg;
	delete rightImg;
	delete creatorIconImg;
	delete dateIconImg;
	delete favColorImg;
	delete genderImg;
	
	/** ImageDatas **/
	Resources::Remove(dialogBox);
	Resources::Remove(close);
	Resources::Remove(closeOver);
	Resources::Remove(left);
	Resources::Remove(right);
	Resources::Remove(creatorIcon);
	Resources::Remove(dateIcon);
	Resources::Remove(gender);
	
	/** Sounds **/
	Resources::Remove(btnSoundOver);
	Resources::Remove(btnClick);

    /** Triggers **/
	delete trigHome;
	delete trigA;
	delete trigB;
	delete trigMinus;
	delete trigPlus;
	delete trigLeft;
	delete trigRight;
		
    /** Texts **/
    delete nameTxt;
	delete creatorTxt;
	delete dateTxt;
	
    /** Window **/
    delete window;
	delete infos;
	
}

void ManageMiiWindow::SetAlignment(int h, int v)
{
    GuiElement::SetAlignment(h, v);
	window->SetAlignment(h, v);
}

void ManageMiiWindow::SetWindowEffect()
{
	move = windowinfo.move;
    
	if(windowinfo.move == ManageWindow::MOVE_IN)
		window->SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_IN, 50);
	
	else if(windowinfo.move == ManageWindow::MOVE_OUT)
		window->SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_OUT, 50);
		
	else if(windowinfo.move == ManageWindow::MOVE_PREV_IN)
		window->SetEffect(EFFECT_SLIDE_LEFT | EFFECT_SLIDE_IN, 50);
	
    else if(windowinfo.move == ManageWindow::MOVE_NEXT_IN)
		window->SetEffect(EFFECT_SLIDE_RIGHT | EFFECT_SLIDE_IN, 50);
    
	else if(windowinfo.move == ManageWindow::MOVE_PREV_OUT)
		window->SetEffect(EFFECT_SLIDE_RIGHT | EFFECT_SLIDE_OUT, 50);
	
    else if(windowinfo.move == ManageWindow::MOVE_NEXT_OUT)
		window->SetEffect(EFFECT_SLIDE_LEFT | EFFECT_SLIDE_OUT, 50);
	
	else if(windowinfo.move == ManageWindow::DEFAULT)
		window->StopEffect();
	
}

int ManageMiiWindow::GetMonth(int month)
{
    switch (month)
	{
		case 0: { snprintf(monthTxt, sizeof(monthTxt), " "); break; }
		case 1: { snprintf(monthTxt, sizeof(monthTxt), tr("January")); break; }
		case 2: { snprintf(monthTxt, sizeof(monthTxt), tr("February")); break; }
		case 3: { snprintf(monthTxt, sizeof(monthTxt), tr("March")); break; }
		case 4: { snprintf(monthTxt, sizeof(monthTxt), tr("April")); break; }
		case 5: { snprintf(monthTxt, sizeof(monthTxt), tr("May")); break; }
		case 6: { snprintf(monthTxt, sizeof(monthTxt), tr("June")); break; }
		case 7: { snprintf(monthTxt, sizeof(monthTxt), tr("July")); break; }
		case 8: { snprintf(monthTxt, sizeof(monthTxt), tr("August")); break; }
		case 9: { snprintf(monthTxt, sizeof(monthTxt), tr("September")); break; }
		case 10: { snprintf(monthTxt, sizeof(monthTxt), tr("October")); break; }
		case 11: { snprintf(monthTxt, sizeof(monthTxt), tr("November")); break; }
		case 12: { snprintf(monthTxt, sizeof(monthTxt), tr("December")); break; }
	}
	return 1;
}

int ManageMiiWindow::GetFavColor(int favColor)
{
	switch (favColor)
	{
		case 0: { red = 255; green = 0; blue = 0; break; }
		case 1: { red = 255; green = 150; blue = 0; break; }
		case 2: { red = 255; green = 255; blue = 0; break; }
		case 3: { red = 140; green = 255; blue = 0; break; }
		case 4: { red = 0; green = 130; blue = 0; break; }
		case 5: { red = 0; green = 100; blue = 255; break; }
		case 6: { red = 0; green = 200; blue = 255; break; }
		case 7: { red = 255; green = 50; blue = 140; break; }
		case 8: { red = 190; green = 0; blue = 255; break; }
		case 9: { red = 140; green = 80; blue = 45; break; }
		case 10: { red = 255; green = 255; blue = 255; break; }
		case 11: { red = 0; green = 0; blue = 0; break; }
	}
	return 1;
}
	
int ManageMiiWindow::GetChoice()
{
    return choice;
}

void ManageMiiWindow::ResetState()
{
    state = STATE_DEFAULT;
	stateChan = -1;

	closeBtn->ResetState();
	leftBtn->ResetState();
	rightBtn->ResetState();
}

void ManageMiiWindow::Disabled()
{
	closeBtn->SetState(STATE_DISABLED);
	leftBtn->SetState(STATE_DISABLED);
	rightBtn->SetState(STATE_DISABLED);
	leftBtn->SetVisible(false);
	rightBtn->SetVisible(false);
}

void ManageMiiWindow::Enabled()
{
	closeBtn->SetState(STATE_DEFAULT);
	leftBtn->SetState(STATE_DEFAULT);
	rightBtn->SetState(STATE_DEFAULT);
	leftBtn->SetVisible(true);
	rightBtn->SetVisible(true);
}

/**
 * Draw the window
 */
void ManageMiiWindow::Draw()
{
	if(!this->IsVisible())
		return;

	window->Draw();

	this->UpdateEffects();
}

void ManageMiiWindow::Update(GuiTrigger * t)
{
	if(state == STATE_DISABLED || !t)
		return;

	closeBtn->Update(t);
	leftBtn->Update(t);
	rightBtn->Update(t);

    if(closeBtn->GetState() == STATE_CLICKED)
	{
		closeBtn->ResetState();
		choice = 1;
		this->SetState(STATE_CLICKED);
	}
	else if(leftBtn->GetState() == STATE_CLICKED)
	{
		leftBtn->ResetState();
		choice = 2;
		this->SetState(STATE_CLICKED);
	}
	else if(rightBtn->GetState() == STATE_CLICKED)
	{
		rightBtn->ResetState();
		choice = 3;
		this->SetState(STATE_CLICKED);
	}
	
	if(updateCB)
		updateCB(this);
}
