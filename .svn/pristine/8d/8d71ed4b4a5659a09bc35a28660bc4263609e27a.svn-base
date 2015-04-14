/****************************************************************************
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
 * TitleBrowser.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include <unistd.h>

#include "../Settings/CSettings.h"
#include "../Language/gettext.h"
#include "../LibWiiGui/gui.h"
#include "../Themes/CTheme.h"
#include "../System/sys.h"
#include "../menu.h"
#include "TitleBrowser.hpp"

#define LISTSIZE	10

TitleBrowser::TitleBrowser()
	:GuiWindow(0, 0)
{
	this->Setup();
}

TitleBrowser::~TitleBrowser()
{
	ResumeGui();
	
	SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_OUT, 50);
	while(parentElement && this->GetEffect() > 0) usleep(100);

	HaltGui();
	if(parentElement)
		((GuiWindow *) parentElement)->Remove(this);
	parentElement = NULL;

	RemoveAll();

	delete WindowImg;
	delete BrowserImg;
	delete CloseImg;
	delete CloseOverImg;
	delete ButtonBoxImgClose;
	
	delete WindowImgData;
	delete BrowserImgData;
	delete CloseImgData;
	delete CloseOverImgData;
	delete ButtonBoxImgData;
	delete ListSelectionImgData;

	delete TitleText;

	delete ScrollBar;

	delete CloseBtn;
	
	delete trigA;
	delete trigHome;
	delete trigB;
	
	while(ListBtn.size() > 0)
		RemoveButton(0);
	
	delete TitleList;
	
	ResumeGui();
}

void TitleBrowser::Setup()
{
	TitleList = new CTitleList();
	
	numEntries = TitleList->GetCount();
	selectedItem = 0;
	pageIndex = 0;
	listChanged = true;
	col2Setted = false;
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
	
	TitleText = new GuiText(tr( "Title Launcher" ), 22, thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
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
	
	ScrollBar = new Scrollbar(288, LISTMODE);
	ScrollBar->SetParent(this);
	ScrollBar->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	ScrollBar->SetPosition(-30, 0);
	ScrollBar->SetScrollSpeed(3);
	ScrollBar->SetEntrieCount(numEntries);
	ScrollBar->SetListChanged(true);
	
	this->SetState(STATE_DEFAULT);
	SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_IN, 50);
}

void TitleBrowser::SetCol2()
{
	if(col2Setted)
		return;
	
	col2 = 0;
	int maxNameWidth = 0;
	for (int i = 0; i < TitleList->GetCount()-1; i++)
	{
		GuiText * text = new GuiText(TitleList->GetID(i).c_str(), 20 , (GXColor){0,0,0,255});
		if (maxNameWidth < text->GetTextWidth())
			maxNameWidth = text->GetTextWidth();
		delete text;
	}
	
	if (col2 < (25 + maxNameWidth + 15))
		col2 = 25 + maxNameWidth + 15;
	
	col2Setted = true;
}

void TitleBrowser::AddButton()
{
	int size = ListBtn.size();
	
	ListBtnID.resize(size+1);
	ListBtnID[size] = new GuiText((char*) NULL, 20, thColor("r=0 g=0 b=0 a=255 - prompt languageSelect list text color"));
	ListBtnID[size]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	ListBtnID[size]->SetPosition(25, 0);
	
	ListBtnName.resize(size+1);
	ListBtnName[size] = new GuiText((char*) NULL, 20, thColor("r=0 g=0 b=0 a=255 - prompt languageSelect list text color"));
	ListBtnName[size]->SetParent(this);
	ListBtnName[size]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	
	ListBtnNameOver.resize(size+1);
	ListBtnNameOver[size] = new GuiText((char*) NULL, 20, thColor("r=0 g=0 b=0 a=255 - prompt languageSelect list text color"));
	ListBtnNameOver[size]->SetParent(this);
	ListBtnNameOver[size]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	
	ListSelectionImg.resize(size+1);
	ListSelectionImg[size] = new GuiImage(ListSelectionImgData);
	ListSelectionImg[size]->SetSkew(0, 0, -20, 0, -20, 0, 0, 0);

	ListBtn.resize(size+1);
	ListBtn[size] = new GuiButton(360, 28);
	ListBtn[size]->SetParent(this);
	ListBtn[size]->SetLabel(ListBtnID[size], 0);
	ListBtn[size]->SetLabel(ListBtnName[size], 1);
	ListBtn[size]->SetLabelOver(ListBtnNameOver[size], 1);
	ListBtn[size]->SetImageOver(ListSelectionImg[size]);
	ListBtn[size]->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	ListBtn[size]->SetPosition(-20, 28*size+4);
	ListBtn[size]->SetTrigger(trigA);
	ListBtn[size]->SetSoundClick(btnSoundClick);
}

void TitleBrowser::RemoveButton(int i)
{
	if(i < 0 || i >= (int) ListBtn.size())
		return;

	if(ListBtnID[i])
		delete ListBtnID[i];
	if(ListBtnName[i])
		delete ListBtnName[i];
	if(ListBtnNameOver[i])
		delete ListBtnNameOver[i];
	if(ListSelectionImg[i])
		delete ListSelectionImg[i];
	if(ListBtn[i])
		delete ListBtn[i];

	ListBtnID.erase(ListBtnID.begin()+i);
	ListBtnName.erase(ListBtnName.begin()+i);
	ListBtnNameOver.erase(ListBtnNameOver.begin()+i);
	ListSelectionImg.erase(ListSelectionImg.begin()+i);
	ListBtn.erase(ListBtn.begin()+i);
}

void TitleBrowser::SetButton(int i, std::string id, std::string name, bool enable)
{
	LOCK(this);
	
	SetCol2();

	if(i < 0)
		return;
	
	else if(i >= (int) ListBtn.size())
		AddButton();
	
	if(!enable)
	{
		ListBtn[i]->SetVisible(false);
		ListBtn[i]->SetState(STATE_DISABLED);
		return;
	}

	ListBtnID[i]->SetText(id);
	
	ListBtnName[i]->SetText(name);
	ListBtnName[i]->SetPosition(col2, 0);
	ListBtnName[i]->SetMaxWidth(360-col2-10, DOTTED);
	
	ListBtnNameOver[i]->SetText(name);
	ListBtnNameOver[i]->SetPosition(col2, 0);
	ListBtnNameOver[i]->SetMaxWidth(360-col2-10, SCROLL_HORIZONTAL);
	
	ListBtn[i]->SetVisible(true);
	
	if(ListBtn[i]->GetState() == STATE_DISABLED)
		ListBtn[i]->SetState(STATE_DEFAULT);
}

int TitleBrowser::GetResult()
{
	if (CloseBtn->GetState() == STATE_CLICKED)
	{
		CloseBtn->ResetState();
		
		result = 0;
	}
	
	return result;
}

void TitleBrowser::SetSelected(int i)
{
	if(i < 0 || i >= (int) ListBtn.size())
		return;

	selectedItem = i;
	ListBtn[selectedItem]->SetState(STATE_SELECTED);
}

void TitleBrowser::Draw()
{
	if(!this->IsVisible())
		return;

	WindowImg->Draw();
	BrowserImg->Draw();
	TitleText->Draw();
	ButtonBoxImgClose->Draw();
	CloseBtn->Draw();
	
	for(u32 i = 0; i < ListBtn.size(); i++)
		ListBtn[i]->Draw();

	ScrollBar->Draw();

	UpdateEffects();
}

void TitleBrowser::Update(GuiTrigger * t)
{
	if(state == STATE_DISABLED || !t)
		return;

	ScrollBar->Update(t);
	CloseBtn->Update(t);
	
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

	if(numEntries != TitleList->GetCount())
	{
		numEntries = TitleList->GetCount();
		ScrollBar->SetEntrieCount(numEntries);
		listChanged = true;
	}

	for(int i = 0; i < LISTSIZE; i++)
	{
		if(listChanged)
		{
			if(pageIndex+i < numEntries)
			{
				std::string id = TitleList->GetID(pageIndex + i);
				std::string name = TitleList->GetName(pageIndex + i);
				
				SetButton(i, id, name, true);
			}
			else
			{
				SetButton(i, NULL, NULL, false);
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
			if((pageIndex+i) == numEntries-1) //!Wii Settings
			{
				ExitApp();
				WII_Initialize();
				WII_ReturnToSettings();
			}
			
			ExitApp();
			WII_Initialize();
			WII_LaunchTitle(TitleList->GetTid(pageIndex+i));
			
			ListBtn[i]->ResetState();
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

int titleBrowser()
{
	int ret = -1;
	TitleBrowser * tb = new TitleBrowser();

	mainWindow->Append(tb);

	while(ret == -1)
	{
		usleep(100);
		
		ret = tb->GetResult();
	}

	delete tb;
	tb = NULL;

	return ret;
}
