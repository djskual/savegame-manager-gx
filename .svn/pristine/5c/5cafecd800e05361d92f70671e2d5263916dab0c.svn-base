/***************************************************************************
 * Copyright (C) 2010
 * by Dimok
 * modified by Dj_Skual
 * for SaveGame Manager Gx 2012
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
#include "ListEmuMiiBrowser.hpp"
#include "../Tools/StringTools.h"
#include "../Language/gettext.h"
#include "../Settings/CSettings.h"
#include "../Themes/CTheme.h"

/**
 * Constructor for the ListEmuMiiBrowser class.
 */
ListEmuMiiBrowser::ListEmuMiiBrowser(Browser * filebrowser, GuiImageData * browserBg, int w, int h)
	: GuiFileBrowser(filebrowser, w, h)
{
	width = w;
	height = h;
	selectedItem = 0;
	oldSelectedItem = -1;
	oldPageIndex = 0;
	numEntries = 0;
	browser = filebrowser;
	listChanged = true; //! trigger an initial list update
	triggerupdate = true; //! trigger disable

	trigA = new SimpleGuiTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);

	bgMiiSelectionImg = new GuiImage(browserBg);
	bgMiiSelectionImg->SetParent(this);
	
	browserSeparator = Resources::GetImageData("browser_separator.png");
	browserSeparatorImg = new GuiImage(browserSeparator);
	browserSeparatorImg->SetParent(this);
	browserSeparatorImg->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	browserSeparatorImg->SetPosition(384, 0);
	
	bgMiiSelectionEntry = Resources::GetImageData("browser_options_entry_bg.png");
	fileMii = Resources::GetImageData("icon_brows_mii.png");
	maleImgData = Resources::GetImageData("icon_mii_male.png");
	femaleImgData = Resources::GetImageData("icon_mii_female.png");
	
	scrollbar = new Scrollbar(browserBg->GetHeight()+20, OPTIONMODE);
	scrollbar->SetParent(this);
	scrollbar->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	scrollbar->SetPosition(4, -10);
	scrollbar->SetScrollSpeed(5);
	
	infosWindow =  new GuiWindow(width-386-13, height);
	infosWindow->SetParent(this);
	infosWindow->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	infosWindow->SetPosition(-13, 0);	
	
	infoIconImg = new GuiImage(fileMii);
	infoNameTxt = new GuiText((char*) NULL, 17, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	infoCreatorTxt = new GuiText((char*) NULL, 16, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	infoDateTxt = new GuiText((char*) NULL, 16, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	infoGenderImg = new GuiImage(24, 24, (GXColor) {0, 0, 0, 0});
	infoColorImg = new GuiImage(fileMii);
	pathTxt = new GuiText((char*) NULL, 16, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	infoPathTxt = new GuiText((char*) NULL, 16, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
}

/**
 * Destructor for the ListEmuMiiBrowser class.
 */
ListEmuMiiBrowser::~ListEmuMiiBrowser()
{
	browser = NULL;
	
	delete bgMiiSelectionImg;
	delete browserSeparatorImg;
	
	delete infosWindow;
	delete infoIconImg;
	delete infoNameTxt;
	delete infoCreatorTxt;
	delete infoDateTxt;
	delete infoGenderImg;
	delete infoColorImg;
	delete pathTxt;
	delete infoPathTxt;
	
	delete bgMiiSelectionEntry;
	delete browserSeparator;
	delete fileMii;
	
	delete trigA;

	delete scrollbar;

	while(miiBtn.size() > 0)
		RemoveButton(0);
}

void ListEmuMiiBrowser::AddButton()
{
	int size = miiBtn.size();

	miiSelectionImg.resize(size+1);
	miiSelectionImg[size] = new GuiImage(bgMiiSelectionEntry);

	miiBtnText.resize(size+1);
	miiBtnText[size] = new GuiText((char*) NULL, 20, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	miiBtnText[size]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	miiBtnText[size]->SetPosition(52,0);
	miiBtnText[size]->SetMaxWidth(miiSelectionImg[size]->GetWidth() - 55, DOTTED);

	miiBtnTextOver.resize(size+1);
	miiBtnTextOver[size] = new GuiText((char*) NULL, 20, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	miiBtnTextOver[size]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	miiBtnTextOver[size]->SetPosition(52,0);
	miiBtnTextOver[size]->SetMaxWidth(miiSelectionImg[size]->GetWidth() - 55, SCROLL_HORIZONTAL);

	miiBtnIcon.resize(size+1);
	miiBtnIcon[size] = new GuiImage(fileMii);;
	miiBtnIcon[size]->SetScale(0.667);
	miiBtnIcon[size]->SetPosition(21,-7);
	
	miiBtn.resize(size+1);
	miiBtn[size] = new GuiButton(miiSelectionImg[size]->GetWidth(),30);
	miiBtn[size]->SetParent(this);
	miiBtn[size]->SetLabel(miiBtnText[size]);
	miiBtn[size]->SetLabelOver(miiBtnTextOver[size]);
	miiBtn[size]->SetIcon(miiBtnIcon[size]);
	miiBtn[size]->SetImageOver(miiSelectionImg[size]);
	miiBtn[size]->SetPosition(2,30*size+3);
	miiBtn[size]->SetTrigger(trigA);
	miiBtn[size]->SetSoundClick(btnSoundClick);
}

void ListEmuMiiBrowser::RemoveButton(int i)
{
	if(i < 0 || i >= (int) miiBtn.size())
		return;

	if(miiBtnText[i])
		delete miiBtnText[i];
	if(miiBtnTextOver[i])
		delete miiBtnTextOver[i];
	if(miiSelectionImg[i])
		delete miiSelectionImg[i];
	if(miiBtnIcon[i])
		delete miiBtnIcon[i];
	if(miiBtn[i])
		delete miiBtn[i];

	miiBtnText.erase(miiBtnText.begin()+i);
	miiBtnTextOver.erase(miiBtnTextOver.begin()+i);
	miiSelectionImg.erase(miiSelectionImg.begin()+i);
	miiBtnIcon.erase(miiBtnIcon.begin()+i);
	miiBtn.erase(miiBtn.begin()+i);
}

void ListEmuMiiBrowser::SetButton(int i, std::string name, bool enable)
{
	if(i < 0)
		return;

	else if(i >= (int) miiBtn.size())
	{
		AddButton();
	}

	if(!enable)
	{
		miiBtn[i]->SetVisible(false);
		miiBtn[i]->SetState(STATE_DISABLED);
		return;
	}

	miiBtnText[i]->SetText(name.c_str());
	miiBtnTextOver[i]->SetText(name.c_str());

	miiBtn[i]->SetVisible(true);

	if(miiBtn[i]->GetState() == STATE_DISABLED)
		miiBtn[i]->SetState(STATE_DEFAULT);
}

void ListEmuMiiBrowser::SetSelected(int i)
{
	if(i < 0 || i >= (int) miiBtn.size())
		return;

	selectedItem = i;
	miiBtn[selectedItem]->SetState(STATE_SELECTED);
}

void ListEmuMiiBrowser::ResetState()
{
	state = STATE_DEFAULT;
	stateChan = -1;
	selectedItem = 0;
	oldSelectedItem = -1;
	oldPageIndex = 0;
	
	for(u32 i = 0; i< miiBtn.size(); i++)
	{
		miiBtn[i]->ResetState();
	}
}

std::string ListEmuMiiBrowser::GetDate()
{
	std::string Date =  "..";
	
	if (browser->GetItemEmuMiiDay(browser->GetPageIndex()+selectedItem) != 0)
		Date = fmt("%d", browser->GetItemEmuMiiDay(browser->GetPageIndex()+selectedItem));
	Date += " / ";
	
	switch (browser->GetItemEmuMiiMonth(browser->GetPageIndex()+selectedItem))
	{
		case 0: { Date += ".."; break; }
		case 1: { Date += tr("January"); break; }
		case 2: { Date += tr("February"); break; }
		case 3: { Date += tr("March"); break; }
		case 4: { Date += tr("April"); break; }
		case 5: { Date += tr("May"); break; }
		case 6: { Date += tr("June"); break; }
		case 7: { Date += tr("July"); break; }
		case 8: { Date += tr("August"); break; }
		case 9: { Date += tr("September"); break; }
		case 10: { Date += tr("October"); break; }
		case 11: { Date += tr("November"); break; }
		case 12: { Date += tr("December"); break; }
	}
	
	return Date;
}

GXColor ListEmuMiiBrowser::GetFavColor()
{
	GXColor color = (GXColor) {0, 0, 0, 0};
	
	switch (browser->GetItemEmuMiiFavColor(browser->GetPageIndex()+selectedItem))
	{
		case 0: { color = (GXColor) {255, 0, 0, 255}; break; }
		case 1: { color = (GXColor) {255, 150, 0, 255}; break; }
		case 2: { color = (GXColor) {255, 255, 0, 255}; break; }
		case 3: { color = (GXColor) {140, 255, 0, 255}; break; }
		case 4: { color = (GXColor) {0, 130, 0, 255}; break; }
		case 5: { color = (GXColor) {0, 100, 255, 255}; break; }
		case 6: { color = (GXColor) {0, 200, 255, 255}; break; }
		case 7: { color = (GXColor) {255, 50, 140, 255}; break; }
		case 8: { color = (GXColor) {190, 0, 255, 255}; break; }
		case 9: { color = (GXColor) {140, 80, 45, 255}; break; }
		case 10: { color = (GXColor) {255, 255, 255, 255}; break; }
		case 11: { color = (GXColor) {0, 0, 0, 255}; break; }
	}
	
	return color;
}

void ListEmuMiiBrowser::SetSelectedInfos()
{
	if((oldSelectedItem == selectedItem) && (oldPageIndex == browser->GetPageIndex()))
		return;
	
	oldSelectedItem = selectedItem;
	oldPageIndex = browser->GetPageIndex();
	
	infosWindow->RemoveAll();
	if (infoIconImg)
		delete infoIconImg;
	if (infoNameTxt)
		delete infoNameTxt;
	if (infoCreatorTxt)
		delete infoCreatorTxt;
	if (infoDateTxt)
		delete infoDateTxt;
	if (infoGenderImg)
		delete infoGenderImg;
	if (infoColorImg)
		delete infoColorImg;
	if (pathTxt)
		delete pathTxt;
	if (infoPathTxt)
		delete infoPathTxt;
	infoIconImg = NULL;
	infoNameTxt = NULL;
	infoCreatorTxt = NULL;
	infoDateTxt = NULL;
	infoGenderImg = NULL;
	infoColorImg = NULL;
	pathTxt = NULL;
	infoPathTxt = NULL;
	
	int y = 20;
	int txtmaxwidth = infosWindow->GetWidth()-25;
	
	infoIconImg = new GuiImage(fileMii);
	infoIconImg->SetParent(infosWindow);
	infoIconImg->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	infoIconImg->SetScale(1.5);
	infoIconImg->SetPosition(0, y);	
	y += infoIconImg->GetHeight()+10;
	
	infoNameTxt = new GuiText(browser->GetItemEmuMiiName(browser->GetPageIndex()+selectedItem), 17, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	infoNameTxt->SetParent(infosWindow);
	infoNameTxt->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	infoNameTxt->SetPosition(10, y);
	infoNameTxt->SetMaxWidth(txtmaxwidth, SCROLL_HORIZONTAL);
	y += 35;
	
	std::string Creator = tr( "Creator:" );
	Creator += " ";
	if(browser->GetItemEmuMiiCreator(browser->GetPageIndex()+selectedItem).size())
		Creator += browser->GetItemEmuMiiCreator(browser->GetPageIndex()+selectedItem);
	else
		Creator += "...";
	infoCreatorTxt = new GuiText(Creator, 16, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	infoCreatorTxt->SetParent(infosWindow);
	infoCreatorTxt->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	infoCreatorTxt->SetPosition(10, y);
	infoCreatorTxt->SetMaxWidth(txtmaxwidth, DOTTED);
	y += 22;
	
	std::string Date = tr( "Date:" );
	Date += " ";
	Date += GetDate();
	infoDateTxt = new GuiText(Date, 16, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	infoDateTxt->SetParent(infosWindow);
	infoDateTxt->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	infoDateTxt->SetPosition(10, y);
	infoDateTxt->SetMaxWidth(txtmaxwidth, DOTTED);
	y += 22;
	
	infoGenderImg = new GuiImage(browser->GetItemEmuMiiFemale(browser->GetPageIndex()+selectedItem) ? femaleImgData : maleImgData);
	infoGenderImg->SetParent(infosWindow);
	infoGenderImg->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	infoGenderImg->SetPosition(10, y);	
	
	infoColorImg = new GuiImage(24, 24, GetFavColor());
	infoColorImg->SetParent(infosWindow);
	infoColorImg->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	infoColorImg->SetPosition(10+24+24, y);
	
	int Y = -40;
	std::string miipath = Settings.EmuNandPath;
	miipath += "/shared2/menu/FaceLib/RFL_DB.dat";
	infoPathTxt = new GuiText(miipath, 16, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	infoPathTxt->SetParent(infosWindow);
	infoPathTxt->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	infoPathTxt->SetPosition(10, Y);
	infoPathTxt->SetLinesToDraw(3);
	infoPathTxt->SetInterline(2);
	infoPathTxt->SetMaxWidth(txtmaxwidth, WRAP);
	
	Y -= 18;
	pathTxt = new GuiText(tr( "Path:" ), 16, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	pathTxt->SetParent(infosWindow);
	pathTxt->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	pathTxt->SetPosition(10, Y);
	
	infosWindow->Append(infoIconImg);
	infosWindow->Append(infoNameTxt);
	infosWindow->Append(infoCreatorTxt);
	infosWindow->Append(infoDateTxt);
	infosWindow->Append(infoGenderImg);
	infosWindow->Append(infoColorImg);
	infosWindow->Append(pathTxt);
	infosWindow->Append(infoPathTxt);
}

/**
 * Draw the button on screen
 */
void ListEmuMiiBrowser::Draw()
{
	if(!this->IsVisible())
		return;
	
	bgMiiSelectionImg->Draw();
	browserSeparatorImg->Draw();
	infosWindow->Draw();
	scrollbar->Draw();
	
	for(u32 i = 0; i < miiBtn.size(); i++)
	{
		miiBtn[i]->Draw();
	}
	
	UpdateEffects();
}

void ListEmuMiiBrowser::Update(GuiTrigger * t)
{
	if(state == STATE_DISABLED || !t || !triggerupdate)
		return;

	if(!browser->GetEmuMiiCount())
	{
		for(int i = 0; i < (int)miiBtn.size() && listChanged; i++)
			SetButton(i, "", false);
		
		if(infosWindow)
			infosWindow->SetVisible(false);
		listChanged = false;
		return;
	}
	
	if(infosWindow && !infosWindow->IsVisible())
		infosWindow->SetVisible(true);
	
	scrollbar->Update(t);

	if(scrollbar->ListChanged())
	{
		selectedItem = scrollbar->GetSelectedItem();
		browser->SetPageIndex(scrollbar->GetSelectedIndex());
		listChanged = true;
	}

	if(browser)
		browser->UpdateMarker(t);
	
	SetSelectedInfos();

	if(t->Right())
	{
		if(browser->GetPageIndex() < browser->GetEmuMiiCount() && browser->GetEmuMiiCount() > PAGESIZE)
		{
			int pageIndex = browser->GetPageIndex() + PAGESIZE;
			if(pageIndex+PAGESIZE >= browser->GetEmuMiiCount())
				pageIndex = browser->GetMiiCount()-PAGESIZE;
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
		if(browser->GetPageIndex() + selectedItem + 1 < browser->GetEmuMiiCount())
		{
			if(selectedItem == (int) miiBtn.size()-1)
			{
				//! move list down by 1
				browser->SetPageIndex(browser->GetPageIndex()+1);
				listChanged = true;
			}
			else if((int) miiBtn.size() > selectedItem+1 && miiBtn[selectedItem+1]->IsVisible())
			{
				miiBtn[selectedItem]->ResetState();
				miiBtn[++selectedItem]->SetState(STATE_SELECTED, t->chan);
			}
		}
	}
	else if(t->Up())
	{
		if(selectedItem == 0 &&	 browser->GetPageIndex() + selectedItem > 0)
		{
			//! move list up by 1
			browser->SetPageIndex(browser->GetPageIndex()-1);
			listChanged = true;
		}
		else if((int) miiBtn.size() > selectedItem && selectedItem > 0)
		{
			miiBtn[selectedItem]->ResetState();
			miiBtn[--selectedItem]->SetState(STATE_SELECTED, t->chan);
		}
	}

	if(numEntries != browser->GetEmuMiiCount())
	{
		numEntries = browser->GetEmuMiiCount();
		scrollbar->SetEntrieCount(numEntries);
		listChanged = true;
	}

	for(int i = 0; i < PAGESIZE; i++)
	{
		while(listChanged && browser->IsGettingList())
			usleep(100);
		
		if(listChanged)
		{
			if(browser->GetPageIndex()+i < browser->GetEmuMiiCount())
			{
				std::string  name = browser->GetItemEmuMiiName(browser->GetPageIndex()+i);
				
				SetButton(i, name, true);
			}
			else
				SetButton(i, "", false);
		}
		
		if(i != selectedItem && miiBtn[i]->GetState() == STATE_SELECTED)
			miiBtn[i]->ResetState();
		else if(i == selectedItem && miiBtn[i]->GetState() == STATE_DEFAULT)
			miiBtn[selectedItem]->SetState(STATE_SELECTED, -1);
		
		int currChan = t->chan;
		
		if(t->wpad.ir.valid && !miiBtn[i]->IsInside(t->wpad.ir.x, t->wpad.ir.y))
			t->chan = -1;
		
		miiBtn[i]->Update(t);
		t->chan = currChan;
		
		if(miiBtn[i]->GetState() == STATE_SELECTED)
		{
			selectedItem = i;
			browser->SetSelectedIndex(browser->GetPageIndex() + i);
		}
		
		if (miiBtn[i]->GetState() == STATE_CLICKED)
			state = STATE_CLICKED;
		
		ItemMarker * IMarker = browser->GetItemMarker();
		int itemCount = IMarker->GetItemcount();
		
		for(int n = 0; n < itemCount; n++)
		{
			if(browser->GetPageIndex() + i == IMarker->GetItemIndex(n))
				miiBtn[i]->SetState(STATE_SELECTED);
		}
	}

	scrollbar->SetPageSize(PAGESIZE);
	scrollbar->SetRowSize(0);
	scrollbar->SetSelectedItem(selectedItem);
	scrollbar->SetSelectedIndex(browser->GetPageIndex());

	listChanged = false;
}
