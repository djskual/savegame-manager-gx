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
 * IconEmuBrowser.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include "IconEmuBrowser.hpp"
#include "../Tools/StringTools.h"
#include "../Saves/SaveTools.h"
#include "../Themes/CTheme.h"
#include "../EmuNand/CEmuList.hpp"
#include "../FileOperations/fileops.h"
#include "../menu.h"
#include "../Settings/CSettings.h"

/**
 * Constructor for the IconEmuBrowser class.
 */
IconEmuBrowser::IconEmuBrowser(Browser * emubrowser, GuiImageData * browserBg, int w, int h)
	: GuiFileBrowser(emubrowser, w, h)
{
	width = w;
	height = h;
	numEntries = 0;
	selectedItem = 0;
	oldSelectedItem = -1;
	browser = emubrowser;
	listChanged = true; //! trigger an initial list update
	triggerupdate = true; //! trigger disable

	trigA = new SimpleGuiTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	
	bgSelectionImg = new GuiImage(browserBg);
	bgSelectionImg->SetParent(this);
	
	bgSelectionEntry = Resources::GetImageData("browser_entry_save_bg.png");
	bgOverSelectionEntry = Resources::GetImageData("browser_entry_save_over_bg.png");
	
	scrollbar = new Scrollbar(browserBg->GetHeight()+20, ICONMODE);
	scrollbar->SetParent(this);
	scrollbar->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	scrollbar->SetPosition(4, -10);
	scrollbar->SetScrollSpeed(5);

	RowSize = 6;
	PageSize = 18;
}

/**
 * Destructor for the IconEmuBrowser class.
 */
IconEmuBrowser::~IconEmuBrowser()
{
	HaltGui();
	
	browser = NULL;
	
	delete bgSelectionImg;
	
	delete bgSelectionEntry;
	delete bgOverSelectionEntry;
	
	delete scrollbar;

	delete trigA;
	
	for(u32 i = 0; i < Buttons.size(); i++)
	{
		if(Buttons[i])
			delete Buttons[i];
		if(FileSelectionImg[i])
			delete FileSelectionImg[i];
		if(SaveBgImg[i])
			delete SaveBgImg[i];
		if(ButtonImg[i])
			delete ButtonImg[i];
		if(Tooltip[i])
			delete Tooltip[i];
	}

	Buttons.clear();
	SaveBgImg.clear();
	FileSelectionImg.clear();
	ButtonImg.clear();
	Tooltip.clear();
}

GuiSaveIcon * IconEmuBrowser::GetSaveIcon(int i)
{	
	u64 tid = browser->GetItemEmuTid(browser->GetPageIndex() + i);
	
	std::string savepath = fmt("%s/title/%08x/%08x/data/banner.bin", Settings.EmuNandPath, TITLE_UPPER(tid), TITLE_LOWER(tid));
	
	u8* buffer = NULL;
	u64 size = 0;
	LoadFileToMem(savepath, &buffer, &size);
	
	GuiSaveIcon * Icon = new GuiSaveIcon(buffer, size);
	
	free(buffer);
	buffer = NULL;
	
	return Icon;
}

void IconEmuBrowser::AddButton()
{
	//! File Icon
	GuiImage * BtnImg = NULL;
	ButtonImg.push_back(BtnImg);

	//! Save bg button
	GuiImage * SaveBg = new GuiImage(bgSelectionEntry);
	SaveBg->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	SaveBg->SetPosition(0, 4);
	SaveBgImg.push_back(SaveBg);
	
	//! Selection Img
	GuiImage * Marker = new GuiImage(bgOverSelectionEntry);
	Marker->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	Marker->SetPosition(0, 4);
	FileSelectionImg.push_back(Marker);

	//! Tooltip
	GuiTooltip * tmpToolTip = new GuiTooltip((char *) NULL);
	tmpToolTip->SetPosition(0, 0);
	Tooltip.push_back(tmpToolTip);

	GuiButton * Btn = new GuiButton(90, 90);
	Btn->SetParent(this);
	Btn->SetIcon(BtnImg);
	Btn->SetImage(SaveBg);
	Btn->SetImageOver(Marker);
	Btn->SetTrigger(trigA);
	Btn->SetSoundClick(btnSoundClick);
	Btn->SetToolTip(tmpToolTip, 0, 0, ALIGN_CENTRE, ALIGN_TOP);
	Buttons.push_back(Btn);
}

void IconEmuBrowser::SetButton(int i, std::string name, bool enable, int x, int y)
{
	if(i < 0)
		return;

	else if(i >= (int) Buttons.size())
	{
		AddButton();
	}

	if(!enable)
	{
		Buttons[i]->SetVisible(false);
		Buttons[i]->SetState(STATE_DISABLED);
		return;
	}

	if(ButtonImg[i])
		delete ButtonImg[i];
	
	ButtonImg[i] = GetSaveIcon(i);
	ButtonImg[i]->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	ButtonImg[i]->SetPosition(0, 4);
	Buttons[i]->SetIcon(ButtonImg[i]);
	
	Tooltip[i]->SetText(name.c_str());
	if(40+x+Tooltip[i]->GetWidth() > width)
	{
		int delta = 40+x+Tooltip[i]->GetWidth()-(Settings.Widescreen ? 720 : 640);
		
		Tooltip[i]->SetPosition(-delta, -30);
	}
	else
	{
		Tooltip[i]->SetPosition(40, -30);
	}

	/*if(Tooltip[i]->GetLeft() < 0)
	{
		Tooltip[i]->SetPosition(10-x-GetLeft(), -30);
	}*/
	
	Buttons[i]->SetVisible(true);
	Buttons[i]->SetPosition(x, y);

	if(Buttons[i]->GetState() == STATE_DISABLED)
		Buttons[i]->SetState(STATE_DEFAULT);
}

void IconEmuBrowser::RemoveButton(int i)
{
	if(i < 0 || i >= (int) Buttons.size())
		return;

	if(SaveBgImg.at(i))
		delete SaveBgImg.at(i);
	SaveBgImg.erase(SaveBgImg.begin()+i);

	if(ButtonImg.at(i))
		delete ButtonImg.at(i);
	ButtonImg.erase(ButtonImg.begin()+i);

	if(FileSelectionImg.at(i))
		delete FileSelectionImg.at(i);
	FileSelectionImg.erase(FileSelectionImg.begin()+i);

	if(Buttons.at(i))
		delete Buttons.at(i);
	Buttons.erase(Buttons.begin()+i);
}

void IconEmuBrowser::SetSelected(int i)
{
	if(i < 0 || i >= PageSize)
		return;

	selectedItem = i;
	Buttons[selectedItem]->SetState(STATE_SELECTED);
}

void IconEmuBrowser::ResetState()
{
	state = STATE_DEFAULT;
	stateChan = -1;
	selectedItem = 0;
	oldSelectedItem = -1;
	SelectTimer.reset();

	for(u32 i=0; i < Buttons.size(); i++)
	{
		Buttons[i]->ResetState();
	}
}

/**
 * Draw the button on screen
 */
void IconEmuBrowser::Draw()
{
	if(!IsVisible())
		return;

	bgSelectionImg->Draw();
	scrollbar->Draw();

	for(u32 i = 0; i < Buttons.size(); i++)
	{
		Buttons[i]->Draw();
	}

	//!needs a redraw for overrendering
	if(parentElement && parentElement->GetState() != STATE_DISABLED)
	{
		if(state == STATE_DISABLED)
			state = STATE_DEFAULT;

		bool tooltip = false;
		if (SelectTimer.elapsed() > 2 && Settings.Tooltips && browser->GetEmuListCount())
		{
			for (int ind = 3; ind >= 0; ind--)
			{
				if (userInput[ind].wpad.ir.valid)
				{
					if (Buttons[selectedItem]->IsInside(userInput[ind].wpad.ir.x, userInput[ind].wpad.ir.y))
					{
						Tooltip[selectedItem]->Draw();
						tooltip = true;
					}
				}
			}
			if(!tooltip)
				SelectTimer.reset();
		}
	}
	else
	{
		if(state == STATE_DEFAULT)
			state = STATE_DISABLED;
	}

	UpdateEffects();
}

void IconEmuBrowser::Update(GuiTrigger * t)
{
	if(state == STATE_DISABLED || !t || !triggerupdate)
		return;
	
	if(!browser->GetEmuListCount())
	{
		for(int i = 0; i < (int)Buttons.size() && listChanged; i++)
			SetButton(i, "", false, 0, 0);
		
		listChanged = false;
		return;
	}
	
	if (oldSelectedItem != selectedItem)
	{
		SelectTimer.reset();
		oldSelectedItem = selectedItem;
	}
	
	scrollbar->Update(t);

	if(scrollbar->ListChanged())
	{
		selectedItem = scrollbar->GetSelectedItem();
		browser->SetPageIndex(scrollbar->GetSelectedIndex());
		listChanged = true;
	}

	if(browser)
		browser->UpdateMarker(t);

	if(t->Right())
	{
		if(browser->GetPageIndex() + selectedItem + 1 < browser->GetEmuListCount())
		{
			if(selectedItem == PageSize-1)
			{
				//! move list down by 1
				browser->SetPageIndex(browser->GetPageIndex()+RowSize);
				Buttons[selectedItem]->ResetState();
				selectedItem = PageSize-RowSize;
				Buttons[selectedItem]->SetState(STATE_SELECTED, t->chan);
				listChanged = true;
			}
			else if(Buttons[selectedItem+1]->IsVisible())
			{
				Buttons[selectedItem]->ResetState();
				Buttons[++selectedItem]->SetState(STATE_SELECTED, t->chan);
			}
		}
	}
	else if(t->Left())
	{
		if(selectedItem == 0 &&	 browser->GetPageIndex() + selectedItem > 0)
		{
			//! move list up by 1
			browser->SetPageIndex(browser->GetPageIndex()-RowSize);
			Buttons[selectedItem]->ResetState();
			selectedItem = RowSize-1;
			Buttons[selectedItem]->SetState(STATE_SELECTED, t->chan);
			listChanged = true;
		}
		else if(selectedItem > 0)
		{
			Buttons[selectedItem]->ResetState();
			Buttons[--selectedItem]->SetState(STATE_SELECTED, t->chan);
		}
	}
	else if(t->Up())
	{
		if(browser->GetPageIndex() + selectedItem - RowSize >= 0)
		{
			Buttons[selectedItem]->ResetState();
			selectedItem = selectedItem-RowSize;
			if(selectedItem < 0)
			{
				//! move list up by 1
				browser->SetPageIndex(browser->GetPageIndex()-RowSize);
				selectedItem = selectedItem+RowSize;
				oldSelectedItem = -1;
			}
			Buttons[selectedItem]->SetState(STATE_SELECTED, t->chan);
			listChanged = true;
		}
	}
	else if(t->Down())
	{
		int i = RowSize;
		while(browser->GetPageIndex()+selectedItem+RowSize >= browser->GetEmuListCount() &&
			  i > 0 && selectedItem > 0 && RowSize < browser->GetEmuListCount())
		{
			--i;
			--selectedItem;
		}
		if(browser->GetPageIndex() + selectedItem + RowSize < browser->GetEmuListCount())
		{
			Buttons[selectedItem]->ResetState();
			selectedItem = selectedItem+RowSize;
			if(selectedItem >= PageSize)
			{
				//! move list down by 1
				browser->SetPageIndex(browser->GetPageIndex()+RowSize);
				selectedItem = selectedItem-RowSize;
				oldSelectedItem = -1;
			}
			Buttons[selectedItem]->SetState(STATE_SELECTED, t->chan);
			listChanged = true;
		}
	}

	//!endNavigation:
	if(numEntries != browser->GetEmuListCount())
	{
		numEntries = browser->GetEmuListCount();
		scrollbar->SetEntrieCount(numEntries);
		listChanged = true;
	}

	u16 x = 12, y = 0;

	for(int i = 0; i < PageSize; i++)
	{
		while(listChanged && browser->IsGettingList())
			usleep(100);
		
		if(listChanged)
		{
			if(browser->GetPageIndex()+i < browser->GetEmuListCount())
			{
				std::string name = browser->GetItemEmuName(browser->GetPageIndex()+i);
				
				SetButton(i, name, true, x, y);
			}
			else
			{
				SetButton(i, "", false, 0, 0);
			}
			x += 90;
			if(x >= (u32) width-80)
			{
				x = 12;
				y += 90;
			}
		}
		
		if(i != selectedItem && Buttons[i]->GetState() == STATE_SELECTED)
			Buttons[i]->ResetState();
		else if(i == selectedItem && Buttons[i]->GetState() == STATE_DEFAULT)
			Buttons[selectedItem]->SetState(STATE_SELECTED, t->chan);
		
		int currChan = t->chan;
		
		if(t->wpad.ir.valid && !Buttons[i]->IsInside(t->wpad.ir.x, t->wpad.ir.y))
			t->chan = -1;
		
		Buttons[i]->Update(t);
		t->chan = currChan;
		
		if(Buttons[i]->GetState() == STATE_CLICKED)
		{
			browser->SetSelectedIndex(browser->GetPageIndex() + i);
			state = STATE_CLICKED;
		}
		
		if(Buttons[i]->GetState() == STATE_SELECTED)
		{
			selectedItem = i;
			browser->SetSelectedIndex(browser->GetPageIndex() + i);
			Tooltip[i]->SetState(STATE_DEFAULT);
		}
		else
		{
			Tooltip[i]->SetState(STATE_DISABLED);
		}
		
		ItemMarker * IMarker = browser->GetItemMarker();
		int itemCount = IMarker->GetItemcount();
		
		for(int n = 0; n < itemCount; n++)
		{
			if(browser->GetPageIndex() + i == IMarker->GetItemIndex(n))
			{
				Buttons[i]->SetState(STATE_SELECTED);
			}
		}
	}

	scrollbar->SetPageSize(PageSize);
	scrollbar->SetRowSize(RowSize);
	scrollbar->SetSelectedItem(selectedItem);
	scrollbar->SetSelectedIndex(browser->GetPageIndex());

	listChanged = false;
}
