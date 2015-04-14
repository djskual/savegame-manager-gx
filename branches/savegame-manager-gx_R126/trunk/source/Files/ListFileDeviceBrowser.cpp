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
#include "ListFileDeviceBrowser.hpp"
#include "../FileOperations/fileops.h"
#include "../Settings/CSettings.h"
#include "../Tools/StringTools.h"
#include "../Saves/SaveTools.h"
#include "../Saves/DataBin/Crypto/aes.h"
#include "../Saves/DataBin/DataBinTools.h"
#include "../Language/gettext.h"
#include "../Themes/CTheme.h"
#include "../LibWiiGui/gui_banner.h"
#include "../LibWiiGui/gui.h"
#include "../menu.h"

/**
 * Constructor for the ListFileDeviceBrowser class.
 */
ListFileDeviceBrowser::ListFileDeviceBrowser(Browser * filebrowser, GuiImageData * browserBg, int w, int h)
	: GuiFileBrowser(filebrowser, w, h)
{
	width = w;
	height = h;
	selectedItem = 0;
	oldSelectedItem = -1;
	oldPageIndex = 0;
	numEntries = 0;
	databintid = 0;
	type = TYPE_UNKNOW;
	browser = filebrowser;
	listChanged = true; //! trigger an initial list update
	triggerupdate = true; //! trigger disable

	trigA = new SimpleGuiTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);

	bgFileSelectionImg = new GuiImage(browserBg);
	bgFileSelectionImg->SetParent(this);
	
	browserSeparator = Resources::GetImageData("browser_separator.png");
	browserSeparatorImg = new GuiImage(browserSeparator);
	browserSeparatorImg->SetParent(this);
	browserSeparatorImg->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	browserSeparatorImg->SetPosition(384, 0);
	
	bgFileSelectionEntry = Resources::GetImageData("browser_options_entry_bg.png");
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
	
	infoIconImg = new GuiImage(fileDefault);
	infoTypeImg = new GuiImage(type_Unknow);
	infoNameTxt = new GuiText((char*) NULL, 16, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	infoTypeTxt = new GuiText((char*) NULL, 16, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	infoSizeTxt = new GuiText((char*) NULL, 16, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	pathTxt = new GuiText((char*) NULL, 16, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	infoPathTxt = new GuiText((char*) NULL, 16, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
}

/**
 * Destructor for the ListFileDeviceBrowser class.
 */
ListFileDeviceBrowser::~ListFileDeviceBrowser()
{
	browser = NULL;
	
	delete bgFileSelectionImg;
	delete browserSeparatorImg;
	
	delete infosWindow;
	delete infoIconImg;
	delete infoTypeImg;
	delete infoNameTxt;
	delete infoTypeTxt;
	delete infoSizeTxt;
	delete pathTxt;
	delete infoPathTxt;
	
	delete bgFileSelectionEntry;
	delete browserSeparator;
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

	while(fileBtn.size() > 0)
		RemoveButton(0);
}

void ListFileDeviceBrowser::AddButton()
{
	int size = fileBtn.size();

	fileSelectionImg.resize(size+1);
	fileSelectionImg[size] = new GuiImage(bgFileSelectionEntry);

	fileBtnText.resize(size+1);
	fileBtnText[size] = new GuiText((char*) NULL, 20, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	fileBtnText[size]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	fileBtnText[size]->SetPosition(52,0);
	fileBtnText[size]->SetMaxWidth(fileSelectionImg[size]->GetWidth() - 55, DOTTED);

	fileBtnTextOver.resize(size+1);
	fileBtnTextOver[size] = new GuiText((char*) NULL, 20, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	fileBtnTextOver[size]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	fileBtnTextOver[size]->SetPosition(52,0);
	fileBtnTextOver[size]->SetMaxWidth(fileSelectionImg[size]->GetWidth() - 55, SCROLL_HORIZONTAL);

	fileBtnIcon.resize(size+1);
	fileBtnIcon[size] = NULL;
	
	fileBtn.resize(size+1);
	fileBtn[size] = new GuiButton(fileSelectionImg[size]->GetWidth(),30);
	fileBtn[size]->SetParent(this);
	fileBtn[size]->SetLabel(fileBtnText[size]);
	fileBtn[size]->SetLabelOver(fileBtnTextOver[size]);
	fileBtn[size]->SetImageOver(fileSelectionImg[size]);
	fileBtn[size]->SetPosition(2,30*size+3);
	fileBtn[size]->SetTrigger(trigA);
	fileBtn[size]->SetSoundClick(btnSoundClick);
}

void ListFileDeviceBrowser::RemoveButton(int i)
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

void ListFileDeviceBrowser::SetButton(int i, std::string name, bool dir, bool save, bool databin, bool mii, bool enable)
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
	
	fileBtnIcon[i] = GetIconFromExt(fileext, dir, save, databin, mii);
	fileBtnIcon[i]->SetScale(0.667);
	fileBtnIcon[i]->SetPosition(21,-7);
	
	fileBtnText[i]->SetText(name.c_str());
	fileBtnTextOver[i]->SetText(name.c_str());

	fileBtn[i]->SetIcon(fileBtnIcon[i]);
	fileBtn[i]->SetVisible(true);

	if(fileBtn[i]->GetState() == STATE_DISABLED)
		fileBtn[i]->SetState(STATE_DEFAULT);
}

GuiImage * ListFileDeviceBrowser::GetTypeIcon()
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

void ListFileDeviceBrowser::FoundSaveType()
{
	type = TYPE_UNKNOW;
	
	u64 tid = 0;
	if(browser->IsSave(browser->GetPageIndex()+selectedItem))
		tid = StrToHex64(browser->GetItemFileName(browser->GetPageIndex()+selectedItem).c_str());
	else if(browser->IsDataBin(browser->GetPageIndex()+selectedItem) && databintid)
		tid = databintid;
	if(!tid)
		return;
	
	std::string ID = titleText((u32)(tid>> 32),(u32)(tid & 0xFFFFFFFF));
	if(ID.empty())
		return;
	
	type = 0;
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

GuiImage * ListFileDeviceBrowser::GetSaveIcon()
{
	u64 size = 0;
	u8* buffer = NULL;
	std::string savepath(browser->GetCurrentSelectedFilepath());
	savepath += "/banner.bin";
	
	LoadFileToMem(savepath.c_str(), &buffer, &size);
	
	GuiSaveIcon * Icon = new GuiSaveIcon(buffer, (u32)size);

	return Icon;
}

GuiImage * ListFileDeviceBrowser::GetDataBinIcon()
{
	u64 stuff_size = 0;
	u8 *stuff = NULL;
	LoadFileToMem(browser->GetCurrentSelectedFilepath().c_str(), &stuff, &stuff_size);
	
	if( stuff_size < 0xf140 ) //!header + backup header
	{
		//gprintf("\n\tListFileDeviceBrowser::GetDataBinIcon -> size is too small");
		return NULL;
	}
	
	u32 size = 0xf0c0;
	u8 header[size];
	memset(header, '\0', size);

	//!decrypt the header
	u8 iv[ 16 ] = SD_IV;
	u8 sdkey[ 16 ] = SD_KEY;
	aes_set_key( sdkey );
	aes_decrypt( iv, stuff, header, size );
	
	databintid = be64(header);
	
	u32 bnrSize = be32(header + 8);
	u8* banner = (u8*)malloc(bnrSize);
	memcpy(banner, header + 0x20, bnrSize);
	
	GuiSaveIcon * Icon = new GuiSaveIcon(banner, bnrSize);
	
	if(stuff) free(stuff);
	
	return Icon;
}

GuiImage * ListFileDeviceBrowser::GetIconFromExt(std::string fileext, bool dir, bool save, bool databin, bool mii)
{
	if (dir)
		return (new GuiImage(fileFolder));

	if (save)
		return (new GuiImage(fileSave));

	if (databin)
		return (new GuiImage(fileDataBin));

	if (mii)
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

std::string ListFileDeviceBrowser::GetTypeFromExt(std::string fileext, bool dir, bool save, bool databin, bool mii)
{
	if (dir)
		return tr( "Folder" );

	if (save)
		return tr( "SaveGame" );

	if (databin)
		return tr( "Data.bin" );

	if (mii)
		return tr( "Mii" );

	if (fileext.compare(".cfg") == 0)
		return tr( "Config" );
	else if (Settings.FileExtensions.CompareHomebrew(fileext.c_str()) == 0)
		return tr( "Homebrew" );
	else if (Settings.FileExtensions.CompareFont(fileext.c_str()) == 0)
		return tr( "Font" );
	else if (Settings.FileExtensions.CompareImage(fileext.c_str()) == 0)
		return tr( "Picture" );
	else if(Settings.FileExtensions.CompareLanguageFiles(fileext.c_str()) == 0)
		return tr( "Language" );
	else if(Settings.FileExtensions.CompareAudio(fileext.c_str()) == 0)
		return tr( "Audio" );
	else if(Settings.FileExtensions.CompareThemeFiles(fileext.c_str()) == 0)
		return tr( "Theme" );
	else if (fileext.compare(".log") == 0 || fileext.compare(".txt") == 0)
		return tr( "Text" );
	else if (fileext.compare(".xml") == 0)
		return tr( "XML" );
	
	return tr( "File" );
}

void ListFileDeviceBrowser::SetSelected(int i)
{
	if(i < 0 || i >= (int) fileBtn.size())
		return;

	selectedItem = i;
	fileBtn[selectedItem]->SetState(STATE_SELECTED);
}

void ListFileDeviceBrowser::ResetState()
{
	state = STATE_DEFAULT;
	stateChan = -1;
	selectedItem = 0;
	oldSelectedItem = -1;
	oldPageIndex = 0;
	databintid = 0;
	type = TYPE_UNKNOW;
	
	for(u32 i = 0; i< fileBtn.size(); i++)
	{
		fileBtn[i]->ResetState();
	}
}

void ListFileDeviceBrowser::SetSelectedInfos()
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
	if (infoTypeTxt)
		delete infoTypeTxt;
	if (infoSizeTxt)
		delete infoSizeTxt;
	if (pathTxt)
		delete pathTxt;
	if (infoPathTxt)
		delete infoPathTxt;
	infoIconImg = NULL;
	infoTypeImg = NULL;
	infoNameTxt = NULL;
	infoTypeTxt = NULL;
	infoSizeTxt = NULL;
	pathTxt = NULL;
	infoPathTxt = NULL;
	
	bool dir = browser->IsDir(browser->GetPageIndex()+selectedItem);
	bool save = browser->IsSave(browser->GetPageIndex()+selectedItem);
	bool databin = browser->IsDataBin(browser->GetPageIndex()+selectedItem);
	bool mii = browser->IsMii(browser->GetPageIndex()+selectedItem);
	
	std::string fileext = "";
	if(!save && !mii && !dir && !databin)
	{
		fileext = browser->GetItemDisplayName(browser->GetPageIndex()+selectedItem);
		int index = fileext.find_last_of('.');
		if(index > 0)
			fileext = fileext.substr(index);
	}
	
	int y = 20;
	int txtmaxwidth = infosWindow->GetWidth()-25;
	
	infoIconImg = new GuiImage();
	infoIconImg->SetParent(infosWindow);
	if(!save && !databin) infoIconImg = GetIconFromExt(fileext, dir, save, databin, mii);
	else if(databin) infoIconImg = GetDataBinIcon();
	else infoIconImg = GetSaveIcon();
	infoIconImg->SetWidescreen(Settings.Widescreen);
	infoIconImg->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	infoIconImg->SetPosition((save || databin) ? 10 : 0, y);	
	if(!save && !databin) infoIconImg->SetScale(1.5);
	y += infoIconImg->GetHeight()+10;
	
	if(save || databin) FoundSaveType();
	infoTypeImg = new GuiImage(GetTypeIcon());
	infoTypeImg->SetWidescreen(Settings.Widescreen);
	infoTypeImg->SetParent(infosWindow);
	infoTypeImg->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	infoTypeImg->SetPosition(-20, 18);	
	
	infoNameTxt = new GuiText(browser->GetItemDisplayName(browser->GetPageIndex()+selectedItem), 16, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	infoNameTxt->SetParent(infosWindow);
	infoNameTxt->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	infoNameTxt->SetPosition(10, y);
	infoNameTxt->SetMaxWidth(txtmaxwidth, SCROLL_HORIZONTAL);
	y += 30;
	
	std::string type = tr( "Type:" );
	type += " ";
	type += GetTypeFromExt(fileext, dir, save, databin, mii);
	infoTypeTxt = new GuiText(type, 16, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	infoTypeTxt->SetParent(infosWindow);
	infoTypeTxt->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	infoTypeTxt->SetPosition(10, y);
	infoTypeTxt->SetMaxWidth(txtmaxwidth, DOTTED);
	y += 22;
	
	std::string size = tr( "Size:" );
	size += " ";
	if(!dir)
	{
		u64 foldersize = 0;
		u32 filenumber = 0;
		if(save)
			GetFolderSize(browser->GetCurrentSelectedFilepath().c_str(), &foldersize, &filenumber);
		else
			foldersize = FileSize(browser->GetCurrentSelectedFilepath().c_str());
		
		if (foldersize < MB_SIZE)
			size += fmt("%0.2fKB", foldersize / KB_SIZE);
		else if (foldersize > MB_SIZE && foldersize < GB_SIZE)
			size += fmt("%0.2fMB", foldersize / MB_SIZE);
		else
			size += fmt("%0.2fGB", foldersize / GB_SIZE);
	}
	else
		size += tr( "Unknown" );
	
	infoSizeTxt = new GuiText(size, 16, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	infoSizeTxt->SetParent(infosWindow);
	infoSizeTxt->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	infoSizeTxt->SetPosition(10, y);
	infoSizeTxt->SetMaxWidth(txtmaxwidth, DOTTED);
	y += 22;
	
	int Y = -40;
	infoPathTxt = new GuiText(browser->GetCurrentPath(), 16, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
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
	if(save || databin) infosWindow->Append(infoTypeImg);
	infosWindow->Append(infoNameTxt);
	infosWindow->Append(infoTypeTxt);
	infosWindow->Append(infoSizeTxt);
	infosWindow->Append(pathTxt);
	infosWindow->Append(infoPathTxt);
}

/**
 * Draw the button on screen
 */
void ListFileDeviceBrowser::Draw()
{
	if(!this->IsVisible())
		return;
	
	bgFileSelectionImg->Draw();
	browserSeparatorImg->Draw();
	infosWindow->Draw();
	scrollbar->Draw();
	
	for(u32 i = 0; i < fileBtn.size(); i++)
	{
		fileBtn[i]->Draw();
	}
	
	UpdateEffects();
}

void ListFileDeviceBrowser::Update(GuiTrigger * t)
{
	if(state == STATE_DISABLED || !t || !triggerupdate || browser->IsGettingList())
		return;

	if(!browser->GetEntrieCount())
	{
		for(int i = 0; i < (int)fileBtn.size() && listChanged; i++)
			SetButton(i, "", false, false, false, false, false);
		
		if(infosWindow)
			infosWindow->SetVisible(false);
		listChanged = false;
		return;
	}
	
	if(infosWindow && !infosWindow->IsVisible())
		infosWindow->SetVisible(true);
	
	scrollbar->Update(t);
	//browser->UpdateMarker(t);
	
	if(scrollbar->ListChanged())
	{
		selectedItem = scrollbar->GetSelectedItem();
		browser->SetPageIndex(scrollbar->GetSelectedIndex());
		listChanged = true;
	}

	SetSelectedInfos();

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
				//! move list down by 1
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
			//! move list up by 1
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
				bool mii = browser->IsMii(browser->GetPageIndex()+i);
				
				SetButton(i, name, dir, save, databin, mii, true);
			}
			else
				SetButton(i, "", false, false, false, false, false);
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
		
		if(fileBtn[i]->GetState() == STATE_SELECTED)
		{
			selectedItem = i;
			browser->SetSelectedIndex(browser->GetPageIndex() + i);
		}
		
		if (fileBtn[i]->GetState() == STATE_CLICKED)
			state = STATE_CLICKED;
		
		/*ItemMarker * IMarker = browser->GetItemMarker();
		int itemCount = IMarker->GetItemcount();
		
		for(int n = 0; n < itemCount; n++)
		{
			if(browser->GetPageIndex() + i == IMarker->GetItemIndex(n))
				fileBtn[i]->SetState(STATE_SELECTED);
		}*/
	}

	scrollbar->SetPageSize(PAGESIZE);
	scrollbar->SetRowSize(0);
	scrollbar->SetSelectedItem(selectedItem);
	scrollbar->SetSelectedIndex(browser->GetPageIndex());

	listChanged = false;
}
