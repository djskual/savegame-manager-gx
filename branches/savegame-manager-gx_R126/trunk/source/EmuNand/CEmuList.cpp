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
 * CEmuList.cpp
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
#include "../Saves/SaveTools.h"
#include "../FileOperations/DirList.h"
#include "../FileOperations/fileops.h"
#include "../Text/FreeTypeGX.h"
#include "../Tools/tools.h"
#include "../Tools/StringTools.h"
#include "CEmuList.hpp"

CEmuList EmuList;

void CEmuList::AddFullEmu()
{
	EmuStruct * newSave = new EmuStruct;
	newSave->name = " ";
	newSave->subname = " ";
	newSave->ID = " ";
	newSave->tid = 0;
	newSave->blocksize = 0;
	newSave->isNotInstalled = false;

	EmuFull.push_back(newSave);
}

void CEmuList::AddFilteredEmu()
{
	EmuStruct * newSave = new EmuStruct;
	newSave->name = " ";
	newSave->subname = " ";
	newSave->ID = " ";
	newSave->tid = 0;
	newSave->blocksize = 0;
	newSave->isNotInstalled = false;

	EmuFiltered.push_back(newSave);
}

void CEmuList::RemoveEntry(int ind)
{
	if(ind < 0 || ind >= (int) EmuFiltered.size())
		return;
	
	EmuFiltered.erase(EmuFiltered.begin() + ind);
}

std::string CEmuList::GetFullName(int ind)
{
	if(ind < 0 || ind >= (int) EmuFull.size())
		return NULL;

	return EmuFull.at(ind)->name;
}

std::string CEmuList::GetName(int ind)
{
	if(ind < 0 || ind >= (int) EmuFiltered.size())
		return NULL;

	return EmuFiltered.at(ind)->name;
}

std::string  CEmuList::GetFullSubname(int ind)
{
	if(ind < 0 || ind >= (int) EmuFull.size())
		return NULL;

	return EmuFull.at(ind)->subname;
}

std::string  CEmuList::GetSubname(int ind)
{
	if(ind < 0 || ind >= (int) EmuFiltered.size())
		return NULL;

	return EmuFiltered.at(ind)->subname;
}

std::string  CEmuList::GetID(int ind)
{
	if(ind < 0 || ind >= (int) EmuFiltered.size())
		return NULL;

	return EmuFiltered.at(ind)->ID;
}

u64 CEmuList::GetFullTid(int ind)
{
	if(ind < 0 || ind >= (int) EmuFull.size())
		return 0;

	return EmuFull.at(ind)->tid;
}

u64 CEmuList::GetTid(int ind)
{
	if(ind < 0 || ind >= (int) EmuFiltered.size())
		return 0;

	return EmuFiltered.at(ind)->tid;
}

float CEmuList::GetBlocks(int ind)
{
	if(ind < 0 || ind >= (int) EmuFiltered.size())
		return -1;

	return EmuFiltered.at(ind)->blocksize;
}


bool CEmuList::GetIsNotInstalled(int ind)
{
	if(ind < 0 || ind >= (int) EmuFiltered.size())
		return false;

	return EmuFiltered.at(ind)->isNotInstalled;
}


void CEmuList::LoadTitlesFromGameTDB()
{
	if(!Settings.GameTDBPath || !Settings.GameTDBTitles)
		return;

	std::string Title;
	std::string Filepath = Settings.GameTDBPath;
	while(Filepath[Filepath.size()-1] == '/')
		Filepath.erase(Filepath.size()-1);
	Filepath += "/wiitdb.xml";

	GameTDB * XML_DB = new GameTDB(Filepath.c_str());
	XML_DB->SetLanguageCode(Settings.GameTDBLanguageCode);

	for(u32 i = 0; i < EmuFull.size(); ++i)
	{
		if(XML_DB->GetTitle((EmuFull.at(i)->ID).c_str(), Title))
			(EmuFull.at(i)->name).assign(Title);
	}
	
	delete XML_DB;
}

void CEmuList::ResetFull()
{
	for(u32 i = 0; i < EmuFull.size(); i++)
		delete EmuFull.at(i);

	EmuFull.clear();
}

void CEmuList::ResetFiltered()
{
	for(u32 i = 0; i < EmuFiltered.size(); i++)
		delete EmuFiltered.at(i);

	EmuFiltered.clear();
}

static bool NameSortCallback(const EmuStruct *a, const EmuStruct *b)
{
	return (strcasecmp(((EmuStruct *)a)->name.c_str(), ((EmuStruct *)b)->name.c_str()) < 0);
}

int CEmuList::Get()
{
	need_refresh = false;
	ResetFull();
	
	u32 saves_found = 0;
	std::string titlePath = fmt("%s/title",Settings.EmuNandPath);
	
	DirList * dirList = new DirList(titlePath, NULL, DirList::Dirs);
	for(int i = 0; i < dirList->GetFilecount(); i++)
	{
		std::string dirName = dirList->GetFilename(i);
		if(!dirName.compare("00000001") || !dirName.compare("00010002") || !dirName.compare("00010005") || !dirName.compare("00010008"))
			continue;
		
		DirList * emuList = new DirList(dirList->GetFilepath(i), ".bin", DirList::Dirs | DirList::Files | DirList::CheckSubfolders);
		for(int j = 0; j < emuList->GetFilecount(); j++)
		{
			std::string banner = emuList->GetFilename(j);
			if(!banner.compare("banner.bin"))
			{
				saves_found++;
				
				if(saves_found > EmuFull.size())
					AddFullEmu();
				
				int pos = EmuFull.size()-1;
				
				//! Set title name & subname
				std::string savepath = GetParentDir(emuList->GetFilepath(j));
				EmuFull.at(pos)->name.assign(GetDeviceName(savepath.c_str()));
				EmuFull.at(pos)->subname.assign(GetDeviceSubname(savepath.c_str()));
				
				//! Set titleid
				std::string str_tid = savepath;
				str_tid.erase(str_tid.find_last_of('/'));
				str_tid.erase(0, str_tid.find_last_of('/')+1);
				str_tid.insert(0, dirName);
				
				u64 tid = StrToHex64(str_tid.c_str());
				EmuFull.at(pos)->tid = tid;
				
				//! Set title ID[4]
				EmuFull.at(pos)->ID.assign(titleText((u32)(tid >> 32),(u32)(tid & 0xFFFFFFFF)));
				
				//! Set title blockSize
				u64 foldersize = 0;
				u32 filenumber = 0;
				GetFolderSize(savepath, &foldersize, &filenumber);
				EmuFull.at(pos)->blocksize = ((float)(foldersize/(float)NAND_BLOCK_SIZE));
				
				//! Check if the save is already installed in real nand
				EmuFull.at(pos)->isNotInstalled = !CheckIfInstalled(tid);
			}
		}
		delete emuList;
	}
	delete dirList;
	
	//! set GameTDB Titles
	LoadTitlesFromGameTDB();
	
	std::sort(EmuFull.begin(), EmuFull.end(), NameSortCallback);
	
	return EmuFull.size();
}

static bool WCharSortCallback(const wchar_t char1, const wchar_t char2)
{
	if (char2 == 0) return true;
	if (char1 == 0) return false;

	return char2 > char1;
}

int CEmuList::FilterList(const wchar_t * emuFilter)
{
	int pos = 0;
	
	ResetFiltered();
	
	if (EmuFull.empty() || need_refresh) Get();
	if (emuFilter) EmuFilter.assign(emuFilter);

	EmuFiltered.clear();
	AvailableSearchChars.clear();

	for (u32 i = 0; i < EmuFull.size(); ++i)
	{
		wchar_t *SaveName = charToWideChar(EmuFull.at(i)->name.c_str());
		if (SaveName && *EmuFilter.c_str())
		{
			if (wcsnicmp(SaveName, EmuFilter.c_str(), EmuFilter.size()) != 0)
			{
				delete [] SaveName;
				continue;
			}
		}
		
		if (SaveName)
		{
			if (wcslen(SaveName) > EmuFilter.size() &&
				AvailableSearchChars.find(towupper(SaveName[EmuFilter.size()])) == std::string::npos &&
				AvailableSearchChars.find(towlower(SaveName[EmuFilter.size()])) == std::string::npos)
			{
				AvailableSearchChars.push_back(SaveName[EmuFilter.size()]);
			}
			
			delete [] SaveName;
		}
		
		if(i >= EmuFiltered.size())
		{
			AddFilteredEmu();
		}
		
		pos = EmuFiltered.size()-1;
		
		EmuFiltered.at(pos)->name.assign(EmuFull.at(i)->name);
		EmuFiltered.at(pos)->subname.assign(EmuFull.at(i)->subname);
		EmuFiltered.at(pos)->ID.assign(EmuFull.at(i)->ID);
		EmuFiltered.at(pos)->tid = EmuFull.at(i)->tid;
		EmuFiltered.at(pos)->blocksize = EmuFull.at(i)->blocksize;
		EmuFiltered.at(pos)->isNotInstalled = EmuFull.at(i)->isNotInstalled;
	}

	AvailableSearchChars.push_back(L'\0');

	if (EmuFiltered.size() < 2)
		AvailableSearchChars.clear();
	
	if (AvailableSearchChars.size() > 1)
		std::sort(AvailableSearchChars.begin(), AvailableSearchChars.end(), WCharSortCallback);
	
	return EmuFiltered.size();
}
