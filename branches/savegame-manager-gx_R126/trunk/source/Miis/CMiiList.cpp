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
 * CMiiList.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include <algorithm>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <wctype.h>

#include "../Text/FreeTypeGX.h"
#include "../FileOperations/Nand.hpp"
#include "../Tools/tools.h"
#include "../Tools/gecko.h"
#include "CMiiList.hpp"

#define MII_FILE "/shared2/menu/FaceLib/RFL_DB.dat"

CMiiList MiiList;

void CMiiList::AddFullMii()
{
	MiiStruct * newMii = new MiiStruct;
	newMii->name = "";
	newMii->creator = "";
	newMii->position = 0;
	newMii->female = 0;
	newMii->month = 0;
	newMii->day = 0;
	newMii->favColor = 0;

	MiisFull.push_back(newMii);
}

void CMiiList::AddFilteredMii()
{
	MiiStruct * newMii = new MiiStruct;
	newMii->name = "";
	newMii->creator = "";
	newMii->position = 0;
	newMii->female = 0;
	newMii->month = 0;
	newMii->day = 0;
	newMii->favColor = 0;

	MiisFiltered.push_back(newMii);
}

void CMiiList::RemoveEntry(int ind)
{
	if(ind < 0 || ind >= (int) MiisFiltered.size())
		return;
	
	MiisFiltered.erase(MiisFiltered.begin() + ind);
}

std::string CMiiList::GetFullName(int ind)
{
	if(ind < 0 || ind >= (int) MiisFull.size())
		return NULL;

	return MiisFull.at(ind)->name;
}

std::string CMiiList::GetName(int ind)
{
	if(ind < 0 || ind >= (int) MiisFiltered.size())
		return NULL;

	return MiisFiltered.at(ind)->name;
}

std::string CMiiList::GetFullCreator(int ind)
{
	if(ind < 0 || ind >= (int) MiisFull.size())
		return NULL;

	return MiisFull.at(ind)->creator;
}

std::string CMiiList::GetCreator(int ind)
{
	if(ind < 0 || ind >= (int) MiisFiltered.size())
		return NULL;

	return MiisFiltered.at(ind)->creator;
}

int CMiiList::GetFullPosition(int ind)
{
	if(ind < 0 || ind >= (int) MiisFull.size())
		return -1;

	return MiisFull.at(ind)->position;
}

int CMiiList::GetPosition(int ind)
{
	if(ind < 0 || ind >= (int) MiisFiltered.size())
		return -1;

	return MiisFiltered.at(ind)->position;
}

bool CMiiList::GetFullFemale(int ind)
{
	if(ind < 0 || ind >= (int) MiisFull.size())
		return 0;

	return MiisFull.at(ind)->female;
}

bool CMiiList::GetFemale(int ind)
{
	if(ind < 0 || ind >= (int) MiisFiltered.size())
		return 0;

	return MiisFiltered.at(ind)->female;
}

int CMiiList::GetFullMonth(int ind)
{
	if(ind < 0 || ind >= (int) MiisFull.size())
		return -1;

	return MiisFull.at(ind)->month;
}

int CMiiList::GetMonth(int ind)
{
	if(ind < 0 || ind >= (int) MiisFiltered.size())
		return -1;

	return MiisFiltered.at(ind)->month;
}

int CMiiList::GetFullDay(int ind)
{
	if(ind < 0 || ind >= (int) MiisFull.size())
		return -1;

	return MiisFull.at(ind)->day;
}

int CMiiList::GetDay(int ind)
{
	if(ind < 0 || ind >= (int) MiisFiltered.size())
		return -1;

	return MiisFiltered.at(ind)->day;
}

int CMiiList::GetFullFavColor(int ind)
{
	if(ind < 0 || ind >= (int) MiisFull.size())
		return -1;

	return MiisFull.at(ind)->favColor;
}

int CMiiList::GetFavColor(int ind)
{
	if(ind < 0 || ind >= (int) MiisFiltered.size())
		return -1;

	return MiisFiltered.at(ind)->favColor;
}

std::string CMiiList::GetMiiNandName(int position)
{
	std::string name;
	
	int fh = Nand::OpenRead(MII_FILE);
	if (fh < 0)
		return "";
	
	Nand::Seek(fh, 4+2+(position*74), 0);
	u16* namedata = (u16*)Tools::AllocateMemory_32(20);
	memset(namedata, 0, round_up(20, 32));
	
	if (Nand::Read(fh, (u8*)namedata, 20) < 0)
	{
		Nand::Close(fh);
		free(namedata);
		namedata = NULL;
		return "";
	}
	Nand::Close(fh);
	
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
 
std::string CMiiList::GetMiiNandCreator(int position)
{
	std::string creator;
	
	int fh = Nand::OpenRead(MII_FILE);
	if (fh < 0)
		return "";
	
	Nand::Seek(fh, 4+0x36+(position*74), 0);
	u16* creatordata = (u16*)Tools::AllocateMemory_32(20);
	memset(creatordata, 0, round_up(20, 32));
	
	if (Nand::Read(fh, (u8*)creatordata, 20) < 0)
	{
		Nand::Close(fh);
		free(creatordata);
		creatordata = NULL;
		return "";
	}
	Nand::Close(fh);
	
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

void CMiiList::GetMiiNandInfos(int start, char * data, int m)
{
	MiisFull[m]->day = 0;

	if (data[start+0x00] & 1) MiisFull[m]->day += 8;
	if (data[start+0x00] & 2) MiisFull[m]->day += 16;

	MiisFull[m]->month = 0;
	if (data[start+0x00] & 4) MiisFull[m]->month += 1;
	if (data[start+0x00] & 8) MiisFull[m]->month += 2;
	if (data[start+0x00] & 16) MiisFull[m]->month += 4;
	if (data[start+0x00] & 32) MiisFull[m]->month += 8;

	MiisFull[m]->female = data[start+0x00] & 64;

	MiisFull[m]->favColor = 0;
	if (data[start+0x01] & 2) MiisFull[m]->favColor += 1;
	if (data[start+0x01] & 4) MiisFull[m]->favColor += 2;
	if (data[start+0x01] & 8) MiisFull[m]->favColor += 4;
	if (data[start+0x01] & 16) MiisFull[m]->favColor += 8;

	if (data[start+0x01] & 32) MiisFull[m]->day += 1;
	if (data[start+0x01] & 64) MiisFull[m]->day += 2;
	if (data[start+0x01] & 128) MiisFull[m]->day += 4;
}

void CMiiList::ResetFull()
{
	for(u32 i = 0; i < MiisFull.size(); i++)
		delete MiisFull.at(i);

	MiisFull.clear();
}

void CMiiList::ResetFiltered()
{
	for(u32 i = 0; i < MiisFiltered.size(); i++)
		delete MiisFiltered.at(i);

	MiisFiltered.clear();
}

static bool NameSortCallback(const MiiStruct *a, const MiiStruct *b)
{
	return (strcasecmp(((MiiStruct *)a)->name.c_str(), ((MiiStruct *)b)->name.c_str()) < 0);
}

int CMiiList::Get()
{
	need_refresh = false;
	ResetFull();
	
	s32 fh = Nand::OpenRead(MII_FILE);
	if (fh < 0)
		return 0;	
	
	//! Get Mii data
	u32 tsize = (MII_SIZE * MII_MAX) + MII_HEADER;
	
	char* miidata = (char*)Tools::AllocateMemory_32(tsize);
	memset(miidata, 0, round_up(tsize, 32));
	
	if (Nand::Read(fh, (u8*)miidata, tsize) < 0)
	{
		Nand::Close(fh);
		free(miidata);
		miidata = NULL;
		return 0;
	}
	Nand::Close(fh);
	
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
		std:: string name;
		
		for (int pos = 0; pos < MII_MAX; pos++)
		{
			name = GetMiiNandName(pos);
			if(!name.empty())
			{
				if(cnt >= MiisFull.size())
					AddFullMii();
				
				start = pos*MII_SIZE+MII_HEADER;
				
				MiisFull[cnt]->name.assign(name);
				MiisFull[cnt]->creator.assign(GetMiiNandCreator(pos));
				MiisFull[cnt]->position = pos;
				GetMiiNandInfos(start, (char*)miidata, cnt);
				
				cnt++;
			}
		}
	}
	
	//! Free memory
	if (miidata)
		free(miidata);
	
	std::sort(MiisFull.begin(), MiisFull.end(), NameSortCallback);
	
	return MiisFull.size();
}

static bool WCharSortCallback(const wchar_t char1, const wchar_t char2)
{
	if (char2 == 0) return true;
	if (char1 == 0) return false;

	return char2 > char1;
}

int CMiiList::FilterList(const wchar_t * miiFilter)
{
	int cnt = 0;
	
	ResetFiltered();
	
	if (MiisFull.empty() || need_refresh) Get();
	if (miiFilter) MiiFilter.assign(miiFilter);

	MiisFiltered.clear();
	AvailableSearchChars.clear();

	for (u32 i = 0; i < MiisFull.size(); ++i)
	{
		wchar_t *MiiName = charToWideChar((MiisFull[i]->name).c_str());
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

		if(i >= MiisFiltered.size())
		{
			AddFilteredMii();
		}
		
		cnt = MiisFiltered.size()-1;
		
		MiisFiltered[cnt]->name.assign(MiisFull[i]->name);
		MiisFiltered[cnt]->creator.assign(MiisFull[i]->creator);
		MiisFiltered[cnt]->position = MiisFull[i]->position;
		MiisFiltered[cnt]->female = MiisFull[i]->female;
		MiisFiltered[cnt]->month = MiisFull[i]->month;
		MiisFiltered[cnt]->day = MiisFull[i]->day;
		MiisFiltered[cnt]->favColor = MiisFull[i]->favColor;
	}

	AvailableSearchChars.push_back(L'\0');

	if (MiisFiltered.size() < 2)
		AvailableSearchChars.clear();
	
	if (AvailableSearchChars.size() > 1)
		std::sort(AvailableSearchChars.begin(), AvailableSearchChars.end(), WCharSortCallback);

	return MiisFiltered.size();
}
