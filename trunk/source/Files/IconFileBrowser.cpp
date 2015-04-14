/****************************************************************************
 * Copyright (C) 2010-2011 Dimok
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
#include "IconFileBrowser.hpp"
#include "../Themes/CTheme.h"
#include "../FileOperations/FileExtensions.h"

/**
 * Constructor for the IconFileBrowser class.
 */
IconFileBrowser::IconFileBrowser(Browser * filebrowser, int w, int h)
	: GuiFileBrowser(filebrowser, w, h)
{
	width = w;
	height = h;
	selectedItem = 0;
	browser = filebrowser;
	taskbarSelected = false;
	UpdateDelay = 0.0f;

	trigA = new GuiTrigger;
	trigA->SetSimpleTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);

	btnSoundOver = Resources::GetSound("button_over.wav");
	btnSoundClick = Resources::GetSound("button_click.wav");

	bgFileSelection = Resources::GetImageData("browser_entry_bg.png");
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

	scrollbar = new Scrollbar(height+20, Scrollbar::ICONMODE);
	scrollbar->SetParent(this);
	scrollbar->SetAlignment(ALIGN_RIGHT | ALIGN_TOP);
	scrollbar->SetPosition(4, -10);
	scrollbar->SetScrollSpeed(Settings.ScrollSpeed);
	scrollbar->SetDPadControl(false);
	scrollbar->listChanged.connect(this, &IconFileBrowser::OnListChange);
	scrollbar->SetButtonScroll(WiiControls.OneButtonScroll | ClassicControls.OneButtonScroll << 16, GCControls.OneButtonScroll);

	RowSize = (width-scrollbar->GetWidth())/90;
	PageSize = RowSize*height/90;
	
	TaskBar::Instance()->GetInstance()->TaskbarStateChanged.connect(this, &IconFileBrowser::OnTaskbarStateChange);
}

/**
 * Destructor for the IconFileBrowser class.
 */
IconFileBrowser::~IconFileBrowser()
{
	browser = NULL;
	Resources::Remove(btnSoundOver);
	Resources::Remove(btnSoundClick);
	Resources::Remove(bgFileSelection);
	Resources::Remove(fileCfg);
	Resources::Remove(fileDataBin);
	Resources::Remove(fileDefault);
	Resources::Remove(fileExe);
	Resources::Remove(fileFolder);
	Resources::Remove(fileFont);
	Resources::Remove(fileImg);
	Resources::Remove(fileLang);
	Resources::Remove(fileMii);
	Resources::Remove(fileSound);
	Resources::Remove(fileTheme);
	Resources::Remove(fileTxt);
	Resources::Remove(fileXml);

	delete scrollbar;

	delete trigA;

	while(!Buttons.empty())
		RemoveButton(0);
}

GuiImage * IconFileBrowser::GetIconFromExt(std::string fileext, bool dir)
{
	if(dir)
		return (new GuiImage(fileFolder));

	if(!fileext.empty())
	{
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
	}

	return (new GuiImage(fileDefault));
}

void IconFileBrowser::AddButton()
{
	GuiImage * BtnImg = NULL;
	ButtonImg.push_back(BtnImg);

	GuiText * BtnTxt = new GuiText((char *) NULL, 14, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	BtnTxt->SetAlignment(ALIGN_CENTER | ALIGN_BOTTOM);
	BtnTxt->SetPosition(0, -20);
	BtnTxt->SetLinesToDraw(2);
	BtnTxt->SetMaxWidth(fileDefault->GetWidth()+33, WRAP);
	ButtonText.push_back(BtnTxt);

	//!Save bg button
	/*GuiImage * SaveBg = new GuiImage(bgSaveFileSelection);
	SaveBg->SetAlignment(ALIGN_CENTER, ALIGN_MIDDLE);
	SaveBg->SetPosition(0, 4);
	SaveBgImg.push_back(SaveBg);*/
	
	GuiImage * Marker = NULL;
	/*GuiImage * Marker = new GuiImage(bgFileSelection);
	Marker->SetAlignment(ALIGN_CENTER | ALIGN_TOP);
	Marker->SetPosition(0, -17);*/
	FileSelectionImg.push_back(Marker);

	GuiTooltip * tmpToolTip = new GuiTooltip((char *) NULL);
	tmpToolTip->SetPosition(0, 0);
	Tooltip.push_back(tmpToolTip);

	GuiButton * Btn = new GuiButton(90, 90);
	Btn->SetParent(this);
	Btn->SetLabel(BtnTxt);
	Btn->SetIcon(BtnImg);
	Btn->SetImageOver(Marker);
	Btn->SetTrigger(trigA);
	Btn->SetSoundClick(btnSoundClick);
	Btn->SetTooltip(tmpToolTip);
	Btn->Clicked.connect(this, &IconFileBrowser::OnButtonClicked);
	Buttons.push_back(Btn);
}

void IconFileBrowser::SetButton(int i, std::wstring name, bool dir, bool enable, int x, int y)
{
	if(i < 0)
		return;

	while(i >= (int) Buttons.size())
		AddButton();

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
	
	ButtonImg[i] = GetIconFromExt(fileext, dir);
	ButtonImg[i]->SetAlignment(ALIGN_CENTER | /*save ? ALIGN_MIDDLE : */ALIGN_TOP);
	ButtonImg[i]->SetPosition(0, /*save ? 4 : */10);
	
	FileSelectionImg[i] = new GuiImage(/*save ? bgSaveOverSelectionEntry : */bgFileSelection);
	FileSelectionImg[i]->SetAlignment(ALIGN_CENTER | /*save ? ALIGN_MIDDLE : */ALIGN_TOP);
	FileSelectionImg[i]->SetPosition(0, /*save ? 4 : */-7);
	Buttons[i]->SetImageOver(FileSelectionImg[i]);
	
	//SaveBgImg[i]->SetVisible(save ? true : false);
	
	ButtonText[i]->SetText(/*save ? " " : */name.c_str());
	
	Tooltip[i]->SetText(name.c_str());
	if(40+x+Tooltip[i]->GetWidth() > width)
	{
		Tooltip[i]->SetPosition(width-(40+x+Tooltip[i]->GetWidth()), -30);
	}
	else
	{
		Tooltip[i]->SetPosition(40, -30);
	}

	if(Tooltip[i]->GetLeft() < 0)
	{
		Tooltip[i]->SetPosition(10-x-GetLeft(), -30);
	}

	Buttons[i]->SetIcon(ButtonImg[i]);
	Buttons[i]->SetVisible(true);
	Buttons[i]->SetPosition(x, y);

	if(Buttons[i]->GetState() == STATE_DISABLED)
		Buttons[i]->SetState(STATE_DEFAULT);
}

void IconFileBrowser::RemoveButton(int i)
{
	if(i < 0 || i >= (int) Buttons.size())
		return;

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

void IconFileBrowser::OnButtonClicked(GuiButton *sender, int pointer UNUSED, const POINT &p UNUSED)
{
	for(u32 i = 0; i < Buttons.size(); i++)
	{
		if(Buttons[i] == sender)
		{
			Clicked(i);
			break;
		}
	}
}

void IconFileBrowser::SetSelected(int i)
{
	if(i < 0 || i >= PAGESIZE)
		return;

	selectedItem = i;
	Buttons[selectedItem]->SetState(STATE_SELECTED);
}

void IconFileBrowser::ResetState()
{
	state = STATE_DEFAULT;
	stateChan = -1;
	selectedItem = 0;

	for(u32 i=0; i < Buttons.size(); i++)
		Buttons[i]->ResetState();
}

void IconFileBrowser::OnListChange(int selItem, int selIndex)
{
	scrollbar->SetSelected(selItem, selIndex);
	selectedItem = selItem;
	browser->SetPageIndex(selIndex);
	
	u16 x = 12, y = 0;

	for(int i = 0; i < PageSize; i++)
	{
		if(selIndex+i < browser->GetEntrieCount())
		{
			std::wstring name = browser->GetItemDisplayName(selIndex+i);
			bool dir = browser->IsDir(selIndex+i);
			
			SetButton(i, name, dir, true, x, y);
		}
		else
			SetButton(i, L"", false, false, 0, 0);
		
		x += 90;
		if(x >= (u32) width-80)
		{
			x = 12;
			y += 90;
		}
	}
}

void IconFileBrowser::CheckNavigation(GuiTrigger * t)
{
	if(t->Right())
	{
		if(browser->GetPageIndex() + selectedItem + 1 < browser->GetEntrieCount())
		{
			if(selectedItem == PageSize-1)
			{
				//! move list down by 1
				browser->SetPageIndex(browser->GetPageIndex()+RowSize);
				Buttons[selectedItem]->ResetState();
				selectedItem = PageSize-RowSize;
				Buttons[selectedItem]->SetState(STATE_SELECTED, t->chan);
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
			}
			Buttons[selectedItem]->SetState(STATE_SELECTED, t->chan);
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
			if(selectedItem >= PageSize)
			{
				//! move list down by 1
				browser->SetPageIndex(browser->GetPageIndex()+RowSize);
				selectedItem = selectedItem-RowSize;
			}
			Buttons[selectedItem]->SetState(STATE_SELECTED, t->chan);
		}
	}
}

/**
 * Draw the button on screen
 */
void IconFileBrowser::Draw()
{
	if(!IsVisible())
		return;
	
	scrollbar->Draw();
	
	for(u32 i = 0; i < Buttons.size(); i++)
		Buttons[i]->Draw();
	
	//! Draw tooltips on top of buttons
	for(u32 i = 0; i < Buttons.size(); i++)
		Tooltip[i]->Draw();
	
	UpdateEffects();
}

void IconFileBrowser::Update(GuiTrigger * t)
{
	if(UpdateLockTimer.elapsed() < UpdateDelay || taskbarSelected)
		return;
	
	UpdateDelay = 0.0f;
	
	if(state == STATE_DISABLED || !t || browser->IsGettingList())
		return;

	CheckNavigation(t);
	scrollbar->Update(t);

	//! Updating the rest only makes sense with a browser
	if(!browser)
		return;

	if(browser->ListChanged())
		OnListChange(browser->GetSelIndex(), browser->GetPageIndex());

	//browser->UpdateMarker(t);

	for(int i = 0; i < PageSize && i < (int) Buttons.size(); i++)
	{
		if(i != selectedItem && Buttons[i]->GetState() == STATE_SELECTED)
			Buttons[i]->ResetState();
		else if(i == selectedItem && Buttons[i]->GetState() == STATE_DEFAULT)
			Buttons[selectedItem]->SetState(STATE_SELECTED, t->chan);

		int currChan = t->chan;

		if(t->wpad->ir.valid && !Buttons[i]->IsInside(t->wpad->ir.x, t->wpad->ir.y))
			t->chan = -1;

		Buttons[i]->Update(t);
		t->chan = currChan;

		if(Buttons[i]->GetState() == STATE_SELECTED)
		{
			selectedItem = i;
			browser->SetSelectedIndex(browser->GetPageIndex() + i);
			Tooltip[i]->SetState(STATE_DEFAULT);
		}
		else
			Tooltip[i]->SetState(STATE_DISABLED);

		/*ItemMarker * IMarker = browser->GetItemMarker();
		int itemCount = IMarker->GetItemcount();

		for(int n = 0; n < itemCount; n++)
		{
			if(browser->GetPageIndex() + i == IMarker->GetItemIndex(n))
			{
				Buttons[i]->SetState(STATE_SELECTED);
				break;
			}
		}*/
	}

	scrollbar->SetEntrieCount(browser->GetEntrieCount());
	scrollbar->SetPageSize(PageSize);
	scrollbar->SetRowSize(RowSize);
	scrollbar->SetSelected(selectedItem, browser->GetPageIndex());
}

void IconFileBrowser::OnTaskbarStateChange(TaskBar * taskbar UNUSED, int State UNUSED)
{
	if(State == STATE_SELECTED)
	{
		taskbarSelected = true;
		for(int i = 0; i < PageSize && i < (int) Buttons.size(); i++)
			Buttons[i]->ResetState();
	}
	else
	{
		for(int i = 0; i < PageSize && i < (int) Buttons.size(); i++)
			Buttons[i]->ResetTooltipTimer();
		
		UpdateLock(0.3);
		taskbarSelected = false;
	}
}

void IconFileBrowser::UpdateLock(float delay)
{
	UpdateDelay = delay;
	UpdateLockTimer.reset();
}
