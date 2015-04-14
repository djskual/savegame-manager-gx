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
#include "../menu.h"
#include "../Themes/CTheme.h"
#include "../Settings/CSettings.h"
#include "gettext.h"
#include "CLangList.hpp"

#define LISTSIZE	10

LanguagesSelectBrowser::LanguagesSelectBrowser()
	:GuiWindow(0, 0)
{
	this->Setup();
}

LanguagesSelectBrowser::~LanguagesSelectBrowser()
{
	ResumeGui();
	SetEffect(EFFECT_FADE, -50);
	while(this->GetEffect() > 0) usleep(100);

	HaltGui();
	if(parentElement)
		((GuiWindow *) parentElement)->Remove(this);

	RemoveAll();

	delete WindowImg;
	delete BrowserImg;
	delete ValidImg;
	delete ValidOverImg;
	delete CloseImg;
	delete CloseOverImg;
	delete ButtonBoxImgClose;
	delete ButtonBoxImgVal;

	delete WindowImgData;
	delete BrowserImgData;
	delete ValidImgData;
	delete ValidOverImgData;
	delete CloseImgData;
	delete CloseOverImgData;
	delete ButtonBoxImgData;
	delete ListSelectionImgData;

	delete TitleText;

	delete ScrollBar;

	delete ValidBtn;
	delete CloseBtn;
	
	delete trigA;
	delete trigHome;
	delete trigB;
	
	while(ListBtn.size() > 0)
		RemoveButton(0);
}

void LanguagesSelectBrowser::Setup()
{
	numEntries = LangList.GetCount();
	selectedItem = 0;
	pageIndex = 0;
	listChanged = true;
	result = -1;
	
	SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	SetPosition(0, 2);

	trigA = new SimpleGuiTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	trigB = new GuiTrigger();
	trigB->SetButtonOnlyTrigger(-1, WiiControls.BackButton | ClassicControls.BackButton << 16, GCControls.BackButton);
	trigHome = new GuiTrigger();
	trigHome->SetButtonOnlyTrigger(-1, WiiControls.HomeButton | ClassicControls.HomeButton << 16, GCControls.HomeButton);
	
	WindowImgData = Resources::GetImageData("dialogue_box.png");
	BrowserImgData = Resources::GetImageData("browser.png");
	ButtonBoxImgData = Resources::GetImageData("browser_select_button_box.png");
	ListSelectionImgData = Resources::GetImageData("browser_options_entry_bg.png");

	ValidImgData = Resources::GetImageData("button_valid.png");
	ValidOverImgData = Resources::GetImageData("button_valid_over.png");
	CloseImgData = Resources::GetImageData("button_close.png");
	CloseOverImgData = Resources::GetImageData("button_close_over.png");
	
	this->SetSize(WindowImgData->GetWidth(), WindowImgData->GetHeight());
	
	WindowImg = new GuiImage(WindowImgData);
	WindowImg->SetParent(this);
	WindowImg->SetSkew(0, -80, 0, -80, 0, 35, 0, 35);

	BrowserImg = new GuiImage(BrowserImgData);
	BrowserImg->SetParent(this);
	BrowserImg->SetPosition(0, 0);
	BrowserImg->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	BrowserImg->SetSkew(90, 0, -130, 0, -130, 12, 90, 12);

	ButtonBoxImgClose = new GuiImage(ButtonBoxImgData);
	ButtonBoxImgClose->SetParent(this);
	ButtonBoxImgClose->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	ButtonBoxImgClose->SetPosition(-25, -45);
	ButtonBoxImgVal = new GuiImage(ButtonBoxImgData);
	ButtonBoxImgVal->SetParent(this);
	ButtonBoxImgVal->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	ButtonBoxImgVal->SetPosition(-70, -45);
	
	TitleText = new GuiText(tr( "Select Languages" ), 22, thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
	TitleText->SetParent(this);
	TitleText->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	TitleText->SetPosition(-20, -47);
	TitleText->SetMaxWidth(255, SCROLL_HORIZONTAL);
	
	CloseImg = new GuiImage(CloseImgData);
	CloseOverImg = new GuiImage(CloseOverImgData);
	CloseBtn = new GuiButton(CloseImg->GetWidth(), CloseImg->GetHeight());
	CloseBtn->SetParent(this);
	CloseBtn->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	CloseBtn->SetPosition(-28, -43);
	CloseBtn->SetImage(CloseImg);
	CloseBtn->SetImageOver(CloseOverImg);
	CloseBtn->SetSoundOver(btnSoundOver);
	CloseBtn->SetSoundClick(btnSoundClick);
	CloseBtn->SetTrigger(trigA);
	CloseBtn->SetTrigger(trigHome);
	CloseBtn->SetTrigger(trigB);
	CloseBtn->SetEffectGrow();
	
	ValidImg = new GuiImage(ValidImgData);
	ValidOverImg = new GuiImage(ValidOverImgData);
	ValidBtn = new GuiButton(ValidImg->GetWidth(), ValidImg->GetHeight());
	ValidBtn->SetParent(this);
	ValidBtn->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	ValidBtn->SetPosition(-73, -43);
	ValidBtn->SetImage(ValidImg);
	ValidBtn->SetImageOver(ValidOverImg);
	ValidBtn->SetSoundOver(btnSoundOver);
	ValidBtn->SetSoundClick(btnSoundClick);
	ValidBtn->SetTrigger(trigA);
	ValidBtn->SetEffectGrow();
	
	ScrollBar = new Scrollbar(288, LISTMODE);
	ScrollBar->SetParent(this);
	ScrollBar->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	ScrollBar->SetPosition(-30, 0);
	ScrollBar->SetScrollSpeed(3);
	ScrollBar->SetEntrieCount(numEntries);
	
	SetState(STATE_DEFAULT);
	SetEffect(EFFECT_FADE, 50);
}

void LanguagesSelectBrowser::AddButton()
{
	int size = ListBtn.size();

	ListBtnText.resize(size+1);
	ListBtnText[size] = new GuiText((char*) NULL, 20, thColor("r=0 g=0 b=0 a=255 - prompt languageSelect list text color"));
	ListBtnText[size]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	ListBtnText[size]->SetPosition(25+28, 0);
	ListBtnText[size]->SetMaxWidth(360 - (25+28), DOTTED);

	ListBtnTextOver.resize(size+1);
	ListBtnTextOver[size] = new GuiText((char*) NULL, 20, thColor("r=0 g=0 b=0 a=255 - prompt languageSelect list text color"));
	ListBtnTextOver[size]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	ListBtnTextOver[size]->SetPosition(25+28, 0);
	ListBtnTextOver[size]->SetMaxWidth(360 - (25+28), SCROLL_HORIZONTAL);

	ListSelectionImg.resize(size+1);
	ListSelectionImg[size] = new GuiImage(ListSelectionImgData);
	ListSelectionImg[size]->SetSkew(0, 0, -20, 0, -20, 0, 0, 0);

	ListBtnCheckbox.resize(size+1);
	ListBtnCheckbox[size] = NULL;
	
	ListBtn.resize(size+1);
	ListBtn[size] = new GuiButton(360, 28);
	ListBtn[size]->SetParent(this);
	ListBtn[size]->SetLabel(ListBtnText[size]);
	ListBtn[size]->SetLabelOver(ListBtnTextOver[size]);
	ListBtn[size]->SetImageOver(ListSelectionImg[size]);
	ListBtn[size]->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	ListBtn[size]->SetPosition(-20, 28*size+4);
	ListBtn[size]->SetTrigger(trigA);
	ListBtn[size]->SetSoundClick(btnSoundClick);
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
	ListBtnCheckbox[i]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	ListBtnCheckbox[i]->SetChecked(needed);
	
	ListBtnText[i]->SetText(filename);
	ListBtnTextOver[i]->SetText(filename);

	ListBtn[i]->SetVisible(true);

	if(ListBtn[i]->GetState() == STATE_DISABLED)
		ListBtn[i]->SetState(STATE_DEFAULT);
}

int LanguagesSelectBrowser::GetResult()
{
	if (CloseBtn->GetState() == STATE_CLICKED)
	{
		CloseBtn->ResetState();
		
		result = 0;
	}
	else if (ValidBtn->GetState() == STATE_CLICKED)
	{
		ValidBtn->ResetState();
		
		int needed = 0;
		
		for (int i = 0; i < LangList.GetCount(); i++)
		{
			if (LangList.Needed(i))
				needed++;
		}
		
		result = needed;
	}
	
	return result;
}

void LanguagesSelectBrowser::SetSelected(int i)
{
	if(i < 0 || i >= (int) ListBtn.size())
		return;

	selectedItem = i;
	ListBtn[selectedItem]->SetState(STATE_SELECTED);
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

	if(ScrollBar->ListChanged())
	{
		selectedItem = ScrollBar->GetSelectedItem();
		pageIndex = ScrollBar->GetSelectedIndex();
		listChanged = true;
	}

	if(t->Right())
	{
		if(pageIndex < numEntries && numEntries > LISTSIZE)
		{
			int pIndex = pageIndex+LISTSIZE;
			if(pIndex+LISTSIZE >= numEntries)
				pIndex = numEntries-LISTSIZE;
			pageIndex = pIndex;
			listChanged = true;
		}
	}
	else if(t->Left())
	{
		if(pageIndex > 0)
		{
			int pIndex = pageIndex - LISTSIZE;
			if(pIndex < 0)
				pIndex = 0;
			pageIndex = pIndex;
			listChanged = true;
		}
	}
	else if(t->Down())
	{
		if(pageIndex+selectedItem+1 < numEntries)
		{
			if(selectedItem == (int) ListBtn.size()-1)
			{
				pageIndex++;
				listChanged = true;
			}
			else if((int) ListBtn.size() > selectedItem+1 && ListBtn[selectedItem+1]->IsVisible())
			{
				ListBtn[selectedItem]->ResetState();
				ListBtn[++selectedItem]->SetState(STATE_SELECTED, t->chan);
			}
		}
	}
	else if(t->Up())
	{
		if(selectedItem == 0 &&	 pageIndex+selectedItem > 0)
		{
			pageIndex--;
			listChanged = true;
		}
		else if((int) ListBtn.size() > selectedItem && selectedItem > 0)
		{
			ListBtn[selectedItem]->ResetState();
			ListBtn[--selectedItem]->SetState(STATE_SELECTED, t->chan);
		}
	}

	if(numEntries != LangList.GetCount())
	{
		numEntries = LangList.GetCount();
		ScrollBar->SetEntrieCount(numEntries);
		listChanged = true;
	}

	for(int i = 0; i < LISTSIZE; i++)
	{
		if(listChanged)
		{
			if(pageIndex+i < numEntries)
			{
				const char * filename = LangList.GetFilename(pageIndex + i);
				bool needed = LangList.Needed(pageIndex + i);
				
				SetButton(i, filename, needed, true);
			}
			else
			{
				SetButton(i, NULL, false, false);
			}
		}
		
		if(i != selectedItem && ListBtn[i]->GetState() == STATE_SELECTED)
			ListBtn[i]->ResetState();
		else if(i == selectedItem && ListBtn[i]->GetState() == STATE_DEFAULT)
			ListBtn[selectedItem]->SetState(STATE_SELECTED, -1);
		
		int currChan = t->chan;
		
		if(t->wpad.ir.valid && !ListBtn[i]->IsInside(t->wpad.ir.x, t->wpad.ir.y))
			t->chan = -1;
		
		ListBtn[i]->Update(t);
		t->chan = currChan;
		
		if (ListBtn[i]->GetState() == STATE_CLICKED)
		{
			LangList.SetNeeded(pageIndex + i, !LangList.Needed(pageIndex + i));
			ListBtn[i]->ResetState();
			ListBtnCheckbox[i]->SetChecked(LangList.Needed(pageIndex + i));
			this->SetSelected(i);
		}
		
		if(ListBtn[i]->GetState() == STATE_SELECTED)
			selectedItem = i;
	}
	
	ScrollBar->SetPageSize(LISTSIZE);
	ScrollBar->SetRowSize(0);
	ScrollBar->SetSelectedItem(selectedItem);
	ScrollBar->SetSelectedIndex(pageIndex);

	listChanged = false;
}

int languagesSelectBrowser()
{
	mainWindow->SetState(STATE_DISABLED);
	
	int ret = -1;
	LanguagesSelectBrowser * lsb = new LanguagesSelectBrowser();

	mainWindow->Append(lsb);
	ResumeGui();

	while(ret == -1)
	{
		usleep(100);

		ret = lsb->GetResult();
	}

	delete lsb;
	lsb = NULL;
	ResumeGui();

	return ret;
}
