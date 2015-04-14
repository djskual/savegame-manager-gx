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
 * MiiAllInfos.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include <unistd.h>

#include "MiiAllInfos.hpp"
#include "../Miis/CMiiList.hpp"
#include "../EmuNand/CEmuMiiList.hpp"
#include "../Themes/CTheme.h"
#include "../Settings/CSettings.h"
#include "../Tools/StringTools.h"
#include "../Language/gettext.h"
#include "../menu.h"

MiiAllInfos::MiiAllInfos(int Selected,
						 int EmuSelected,
						 std::string Creator,
						 int Female,
						 int Day,
						 int Month,
						 int FavColor)
	: GuiWindow(0, 0)
{
	creatorIcon = Resources::GetImageData("icon_mii_creator.png");
	creatorImg = new GuiImage(creatorIcon);
	creatorImg->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	
	dateIcon = Resources::GetImageData("icon_mii_date.png");
	dateImg = new GuiImage(dateIcon);
	dateImg->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	
	SetInfos(Selected, EmuSelected, Creator, Female, Day, Month, FavColor);
}

MiiAllInfos::~MiiAllInfos()
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

void MiiAllInfos::SetInfos(int Selected,
						   int EmuSelected,
						   std::string Creator,
						   int Female,
						   int Day,
						   int Month,
						   int FavColor)
{
	miiSelected = Selected;
	emuMiiSelected = EmuSelected;
	
	miiCreator = Creator;
	miiFemale = Female;
	miiDay = Day;
	miiMonth = Month;
	miiFavColor = FavColor;
	
	if (btnCreator)
		Remove(btnCreator);
	if (btnDate)
		Remove(btnDate);
	if (genderImg)
		Remove(genderImg);
	if (favColorImg)
		Remove(favColorImg);
	
	creatorTxt = new GuiText(GetCreator(), 22, thColor("r=255 g=255 b=255 a=255 - manage windows text color"));
	if (Settings.wsprompt) creatorTxt->SetWidescreen(Settings.Widescreen);
	creatorTxt->SetAlignment(ALIGN_RIGHT, ALIGN_MIDDLE);
	creatorTxt->SetMaxWidth(350, SCROLL_HORIZONTAL);
	
	btnCreator = new GuiButton(creatorTxt->GetWidth()+creatorImg->GetWidth()+15, creatorImg->GetHeight());
	btnCreator->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	btnCreator->SetLabel(creatorTxt);
	btnCreator->SetIcon(creatorImg);
	btnCreator->SetState(STATE_DISABLED);
	
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
	if (miiSelected >= 0)
		_female = MiiList.GetFullFemale(miiSelected);
	else if (emuMiiSelected >= 0)
		_female = EmuMiiList.GetFullFemale(emuMiiSelected);
	else
		_female = miiFemale;
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

std::string MiiAllInfos::GetCreator()
{
	std::string creator =  "...";
	if (miiSelected >= 0 && MiiList.GetFullCreator(miiSelected).size())
		creator.assign(MiiList.GetFullCreator(miiSelected));
	else if (emuMiiSelected >= 0 && EmuMiiList.GetFullCreator(emuMiiSelected).size())
		creator.assign(EmuMiiList.GetFullCreator(emuMiiSelected));
	else if (miiCreator.size())
		creator.assign(miiCreator);
	
	return creator;
}

std::string MiiAllInfos::GetDate()
{
	std::string date =  "..";
	if (miiSelected >= 0 && MiiList.GetFullDay(miiSelected))
		date = fmt("%d", MiiList.GetFullDay(miiSelected));
	else if (emuMiiSelected >= 0 && EmuMiiList.GetFullDay(emuMiiSelected))
		date = fmt("%d", EmuMiiList.GetFullDay(emuMiiSelected));
	else if (miiDay)
		date = fmt("%d", miiDay);
	date += " / ";
	
	int _month = 0;
	if (miiSelected >= 0 && MiiList.GetFullMonth(miiSelected))
		_month = MiiList.GetFullMonth(miiSelected);
	else if (emuMiiSelected >= 0 && EmuMiiList.GetFullMonth(emuMiiSelected))
		_month = EmuMiiList.GetFullMonth(emuMiiSelected);
	else if (miiMonth)
		_month = miiMonth;
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

GXColor MiiAllInfos::GetFavColor()
{
	GXColor color = (GXColor) {0, 0, 0, 0};
	
	int _favcolor = 0;
	if (miiSelected >= 0 && MiiList.GetFullFavColor(miiSelected))
		_favcolor = MiiList.GetFullFavColor(miiSelected);
	else if (emuMiiSelected >= 0 && EmuMiiList.GetFullFavColor(emuMiiSelected))
		_favcolor = EmuMiiList.GetFullFavColor(emuMiiSelected);
	else if (miiFavColor)
		_favcolor = miiFavColor;
	switch (_favcolor)
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
