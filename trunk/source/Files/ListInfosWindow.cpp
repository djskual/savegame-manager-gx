/****************************************************************************
 * Copyright (C) 2012 Dj_Skual
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
#include "ListInfosWindow.h"
#include "../FileOperations/fileops.h"
#include "../Themes/CTheme.h"
#include "../Saves/SaveTools.h"

InfosWindow::InfosWindow(int w, int h)
	: GuiFrame(w, h)
{
	fileType = TYPE_UNKNOW;
	
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
	
	infoIconImg = NULL;
	infoTypeImg = NULL;
	infoNameTxt = NULL;
	infoTypeTxt = NULL;
	infoSizeTxt = NULL;
	pathTxt = NULL;
	infoPathTxt = NULL;
}

InfosWindow::~InfosWindow()
{
	RemoveAll();
	
	delete infoIconImg;
	delete infoTypeImg;
	delete infoNameTxt;
	delete infoTypeTxt;
	delete infoSizeTxt;
	delete pathTxt;
	delete infoPathTxt;
	
	Resources::Remove(fileCfg);
	Resources::Remove(fileDataBin);
	Resources::Remove(fileDefault);
	Resources::Remove(fileExe);
	Resources::Remove(fileFolder);
	Resources::Remove(fileFont);
	Resources::Remove(fileImg);
	Resources::Remove(fileLang);
	Resources::Remove(fileMii);
	Resources::Remove(fileSave);
	Resources::Remove(fileSound);
	Resources::Remove(fileTheme);
	Resources::Remove(fileTxt);
	Resources::Remove(fileXml);
	
	Resources::Remove(type_Wii);
	Resources::Remove(type_Wiiware);
	Resources::Remove(type_Gamecube);
	Resources::Remove(type_VC_NES);
	Resources::Remove(type_VC_SNES);
	Resources::Remove(type_VC_N64);
	Resources::Remove(type_VC_SMS);
	Resources::Remove(type_VC_MD);
	Resources::Remove(type_VC_PCE);
	Resources::Remove(type_VC_Neogeo);
	Resources::Remove(type_VC_Arcade);
	Resources::Remove(type_VC_C64);
	Resources::Remove(type_Channel);
	Resources::Remove(type_Unknow);
}

void InfosWindow::SetInfos(std::wstring ItemDisplayName, std::string CurrentSelectedFilepath, bool Dir, bool Save, bool Databin, bool Mii)
{
	RemoveAll();
	
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
	
	dir = Dir;
	save = Save;
	databin = Databin;
	mii = Mii;
	fileext = "";
	itemName = CurrentSelectedFilepath;
	itemName.erase(0, itemName.find_last_of("/")+1);
	
	if(!save && !mii && !dir && !databin)
	{
		fileext = itemName;
		int index = fileext.find_last_of('.');
		if(index > 0)
			fileext = fileext.substr(index);
	}
	
	int y = 0;
	int txtmaxwidth = this->GetWidth();
	
	infoIconImg = new GuiImage();
	/*if(save) GetSaveIcon();
	else if(databin) GetDataBinIcon();
	else */infoIconImg = GetIconFromExt();
	infoIconImg->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	infoIconImg->SetPosition((save || databin) ? 10 : 0, y);	
	if(!save && !databin) infoIconImg->SetScale(1.5);
	y += infoIconImg->GetHeight()+10;
	
	if(save || databin) FoundSaveType();
	infoTypeImg = new GuiImage(GetTypeIcon());
	infoTypeImg->SetAlignment(ALIGN_RIGHT | ALIGN_TOP);
	infoTypeImg->SetPosition(-20, 18);	
	
	infoNameTxt = new GuiText(ItemDisplayName.c_str(), 16, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	infoNameTxt->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	infoNameTxt->SetPosition(0, y);
	infoNameTxt->SetMaxWidth(txtmaxwidth, SCROLL_HORIZONTAL);
	y += 30;
	
	std::string type = tr( "Type:" );
	type += " ";
	type += GetTypeFromExt();
	infoTypeTxt = new GuiText(type.c_str(), 16, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	infoTypeTxt->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	infoTypeTxt->SetPosition(0, y);
	infoTypeTxt->SetMaxWidth(txtmaxwidth, DOTTED);
	y += 22;
	
	std::string size = tr( "Size:" );
	size += " ";
	if(!dir)
	{
		u64 foldersize = 0;
		u32 filenumber = 0;
		if(save)
			GetFolderSize(CurrentSelectedFilepath, &foldersize, &filenumber);
		else
			foldersize = FileSize(CurrentSelectedFilepath);
		
		if (foldersize < MBSIZE)
			size += fmt("%0.2fKB", foldersize / KBSIZE);
		else if (foldersize > MBSIZE && foldersize < GBSIZE)
			size += fmt("%0.2fMB", foldersize / MBSIZE);
		else
			size += fmt("%0.2fGB", foldersize / GBSIZE);
	}
	else
		size += tr( "Unknown" );
	
	infoSizeTxt = new GuiText(size.c_str(), 16, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	infoSizeTxt->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	infoSizeTxt->SetPosition(0, y);
	infoSizeTxt->SetMaxWidth(txtmaxwidth, DOTTED);
	y += 22;
	
	int Y = -40;
	std::string CurrentPath = CurrentSelectedFilepath;
	CurrentPath.erase(CurrentPath.find_last_of("/")+1);
	
	infoPathTxt = new GuiText(CurrentPath.c_str(), 16, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	infoPathTxt->SetAlignment(ALIGN_LEFT | ALIGN_BOTTOM);
	infoPathTxt->SetPosition(0, Y);
	infoPathTxt->SetLinesToDraw(3);
	infoPathTxt->SetMaxWidth(txtmaxwidth, WRAP);
	
	Y -= 18;
	pathTxt = new GuiText(tr( "Path:" ), 16, thColor("r=0 g=0 b=0 a=255 - mainbrowser entry text color"));
	pathTxt->SetAlignment(ALIGN_LEFT | ALIGN_BOTTOM);
	pathTxt->SetPosition(0, Y);
	
	Append(infoIconImg);
	if(save || databin) Append(infoTypeImg);
	Append(infoNameTxt);
	Append(infoTypeTxt);
	Append(infoSizeTxt);
	Append(pathTxt);
	Append(infoPathTxt);
}

GuiImage * InfosWindow::GetIconFromExt()
{
	if (dir)
		return (new GuiImage(fileFolder));

	if (save)
		return (new GuiImage(fileSave));

	if (databin)
		return (new GuiImage(fileDataBin));

	if (mii)
		return (new GuiImage(fileMii));

	if(!fileext.compare(".cfg"))
		return (new GuiImage(fileCfg));
	else if (!Settings.FileExtensions.CompareHomebrew(fileext.c_str()))
		return (new GuiImage(fileExe));
	else if(!Settings.FileExtensions.CompareFont(fileext.c_str()))
		return (new GuiImage(fileFont));
	else if (!Settings.FileExtensions.CompareImage(fileext.c_str()))
		return (new GuiImage(fileImg));
	else if(!Settings.FileExtensions.CompareLanguageFiles(fileext.c_str()))
		return (new GuiImage(fileLang));
	else if(!Settings.FileExtensions.CompareAudio(fileext.c_str()))
		return (new GuiImage(fileSound));
	else if(!Settings.FileExtensions.CompareThemeFiles(fileext.c_str()))
		return (new GuiImage(fileTheme));
	else if(!fileext.compare(".log") ||
			!fileext.compare(".txt"))
		return (new GuiImage(fileTxt));
	else if(!fileext.compare(".xml"))
		return (new GuiImage(fileXml));
	
	return (new GuiImage(fileDefault));
}

std::string InfosWindow::GetTypeFromExt()
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

GuiImageData * InfosWindow::GetTypeIcon()
{
	switch (fileType)
	{
		case TYPE_WII:
			return type_Wii;
		case TYPE_WIIWARE:
			return type_Wiiware;
		case TYPE_GAMECUBE:
			return type_Gamecube;
		case TYPE_VC_NES:
			return type_VC_NES;
		case TYPE_VC_SNES:
			return type_VC_SNES;
		case TYPE_VC_N64:
			return type_VC_N64;
		case TYPE_VC_SMS:
			return type_VC_SMS;
		case TYPE_VC_MD:
			return type_VC_MD;
		case TYPE_VC_PCE:
			return type_VC_PCE;
		case TYPE_VC_NEOGEO:
			return type_VC_Neogeo;
		case TYPE_VC_ARCADE:
			return type_VC_Arcade;
		case TYPE_VC_C64:
			return type_VC_C64;
		case TYPE_CHANNEL:
			return type_Channel;
		case TYPE_UNKNOW:
		default:
			return type_Unknow;
	}
	
	return type_Unknow;
}

void InfosWindow::FoundSaveType()
{
	fileType = TYPE_UNKNOW;
	
	u64 tid = 0;
	if(save)
		tid = StrToHex64(itemName.c_str());
	/*else if(databintid)
		tid = databintid;*/
	if(!tid)
		return;
	
	std::string ID = titleText((u32)(tid>> 32),(u32)(tid & 0xFFFFFFFF));
	if(ID.empty())
		return;
	
	std::string id(ID);
	std::string id2(ID);
	id.erase(1);
	id2.erase(2);
	
	if(!id.compare("R") || !id.compare("S") || !id.compare("D"))
		fileType = TYPE_WII;
	else if(!id.compare("W"))
		fileType = TYPE_WIIWARE;
	else if(!id.compare("G") || !id.compare("P") || !id.compare("D") || !id.compare("U"))
		fileType = TYPE_GAMECUBE;
	else if(!id.compare("F"))
		fileType = TYPE_VC_NES;
	else if(!id.compare("J"))
		fileType = TYPE_VC_SNES;
	else if(!id.compare("N"))
		fileType = TYPE_VC_N64;
	else if(!id.compare("L"))
		fileType = TYPE_VC_SMS;
	else if(!id.compare("M"))
		fileType = TYPE_VC_MD;
	else if(!id.compare("P"))
		fileType = TYPE_VC_PCE;
	else if(!id.compare("E"))
	{
		if(!id2.compare("EA") || !id2.compare("EB"))
			fileType = TYPE_VC_NEOGEO;
		else
			fileType = TYPE_VC_ARCADE;
	}
	else if(!id.compare("C"))
		fileType = TYPE_VC_C64;
	else if(!id.compare("H") || !id2.compare("JO"))
		fileType = TYPE_CHANNEL;
}

/*void InfosWindow::GetSaveIcon()
{
	u64 size = 0;
	u8* buffer = NULL;
	std::string savepath(browser->GetCurrentSelectedFilepath());
	savepath += "/banner.bin";
	
	LoadFileToMem(savepath.c_str(), &buffer, &size);
	
	GuiSaveIcon * Icon = new GuiSaveIcon(buffer, (u32)size);

	infoIconImg = Icon;
}*/

/*void InfosWindow::GetDataBinIcon()
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
	
	infoIconImg = Icon;
}*/
