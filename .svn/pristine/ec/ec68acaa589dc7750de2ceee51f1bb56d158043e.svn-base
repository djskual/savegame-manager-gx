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
#include "ListEmuBrowser.hpp"
#include "../EmuNand/CEmuList.hpp"
#include "../Settings/CSettings.h"
#include "../Tools/StringTools.h"
#include "../Saves/SaveTools.h"
#include "../Language/gettext.h"
#include "../Themes/CTheme.h"
#include "../FileOperations/fileops.h"
#include "../LibWiiGui/gui_banner.h"
#include "../menu.h"

/**
 * Constructor for the ListEmuBrowser class.
 */
ListEmuBrowser::ListEmuBrowser(Browser * emubrowser, GuiImageData * browserBg, int w, int h)
	: GuiFileBrowser(emubrowser, w, h)
{
	width = w;
	height = h;
	selectedItem = 0;
	oldSelectedItem = -1;
	oldPageIndex = 0;
	numEntries = 0;
	type = TYPE_UNKNOW;
	browser = emubrowser;
	listChanged = true; //! trigger an initial list update
	triggerupdate = true; //! trigger disable

	trigA = new SimpleGuiTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);

	bgSaveSelectionImg = new GuiImage(browserBg);
	bgSaveSelectionImg->SetParent(this);
	
	browserSeparator = Resources::GetImageData("browser_separator.png");
	browserSeparatorImg = new GuiImage(browserSeparator);
	browserSeparatorImg->SetParent(this);
	browserSeparatorImg->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	browserSeparatorImg->SetPosition(384, 0);
	
	bgSaveSelectionEntry = Resources::GetImageData("browser_options_entry_bg.png");
	fileSave = Resources::GetImageData("icon_brows_save.png");
	
	type_Wii = Resources::GetImageData("icon_type_wii.png");
	type_Wiiware = Resources::GetImageData("icon_type_wiiware.png");
	type_Gamecube = Resources::GetImageData("icon_type_gamecube.png");
	type_VC_NES = Resources::GetImageData("icon_type_vc_nes.png");
	type_VC_SNES = Resources::GetImageData("icon_type_vc_snes.png");
	type_VC_N64 = Resources::GetImageData("icon_type_vc_n64.png");
	type_VC_SMS = Resources::GetImageData("icon_type_vc_sms.png");
	type_VC_MD = Resources::GetImageData("icon_type_vc_md.png");
	type_VC_PCE = Resources::GetImageData("icon_type_vc_pce.png");
	type_VC_Neogeo = Resources::GetImageData("icon_type_vc_neogeo.png");
	type_VC_Arcade = Resources::GetImageData("icon_type_vc_arcade.png");
	type_VC_C64 = Resources::GetImageData("icon_type_vc_c64.png");
	type_Channel = Resources::GetImageData("icon_type_channel.png");
	type_Unknow = Resources::GetImageData("icon_type_unknow.png");
	
	scrollbar = new Scrollbar(browserBg->GetHeight()+20, OPTIONMODE);
	scrollbar->SetParent(this);
	scrollbar->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	scrollbar->SetPosition(4, -10);
	scrollbar->SetScrollSpeed(5);
	
	infosWindow =  new GuiWindow(width-386-13, height);
	infosWindow->SetParent(this);
	infosWindow->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	infosWindow->SetPosition(-13, 0);	
	
	infoIconImg = new GuiImage(fileSave);
	infoTypeImg = new GuiImage(type_Unknow);
	infoNameTxt = new GuiText((char*) NULL, 17, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	infoSubnameTxt = new GuiText((char*) NULL, 15, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	infoIDTxt = new GuiText((char*) NULL, 16, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	infoSizeTxt = new GuiText((char*) NULL, 16, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	pathTxt = new GuiText((char*) NULL, 16, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	infoPathTxt = new GuiText((char*) NULL, 16, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
}

/**
 * Destructor for the ListEmuBrowser class.
 */
ListEmuBrowser::~ListEmuBrowser()
{
	browser = NULL;
	
	delete bgSaveSelectionImg;
	delete browserSeparatorImg;
	
	delete infosWindow;
	delete infoIconImg;
	delete infoTypeImg;
	delete infoNameTxt;
	delete infoSubnameTxt;
	delete infoIDTxt;
	delete infoSizeTxt;
	delete pathTxt;
	delete infoPathTxt;
	
	delete bgSaveSelectionEntry;
	delete browserSeparator;
	delete fileSave;
	
	delete  type_Wii;
	delete  type_Wiiware;
	delete  type_Gamecube;
	delete  type_VC_NES;
	delete  type_VC_SNES;
	delete  type_VC_N64;
	delete  type_VC_SMS;
	delete  type_VC_MD;
	delete  type_VC_PCE;
	delete  type_VC_Neogeo;
	delete  type_VC_Arcade;
	delete  type_VC_C64;
	delete  type_Channel;
	delete  type_Unknow;

	delete trigA;

	delete scrollbar;

	while(saveBtn.size() > 0)
		RemoveButton(0);
}

void ListEmuBrowser::AddButton()
{
	int size = saveBtn.size();

	saveSelectionImg.resize(size+1);
	saveSelectionImg[size] = new GuiImage(bgSaveSelectionEntry);

	saveBtnText.resize(size+1);
	saveBtnText[size] = new GuiText((char*) NULL, 20, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	saveBtnText[size]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	saveBtnText[size]->SetPosition(52,0);
	saveBtnText[size]->SetMaxWidth(saveSelectionImg[size]->GetWidth() - 55, DOTTED);

	saveBtnTextOver.resize(size+1);
	saveBtnTextOver[size] = new GuiText((char*) NULL, 20, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	saveBtnTextOver[size]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	saveBtnTextOver[size]->SetPosition(52,0);
	saveBtnTextOver[size]->SetMaxWidth(saveSelectionImg[size]->GetWidth() - 55, SCROLL_HORIZONTAL);

	saveBtnIcon.resize(size+1);
	saveBtnIcon[size] = new GuiImage(fileSave);;
	saveBtnIcon[size]->SetScale(0.667);
	saveBtnIcon[size]->SetPosition(21,-7);
	
	saveBtn.resize(size+1);
	saveBtn[size] = new GuiButton(saveSelectionImg[size]->GetWidth(),30);
	saveBtn[size]->SetParent(this);
	saveBtn[size]->SetLabel(saveBtnText[size]);
	saveBtn[size]->SetLabelOver(saveBtnTextOver[size]);
	saveBtn[size]->SetIcon(saveBtnIcon[size]);
	saveBtn[size]->SetImageOver(saveSelectionImg[size]);
	saveBtn[size]->SetPosition(2,30*size+3);
	saveBtn[size]->SetTrigger(trigA);
	saveBtn[size]->SetSoundClick(btnSoundClick);
}

void ListEmuBrowser::RemoveButton(int i)
{
	if(i < 0 || i >= (int) saveBtn.size())
		return;

	if(saveBtnText[i])
		delete saveBtnText[i];
	if(saveBtnTextOver[i])
		delete saveBtnTextOver[i];
	if(saveSelectionImg[i])
		delete saveSelectionImg[i];
	if(saveBtnIcon[i])
		delete saveBtnIcon[i];
	if(saveBtn[i])
		delete saveBtn[i];

	saveBtnText.erase(saveBtnText.begin()+i);
	saveBtnTextOver.erase(saveBtnTextOver.begin()+i);
	saveSelectionImg.erase(saveSelectionImg.begin()+i);
	saveBtnIcon.erase(saveBtnIcon.begin()+i);
	saveBtn.erase(saveBtn.begin()+i);
}

void ListEmuBrowser::SetButton(int i, std::string name, bool enable)
{
	if(i < 0)
		return;

	else if(i >= (int) saveBtn.size())
	{
		AddButton();
	}

	if(!enable)
	{
		saveBtn[i]->SetVisible(false);
		saveBtn[i]->SetState(STATE_DISABLED);
		return;
	}

	saveBtnText[i]->SetText(name.c_str());
	saveBtnTextOver[i]->SetText(name.c_str());

	saveBtn[i]->SetVisible(true);

	if(saveBtn[i]->GetState() == STATE_DISABLED)
		saveBtn[i]->SetState(STATE_DEFAULT);
}

GuiImage * ListEmuBrowser::GetTypeIcon()
{
	switch (type)
	{
		case TYPE_WII:
			return (new GuiImage(type_Wii));
		case TYPE_WIIWARE:
			return (new GuiImage(type_Wiiware));
		case TYPE_GAMECUBE:
			return (new GuiImage(type_Gamecube));
		case TYPE_VC_NES:
			return (new GuiImage(type_VC_NES));
		case TYPE_VC_SNES:
			return (new GuiImage(type_VC_SNES));
		case TYPE_VC_N64:
			return (new GuiImage(type_VC_N64));
		case TYPE_VC_SMS:
			return (new GuiImage(type_VC_SMS));
		case TYPE_VC_MD:
			return (new GuiImage(type_VC_MD));
		case TYPE_VC_PCE:
			return (new GuiImage(type_VC_PCE));
		case TYPE_VC_NEOGEO:
			return (new GuiImage(type_VC_Neogeo));
		case TYPE_VC_ARCADE:
			return (new GuiImage(type_VC_Arcade));
		case TYPE_VC_C64:
			return (new GuiImage(type_VC_C64));
		case TYPE_CHANNEL:
			return (new GuiImage(type_Channel));
		case TYPE_UNKNOW:
		default:
			return (new GuiImage(type_Unknow));
	}
	
	return (new GuiImage(type_Unknow));
}

void ListEmuBrowser::FoundSaveType()
{
	type = TYPE_UNKNOW;
	
	std::string ID = browser->GetItemEmuID(browser->GetPageIndex()+selectedItem);
	if(ID.empty())
		return;
	
	std::string id(ID);
	std::string id2(ID);
	id.erase(1);
	id2.erase(2);
	
	if(!id.compare("R") || !id.compare("S") || !id.compare("D"))
		type = TYPE_WII;
	else if(!id.compare("W"))
		type = TYPE_WIIWARE;
	else if(!id.compare("G") || !id.compare("P") || !id.compare("D") || !id.compare("U"))
		type = TYPE_GAMECUBE;
	else if(!id.compare("F"))
		type = TYPE_VC_NES;
	else if(!id.compare("J"))
		type = TYPE_VC_SNES;
	else if(!id.compare("N"))
		type = TYPE_VC_N64;
	else if(!id.compare("L"))
		type = TYPE_VC_SMS;
	else if(!id.compare("M"))
		type = TYPE_VC_MD;
	else if(!id.compare("P"))
		type = TYPE_VC_PCE;
	else if(!id.compare("E"))
	{
		if(!id2.compare("EA") || !id2.compare("EB"))
			type = TYPE_VC_NEOGEO;
		else
			type = TYPE_VC_ARCADE;
	}
	else if(!id.compare("C"))
		type = TYPE_VC_C64;
	else if(!id.compare("H") || !id2.compare("JO"))
		type = TYPE_CHANNEL;
}

GuiImage * ListEmuBrowser::GetSaveIcon()
{	
	int i = browser->GetPageIndex() + selectedItem;
	std::string savepath = fmt("%s/title/%08x/%08x/data/banner.bin", Settings.EmuNandPath, (u32)(EmuList.GetTid(i) >> 32), (u32)(EmuList.GetTid(i) & 0xFFFFFFFF));
	
	u8* buffer = NULL;
	u64 size = 0;
	LoadFileToMem(savepath, &buffer, &size);
	
	GuiSaveIcon * Icon = new GuiSaveIcon(buffer, size);
	
	free(buffer);
	buffer = NULL;
	
	return Icon;
}

void ListEmuBrowser::SetSelected(int i)
{
	if(i < 0 || i >= (int) saveBtn.size())
		return;

	selectedItem = i;
	saveBtn[selectedItem]->SetState(STATE_SELECTED);
}

void ListEmuBrowser::ResetState()
{
	state = STATE_DEFAULT;
	stateChan = -1;
	selectedItem = 0;
	oldSelectedItem = -1;
	oldPageIndex = 0;
	type = TYPE_UNKNOW;
	
	for(u32 i = 0; i< saveBtn.size(); i++)
	{
		saveBtn[i]->ResetState();
	}
}

void ListEmuBrowser::SetSelectedInfos()
{
	if((oldSelectedItem == selectedItem) && (oldPageIndex == browser->GetPageIndex()))
		return;
	
	oldSelectedItem = selectedItem;
	oldPageIndex = browser->GetPageIndex();
	
	infosWindow->RemoveAll();
	if (infoIconImg)
		delete infoIconImg;
	if (infoTypeImg)
		delete infoTypeImg;
	if (infoNameTxt)
		delete infoNameTxt;
	if (infoSubnameTxt)
		delete infoSubnameTxt;
	if (infoIDTxt)
		delete infoIDTxt;
	if (infoSizeTxt)
		delete infoSizeTxt;
	if (pathTxt)
		delete pathTxt;
	if (infoPathTxt)
		delete infoPathTxt;
	infoIconImg = NULL;
	infoTypeImg = NULL;
	infoNameTxt = NULL;
	infoSubnameTxt = NULL;
	infoIDTxt = NULL;
	infoSizeTxt = NULL;
	pathTxt = NULL;
	infoPathTxt = NULL;
	
	int y = 18;
	int txtmaxwidth = infosWindow->GetWidth()-25;
	
	FoundSaveType();
	infoTypeImg = new GuiImage(GetTypeIcon());
	infoTypeImg->SetParent(infosWindow);
	infoTypeImg->SetWidescreen(Settings.Widescreen);
	infoTypeImg->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	infoTypeImg->SetPosition(-20, y);
	y += 2;
	
	infoIconImg = new GuiImage(fileSave);
	infoIconImg->SetParent(infosWindow);
	infoIconImg = GetSaveIcon();
	infoIconImg->SetWidescreen(Settings.Widescreen);
	infoIconImg->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	infoIconImg->SetPosition(10, y);	
	y += infoIconImg->GetHeight()+10;
	
	infoNameTxt = new GuiText(browser->GetItemEmuName(browser->GetPageIndex()+selectedItem), 17, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	infoNameTxt->SetParent(infosWindow);
	infoNameTxt->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	infoNameTxt->SetPosition(10, y);
	infoNameTxt->SetMaxWidth(txtmaxwidth, SCROLL_HORIZONTAL);
	y += 21;
	
	infoSubnameTxt = new GuiText(browser->GetItemEmuSubname(browser->GetPageIndex()+selectedItem), 15, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	infoSubnameTxt->SetParent(infosWindow);
	infoSubnameTxt->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	infoSubnameTxt->SetPosition(10, y);
	infoSubnameTxt->SetMaxWidth(txtmaxwidth, DOTTED);
	y += 28;
	
	std::string ID = tr( "ID:" );
	ID += " ";
	ID += browser->GetItemEmuID(browser->GetPageIndex()+selectedItem);
	infoIDTxt = new GuiText(ID, 16, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	infoIDTxt->SetParent(infosWindow);
	infoIDTxt->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	infoIDTxt->SetPosition(10, y);
	infoIDTxt->SetMaxWidth(txtmaxwidth, DOTTED);
	y += 22;
	
	std::string size = tr( "Size:" );
	size += fmt(" %0.f %s", ceil(browser->GetItemEmuBlocks(browser->GetPageIndex()+selectedItem)), tr( "Block(s)" ));
	infoSizeTxt = new GuiText(size, 16, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	infoSizeTxt->SetParent(infosWindow);
	infoSizeTxt->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	infoSizeTxt->SetPosition(10, y);
	infoSizeTxt->SetMaxWidth(txtmaxwidth, DOTTED);
	y += 22;
	
	int Y = -40;
	infoPathTxt = new GuiText(GetCurrentPath(), 16, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
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
	infosWindow->Append(infoTypeImg);
	infosWindow->Append(infoIDTxt);
	infosWindow->Append(infoNameTxt);
	infosWindow->Append(infoSubnameTxt);
	infosWindow->Append(infoSizeTxt);
	infosWindow->Append(pathTxt);
	infosWindow->Append(infoPathTxt);
}

std::string ListEmuBrowser::GetCurrentPath()
{
	std::string CurrentPath;
	
	u64 tid = browser->GetItemEmuTid(browser->GetPageIndex()+selectedItem);
	CurrentPath = fmt("%s/title/%08x/%08x/data", Settings.EmuNandPath, (u32)(tid >> 32), (u32)(tid & 0xFFFFFFFF));
	
	return CurrentPath;
}

/**
 * Draw the button on screen
 */
void ListEmuBrowser::Draw()
{
	if(!this->IsVisible())
		return;
	
	bgSaveSelectionImg->Draw();
	browserSeparatorImg->Draw();
	infosWindow->Draw();
	scrollbar->Draw();
	
	for(u32 i = 0; i < saveBtn.size(); i++)
	{
		saveBtn[i]->Draw();
	}
	
	UpdateEffects();
}

void ListEmuBrowser::Update(GuiTrigger * t)
{
	if(state == STATE_DISABLED || !t || !triggerupdate)
		return;

	if(!browser->GetEmuListCount())
	{
		for(int i = 0; i < (int)saveBtn.size() && listChanged; i++)
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
		if(browser->GetPageIndex() < browser->GetEmuListCount() && browser->GetEmuListCount() > PAGESIZE)
		{
			int pageIndex = browser->GetPageIndex() + PAGESIZE;
			if(pageIndex+PAGESIZE >= browser->GetEmuListCount())
				pageIndex = browser->GetEmuListCount()-PAGESIZE;
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
		if(browser->GetPageIndex() + selectedItem + 1 < browser->GetEmuListCount())
		{
			if(selectedItem == (int) saveBtn.size()-1)
			{
				//! move list down by 1
				browser->SetPageIndex(browser->GetPageIndex()+1);
				listChanged = true;
			}
			else if((int) saveBtn.size() > selectedItem+1 && saveBtn[selectedItem+1]->IsVisible())
			{
				saveBtn[selectedItem]->ResetState();
				saveBtn[++selectedItem]->SetState(STATE_SELECTED, t->chan);
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
		else if((int) saveBtn.size() > selectedItem && selectedItem > 0)
		{
			saveBtn[selectedItem]->ResetState();
			saveBtn[--selectedItem]->SetState(STATE_SELECTED, t->chan);
		}
	}

	if(numEntries != browser->GetEmuListCount())
	{
		numEntries = browser->GetEmuListCount();
		scrollbar->SetEntrieCount(numEntries);
		listChanged = true;
	}

	for(int i = 0; i < PAGESIZE; i++)
	{
		while(listChanged && browser->IsGettingList())
			usleep(100);
		
		if(listChanged)
		{
			if(browser->GetPageIndex()+i < browser->GetEmuListCount())
			{
				std::string  name = browser->GetItemEmuName(browser->GetPageIndex()+i);
				
				SetButton(i, name, true);
			}
			else
				SetButton(i, "", false);
		}
		
		if(i != selectedItem && saveBtn[i]->GetState() == STATE_SELECTED)
			saveBtn[i]->ResetState();
		else if(i == selectedItem && saveBtn[i]->GetState() == STATE_DEFAULT)
			saveBtn[selectedItem]->SetState(STATE_SELECTED, -1);
		
		int currChan = t->chan;
		
		if(t->wpad.ir.valid && !saveBtn[i]->IsInside(t->wpad.ir.x, t->wpad.ir.y))
			t->chan = -1;
		
		saveBtn[i]->Update(t);
		t->chan = currChan;
		
		if(saveBtn[i]->GetState() == STATE_SELECTED)
		{
			selectedItem = i;
			browser->SetSelectedIndex(browser->GetPageIndex() + i);
		}
		
		if (saveBtn[i]->GetState() == STATE_CLICKED)
			state = STATE_CLICKED;
		
		ItemMarker * IMarker = browser->GetItemMarker();
		int itemCount = IMarker->GetItemcount();
		
		for(int n = 0; n < itemCount; n++)
		{
			if(browser->GetPageIndex() + i == IMarker->GetItemIndex(n))
				saveBtn[i]->SetState(STATE_SELECTED);
		}
	}

	scrollbar->SetPageSize(PAGESIZE);
	scrollbar->SetRowSize(0);
	scrollbar->SetSelectedItem(selectedItem);
	scrollbar->SetSelectedIndex(browser->GetPageIndex());

	listChanged = false;
}
