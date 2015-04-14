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
 * CSaveList.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include <algorithm>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <wctype.h>

#include "../Settings/CSettings.h"
#include "../XML/GameTDB.hpp"
#include "../System/nandtitle.h"
#include "../FileOperations/Nand.hpp"
#include "../Text/FreeTypeGX.h"
#include "../Tools/tools.h"
#include "SaveTools.h"
#include "CSaveList.hpp"

CSaveList SaveList;

void CSaveList::AddFullSave()
{
	SaveStruct * newSave = new SaveStruct;
	newSave->name = " ";
	newSave->subname = " ";
	newSave->ID = " ";
	newSave->tid = 0;
	newSave->blocksize = 0;

	SavesFull.push_back(newSave);
}

void CSaveList::AddFilteredSave()
{
	SaveStruct * newSave = new SaveStruct;
	newSave->name = " ";
	newSave->subname = " ";
	newSave->ID = " ";
	newSave->tid = 0;
	newSave->blocksize = 0;

	SavesFiltered.push_back(newSave);
}

std::string CSaveList::GetFullName(int ind)
{
	if(ind < 0 || ind >= (int) SavesFull.size())
		return NULL;

	return SavesFull.at(ind)->name;
}

std::string CSaveList::GetName(int ind)
{
	if(ind < 0 || ind >= (int) SavesFiltered.size())
		return NULL;

	return SavesFiltered.at(ind)->name;
}

std::string  CSaveList::GetFullSubname(int ind)
{
	if(ind < 0 || ind >= (int) SavesFull.size())
		return NULL;

	return SavesFull.at(ind)->subname;
}

std::string  CSaveList::GetSubname(int ind)
{
	if(ind < 0 || ind >= (int) SavesFiltered.size())
		return NULL;

	return SavesFiltered.at(ind)->subname;
}

std::string  CSaveList::GetID(int ind)
{
	if(ind < 0 || ind >= (int) SavesFiltered.size())
		return NULL;

	return SavesFiltered.at(ind)->ID;
}

u64 CSaveList::GetFullTid(int ind)
{
	if(ind < 0 || ind >= (int) SavesFull.size())
		return 0;

	return SavesFull.at(ind)->tid;
}

u64 CSaveList::GetTid(int ind)
{
	if(ind < 0 || ind >= (int) SavesFiltered.size())
		return 0;

	return SavesFiltered.at(ind)->tid;
}

float CSaveList::GetBlocks(int ind)
{
	if(ind < 0 || ind >= (int) SavesFiltered.size())
		return -1;

	return SavesFiltered.at(ind)->blocksize;
}


void CSaveList::LoadTitlesFromGameTDB(const char * path)
{
	if(!path || !Settings.GameTDBTitles)
		return;

	std::string Title;
	std::string Filepath = path;
	if(path[strlen(path)-1] != '/')
		Filepath += '/';
	Filepath += "wiitdb.xml";

	GameTDB * XML_DB = new GameTDB(Filepath.c_str());
	XML_DB->SetLanguageCode(Settings.GameTDBLanguageCode);

	for(u32 i = 0; i < SavesFull.size(); ++i)
	{
		if(XML_DB->GetTitle((SavesFull.at(i)->ID).c_str(), Title))
			(SavesFull.at(i)->name).assign(Title);
	}
	
	delete XML_DB;
}

void CSaveList::ResetFull()
{
	for(u32 i = 0; i < SavesFull.size(); i++)
		delete SavesFull.at(i);

	SavesFull.clear();
}

void CSaveList::ResetFiltered()
{
	for(u32 i = 0; i < SavesFiltered.size(); i++)
		delete SavesFiltered.at(i);

	SavesFiltered.clear();
}

static bool NameSortCallback(const SaveStruct *a, const SaveStruct *b)
{
	return (strcasecmp(((SaveStruct *)a)->name.c_str(), ((SaveStruct *)b)->name.c_str()) < 0);
}

int CSaveList::Get()
{
	ResetFull();
	
	int cnt = 0;
	u32  titleCnt;

	//! Get title list
	int ret = NandTitles.Get();
	if(ret < 0)
		return -1;
	
	if((titleCnt = NandTitles.Count()) == 0)
		return 0;
	
	for (u32 i = 0; i < titleCnt; i++)
	{
		u64 tid = NandTitles.At(i);
		
		//! Generate dirpath
		char savepath[128];
		GetNandPath(tid, savepath);
		
		//! Check for title savegame
		if (CheckTitle(savepath))
		{
			if(i >= SavesFull.size())
				AddFullSave();
			
			cnt = SavesFull.size()-1;
			
			//! Set title name & subname
			(SavesFull.at(cnt)->name).assign(GetNandName(tid));
			(SavesFull.at(cnt)->subname).assign(GetNandSubname(tid));
			
			//! Set title ID[4]
			(SavesFull.at(cnt)->ID).assign(titleText((u32)(tid >> 32),(u32)(tid & 0xFFFFFFFF)));
			
			//! Set titleid
			SavesFull.at(cnt)->tid = tid;
			
			//! Set title blockSize
			SavesFull.at(cnt)->blocksize = ((float)(Nand::GetDirSize(savepath)/(float)NAND_BLOCK_SIZE));
		}
	}
	
	//! set GameTDB Titles
	LoadTitlesFromGameTDB(Settings.GameTDBPath);
	
	std::sort(SavesFull.begin(), SavesFull.end(), NameSortCallback);
	
	return SavesFull.size();
}

static bool WCharSortCallback(const wchar_t char1, const wchar_t char2)
{
	if (char2 == 0) return true;
	if (char1 == 0) return false;

	return char2 > char1;
}

int CSaveList::FilterList(const wchar_t * saveFilter)
{
	int cnt = 0;
	
	ResetFiltered();
	
	if (SavesFull.empty()) Get();
	if (saveFilter) SaveFilter.assign(saveFilter);

	SavesFiltered.clear();
	AvailableSearchChars.clear();

	for (u32 i = 0; i < SavesFull.size(); ++i)
	{
		wchar_t *SaveName = charToWideChar((SavesFull.at(i)->name).c_str());
		if (SaveName && *SaveFilter.c_str())
		{
			if (wcsnicmp(SaveName, SaveFilter.c_str(), SaveFilter.size()) != 0)
			{
				delete [] SaveName;
				continue;
			}
		}

		if (SaveName)
		{
			if (wcslen(SaveName) > SaveFilter.size() &&
				AvailableSearchChars.find(towupper(SaveName[SaveFilter.size()])) == std::string::npos &&
				AvailableSearchChars.find(towlower(SaveName[SaveFilter.size()])) == std::string::npos)
			{
				AvailableSearchChars.push_back(SaveName[SaveFilter.size()]);
			}

			delete [] SaveName;
		}

		if(i >= SavesFiltered.size())
		{
			AddFilteredSave();
		}
		
		cnt = SavesFiltered.size()-1;
		
		(SavesFiltered.at(cnt)->name).assign(SavesFull.at(i)->name);
		(SavesFiltered.at(cnt)->subname).assign(SavesFull.at(i)->subname);
		(SavesFiltered.at(cnt)->ID).assign(SavesFull.at(i)->ID);
		SavesFiltered.at(cnt)->tid = SavesFull.at(i)->tid;
		SavesFiltered.at(cnt)->blocksize = SavesFull.at(i)->blocksize;
	}

	AvailableSearchChars.push_back(L'\0');

	if (SavesFiltered.size() < 2)
		AvailableSearchChars.clear();
	
	if (AvailableSearchChars.size() > 1)
		std::sort(AvailableSearchChars.begin(), AvailableSearchChars.end(), WCharSortCallback);
	
	return SavesFiltered.size();
}
