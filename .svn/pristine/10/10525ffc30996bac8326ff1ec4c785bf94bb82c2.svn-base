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
 * MiiInfos.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include <unistd.h>

#include "MiiInfos.hpp"
#include "../Miis/CMiiList.hpp"
#include "../EmuNand/CEmuMiiList.hpp"
#include "../Files/CFileList.hpp"
#include "../Themes/CTheme.h"
#include "../Settings/CSettings.h"
#include "../Tools/StringTools.h"
#include "../Menu/menus.h"

MiiInfos::MiiInfos(int Selected, int Source)
	: GuiWindow(0, 0)
{
	miiSelected = Selected;
	source = Source;
	
	Creator = "...";
	Female = 0;
	Month = 0;
	Day = 0;
	FavColor = 0;
	
	if (source == BROWSE_SDUSB)
	{
		std::string path = Settings.BrowserPath;
		path += "/";
		path += FileList.GetFilename(miiSelected);
		GetInfos(path);
	}
	
	creatorIcon = Resources::GetImageData("icon_mii_creator.png");
	creatorImg = new GuiImage(creatorIcon);
	creatorImg->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	
	creatorTxt = new GuiText(GetCreator(), 22, thColor("r=255 g=255 b=255 a=255 - manage windows text color"));
	if (Settings.wsprompt) creatorTxt->SetWidescreen(Settings.Widescreen);
	creatorTxt->SetAlignment(ALIGN_RIGHT, ALIGN_MIDDLE);
	creatorTxt->SetMaxWidth(350, SCROLL_HORIZONTAL);
	
	btnCreator = new GuiButton(creatorTxt->GetWidth()+creatorImg->GetWidth()+15, creatorImg->GetHeight());
	btnCreator->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	btnCreator->SetLabel(creatorTxt);
	btnCreator->SetIcon(creatorImg);
	btnCreator->SetState(STATE_DISABLED);
	
	dateIcon = Resources::GetImageData("icon_mii_date.png");
	dateImg = new GuiImage(dateIcon);
	dateImg->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	
	dateTxt = new GuiText(GetDate(), 22, thColor("r=255 g=255 b=255 a=255 - manage windows text color"));
	if (Settings.wsprompt) dateTxt->SetWidescreen(Settings.Widescreen);
	dateTxt->SetAlignment(ALIGN_RIGHT, ALIGN_MIDDLE);
	dateTxt->SetMaxWidth(350, SCROLL_HORIZONTAL);
	
	btnDate = new GuiButton(dateTxt->GetWidth()+dateImg->GetWidth()+15, dateImg->GetHeight());
	btnDate->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	btnDate->SetLabel(dateTxt);
	btnDate->SetIcon(dateImg);
	btnDate->SetState(STATE_DISABLED);
	
	int _female = 0;
	if (source == BROWSE_MII)
		_female = MiiList.GetFemale(miiSelected);
	else if (source == BROWSE_EMU_MII)
		_female = EmuMiiList.GetFemale(miiSelected);
	else if  (source == BROWSE_SDUSB)
		_female = Female;
	genderIcon = Resources::GetImageData(_female ? "icon_mii_female.png" : "icon_mii_male.png");
	genderImg = new GuiImage(genderIcon);
	genderImg->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	
	favColorImg = new GuiImage(24, 24, GetFavColor());
	favColorImg->SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
	
	int x = ((btnDate->GetWidth() > btnCreator->GetWidth()) ? btnDate->GetWidth() : btnCreator->GetWidth()) + 50 + 24;
	int y = btnDate->GetHeight() + btnCreator->GetHeight() + 10;
	
	this->SetSize(x, y);
	
	Append(btnCreator);
	Append(btnDate);
	Append(genderImg);
	Append(favColorImg);
}

MiiInfos::~MiiInfos()
{
	RemoveAll();
	
	delete creatorImg;
	delete dateImg;
	delete genderImg;
	delete favColorImg;

	delete creatorIcon;
	delete dateIcon;
	delete genderIcon;
	
	delete creatorTxt;
	delete dateTxt;
	
	delete btnCreator;
	delete btnDate;
}

std::string MiiInfos::GetCreator()
{
	std::string creator =  "...";
	
	if ((source == BROWSE_MII) && (MiiList.GetCreator(miiSelected).size() > 0))
		creator = MiiList.GetCreator(miiSelected);
	else if ((source == BROWSE_EMU_MII) && (EmuMiiList.GetCreator(miiSelected).size() > 0))
		creator = EmuMiiList.GetCreator(miiSelected);
	else if ((source == BROWSE_SDUSB) && (Creator.size() > 0))
		creator = Creator;
	
	return creator;
}

std::string MiiInfos::GetDate()
{
	std::string date =  "..";
	
	if ((source == BROWSE_MII) && (MiiList.GetDay(miiSelected) != 0))
		date = fmt("%d", MiiList.GetDay(miiSelected));
	else if((source == BROWSE_EMU_MII) && (EmuMiiList.GetDay(miiSelected) != 0))
		date = fmt("%d", EmuMiiList.GetDay(miiSelected));
	else if((source == BROWSE_SDUSB)  && (Day != 0))
		date = fmt("%d", Day);
	date += " / ";
	
	int _month = 0;
	if (source == BROWSE_MII)
		_month = MiiList.GetMonth(miiSelected);
	else if (source == BROWSE_EMU_MII)
		_month = EmuMiiList.GetMonth(miiSelected);
	else
		_month = Month;
	
	switch (_month)
	{
		case 0: { date += ".."; break; }
		case 1: { date += tr("January"); break; }
		case 2: { date += tr("February"); break; }
		case 3: { date += tr("March"); break; }
		case 4: { date += tr("April"); break; }
		case 5: { date += tr("May"); break; }
		case 6: { date += tr("June"); break; }
		case 7: { date += tr("July"); break; }
		case 8: { date += tr("August"); break; }
		case 9: { date += tr("September"); break; }
		case 10: { date += tr("October"); break; }
		case 11: { date += tr("November"); break; }
		case 12: { date += tr("December"); break; }
	}
	return date;
}

GXColor MiiInfos::GetFavColor()
{
	GXColor color = (GXColor) {0, 0, 0, 0};
	
	int _favColor = 0;
	if (source == BROWSE_MII)
		_favColor = MiiList.GetFavColor(miiSelected);
	else if (source == BROWSE_EMU_MII)
		_favColor = EmuMiiList.GetFavColor(miiSelected);
	else
		_favColor = FavColor;
	
	switch (_favColor)
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

/***************************************************************************
 * Get Mii Infos
 **************************************************************************/
static char * readMii(std::string path)
{
	FILE * File = fopen(path.c_str(), "rb");
	if (File == NULL)
		return NULL;
	
	char * buffer = (char*) malloc (74);
	fread(buffer,1,74,File);
	fclose (File);
	
	return buffer;
}

void MiiInfos::GetInfos(std::string miipath)
{
	GetDeviceCreator(miipath);
	
	char * data;
	data = readMii(miipath);
	if (!data)
		return;
	
	if (data[0x00] & 1) Day += 8;
	if (data[0x00] & 2) Day += 16;
	if (data[0x01] & 32) Day += 1;
	if (data[0x01] & 64) Day += 2;
	if (data[0x01] & 128) Day += 4;
	
	if (data[0x00] & 4) Month += 1;
	if (data[0x00] & 8) Month += 2;
	if (data[0x00] & 16) Month += 4;
	if (data[0x00] & 32) Month += 8;
	
	Female = data[0x00] & 64;
	
	if (data[0x01] & 2) FavColor += 1;
	if (data[0x01] & 4) FavColor += 2;
	if (data[0x01] & 8) FavColor += 4;
	if (data[0x01] & 16) FavColor += 8;
	
	free(data);
	data = NULL;
}

void MiiInfos::GetDeviceCreator(std::string miipath)
{
	FILE *fp = fopen(miipath.c_str(),"rb");

	if(fp == NULL)
		return;
	
	fseek(fp , 0x36, SEEK_SET);
	u16* creatordata = (u16*)memalign(32, 20);
	memset(creatordata, 0, sizeof(creatordata));
	
	fread(creatordata,1,20,fp);
	
	fclose(fp);
	
	wchar_t wcreator[20];
	memset(wcreator, 0, 20);
	
	u32 i = 0;
	for (i = 0; i < 20; i++){
		wcreator[i] = (wchar_t)creatordata[i];
	}
	
	free(creatordata);
	creatordata = NULL;
	
	wString *ws_creator = new wString(wcreator);
	
	Creator = (ws_creator->toUTF8()).c_str();
	
	delete ws_creator;
}
