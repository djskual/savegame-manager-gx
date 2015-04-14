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
 * CEmuMiiList.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include <algorithm>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <wctype.h>

#include "../Text/FreeTypeGX.h"
#include "../Settings/CSettings.h"
#include "../Tools/tools.h"
#include "../Tools/gecko.h"
#include "CEmuMiiList.hpp"

#define MII_FILE "/shared2/menu/FaceLib/RFL_DB.dat"

CEmuMiiList EmuMiiList;

void CEmuMiiList::AddFullMii()
{
	EmuMiiStruct * newMii = new EmuMiiStruct;
	newMii->name = "";
	newMii->creator = "";
	newMii->position = 0;
	newMii->female = 0;
	newMii->month = 0;
	newMii->day = 0;
	newMii->favColor = 0;

	EmuMiisFull.push_back(newMii);
}

void CEmuMiiList::AddFilteredMii()
{
	EmuMiiStruct * newMii = new EmuMiiStruct;
	newMii->name = "";
	newMii->creator = "";
	newMii->position = 0;
	newMii->female = 0;
	newMii->month = 0;
	newMii->day = 0;
	newMii->favColor = 0;

	EmuMiisFiltered.push_back(newMii);
}

void CEmuMiiList::RemoveEntry(int ind)
{
	if(ind < 0 || ind >= (int) EmuMiisFiltered.size())
		return;
	
	EmuMiisFiltered.erase(EmuMiisFiltered.begin() + ind);
}

std::string CEmuMiiList::GetFullName(int ind)
{
	if(ind < 0 || ind >= (int) EmuMiisFull.size())
		return NULL;

	return EmuMiisFull.at(ind)->name;
}

std::string CEmuMiiList::GetName(int ind)
{
	if(ind < 0 || ind >= (int) EmuMiisFiltered.size())
		return NULL;

	return EmuMiisFiltered.at(ind)->name;
}

std::string CEmuMiiList::GetFullCreator(int ind)
{
	if(ind < 0 || ind >= (int) EmuMiisFull.size())
		return NULL;

	return EmuMiisFull.at(ind)->creator;
}

std::string CEmuMiiList::GetCreator(int ind)
{
	if(ind < 0 || ind >= (int) EmuMiisFiltered.size())
		return NULL;

	return EmuMiisFiltered.at(ind)->creator;
}

int CEmuMiiList::GetFullPosition(int ind)
{
	if(ind < 0 || ind >= (int) EmuMiisFull.size())
		return -1;

	return EmuMiisFull.at(ind)->position;
}

int CEmuMiiList::GetPosition(int ind)
{
	if(ind < 0 || ind >= (int) EmuMiisFiltered.size())
		return -1;

	return EmuMiisFiltered.at(ind)->position;
}

bool CEmuMiiList::GetFullFemale(int ind)
{
	if(ind < 0 || ind >= (int) EmuMiisFull.size())
		return 0;

	return EmuMiisFull.at(ind)->female;
}

bool CEmuMiiList::GetFemale(int ind)
{
	if(ind < 0 || ind >= (int) EmuMiisFiltered.size())
		return 0;

	return EmuMiisFiltered.at(ind)->female;
}

int CEmuMiiList::GetFullMonth(int ind)
{
	if(ind < 0 || ind >= (int) EmuMiisFull.size())
		return -1;

	return EmuMiisFull.at(ind)->month;
}

int CEmuMiiList::GetMonth(int ind)
{
	if(ind < 0 || ind >= (int) EmuMiisFiltered.size())
		return -1;

	return EmuMiisFiltered.at(ind)->month;
}

int CEmuMiiList::GetFullDay(int ind)
{
	if(ind < 0 || ind >= (int) EmuMiisFull.size())
		return -1;

	return EmuMiisFull.at(ind)->day;
}

int CEmuMiiList::GetDay(int ind)
{
	if(ind < 0 || ind >= (int) EmuMiisFiltered.size())
		return -1;

	return EmuMiisFiltered.at(ind)->day;
}

int CEmuMiiList::GetFullFavColor(int ind)
{
	if(ind < 0 || ind >= (int) EmuMiisFull.size())
		return -1;

	return EmuMiisFull.at(ind)->favColor;
}

int CEmuMiiList::GetFavColor(int ind)
{
	if(ind < 0 || ind >= (int) EmuMiisFiltered.size())
		return -1;

	return EmuMiisFiltered.at(ind)->favColor;
}

std::string CEmuMiiList::GetMiiNandName(int position)
{
	std::string name;
	
	FILE *fp = fopen(EmuMiiDBPath.c_str(),"rb");
	if(fp == NULL)
		return "";
	
	fseek(fp , 4+2+(position*74), SEEK_SET);
	u16* namedata = (u16*)memalign(32, 20);
	memset(namedata, 0, sizeof(namedata));
	
	fread(namedata,1,20,fp);
	
	fclose(fp);
	
	wchar_t wname[11];
	memset(wname, 0, sizeof(wname));
	
	int i = 0;
	for (i = 0; i < 10; i++)
		wname[i] = (wchar_t)namedata[i];
	
	free(namedata);
	namedata = NULL;
	
	wString *ws_name = new wString(wname);
	
	name = (ws_name->toUTF8());
	
	delete ws_name;
	
	return name;
}
 
std::string CEmuMiiList::GetMiiNandCreator(int position)
{
	std::string creator;
	
	FILE *fp = fopen(EmuMiiDBPath.c_str(),"rb");
	if(fp == NULL)
		return "";
	
	fseek(fp , 4+0x36+(position*74), SEEK_SET);
	u16* creatordata = (u16*)memalign(32, 20);
	memset(creatordata, 0, sizeof(creatordata));
	
	fread(creatordata,1,20,fp);
	
	fclose(fp);
	
	wchar_t wcreator[11];
	memset(wcreator, 0, sizeof(wcreator));
	
	int i = 0;
	for (i = 0; i < 10; i++)
		wcreator[i] = (wchar_t)creatordata[i];
	
	free(creatordata);
	creatordata = NULL;
	
	wString *ws_creator = new wString(wcreator);
	
	creator = (ws_creator->toUTF8());
	
	delete ws_creator;
	
	return creator;
}

void CEmuMiiList::GetMiiNandInfos(int start, char * data, int m)
{
	EmuMiisFull[m]->day = 0;

	if (data[start+0x00] & 1) EmuMiisFull[m]->day += 8;
	if (data[start+0x00] & 2) EmuMiisFull[m]->day += 16;

	EmuMiisFull[m]->month = 0;
	if (data[start+0x00] & 4) EmuMiisFull[m]->month += 1;
	if (data[start+0x00] & 8) EmuMiisFull[m]->month += 2;
	if (data[start+0x00] & 16) EmuMiisFull[m]->month += 4;
	if (data[start+0x00] & 32) EmuMiisFull[m]->month += 8;

	EmuMiisFull[m]->female = data[start+0x00] & 64;

	EmuMiisFull[m]->favColor = 0;
	if (data[start+0x01] & 2) EmuMiisFull[m]->favColor += 1;
	if (data[start+0x01] & 4) EmuMiisFull[m]->favColor += 2;
	if (data[start+0x01] & 8) EmuMiisFull[m]->favColor += 4;
	if (data[start+0x01] & 16) EmuMiisFull[m]->favColor += 8;

	if (data[start+0x01] & 32) EmuMiisFull[m]->day += 1;
	if (data[start+0x01] & 64) EmuMiisFull[m]->day += 2;
	if (data[start+0x01] & 128) EmuMiisFull[m]->day += 4;
}

void CEmuMiiList::ResetFull()
{
	for(u32 i = 0; i < EmuMiisFull.size(); i++)
		delete EmuMiisFull.at(i);

	EmuMiisFull.clear();
}

void CEmuMiiList::ResetFiltered()
{
	for(u32 i = 0; i < EmuMiisFiltered.size(); i++)
		delete EmuMiisFiltered.at(i);

	EmuMiisFiltered.clear();
}

static bool NameSortCallback(const EmuMiiStruct *a, const EmuMiiStruct *b)
{
	return (strcasecmp(((EmuMiiStruct *)a)->name.c_str(), ((EmuMiiStruct *)b)->name.c_str()) < 0);
}

int CEmuMiiList::Get()
{
	need_refresh = false;
	ResetFull();
	
	//! get MiiDB emulate path
	EmuMiiDBPath = Settings.EmuNandPath;
	EmuMiiDBPath += MII_FILE;
	
	FILE *fp = fopen(EmuMiiDBPath.c_str(),"rb");
	if(fp == NULL)
		return 0;	
	
	//! Get Mii data
	u32 tsize = (MII_SIZE * MII_MAX) + MII_HEADER;
	
	char* miidata = (char*)memalign(32, tsize);
	memset(miidata, 0, sizeof(tsize));
	
	if (fread(miidata, 1, tsize, fp) != tsize)
	{
		fclose(fp);
		free(miidata);
		miidata = NULL;
		return 0;
	}
	fclose(fp);
	
	if (miidata[0] != 'R' || miidata[1] != 'N' || miidata[2] != 'O' || miidata[3] != 'D')
	{
		gprintf("Mii version %c%c%c%c is not compatible with libmii\n", miidata[0], miidata[1], miidata[2], miidata[3]);
		free(miidata);
		miidata = NULL;
		return 0;
	}
	else
	{
		int start;
		u32 cnt = 0;
		std::string name;
		
		for (int pos = 0; pos < MII_MAX; pos++)
		{
			name = GetMiiNandName(pos);
			if(!name.empty())
			{
				if(cnt >= EmuMiisFull.size())
					AddFullMii();
				
				start = pos*MII_SIZE+MII_HEADER;
				
				EmuMiisFull[cnt]->name.assign(name);
				EmuMiisFull[cnt]->creator.assign(GetMiiNandCreator(pos));
				EmuMiisFull[cnt]->position = pos;
				GetMiiNandInfos(start, (char*)miidata, cnt);
				
				cnt++;
			}
		}
	}
	
	//! Free memory
	if (miidata)
		free(miidata);
	
	std::sort(EmuMiisFull.begin(), EmuMiisFull.end(), NameSortCallback);
	
	return EmuMiisFull.size();
}

static bool WCharSortCallback(const wchar_t char1, const wchar_t char2)
{
	if (char2 == 0) return true;
	if (char1 == 0) return false;

	return char2 > char1;
}

int CEmuMiiList::FilterList(const wchar_t * miiFilter)
{
	int cnt = 0;
	
	ResetFiltered();
	
	if (EmuMiisFull.empty() || need_refresh) Get();
	if (miiFilter) MiiFilter.assign(miiFilter);

	EmuMiisFiltered.clear();
	AvailableSearchChars.clear();

	for (u32 i = 0; i < EmuMiisFull.size(); ++i)
	{
		wchar_t *MiiName = charToWideChar((EmuMiisFull[i]->name).c_str());
		if (MiiName && *MiiFilter.c_str())
		{
			if (wcsnicmp(MiiName, MiiFilter.c_str(), MiiFilter.size()) != 0)
			{
				delete [] MiiName;
				continue;
			}
		}

		if (MiiName)
		{
			if (wcslen(MiiName) > MiiFilter.size() &&
				AvailableSearchChars.find(towupper(MiiName[MiiFilter.size()])) == std::string::npos &&
				AvailableSearchChars.find(towlower(MiiName[MiiFilter.size()])) == std::string::npos)
			{
				AvailableSearchChars.push_back(MiiName[MiiFilter.size()]);
			}

			delete [] MiiName;
		}

		if(i >= EmuMiisFiltered.size())
		{
			AddFilteredMii();
		}
		
		cnt = EmuMiisFiltered.size()-1;
		
		EmuMiisFiltered[cnt]->name.assign(EmuMiisFull[i]->name);
		EmuMiisFiltered[cnt]->creator.assign(EmuMiisFull[i]->creator);
		EmuMiisFiltered[cnt]->position = EmuMiisFull[i]->position;
		EmuMiisFiltered[cnt]->female = EmuMiisFull[i]->female;
		EmuMiisFiltered[cnt]->month = EmuMiisFull[i]->month;
		EmuMiisFiltered[cnt]->day = EmuMiisFull[i]->day;
		EmuMiisFiltered[cnt]->favColor = EmuMiisFull[i]->favColor;
	}

	AvailableSearchChars.push_back(L'\0');

	if (EmuMiisFiltered.size() < 2)
		AvailableSearchChars.clear();
	
	if (AvailableSearchChars.size() > 1)
		std::sort(AvailableSearchChars.begin(), AvailableSearchChars.end(), WCharSortCallback);

	return EmuMiisFiltered.size();
}
