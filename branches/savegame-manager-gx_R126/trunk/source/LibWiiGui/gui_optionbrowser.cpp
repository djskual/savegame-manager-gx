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
 * gui_optionbrowser.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include <unistd.h>

#include "gui_optionbrowser.hpp"
#include "../Settings/CSettings.h"
#include "../Themes/CTheme.h"
#include "../menu.h"

#define LISTSIZE	9

GuiOptionBrowser::GuiOptionBrowser(OptionList * l)
	:GuiWindow(0, 0)
{
	options = l;
	this->Setup();
}

GuiOptionBrowser::~GuiOptionBrowser()
{
	HaltGui();
	if(parentElement)
		((GuiWindow *) parentElement)->Remove(this);

	RemoveAll();

	delete BrowserImg;
	
	delete BrowserImgData;
	delete ListSelectionImgData;

	delete ScrollBar;

	delete trigA;
	
	while(ListBtn.size() > 0)
		RemoveButton(0);
	
	ResumeGui();
}

void GuiOptionBrowser::Setup()
{
	numEntries = options->GetLength();
	selectedItem = 0;
	pageIndex = 0;
	listChanged = true;
	col2Setted = false;
	clicked = -1;
	
	SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	SetPosition(0, 2);

	trigA = new SimpleGuiTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	
	BrowserImgData = Resources::GetImageData("browser_options.png");
	ListSelectionImgData = Resources::GetImageData("browser_options_entry_bg.png");

	this->SetSize(BrowserImgData->GetWidth(), BrowserImgData->GetHeight());
	
	BrowserImg = new GuiImage(BrowserImgData);
	BrowserImg->SetParent(this);
	BrowserImg->SetPosition(0, 0);
	BrowserImg->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	
	ScrollBar = new Scrollbar(BrowserImgData->GetHeight()+20, OPTIONMODE);
	ScrollBar->SetParent(this);
	ScrollBar->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	ScrollBar->SetPosition(6, -10);
	ScrollBar->SetScrollSpeed(3);
	ScrollBar->SetEntrieCount(numEntries);
	ScrollBar->SetListChanged(true);
	
	SetState(STATE_DEFAULT);
	SetEffect(EFFECT_FADE, 50);
}

void GuiOptionBrowser::SetCol2()
{
	if(col2Setted)
		return;
	
	col2 = 0;
	int maxNameWidth = 0;
	for (int i = 0; i < options->GetLength(); i++)
	{
		GuiText * text = new GuiText(options->GetName(i), 20 , (GXColor){0,0,0,255});
		if (maxNameWidth < text->GetTextWidth())
			maxNameWidth = text->GetTextWidth();
		delete text;
	}
	
	if (col2 < (25 + maxNameWidth + 15))
		col2 = 25 + maxNameWidth + 15;
	
	col2Setted = true;
}

void GuiOptionBrowser::AddButton()
{
	int size = ListBtn.size();
	
	ListBtnName.resize(size+1);
	ListBtnName[size] = new GuiText((char*) NULL, 20, thColor("r=0 g=0 b=0 a=255 - prompt languageSelect list text color"));
	ListBtnName[size]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	ListBtnName[size]->SetPosition(25, 0);
	
	ListBtnNameOver.resize(size+1);
	ListBtnNameOver[size] = new GuiText((char*) NULL, 20, thColor("r=0 g=0 b=0 a=255 - prompt languageSelect list text color"));
	ListBtnNameOver[size]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	ListBtnNameOver[size]->SetPosition(25, 0);
	
	ListBtnVal.resize(size+1);
	ListBtnVal[size] = new GuiText((char*) NULL, 20, thColor("r=0 g=0 b=0 a=255 - prompt languageSelect list text color"));
	ListBtnVal[size]->SetParent(this);
	ListBtnVal[size]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	
	ListBtnValOver.resize(size+1);
	ListBtnValOver[size] = new GuiText((char*) NULL, 20, thColor("r=0 g=0 b=0 a=255 - prompt languageSelect list text color"));
	ListBtnValOver[size]->SetParent(this);
	ListBtnValOver[size]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	
	ListSelectionImg.resize(size+1);
	ListSelectionImg[size] = new GuiImage(ListSelectionImgData);
	
	ListBtn.resize(size+1);
	ListBtn[size] = new GuiButton(BrowserImgData->GetWidth()-(8+ScrollBar->GetWidth()-6), 29);
	ListBtn[size]->SetParent(this);
	ListBtn[size]->SetLabel(ListBtnName[size], 0);
	ListBtn[size]->SetLabel(ListBtnVal[size], 1);
	ListBtn[size]->SetLabelOver(ListBtnValOver[size], 1);
	ListBtn[size]->SetImageOver(ListSelectionImg[size]);
	ListBtn[size]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	ListBtn[size]->SetPosition(8, 29*size+9);
	ListBtn[size]->SetTrigger(trigA);
	ListBtn[size]->SetSoundClick(btnSoundClick);
}

void GuiOptionBrowser::RemoveButton(int i)
{
	if(i < 0 || i >= (int) ListBtn.size())
		return;

	if(ListBtnName[i])
		delete ListBtnName[i];
	if(ListBtnNameOver[i])
		delete ListBtnNameOver[i];
	if(ListBtnVal[i])
		delete ListBtnVal[i];
	if(ListBtnValOver[i])
		delete ListBtnValOver[i];
	if(ListSelectionImg[i])
		delete ListSelectionImg[i];
	if(ListBtn[i])
		delete ListBtn[i];

	ListBtnName.erase(ListBtnName.begin()+i);
	ListBtnNameOver.erase(ListBtnNameOver.begin()+i);
	ListBtnVal.erase(ListBtnVal.begin()+i);
	ListBtnValOver.erase(ListBtnValOver.begin()+i);
	ListSelectionImg.erase(ListSelectionImg.begin()+i);
	ListBtn.erase(ListBtn.begin()+i);
}

void GuiOptionBrowser::SetButton(int i, const char * name, const char * value, bool enable)
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

	ListBtnName[i]->SetText(name);
	ListBtnNameOver[i]->SetText(name);
	
	ListBtnVal[i]->SetText(value);
	ListBtnVal[i]->SetPosition(col2, 0);
	ListBtnVal[i]->SetMaxWidth(BrowserImgData->GetWidth()-(8+ScrollBar->GetWidth()-6+col2), DOTTED);
	
	ListBtnValOver[i]->SetText(value);
	ListBtnValOver[i]->SetPosition(col2, 0);
	ListBtnValOver[i]->SetMaxWidth(BrowserImgData->GetWidth()-(8+ScrollBar->GetWidth()-6+col2), SCROLL_HORIZONTAL);
	
	ListBtn[i]->SetVisible(true);
	
	if(ListBtn[i]->GetState() == STATE_DISABLED)
		ListBtn[i]->SetState(STATE_DEFAULT);
}

int GuiOptionBrowser::GetClickedOption()
{
	int ret = clicked;
	clicked = -1;
	
	return ret;
}

void GuiOptionBrowser::SetSelected(int i)
{
	if(i < 0 || i >= (int) ListBtn.size())
		return;

	selectedItem = i;
	ListBtn[selectedItem]->SetState(STATE_SELECTED);
}

void GuiOptionBrowser::Draw()
{
	if(!this->IsVisible())
		return;

	BrowserImg->Draw();
	
	for(u32 i = 0; i < ListBtn.size(); i++)
		ListBtn[i]->Draw();

	ScrollBar->Draw();

	UpdateEffects();
}

void GuiOptionBrowser::Update(GuiTrigger * t)
{
	if(state == STATE_DISABLED || !t)
		return;
	
	ScrollBar->Update(t);
	
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

	if(numEntries != options->GetLength())
	{
		numEntries = options->GetLength();
		ScrollBar->SetEntrieCount(numEntries);
		listChanged = true;
	}
	
	for(int i = 0; i < LISTSIZE; i++)
	{
		if(listChanged)
		{
			if(pageIndex+i < numEntries)
			{
				const char * name = options->GetName(pageIndex + i);
				const char * value = options->GetValue(pageIndex + i);
				
				SetButton(i, name, value, true);
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
			clicked = pageIndex + i;
			ListBtn[i]->ResetState();
		}
		if(ListBtn[i]->GetState() == STATE_SELECTED)
			selectedItem = i;
	}
	
	ScrollBar->SetPageSize(LISTSIZE);
	ScrollBar->SetRowSize(0);
	ScrollBar->SetSelectedItem(selectedItem);
	ScrollBar->SetSelectedIndex(pageIndex);

	if (options->IsChanged())
	{
		for(int i = 0; i < LISTSIZE; i++)
		{
			if(pageIndex+i < numEntries)
				SetButton(i, options->GetName(pageIndex + i), options->GetValue(pageIndex + i), true);
			else
				SetButton(i, NULL, NULL, false);
		}
	}
	
	listChanged = false;
}
