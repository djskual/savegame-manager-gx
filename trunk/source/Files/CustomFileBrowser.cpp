/***************************************************************************
 * Copyright (C) 2010
 * by Dimok
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
#include "CustomFileBrowser.hpp"
#include "../Themes/CTheme.h"

/**
 * Constructor for the CustomFileBrowser class.
 */
CustomFileBrowser::CustomFileBrowser(Browser * filebrowser, int w, int h)
	: GuiFileBrowser(filebrowser, w, h)
{
	width = w;
	height = h;
	selectedItem = 0;
	numEntries = 0;
	browser = filebrowser;

	trigA = new GuiTrigger;
	trigA->SetSimpleTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);

	btnSoundOver = Resources::GetSound("button_over.wav");
	btnSoundClick = Resources::GetSound("button_click.wav");

	bgFileSelectionEntry = Resources::GetImageData("browser_bg_selection.png");
	fileFolder = Resources::GetImageData("icon_brows_folder.png");
	fileFont = Resources::GetImageData("icon_brows_font.png");
	fileLang = Resources::GetImageData("icon_brows_lang.png");
	
	scrollbar = new Scrollbar(height-10, Scrollbar::CUSTOMMODE);
	scrollbar->SetParent(this);
	scrollbar->SetAlignment(ALIGN_RIGHT | ALIGN_BOTTOM);
	scrollbar->SetPosition(50, 0);
	scrollbar->SetScrollSpeed(Settings.ScrollSpeed);
	scrollbar->listChanged.connect(this, &CustomFileBrowser::OnListChange);
	scrollbar->SetButtonScroll(WiiControls.OneButtonScroll | ClassicControls.OneButtonScroll << 16, GCControls.OneButtonScroll);
	
	PageSize = height/30;
}

/**
 * Destructor for the CustomFileBrowser class.
 */
CustomFileBrowser::~CustomFileBrowser()
{
	browser = NULL;
	
	Resources::Remove(btnSoundOver);
	Resources::Remove(btnSoundClick);
	
	Resources::Remove(bgFileSelectionEntry);
	Resources::Remove(fileFolder);
	Resources::Remove(fileFont);
	Resources::Remove(fileLang);
	
	delete trigA;

	delete scrollbar;
	
	while(!fileBtn.empty())
		RemoveButton(0);
}

void CustomFileBrowser::AddButton()
{
	int size = fileBtn.size();

	fileSelectionImg.resize(size+1);
	fileSelectionImg[size] = new GuiImage(bgFileSelectionEntry);
	fileSelectionImg[size]->SetScaleX(0.8125);
	fileSelectionImg[size]->SetPosition(-36, 0);

	fileBtnText.resize(size+1);
	fileBtnText[size] = new GuiText((char*) NULL, 20, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	fileBtnText[size]->SetAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
	fileBtnText[size]->SetPosition(30,0);
	fileBtnText[size]->SetMaxWidth(width - 30, DOTTED);

	fileBtnTextOver.resize(size+1);
	fileBtnTextOver[size] = new GuiText((char*) NULL, 20, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	fileBtnTextOver[size]->SetAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
	fileBtnTextOver[size]->SetPosition(30,0);
	fileBtnTextOver[size]->SetMaxWidth(width - 40, SCROLL_HORIZONTAL);

	fileBtnIcon.resize(size+1);
	fileBtnIcon[size] = NULL;

	fileBtn.resize(size+1);
	fileBtn[size] = new GuiButton(width,30);
	fileBtn[size]->SetParent(this);
	fileBtn[size]->SetLabel(fileBtnText[size], 0);
	fileBtn[size]->SetLabelOver(fileBtnTextOver[size]);
	fileBtn[size]->SetImageOver(fileSelectionImg[size]);
	fileBtn[size]->SetPosition(0,30*size+1);
	fileBtn[size]->SetTrigger(trigA);
	fileBtn[size]->SetSoundClick(btnSoundClick);
	fileBtn[size]->Clicked.connect(this, &CustomFileBrowser::OnClicked);
}

void CustomFileBrowser::RemoveButton(int i)
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

void CustomFileBrowser::SetButton(int i, std::wstring name, bool dir, bool enable)
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

	std::string fileext = wstr2str(name);
	int index = fileext.find_last_of('.');
	if(index > 0)
	{
		fileext = fileext.substr(index);
		if(!fileext.compare("."))
			fileext = "";
	}
	else
		fileext = "";
	
	fileBtnIcon[i] = GetIconFromExt(fileext, dir);
	fileBtnIcon[i]->SetScale(0.667);
	fileBtnIcon[i]->SetPosition(-7, -7);
	
	fileBtnText[i]->SetText(name);
	fileBtnTextOver[i]->SetText(name);

	fileBtn[i]->SetIcon(fileBtnIcon[i]);
	fileBtn[i]->SetVisible(true);

	if(fileBtn[i]->GetState() == STATE_DISABLED)
		fileBtn[i]->SetState(STATE_DEFAULT);
}

GuiImage * CustomFileBrowser::GetIconFromExt(std::string fileext, bool dir)
{
	if(dir)
		return (new GuiImage(fileFolder));

	if(!fileext.empty())
	{
		if(Settings.FileExtensions.CompareFont(fileext.c_str()) == 0)
		{
			return (new GuiImage(fileFont));
		}
		else if(Settings.FileExtensions.CompareLanguageFiles(fileext.c_str()) == 0)
		{
			return (new GuiImage(fileLang));
		}
	}

	return (new GuiImage(fileFont));
}

void CustomFileBrowser::OnClicked(GuiButton *sender, int pointer UNUSED, const POINT &p UNUSED)
{
	for(u32 i = 0; i < fileBtn.size(); i++)
	{
		if(fileBtn[i] == sender)
		{
			Clicked(i);
			break;
		}
	}
}

void CustomFileBrowser::SetSelected(int i)
{
	if(i < 0 || i >= (int) fileBtn.size())
		return;

	selectedItem = i;
	fileBtn[selectedItem]->SetState(STATE_SELECTED);
}

void CustomFileBrowser::ResetState()
{
	state = STATE_DEFAULT;
	stateChan = -1;
	selectedItem = 0;

	for(u32 i = 0; i< fileBtn.size(); i++)
	{
		fileBtn[i]->ResetState();
	}
}

void CustomFileBrowser::OnListChange(int selItem, int selIndex)
{
	scrollbar->SetSelected(selItem, selIndex);
	selectedItem = selItem;
	browser->SetPageIndex(selIndex);

	for(int i = 0; i < PageSize; i++)
	{
		if(selIndex+i < browser->GetEntrieCount())
		{
			std::wstring name = browser->GetItemDisplayName(selIndex+i);
			bool dir = browser->IsDir(selIndex+i);
			
			SetButton(i, name, dir, true);
		}
		else
			SetButton(i, L"", false, false);
	}
}

/**
 * Draw the button on screen
 */
void CustomFileBrowser::Draw()
{
	if(!this->IsVisible())
		return;

	scrollbar->Draw();

	for(u32 i = 0; i < fileBtn.size(); i++)
		fileBtn[i]->Draw();

	UpdateEffects();
}

void CustomFileBrowser::Update(GuiTrigger * t)
{
	if(state == STATE_DISABLED || !t || browser->IsGettingList())
		return;

	scrollbar->Update(t);

	//! Updating the rest only makes sense with a browser
	if(!browser)
		return;

	if(numEntries != browser->GetEntrieCount())
	{
		numEntries = browser->GetEntrieCount();
		scrollbar->SetEntrieCount(numEntries);
	}

	if(browser->ListChanged())
		OnListChange(browser->GetSelIndex(), browser->GetPageIndex());

	for(int i = 0; i < (int) fileBtn.size(); i++)
	{
		if(i != selectedItem && fileBtn[i]->GetState() == STATE_SELECTED)
			fileBtn[i]->ResetState();
		else if(i == selectedItem && fileBtn[i]->GetState() == STATE_DEFAULT)
			fileBtn[selectedItem]->SetState(STATE_SELECTED, t->chan);

		int currChan = t->chan;

		if(t->wpad->ir.valid && !fileBtn[i]->IsInside(t->wpad->ir.x, t->wpad->ir.y))
			t->chan = -1;

		fileBtn[i]->Update(t);
		t->chan = currChan;

		if(fileBtn[i]->GetState() == STATE_SELECTED)
		{
			selectedItem = i;
			browser->SetSelectedIndex(browser->GetPageIndex() + i);
		}
	}

	scrollbar->SetPageSize(fileBtn.size());
	scrollbar->SetRowSize(0);
	scrollbar->SetSelected(selectedItem, browser->GetPageIndex());
}
