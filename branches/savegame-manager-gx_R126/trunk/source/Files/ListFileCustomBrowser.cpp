/***************************************************************************
 * Copyright (C) 2010
 * by Dimok
 * modified by Dj_Skual
 * for SaveGame Manager Gx 2011
 *
 * Original ListBrowser by Tantric (C) 2009
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
#include "ListFileCustomBrowser.hpp"
#include "../Settings/CSettings.h"
#include "../Themes/CTheme.h"
#include "../LibWiiGui/gui.h"
#include "../menu.h"

/**
 * Constructor for the ListFileCustomBrowser class.
 */
ListFileCustomBrowser::ListFileCustomBrowser(Browser * filebrowser, int w, int h)
	: GuiFileBrowser(filebrowser, w, h)
{
	width = w;
	height = h;
	selectedItem = 0;
	numEntries = 0;
	browser = filebrowser;
	listChanged = true; // trigger an initial list update
	triggerupdate = true; // trigger disable

	trigA = new SimpleGuiTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);

	bgFileSelectionEntry = Resources::GetImageData("browser_select_entry_bg.png");
	fileCfg = Resources::GetImageData("icon_brows_cfg.png");
	fileDataBin = Resources::GetImageData("icon_brows_databin.png");
	fileDefault = Resources::GetImageData("icon_brows_default.png");
	fileExe = Resources::GetImageData("icon_brows_exe.png");
	fileFolder = Resources::GetImageData("icon_brows_folder.png");
	fileFont = Resources::GetImageData("icon_brows_font.png");
	fileImg = Resources::GetImageData("icon_brows_img.png");
	fileLang = Resources::GetImageData("icon_brows_lang.png");
	fileMii = Resources::GetImageData("icon_brows_mii.png");
	fileSave = Resources::GetImageData("icon_brows_save.png");
	fileSound = Resources::GetImageData("icon_brows_sound.png");
	fileTheme = Resources::GetImageData("icon_brows_theme.png");
	fileTxt = Resources::GetImageData("icon_brows_txt.png");
	fileXml = Resources::GetImageData("icon_brows_xml.png");

	scrollbar = new Scrollbar(264, LISTMODE);
	scrollbar->SetParent(this);
	scrollbar->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	scrollbar->SetPosition(48, 12);
	scrollbar->SetScrollSpeed(5);
}

/**
 * Destructor for the ListFileCustomBrowser class.
 */
ListFileCustomBrowser::~ListFileCustomBrowser()
{
	browser = NULL;
	
	delete bgFileSelectionEntry;
	delete fileCfg;
	delete fileDataBin;
	delete fileDefault;
	delete fileExe;
	delete fileFolder;
	delete fileFont;
	delete fileImg;
	delete fileLang;
	delete fileMii;
	delete fileSave;
	delete fileSound;
	delete fileTheme;
	delete fileTxt;
	delete fileXml;

	delete trigA;

	delete scrollbar;

	while(fileBtn.size() > 0)
		RemoveButton(0);
}

void ListFileCustomBrowser::AddButton()
{
	int size = fileBtn.size();

	fileBtnText.resize(size+1);
	fileBtnText[size] = new GuiText((char*) NULL, 20, thColor("r=0 g=0 b=0 a=255 - select browser entry text color"));
	fileBtnText[size]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	fileBtnText[size]->SetPosition(52,0);
	fileBtnText[size]->SetMaxWidth(width - 55, DOTTED);

	fileBtnTextOver.resize(size+1);
	fileBtnTextOver[size] = new GuiText((char*) NULL, 20, thColor("r=0 g=0 b=0 a=255 - select browser entry text color"));
	fileBtnTextOver[size]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	fileBtnTextOver[size]->SetPosition(52,0);
	fileBtnTextOver[size]->SetMaxWidth(width - 55, SCROLL_HORIZONTAL);

	fileSelectionImg.resize(size+1);
	fileSelectionImg[size] = new GuiImage(bgFileSelectionEntry);

	fileBtnIcon.resize(size+1);
	fileBtnIcon[size] = NULL;
	
	fileBtn.resize(size+1);
	fileBtn[size] = new GuiButton(width,30);
	fileBtn[size]->SetParent(this);
	fileBtn[size]->SetLabel(fileBtnText[size]);
	fileBtn[size]->SetLabelOver(fileBtnTextOver[size]);
	fileBtn[size]->SetImageOver(fileSelectionImg[size]);
	fileBtn[size]->SetPosition(2,30*size+3);
	fileBtn[size]->SetTrigger(trigA);
	fileBtn[size]->SetSoundClick(btnSoundClick);
}

void ListFileCustomBrowser::RemoveButton(int i)
{
	if(i < 0 || i >= (int) fileBtn.size())
		return;

	if(fileBtnText[i])
		delete fileBtnText[i];
	if(fileBtnTextOver[i])
		delete fileBtnTextOver[i];
	if(fileSelectionImg[i])
		delete fileSelectionImg[i];
	if(fileBtnIcon[i])
		delete fileBtnIcon[i];
	if(fileBtn[i])
		delete fileBtn[i];

	fileBtnText.erase(fileBtnText.begin()+i);
	fileBtnTextOver.erase(fileBtnTextOver.begin()+i);
	fileSelectionImg.erase(fileSelectionImg.begin()+i);
	fileBtnIcon.erase(fileBtnIcon.begin()+i);
	fileBtn.erase(fileBtn.begin()+i);
}

void ListFileCustomBrowser::SetButton(int i, std::string name, bool dir, bool save, bool databin, bool enable)
{
	if(i < 0)
		return;

	else if(i >= (int) fileBtn.size())
	{
		AddButton();
	}

	if(!enable)
	{
		fileBtn[i]->SetVisible(false);
		fileBtn[i]->SetState(STATE_DISABLED);
		return;
	}

	if(fileBtnIcon[i])
		delete fileBtnIcon[i];

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
	
	fileBtnIcon[i] = GetIconFromExt(fileext, dir, save, databin);
	fileBtnIcon[i]->SetScale(0.667);
	fileBtnIcon[i]->SetPosition(21,-7);
	
	fileBtnText[i]->SetText(name.c_str());
	fileBtnTextOver[i]->SetText(name.c_str());

	fileBtn[i]->SetIcon(fileBtnIcon[i]);
	fileBtn[i]->SetVisible(true);

	if(fileBtn[i]->GetState() == STATE_DISABLED)
		fileBtn[i]->SetState(STATE_DEFAULT);
}

GuiImage * ListFileCustomBrowser::GetIconFromExt(std::string fileext, bool dir, bool save, bool databin)
{
	if (dir)
		return (new GuiImage(fileFolder));

	if (save)
		return (new GuiImage(fileSave));

	if (databin)
		return (new GuiImage(fileDataBin));

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
	else if(fileext.compare(".miigx") == 0)
	{
		return (new GuiImage(fileMii));
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

void ListFileCustomBrowser::SetSelected(int i)
{
	if(i < 0 || i >= (int) fileBtn.size())
		return;

	selectedItem = i;
	fileBtn[selectedItem]->SetState(STATE_SELECTED);
}

void ListFileCustomBrowser::ResetState()
{
	state = STATE_DEFAULT;
	stateChan = -1;
	selectedItem = 0;

	for(u32 i = 0; i< fileBtn.size(); i++)
	{
		fileBtn[i]->ResetState();
	}
}

/**
 * Draw the button on screen
 */
void ListFileCustomBrowser::Draw()
{
	if(!this->IsVisible())
		return;

	for(u32 i = 0; i < fileBtn.size(); i++)
	{
		fileBtn[i]->Draw();
	}

	scrollbar->Draw();

	UpdateEffects();
}

void ListFileCustomBrowser::Update(GuiTrigger * t)
{
	if(state == STATE_DISABLED || !t || !triggerupdate || browser->IsGettingList())
		return;

	if(!browser->GetEntrieCount())
	{
		for(int i = 0; i < (int)fileBtn.size() && listChanged; i++)
			SetButton(i, "", false, false, false, false);
		
		listChanged = false;
		return;
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
		if(browser->GetPageIndex() < browser->GetEntrieCount() && browser->GetEntrieCount() > PAGESIZE)
		{
			int pageIndex = browser->GetPageIndex() + PAGESIZE;
			if(pageIndex+PAGESIZE >= browser->GetEntrieCount())
				pageIndex = browser->GetEntrieCount()-PAGESIZE;
			browser->SetPageIndex(pageIndex);
			listChanged = true;
		}
	}
	else if(t->Left())
	{
		if(browser->GetPageIndex() > 0)
		{
			int pageIndex = browser->GetPageIndex() - PAGESIZE;
			if(pageIndex < 0)
				pageIndex = 0;
			browser->SetPageIndex(pageIndex);
			listChanged = true;
		}
	}
	else if(t->Down())
	{
		if(browser->GetPageIndex() + selectedItem + 1 < browser->GetEntrieCount())
		{
			if(selectedItem == (int) fileBtn.size()-1)
			{
				// move list down by 1
				browser->SetPageIndex(browser->GetPageIndex()+1);
				listChanged = true;
			}
			else if((int) fileBtn.size() > selectedItem+1 && fileBtn[selectedItem+1]->IsVisible())
			{
				fileBtn[selectedItem]->ResetState();
				fileBtn[++selectedItem]->SetState(STATE_SELECTED, t->chan);
			}
		}
	}
	else if(t->Up())
	{
		if(selectedItem == 0 &&	 browser->GetPageIndex() + selectedItem > 0)
		{
			// move list up by 1
			browser->SetPageIndex(browser->GetPageIndex()-1);
			listChanged = true;
		}
		else if((int) fileBtn.size() > selectedItem && selectedItem > 0)
		{
			fileBtn[selectedItem]->ResetState();
			fileBtn[--selectedItem]->SetState(STATE_SELECTED, t->chan);
		}
	}

	//! Navigation end:
	if(numEntries != browser->GetEntrieCount() || browser->ListChanged())
	{
		numEntries = browser->GetEntrieCount();
		scrollbar->SetEntrieCount(numEntries);
		listChanged = true;
	}

	for(int i = 0; i < PAGESIZE; i++)
	{
		if(listChanged)
		{
			if(browser->GetPageIndex()+i < browser->GetEntrieCount())
			{
				std::string  name = browser->GetItemDisplayName(browser->GetPageIndex()+i);
				bool dir = browser->IsDir(browser->GetPageIndex()+i);
				bool save = browser->IsSave(browser->GetPageIndex()+i);
				bool databin = browser->IsDataBin(browser->GetPageIndex()+i);
				
				SetButton(i, name, dir, save, databin, true);
			}
			else
			{
				SetButton(i, "", false, false, false, false);
			}
		}
		
		if(i != selectedItem && fileBtn[i]->GetState() == STATE_SELECTED)
			fileBtn[i]->ResetState();
		else if(i == selectedItem && fileBtn[i]->GetState() == STATE_DEFAULT)
			fileBtn[selectedItem]->SetState(STATE_SELECTED, -1);
		
		int currChan = t->chan;
		
		if(t->wpad.ir.valid && !fileBtn[i]->IsInside(t->wpad.ir.x, t->wpad.ir.y))
			t->chan = -1;
		
		fileBtn[i]->Update(t);
		t->chan = currChan;
		
		if (fileBtn[i]->GetState() == STATE_CLICKED)
		{
			browser->SetSelectedIndex(browser->GetPageIndex() + i);
			state = STATE_CLICKED;
		}
		
		if(fileBtn[i]->GetState() == STATE_SELECTED)
		{
			selectedItem = i;
			browser->SetSelectedIndex(browser->GetPageIndex() + i);
		}
		
		/*ItemMarker * IMarker = browser->GetItemMarker();
		int itemCount = IMarker->GetItemcount();
		
		for(int n = 0; n < itemCount; n++)
		{
			if(browser->GetPageIndex() + i == IMarker->GetItemIndex(n))
			{
				fileBtn[i]->SetState(STATE_SELECTED);
			}
		}*/
	}

	scrollbar->SetPageSize(PAGESIZE);
	scrollbar->SetRowSize(0);
	scrollbar->SetSelectedItem(selectedItem);
	scrollbar->SetSelectedIndex(browser->GetPageIndex());

	listChanged = false;
}
