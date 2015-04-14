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
#include "FlyingButtonsMenu.hpp"
#include "../../Themes/CTheme.h"
#include "../../Controls/Application.h"

#define FADE_SPEED 20
#define SLIDE_SPEED 35
#define DISPLAY_BUTTONS    4
#define MAX_INDICATORS     5

FlyingButtonsMenu::FlyingButtonsMenu()
	: GuiFrame(screenwidth, screenheight)
{
	currentPage = 0;
	goLeftImg = NULL;
	goLeftBtn = NULL;
	goRightImg = NULL;
	goRightBtn = NULL;
	
	btnSoundClick = Resources::GetSound("button_click.wav");
	btnSoundOver = Resources::GetSound("button_over.wav");
	btnSoundClick->SetVolume(Settings.SFXVolume);
	btnSoundOver->SetVolume(Settings.SFXVolume);
	
	trigA = new GuiTrigger();
	trigB = new GuiTrigger();
	trigL = new GuiTrigger();
	trigR = new GuiTrigger();
	trigMinus = new GuiTrigger();
	trigPlus = new GuiTrigger();
	trigA->SetSimpleTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	trigB->SetButtonOnlyTrigger(-1, WiiControls.BackButton | ClassicControls.BackButton << 16, GCControls.BackButton);
	trigL->SetButtonOnlyTrigger(-1, WiiControls.LeftButton | ClassicControls.LeftButton << 16, GCControls.LeftButton);
	trigR->SetButtonOnlyTrigger(-1, WiiControls.RightButton | ClassicControls.RightButton << 16, GCControls.RightButton);
	trigMinus->SetButtonOnlyTrigger(-1, WiiControls.PrevButton | ClassicControls.PrevButton << 16, 0);
	trigPlus->SetButtonOnlyTrigger(-1, WiiControls.NextButton | ClassicControls.NextButton << 16, 0);

	pageIndicatorImgData = Resources::GetImageData("settings_pageindicator.png");
	arrowLeft = Resources::GetImageData("arrow_left.png");
	arrowRight = Resources::GetImageData("arrow_right.png");

	goLeftImg = new GuiImage(arrowLeft);
	goLeftBtn = new GuiButton(goLeftImg->GetWidth(), goLeftImg->GetHeight());
	goLeftBtn->SetAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
	goLeftBtn->SetPosition(25, -25);
	goLeftBtn->SetImage(goLeftImg);
	goLeftBtn->SetSoundOver(btnSoundOver);
	goLeftBtn->SetSoundClick(btnSoundClick);
	goLeftBtn->SetEffectGrow();
	goLeftBtn->SetTrigger(trigA);
	goLeftBtn->SetTrigger(trigL);
	goLeftBtn->SetTrigger(trigMinus);
	goLeftBtn->Clicked.connect(this, &FlyingButtonsMenu::OnButtonClick);
	
	goRightImg = new GuiImage(arrowRight);
	goRightBtn = new GuiButton(goRightImg->GetWidth(), goRightImg->GetHeight());
	goRightBtn->SetAlignment(ALIGN_RIGHT | ALIGN_MIDDLE);
	goRightBtn->SetPosition(-25, -25);
	goRightBtn->SetImage(goRightImg);
	goRightBtn->SetSoundOver(btnSoundOver);
	goRightBtn->SetSoundClick(btnSoundClick);
	goRightBtn->SetEffectGrow();
	goRightBtn->SetTrigger(trigA);
	goRightBtn->SetTrigger(trigR);
	goRightBtn->SetTrigger(trigPlus);
	goRightBtn->Clicked.connect(this, &FlyingButtonsMenu::OnButtonClick);
	
	SetState(STATE_DISABLED);
}

FlyingButtonsMenu::~FlyingButtonsMenu()
{
	if(state == STATE_DEFAULT)
		FadeOut();

	if(goLeftImg)
		delete goLeftImg;
	if(goLeftBtn)
		delete goLeftBtn;
	if(goRightImg)
		delete goRightImg;
	if(goRightBtn)
		delete goRightBtn;
	goLeftImg = NULL;
	goLeftBtn = NULL;
	goRightImg = NULL;
	goRightBtn = NULL;
	
	for(u32 i = 0; i < mainButton.size(); ++i)
	{
		delete mainButtonImg[i];
		delete mainButtonImgOver[i];
		delete mainButtonTxt[i];
		delete mainButton[i];
	}
	for(u32 i = 0; i < pageIndicatorBtn.size(); ++i)
	{
		delete pageIndicatorImg[i];
		delete pageIndicatorTxt[i];
		delete pageIndicatorBtn[i];
	}
	
	pageIndicatorImg.clear();
	pageIndicatorTxt.clear();
	pageIndicatorBtn.clear();
	mainButtonImg.clear();
	mainButtonImgOver.clear();
	mainButtonTxt.clear();
	mainButton.clear();

	delete trigA;
	delete trigB;
	delete trigL;
	delete trigR;
	delete trigMinus;
	delete trigPlus;
	
	Resources::Remove(pageIndicatorImgData);
	Resources::Remove(arrowLeft);
	Resources::Remove(arrowRight);
	
	Resources::Remove(btnSoundClick);
	Resources::Remove(btnSoundOver);
}

void FlyingButtonsMenu::hide()
{
	Application::Instance()->Remove(this);
	RemoveAll();
	
	SetState(STATE_DISABLED);
}

void FlyingButtonsMenu::show()
{
	SetState(STATE_DEFAULT);
	
	Append(goLeftBtn);
	Append(goRightBtn);
	AddMainButtons();
}

void FlyingButtonsMenu::FadeIn()
{
	SetEffect(EFFECT_FADE, FADE_SPEED);
	
	show();
}

void FlyingButtonsMenu::FadeOut()
{
	SetEffect(EFFECT_FADE, -FADE_SPEED);
	while(this->IsAnimated())
		Application::Instance()->updateEvents();

	hide();
}

void FlyingButtonsMenu::SetPageIndicators()
{
	for(u32 i = 0; i < pageIndicatorBtn.size(); ++i)
	{
		Remove(pageIndicatorBtn[i]);
		delete pageIndicatorImg[i];
		delete pageIndicatorTxt[i];
		delete pageIndicatorBtn[i];
	}
	pageIndicatorImg.clear();
	pageIndicatorTxt.clear();
	pageIndicatorBtn.clear();

	int indicatorCount = ceil(mainButton.size()/(1.0f*DISPLAY_BUTTONS));

	firstIndicator = 0;
	if(currentPage > (int) floor(MAX_INDICATORS/2.0f))
	{
		if(indicatorCount-MAX_INDICATORS < currentPage - (int) floor(MAX_INDICATORS/2.0f))
			firstIndicator = indicatorCount-MAX_INDICATORS;
		else
			firstIndicator = currentPage - (int) floor(MAX_INDICATORS/2.0f);
	}
	if(firstIndicator < 0)
		firstIndicator = 0;

	int displayedIndicators = indicatorCount > MAX_INDICATORS ? MAX_INDICATORS : indicatorCount;

	for(int i = 0, n = firstIndicator; i < MAX_INDICATORS && n < indicatorCount; ++i, ++n)
	{
		pageIndicatorImg.push_back(new GuiImage(pageIndicatorImgData));
		pageIndicatorTxt.push_back(new GuiText(fmt("%i", n+1), 22, thColor("r=0 g=0 b=0 a=255 - flyingbutton menu title text color")));
		pageIndicatorBtn.push_back(new GuiButton(pageIndicatorImgData->GetWidth(), pageIndicatorImgData->GetHeight()));
		pageIndicatorBtn[i]->SetAlignment(ALIGN_CENTER | ALIGN_TOP);
		pageIndicatorBtn[i]->SetPosition(270-displayedIndicators*35+35*i, 400);
		pageIndicatorBtn[i]->SetImage(pageIndicatorImg[i]);
		pageIndicatorBtn[i]->SetLabel(pageIndicatorTxt[i]);
		pageIndicatorBtn[i]->SetSoundOver(btnSoundOver);
		pageIndicatorBtn[i]->SetSoundClick(btnSoundClick);
		pageIndicatorBtn[i]->SetTrigger(trigA);
		pageIndicatorBtn[i]->SetEffectGrow();
		pageIndicatorBtn[i]->SetAlpha(n == currentPage ? 255 : 50);
		pageIndicatorBtn[i]->Clicked.connect(this, &FlyingButtonsMenu::OnButtonClick);
		Append(pageIndicatorBtn[i]);
	}
}

void FlyingButtonsMenu::SetMainButton(int position, std::string buttonText, GuiImageData * imageData, GuiImageData * imageOver)
{
	if(position < (int) mainButton.size())
	{
		delete mainButtonImg[position];
		delete mainButtonImgOver[position];
		delete mainButtonTxt[position];
		delete mainButton[position];
	}
	else
	{
		mainButtonImg.resize(position+1);
		mainButtonImgOver.resize(position+1);
		mainButtonTxt.resize(position+1);
		mainButton.resize(position+1);
	}

	mainButtonImg[position] = new GuiImage(imageData);
	mainButtonImgOver[position] = new GuiImage(imageOver);

	mainButtonTxt[position] = new GuiText(buttonText.c_str(), 22, thColor("r=0 g=0 b=0 a=255 - flyingbutton menu mainbutton text color"));
	mainButtonTxt[position]->SetMaxWidth(mainButtonImg[position]->GetWidth());

	mainButton[position] = new GuiButton(imageData->GetWidth(), imageData->GetHeight());
	mainButton[position]->SetAlignment(ALIGN_CENTER | ALIGN_TOP);
	mainButton[position]->SetPosition(0, 90+(position % DISPLAY_BUTTONS)*70);
	mainButton[position]->SetImage(mainButtonImg[position]);
	mainButton[position]->SetImageOver(mainButtonImgOver[position]);
	mainButton[position]->SetLabel(mainButtonTxt[position]);
	mainButton[position]->SetSoundOver(btnSoundOver);
	mainButton[position]->SetSoundClick(btnSoundClick);
	mainButton[position]->SetEffectGrow();
	mainButton[position]->SetTrigger(trigA);
	mainButton[position]->Clicked.connect(this, &FlyingButtonsMenu::OnButtonClick);
}

void FlyingButtonsMenu::AddMainButtons()
{
	for(u32 i = 0; i < mainButton.size(); ++i)
		Remove(mainButton[i]);
	
	int firstItem = currentPage*DISPLAY_BUTTONS;
	
	for(int i = firstItem; i < (int) mainButton.size() && i < firstItem+DISPLAY_BUTTONS; ++i)
		Append(mainButton[i]);
	
	SetPageIndicators();
}

void FlyingButtonsMenu::ShowButtonsEffects(int effect, int effectSpeed)
{
	int firstItem = currentPage*DISPLAY_BUTTONS;
	if(firstItem < 0)
		firstItem = 0;
	
	for(int i = firstItem; i < (int) mainButton.size() && i < firstItem+DISPLAY_BUTTONS; ++i)
	{
		mainButton[i]->ResetEffects();
		mainButton[i]->SetEffect(effect, effectSpeed);
	}
	
	if(firstItem < 0 || firstItem >= (int) mainButton.size())
		return;
	
	while (mainButton[firstItem]->IsAnimated())
		Application::Instance()->updateEvents();
	
	//! Allow button grow only after slide animation is done
	for(int i = firstItem; i < (int) mainButton.size() && i < firstItem+DISPLAY_BUTTONS; ++i)
		mainButton[i]->SetEffectGrow();
}

void FlyingButtonsMenu::SlideButtons(int direction)
{
	int SlideEffectIN = 0;
	
	if(direction == SLIDE_LEFT)
	{
		ShowButtonsEffects(EFFECT_SLIDE_RIGHT | EFFECT_SLIDE_OUT, SLIDE_SPEED);
		SlideEffectIN = EFFECT_SLIDE_LEFT | EFFECT_SLIDE_IN;
		
		currentPage--;
		
		if(currentPage < 0)
			currentPage = !mainButton.empty() ? ceil(mainButton.size()/4.0f)-1 : 0;
	}
	else
	{
		ShowButtonsEffects(EFFECT_SLIDE_LEFT | EFFECT_SLIDE_OUT, SLIDE_SPEED);
		SlideEffectIN = EFFECT_SLIDE_RIGHT | EFFECT_SLIDE_IN;
		
		currentPage++;
		
		if(currentPage >= ceil(mainButton.size()/4.0f))
			currentPage = 0;
	}
	
	AddMainButtons();
	
	ShowButtonsEffects(SlideEffectIN, SLIDE_SPEED);
}

void FlyingButtonsMenu::OnButtonClick(GuiButton *sender, int pointer UNUSED, const POINT &p UNUSED)
{
	sender->ResetState();
	
	if(sender == goRightBtn)
		SlideButtons(SLIDE_RIGHT);
	else if(sender == goLeftBtn)
		SlideButtons(SLIDE_LEFT);
	
	for(u32 i = 0; i < pageIndicatorBtn.size(); ++i)
	{
		if(sender != pageIndicatorBtn[i])
			continue;
		
		if(firstIndicator+i < (u32) currentPage)
		{
			ShowButtonsEffects(EFFECT_SLIDE_RIGHT | EFFECT_SLIDE_OUT, SLIDE_SPEED);
			currentPage = firstIndicator+i;
			AddMainButtons();
			ShowButtonsEffects(EFFECT_SLIDE_LEFT | EFFECT_SLIDE_IN, SLIDE_SPEED);
		}
		else if(firstIndicator+i > (u32) currentPage)
		{
			ShowButtonsEffects(EFFECT_SLIDE_LEFT | EFFECT_SLIDE_OUT, SLIDE_SPEED);
			currentPage = firstIndicator+i;
			AddMainButtons();
			ShowButtonsEffects(EFFECT_SLIDE_RIGHT | EFFECT_SLIDE_IN, SLIDE_SPEED);
		}
	}

	for(u32 i = 0; i < mainButton.size(); ++i)
	{
		if(sender != mainButton[i])
			continue;
		
		FlyingBtnMenuClicked(i);
		break;
	}
}
