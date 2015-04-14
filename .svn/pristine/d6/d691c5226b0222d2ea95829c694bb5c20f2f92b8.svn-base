/***************************************************************************
 * Copyright (C) 2010
 * by Dimok
 * modified by Dj_Skual
 * for SaveGame Manager Gx 2011
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
 * for WiiXplorer 2010
 ***************************************************************************/
#include "IconFileDeviceBrowser.hpp"
#include "../FileOperations/fileops.h"
#include "../Themes/CTheme.h"
#include "../Settings/CSettings.h"
#include "../menu.h"

/**
 * Constructor for the IconFileDeviceBrowser class.
 */
IconFileDeviceBrowser::IconFileDeviceBrowser(Browser * filebrowser, GuiImageData * browserBg, int w, int h)
	: GuiFileBrowser(filebrowser, w, h)
{
	width = w;
	height = h;
	numEntries = 0;
	selectedItem = 0;
	oldSelectedItem = -1;
	browser = filebrowser;
	listChanged = true; // trigger an initial list update
	triggerupdate = true; // trigger disable

	trigA = new SimpleGuiTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	
	bgFileSelectionImg = new GuiImage(browserBg);
	bgFileSelectionImg->SetParent(this);
	
	bgFileSelectionEntry = Resources::GetImageData("browser_entry_bg.png");
	bgSaveSelectionEntry = Resources::GetImageData("browser_entry_save_bg.png");
	bgSaveOverSelectionEntry = Resources::GetImageData("browser_entry_save_over_bg.png");
	fileCfg = Resources::GetImageData("icon_brows_cfg.png");
	fileDataBin = Resources::GetImageData("icon_brows_databin.png");
	fileDefault = Resources::GetImageData("icon_brows_default.png");
	fileExe = Resources::GetImageData("icon_brows_exe.png");
	fileFolder = Resources::GetImageData("icon_brows_folder.png");
	fileFont = Resources::GetImageData("icon_brows_font.png");
	fileImg = Resources::GetImageData("icon_brows_img.png");
	fileLang = Resources::GetImageData("icon_brows_lang.png");
	fileMii = Resources::GetImageData("icon_brows_mii.png");
	fileSound = Resources::GetImageData("icon_brows_sound.png");
	fileTheme = Resources::GetImageData("icon_brows_theme.png");
	fileTxt = Resources::GetImageData("icon_brows_txt.png");
	fileXml = Resources::GetImageData("icon_brows_xml.png");

	scrollbar = new Scrollbar(browserBg->GetHeight()+20, ICONMODE);
	scrollbar->SetParent(this);
	scrollbar->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	scrollbar->SetPosition(4, -10);
	scrollbar->SetScrollSpeed(5);

	RowSize = 6;
	PageSize = 18;
}

/**
 * Destructor for the IconFileDeviceBrowser class.
 */
IconFileDeviceBrowser::~IconFileDeviceBrowser()
{
	HaltGui();
	
	browser = NULL;
	
	delete bgFileSelectionImg;
	
	delete bgFileSelectionEntry;
	delete bgSaveSelectionEntry;
	delete bgSaveOverSelectionEntry;
	delete fileCfg;
	delete fileDataBin;
	delete fileDefault;
	delete fileExe;
	delete fileFolder;
	delete fileFont;
	delete fileImg;
	delete fileLang;
	delete fileMii;
	delete fileSound;
	delete fileTheme;
	delete fileTxt;
	delete fileXml;

	delete scrollbar;

	delete trigA;
	
	for(u32 i = 0; i < Buttons.size(); i++)
	{
		if(Buttons[i])
			delete Buttons[i];
		if(ButtonText[i])
			delete ButtonText[i];
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
	ButtonText.clear();
	ButtonImg.clear();
	Tooltip.clear();
}

GuiSaveIcon * IconFileDeviceBrowser::GetSaveIcon(int i)
{	
	u64 size = 0;
	u8* buffer = NULL;
	
	std::string savepath(browser->GetCurrentPath());
	savepath += "/";
	savepath += browser->GetItemFileName(browser->GetPageIndex()+i);
	savepath += "/banner.bin";
	
	LoadFileToMem(savepath.c_str(), &buffer, &size);
	
	GuiSaveIcon * Icon = new GuiSaveIcon(buffer, (u32)size);
	
	free(buffer);
	buffer = NULL;
	
	return Icon;
}

GuiImage * IconFileDeviceBrowser::GetIconFromExt(std::string fileext, bool dir, bool save, bool databin, bool mii, int i)
{
	if(dir)
		return (new GuiImage(fileFolder));

	if(save)
		return GetSaveIcon(i);

	if(databin)
		return (new GuiImage(fileDataBin));
	
	if(mii)
		return (new GuiImage(fileMii));

	if(fileext.compare(".cfg") == 0)
	{
		return (new GuiImage(fileCfg));
	}
	else if (Settings.FileExtensions.CompareHomebrew(fileext.c_str()) == 0)
	{
		return (new GuiImage(fileExe));
	}
	else if(Settings.FileExtensions.CompareFont(fileext.c_str()) == 0)
	{
		return (new GuiImage(fileFont));
	}
	else if (Settings.FileExtensions.CompareImage(fileext.c_str()) == 0)
	{
		return (new GuiImage(fileImg));
	}
	else if(Settings.FileExtensions.CompareLanguageFiles(fileext.c_str()) == 0)
	{
		return (new GuiImage(fileLang));
	}
	else if(Settings.FileExtensions.CompareAudio(fileext.c_str()) == 0)
	{
		return (new GuiImage(fileSound));
	}
	else if(Settings.FileExtensions.CompareThemeFiles(fileext.c_str()) == 0)
	{
		return (new GuiImage(fileTheme));
	}
	else if(fileext.compare(".log") == 0 ||
			fileext.compare(".txt") == 0)
	{
		return (new GuiImage(fileTxt));
	}
	else if(fileext.compare(".xml") == 0)
	{
		return (new GuiImage(fileXml));
	}
	
	return (new GuiImage(fileDefault));
}

void IconFileDeviceBrowser::AddButton()
{
	//!File Icon
	GuiImage * BtnImg = NULL;
	ButtonImg.push_back(BtnImg);

	//!File Name
	GuiText * BtnTxt = new GuiText((char *) NULL, 14, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	BtnTxt->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	BtnTxt->SetPosition(0, -20);
	BtnTxt->SetLinesToDraw(2);
	BtnTxt->SetMaxWidth(75, WRAP);
	ButtonText.push_back(BtnTxt);

	//!Save bg button
	GuiImage * SaveBg = new GuiImage(bgSaveSelectionEntry);
	SaveBg->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	SaveBg->SetPosition(0, 4);
	SaveBgImg.push_back(SaveBg);
	
	//!selection img
	GuiImage * Marker = NULL;
	FileSelectionImg.push_back(Marker);

	//!tooltip
	GuiTooltip * tmpToolTip = new GuiTooltip((char *) NULL);
	tmpToolTip->SetPosition(0, 0);
	Tooltip.push_back(tmpToolTip);

	GuiButton * Btn = new GuiButton(90, 90);
	Btn->SetParent(this);
	Btn->SetLabel(BtnTxt);
	Btn->SetIcon(BtnImg);
	Btn->SetImage(SaveBg);
	Btn->SetImageOver(Marker);
	Btn->SetTrigger(trigA);
	Btn->SetSoundClick(btnSoundClick);
	Btn->SetToolTip(tmpToolTip, 0, 0, ALIGN_CENTRE, ALIGN_TOP);
	Buttons.push_back(Btn);
}

void IconFileDeviceBrowser::SetButton(int i, std::string name, bool dir, bool save, bool databin, bool mii, bool enable, int x, int y)
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
	if(FileSelectionImg[i])
		delete FileSelectionImg[i];
	
	std::string fileext = name;
	int index = fileext.find_last_of('.');
	if(index > 0)
	{
		fileext = fileext.substr(index);
		if(!fileext.compare("."))
			fileext = "";
	}
	else
		fileext = "";
	
	ButtonImg[i] = GetIconFromExt(fileext, dir, save, databin, mii, i);
	ButtonImg[i]->SetAlignment(ALIGN_CENTRE, save ? ALIGN_MIDDLE : ALIGN_TOP);
	ButtonImg[i]->SetPosition(0, save ? 4 : 10);
	Buttons[i]->SetIcon(ButtonImg[i]);
	
	FileSelectionImg[i] = new GuiImage(save ? bgSaveOverSelectionEntry : bgFileSelectionEntry);
	FileSelectionImg[i]->SetAlignment(ALIGN_CENTRE, save ? ALIGN_MIDDLE : ALIGN_TOP);
	FileSelectionImg[i]->SetPosition(0, save ? 4 : -7);
	Buttons[i]->SetImageOver(FileSelectionImg[i]);
	
	SaveBgImg[i]->SetVisible(save ? true : false);
	
	ButtonText[i]->SetText(save ? " " : name.c_str());
	
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

void IconFileDeviceBrowser::RemoveButton(int i)
{
	if(i < 0 || i >= (int) Buttons.size())
		return;

	if(SaveBgImg.at(i))
		delete SaveBgImg.at(i);
	SaveBgImg.erase(SaveBgImg.begin()+i);

	if(ButtonImg.at(i))
		delete ButtonImg.at(i);
	ButtonImg.erase(ButtonImg.begin()+i);

	if(ButtonText.at(i))
		delete ButtonText.at(i);
	ButtonText.erase(ButtonText.begin()+i);

	if(FileSelectionImg.at(i))
		delete FileSelectionImg.at(i);
	FileSelectionImg.erase(FileSelectionImg.begin()+i);

	if(Buttons.at(i))
		delete Buttons.at(i);
	Buttons.erase(Buttons.begin()+i);
}

void IconFileDeviceBrowser::SetSelected(int i)
{
	if(i < 0 || i >= PageSize)
		return;

	selectedItem = i;
	Buttons[selectedItem]->SetState(STATE_SELECTED);
}

void IconFileDeviceBrowser::ResetState()
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
void IconFileDeviceBrowser::Draw()
{
	if(!IsVisible())
		return;

	bgFileSelectionImg->Draw();
	scrollbar->Draw();

	for(u32 i = 0; i < Buttons.size(); i++)
	{
		Buttons[i]->Draw();
	}

	//needs a redraw for overrendering
	if (parentElement && parentElement->GetState() != STATE_DISABLED)
	{
		if(state == STATE_DISABLED)
			state = STATE_DEFAULT;

		bool tooltip = false;
		if (SelectTimer.elapsed() > 2 && Settings.Tooltips && browser->GetEntrieCount())
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

void IconFileDeviceBrowser::Update(GuiTrigger * t)
{
	if(state == STATE_DISABLED || !t || !triggerupdate || browser->IsGettingList())
		return;

	if(!browser->GetEntrieCount())
	{
		for(int i = 0; i < (int)Buttons.size() && listChanged; i++)
			SetButton(i, "", false, false, false, false, false, 0, 0);
		
		listChanged = false;
		return;
	}
	
	if (oldSelectedItem != selectedItem)
	{
		SelectTimer.reset();
		oldSelectedItem = selectedItem;
	}
	
	scrollbar->Update(t);
	//browser->UpdateMarker(t);
	
	if(scrollbar->ListChanged())
	{
		selectedItem = scrollbar->GetSelectedItem();
		browser->SetPageIndex(scrollbar->GetSelectedIndex());
		listChanged = true;
	}

	if(t->Right())
	{
		if(browser->GetPageIndex() + selectedItem + 1 < browser->GetEntrieCount())
		{
			if(selectedItem == PageSize-1)
			{
				// move list down by 1
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
			// move list up by 1
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
			bool changed = false;
			if(selectedItem < 0)
			{
				// move list up by 1
				browser->SetPageIndex(browser->GetPageIndex()-RowSize);
				selectedItem = selectedItem+RowSize;
				oldSelectedItem = -1;
				changed = true;
			}
			Buttons[selectedItem]->SetState(STATE_SELECTED, t->chan);
			if(changed) listChanged = true;
		}
	}
	else if(t->Down())
	{
		int i = RowSize;
		while(browser->GetPageIndex()+selectedItem+RowSize >= browser->GetEntrieCount() &&
			  i > 0 && selectedItem > 0 && RowSize < browser->GetEntrieCount())
		{
			--i;
			--selectedItem;
		}
		if(browser->GetPageIndex() + selectedItem + RowSize < browser->GetEntrieCount())
		{
			Buttons[selectedItem]->ResetState();
			selectedItem = selectedItem+RowSize;
			bool changed = false;
			if(selectedItem >= PageSize)
			{
				// move list down by 1
				browser->SetPageIndex(browser->GetPageIndex()+RowSize);
				selectedItem = selectedItem-RowSize;
				oldSelectedItem = -1;
				changed = true;
			}
			Buttons[selectedItem]->SetState(STATE_SELECTED, t->chan);
			if(changed) listChanged = true;
		}
	}

	//! Navigation end:
	if((numEntries != browser->GetEntrieCount()) || browser->ListChanged())
	{
		numEntries = browser->GetEntrieCount();
		scrollbar->SetEntrieCount(numEntries);
		listChanged = true;
	}
	
	u16 x = 12, y = 0;

	for(int i = 0; i < PageSize; i++)
	{
		if(listChanged)
		{
			if(browser->GetPageIndex()+i < browser->GetEntrieCount())
			{
				std::string name = browser->GetItemDisplayName(browser->GetPageIndex()+i);
				bool dir = browser->IsDir(browser->GetPageIndex()+i);
				bool save = browser->IsSave(browser->GetPageIndex()+i);
				bool databin = browser->IsDataBin(browser->GetPageIndex()+i);
				bool mii = browser->IsMii(browser->GetPageIndex()+i);
				
				SetButton(i, name, dir, save, databin, mii, true, x, y);
			}
			else
			{
				SetButton(i, "", false, false, false, false, false, 0, 0);
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
			Buttons[selectedItem]->SetState(STATE_SELECTED, -1);

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

		/*ItemMarker * IMarker = browser->GetItemMarker();
		int itemCount = IMarker->GetItemcount();

		for(int n = 0; n < itemCount; n++)
		{
			if(browser->GetPageIndex() + i == IMarker->GetItemIndex(n))
			{
				Buttons[i]->SetState(STATE_SELECTED);
			}
		}*/
	}

	scrollbar->SetPageSize(PageSize);
	scrollbar->SetRowSize(RowSize);
	scrollbar->SetSelectedItem(selectedItem);
	scrollbar->SetSelectedIndex(browser->GetPageIndex());

	listChanged = false;
}
