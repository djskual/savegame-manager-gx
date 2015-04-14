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
 * LanguagesSelectBrowser
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include "LanguagesSelectBrowser.hpp"
#include "../Controls/Application.h"
#include "../Themes/CTheme.h"
#include "CLangList.hpp"

#define LISTSIZE	10

LanguagesSelectBrowser::LanguagesSelectBrowser()
	: GuiFrame(0, 0)
{
	this->Setup();
}

LanguagesSelectBrowser::~LanguagesSelectBrowser()
{
	RemoveAll();

	delete WindowImg;
	delete BrowserImg;
	delete ButtonBoxImgClose;
	delete ButtonBoxImgVal;

	Resources::Remove(WindowImgData);
	Resources::Remove(BrowserImgData);
	Resources::Remove(ButtonBoxImgData);
	Resources::Remove(ListSelectionImgData);

	Resources::Remove(btnSoundClick);
	Resources::Remove(btnSoundOver);

	delete TitleText;

	delete ScrollBar;

	delete ValidBtn;
	delete CloseBtn;
	
	while(ListBtn.size() > 0)
		RemoveButton(0);
}

void LanguagesSelectBrowser::Setup()
{
	numEntries = 0;
	selectedItem = 0;
	pageIndex = 0;
	result = -1;
	
	btnSoundClick = Resources::GetSound("button_click.wav");
	btnSoundOver = Resources::GetSound("button_over.wav");
	btnSoundClick->SetVolume(Settings.SFXVolume);
	btnSoundOver->SetVolume(Settings.SFXVolume);
	
	trigA.SetSimpleTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	
	WindowImgData = Resources::GetImageData("dialogue_box.png");
	BrowserImgData = Resources::GetImageData("browser.png");
	ButtonBoxImgData = Resources::GetImageData("browser_select_button_box.png");
	ListSelectionImgData = Resources::GetImageData("browser_bg_selection.png");

	this->SetSize(472, 436);
	
	WindowImg = new GuiImage(WindowImgData);
	WindowImg->SetParent(this);
	WindowImg->SetScaleY(1.3625);
	
	BrowserImg = new GuiImage(BrowserImgData);
	BrowserImg->SetParent(this);
	BrowserImg->SetPosition(-22, 28);
	BrowserImg->SetAlignment(ALIGN_CENTER | ALIGN_TOP);
	BrowserImg->SetScaleX(0.6233);
	BrowserImg->SetScaleY(1.0435);
	
	ButtonBoxImgClose = new GuiImage(ButtonBoxImgData);
	ButtonBoxImgClose->SetParent(this);
	ButtonBoxImgClose->SetAlignment(ALIGN_RIGHT | ALIGN_TOP);
	ButtonBoxImgClose->SetPosition(-25, -26);
	ButtonBoxImgVal = new GuiImage(ButtonBoxImgData);
	ButtonBoxImgVal->SetParent(this);
	ButtonBoxImgVal->SetAlignment(ALIGN_RIGHT | ALIGN_TOP);
	ButtonBoxImgVal->SetPosition(-70, -26);
	
	TitleText = new GuiText(tr( "Select Languages" ), 22, thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
	TitleText->SetParent(this);
	TitleText->SetAlignment(ALIGN_CENTER | ALIGN_TOP);
	TitleText->SetPosition(-20, -28);
	TitleText->SetMaxWidth(255, SCROLL_HORIZONTAL);
	
	CloseBtn = new PictureButton("button_close.png", "button_close_over.png", btnSoundClick, btnSoundOver);
	CloseBtn->SetParent(this);
	CloseBtn->SetAlignment(ALIGN_RIGHT | ALIGN_TOP);
	CloseBtn->SetPosition(-28, -24);
	CloseBtn->SetTrigger(&trigA);
	CloseBtn->Clicked.connect(this, &LanguagesSelectBrowser::OnButtonClick);
	
	ValidBtn = new PictureButton("button_valid.png", "button_valid_over.png", btnSoundClick, btnSoundOver);
	ValidBtn->SetParent(this);
	ValidBtn->SetAlignment(ALIGN_RIGHT | ALIGN_TOP);
	ValidBtn->SetPosition(-73, -24);
	ValidBtn->SetTrigger(&trigA);
	ValidBtn->Clicked.connect(this, &LanguagesSelectBrowser::OnButtonClick);
	
	ScrollBar = new Scrollbar(288, Scrollbar::CUSTOMMODE);
	ScrollBar->SetParent(this);
	ScrollBar->SetAlignment(ALIGN_RIGHT | ALIGN_TOP);
	ScrollBar->SetPosition(-32, 22);
	ScrollBar->SetScrollSpeed(Settings.ScrollSpeed);
	ScrollBar->listChanged.connect(this, &LanguagesSelectBrowser::OnListChange);
	ScrollBar->SetButtonScroll(WiiControls.OneButtonScroll | ClassicControls.OneButtonScroll << 16, GCControls.OneButtonScroll);
	
	SetAlignment(ALIGN_CENTER | ALIGN_MIDDLE);
	SetPosition(0, 38);
	SetEffect(EFFECT_FADE, 20);
}

void LanguagesSelectBrowser::AddButton()
{
	int size = ListBtn.size();

	ListBtnText.resize(size+1);
	ListBtnText[size] = new GuiText((char*) NULL, 20, thColor("r=0 g=0 b=0 a=255 - prompt languageSelect list text color"));
	ListBtnText[size]->SetAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
	ListBtnText[size]->SetPosition(25+28, 0);
	ListBtnText[size]->SetMaxWidth(360 - (25+28), DOTTED);

	ListBtnTextOver.resize(size+1);
	ListBtnTextOver[size] = new GuiText((char*) NULL, 20, thColor("r=0 g=0 b=0 a=255 - prompt languageSelect list text color"));
	ListBtnTextOver[size]->SetAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
	ListBtnTextOver[size]->SetPosition(25+28, 0);
	ListBtnTextOver[size]->SetMaxWidth(360 - (25+28), SCROLL_HORIZONTAL);

	ListSelectionImg.resize(size+1);
	ListSelectionImg[size] = new GuiImage(ListSelectionImgData);
	ListSelectionImg[size]->SetScaleX(0.8);
	
	ListBtnCheckbox.resize(size+1);
	ListBtnCheckbox[size] = NULL;
	
	ListBtn.resize(size+1);
	ListBtn[size] = new GuiButton(364, 28);
	ListBtn[size]->SetParent(this);
	ListBtn[size]->SetLabel(ListBtnText[size]);
	ListBtn[size]->SetLabelOver(ListBtnTextOver[size]);
	ListBtn[size]->SetImageOver(ListSelectionImg[size]);
	ListBtn[size]->SetAlignment(ALIGN_CENTER | ALIGN_TOP);
	ListBtn[size]->SetPosition(-40, 28*size+4+22);
	ListBtn[size]->SetTrigger(&trigA);
	ListBtn[size]->SetSoundClick(btnSoundClick);
	ListBtn[size]->Clicked.connect(this, &LanguagesSelectBrowser::OnListClick);
}

void LanguagesSelectBrowser::RemoveButton(int i)
{
	if(i < 0 || i >= (int) ListBtn.size())
		return;

	if(ListBtnText[i])
		delete ListBtnText[i];
	if(ListBtnTextOver[i])
		delete ListBtnTextOver[i];
	if(ListSelectionImg[i])
		delete ListSelectionImg[i];
	if(ListBtnCheckbox[i])
		delete ListBtnCheckbox[i];
	if(ListBtn[i])
		delete ListBtn[i];

	ListBtnText.erase(ListBtnText.begin()+i);
	ListBtnTextOver.erase(ListBtnTextOver.begin()+i);
	ListSelectionImg.erase(ListSelectionImg.begin()+i);
	ListBtnCheckbox.erase(ListBtnCheckbox.begin()+i);
	ListBtn.erase(ListBtn.begin()+i);
}

void LanguagesSelectBrowser::SetButton(int i, const char * filename, bool needed, bool enable)
{
	if(i < 0)
		return;

	else if(i >= (int) ListBtn.size())
	{
		AddButton();
	}

	if(!enable)
	{
		ListBtn[i]->SetVisible(false);
		ListBtn[i]->SetState(STATE_DISABLED);
		return;
	}

	if(ListBtnCheckbox[i])
		delete ListBtnCheckbox[i];

	ListBtnCheckbox[i] = new GuiCheckbox(20, 20, GuiCheckbox::CHECKSIGN);
	ListBtnCheckbox[i]->SetParent(ListBtn[i]);
	ListBtnCheckbox[i]->SetPosition(25, 0);
	ListBtnCheckbox[i]->SetAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
	ListBtnCheckbox[i]->SetChecked(needed);
	
	ListBtnText[i]->SetText(filename);
	ListBtnTextOver[i]->SetText(filename);

	ListBtn[i]->SetVisible(true);

	if(ListBtn[i]->GetState() == STATE_DISABLED)
		ListBtn[i]->SetState(STATE_DEFAULT);
}

void LanguagesSelectBrowser::OnButtonClick(GuiButton *sender, int pointer UNUSED, const POINT &p UNUSED)
{
	if (sender == CloseBtn)
	{
		result = 0;
	}
	else
	{
		int needed = 0;
		
		for (int i = 0; i < LangList.GetCount(); i++)
		{
			if (LangList.Needed(i))
				needed++;
		}
		
		result = needed;
	}
}

void LanguagesSelectBrowser::OnListClick(GuiButton *sender, int pointer UNUSED, const POINT &p UNUSED)
{
	for(u32 i = 0; i < ListBtn.size(); i++)
	{
		if(ListBtn[i] == sender)
		{
			LangList.SetNeeded(pageIndex + i, !LangList.Needed(pageIndex + i));
			ListBtnCheckbox[i]->SetChecked(LangList.Needed(pageIndex + i));
			break;
		}
	}
}

void LanguagesSelectBrowser::OnListChange(int selItem, int selIndex)
{
	ScrollBar->SetSelected(selItem, selIndex);
	selectedItem = selItem;
	pageIndex = selIndex;

	for(int i = 0; i < LISTSIZE; i++)
	{
		if(pageIndex+i < LangList.GetCount())
		{
			const char * filename = LangList.GetFilename(pageIndex + i);
			bool needed = LangList.Needed(pageIndex + i);
			
			SetButton(i, filename, needed, true);
		}
		else
			SetButton(i, NULL, false, false);
	}
}

void LanguagesSelectBrowser::Draw()
{
	if(!this->IsVisible())
		return;

	WindowImg->Draw();
	BrowserImg->Draw();
	TitleText->Draw();
	ButtonBoxImgClose->Draw();
	ButtonBoxImgVal->Draw();
	CloseBtn->Draw();
	ValidBtn->Draw();
	
	for(u32 i = 0; i < ListBtn.size(); i++)
	{
		ListBtn[i]->Draw();
		ListBtnCheckbox[i]->Draw();
	}

	ScrollBar->Draw();
	
	UpdateEffects();
}

void LanguagesSelectBrowser::Update(GuiTrigger * t)
{
	if(state == STATE_DISABLED || !t)
		return;
	
	ScrollBar->Update(t);
	CloseBtn->Update(t);
	ValidBtn->Update(t);
	
	if(numEntries != LangList.GetCount())
	{
		numEntries = LangList.GetCount();
		ScrollBar->SetEntrieCount(numEntries);
	}

	for(int i = 0; i < (int) ListBtn.size(); i++)
	{
		if(i != selectedItem && ListBtn[i]->GetState() == STATE_SELECTED)
			ListBtn[i]->ResetState();
		else if(i == selectedItem && ListBtn[i]->GetState() == STATE_DEFAULT)
			ListBtn[selectedItem]->SetState(STATE_SELECTED, t->chan);
		
		int currChan = t->chan;
		
		if(t->wpad->ir.valid && !ListBtn[i]->IsInside(t->wpad->ir.x, t->wpad->ir.y))
			t->chan = -1;
		
		ListBtn[i]->Update(t);
		t->chan = currChan;
		
		if(ListBtn[i]->GetState() == STATE_SELECTED)
			selectedItem = i;
	}
	
	ScrollBar->SetPageSize(ListBtn.size());
	ScrollBar->SetRowSize(0);
	ScrollBar->SetSelected(selectedItem, pageIndex);
}

int languagesSelectBrowser()
{
	int ret = -1;
	LanguagesSelectBrowser * lsb = new LanguagesSelectBrowser();
	lsb->DimBackground(true);
	Application::Instance()->Append(lsb);
	Application::Instance()->SetUpdateOnly(lsb);

	while((ret = lsb->GetResult()) < 0)
	{
		Application::Instance()->updateEvents();
	}

	lsb->SetEffect(EFFECT_FADE, -20);
	while(lsb->IsAnimated())
		Application::Instance()->updateEvents();
	
	Application::Instance()->PushForDelete(lsb);
	
	return ret;
}
