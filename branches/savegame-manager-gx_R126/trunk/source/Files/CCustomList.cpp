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
 * CCustomList.cpp
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
#include "../Text/FreeTypeGX.h"
#include "../Saves/SaveTools.h"
#include "CCustomList.hpp"

CCustomList CustomList;

void CCustomList::AddFullFile()
{
	FileCustomStruct * newFile = new FileCustomStruct;
	newFile->filename = "";
	newFile->displayname = "";
	newFile->isdir = false;
	newFile->ismii = false;
	newFile->issave = false;
	newFile->isdatabin = false;
	newFile->isNotInstalled = false;

	FilesFull.push_back(newFile);
}

void CCustomList::AddFilteredFile()
{
	FileCustomStruct * newFile = new FileCustomStruct;
	newFile->filename = "";
	newFile->displayname = "";
	newFile->isdir = false;
	newFile->ismii = false;
	newFile->issave = false;
	newFile->isdatabin = false;
	newFile->isNotInstalled = false;

	FilesFiltered.push_back(newFile);
}

void CCustomList::RemoveEntry(int ind)
{
	if(ind < 0 || ind >= (int) FilesFiltered.size())
		return;
	
	FilesFiltered.erase(FilesFiltered.begin() + ind);
}

std::string CCustomList::GetFilename(int ind)
{
	if(ind < 0 || ind >= (int) FilesFiltered.size())
		return NULL;

	return FilesFiltered.at(ind)->filename;
}

std::string CCustomList::GetDisplayname(int ind)
{
	if(ind < 0 || ind >= (int) FilesFiltered.size())
		return NULL;

	return FilesFiltered.at(ind)->displayname;
}

bool CCustomList::GetIsDir(int ind)
{
	if(ind < 0 || ind >= (int) FilesFiltered.size())
		return 0;

	return FilesFiltered.at(ind)->isdir;
}

bool CCustomList::GetIsMii(int ind)
{
	if(ind < 0 || ind >= (int) FilesFiltered.size())
		return 0;

	return FilesFiltered.at(ind)->ismii;
}

bool CCustomList::GetIsSave(int ind)
{
	if(ind < 0 || ind >= (int) FilesFiltered.size())
		return 0;

	return FilesFiltered.at(ind)->issave;
}

bool CCustomList::GetIsDataBin(int ind)
{
	if(ind < 0 || ind >= (int) FilesFiltered.size())
		return 0;

	return FilesFiltered.at(ind)->isdatabin;
}

bool CCustomList::GetIsNotInstalled(int ind)
{
	if(ind < 0 || ind >= (int) FilesFiltered.size())
		return 0;

	return FilesFiltered.at(ind)->isNotInstalled;
}


void CCustomList::ResetFull()
{
	for(u32 i = 0; i < FilesFull.size(); i++)
	{
		delete FilesFull.at(i);
	}

	FilesFull.clear();
}

void CCustomList::ResetFiltered()
{
	for(u32 i = 0; i < FilesFiltered.size(); i++)
	{
		delete FilesFiltered.at(i);
	}

	FilesFiltered.clear();
}

int CCustomList::SetFile (int i,
						  std::string filename,
						  std::string displayname,
						  bool isdir,
						  bool ismii,
						  bool issave,
						  bool isdatabin,
						  bool isNotInstalled)
{
	if(i >= (int)FilesFull.size())
		AddFullFile();
		
	u32 cnt = FilesFull.size()-1;
	
	FilesFull[cnt]->filename.assign(filename);
	FilesFull[cnt]->displayname.assign(displayname);
	FilesFull[cnt]->isdir = isdir;
	FilesFull[cnt]->ismii = ismii;
	FilesFull[cnt]->issave = issave;
	FilesFull[cnt]->isdatabin = isdatabin;
	FilesFull[cnt]->isNotInstalled = isNotInstalled;
	
	return FilesFull.size();
}

int CCustomList::SetFilteredFile (int i,
								  std::string filename,
								  std::string displayname,
								  bool isdir,
								  bool ismii,
								  bool issave,
								  bool isdatabin,
								  bool isNotInstalled)
{
	if(i >= (int)FilesFiltered.size())
		AddFilteredFile();
		
	u32 cnt = FilesFiltered.size()-1;
	
	FilesFiltered[cnt]->filename.assign(filename);
	FilesFiltered[cnt]->displayname.assign(displayname);
	FilesFiltered[cnt]->isdir = isdir;
	FilesFiltered[cnt]->ismii = ismii;
	FilesFiltered[cnt]->issave = issave;
	FilesFiltered[cnt]->isdatabin = isdatabin;
	FilesFiltered[cnt]->isNotInstalled = isNotInstalled;
	
	return FilesFiltered.size();
}

void CCustomList::LoadTitlesFromGameTDB()
{
	if(!Settings.GameTDBTitles)
		return;

	std::string Title;
	std::string Filepath = Settings.GameTDBPath;
	if(Filepath[Filepath.size()-1] != '/')
		Filepath += '/';
	Filepath += "wiitdb.xml";

	GameTDB * XML_DB = new GameTDB(Filepath.c_str());
	XML_DB->SetLanguageCode(Settings.GameTDBLanguageCode);

	for(u32 ind = 0; ind < FilesFull.size(); ind++)
	{
		if(FilesFull[ind]->issave == true)
		{
			u64 tid = StrToHex64((FilesFull[ind]->filename).c_str());
			std::string ID = titleText((u32)(tid>> 32),(u32)(tid & 0xFFFFFFFF));
			
			if(XML_DB->GetTitle(ID.c_str(), Title))
				FilesFull[ind]->displayname.assign(Title);
		}
	}
	
	delete XML_DB;
}

static bool WCharSortCallback(const wchar_t char1, const wchar_t char2)
{
	if (char2 == 0) return true;
	if (char1 == 0) return false;

	return char2 > char1;
}

int CCustomList::FilterList(const wchar_t * fileFilter)
{
	int cnt = 0;
	
	ResetFiltered();
	
	if (FilesFull.empty()) return 0;
	if (fileFilter) FileFilter.assign(fileFilter);

	FilesFiltered.clear();
	AvailableSearchChars.clear();

	for (u32 i = 0; i < FilesFull.size(); ++i)
	{
		wchar_t *Filename = charToWideChar(FilesFull[i]->displayname.c_str());
		if (Filename && *FileFilter.c_str())
		{
			if (wcsnicmp(Filename, FileFilter.c_str(), FileFilter.size()) != 0)
			{
				delete [] Filename;
				continue;
			}
		}

		if (Filename)
		{
			if (wcslen(Filename) > FileFilter.size() &&
				AvailableSearchChars.find(towupper(Filename[FileFilter.size()])) == std::string::npos &&
				AvailableSearchChars.find(towlower(Filename[FileFilter.size()])) == std::string::npos)
			{
				AvailableSearchChars.push_back(Filename[FileFilter.size()]);
			}

			delete [] Filename;
		}

		if(i >= FilesFiltered.size())
		{
			AddFilteredFile();
		}
		
		cnt = FilesFiltered.size()-1;
		
		FilesFiltered[cnt]->filename.assign(FilesFull[i]->filename);
		FilesFiltered[cnt]->displayname.assign(FilesFull[i]->displayname);
		FilesFiltered[cnt]->isdir = FilesFull[i]->isdir;
		FilesFiltered[cnt]->ismii = FilesFull[i]->ismii;
		FilesFiltered[cnt]->issave = FilesFull[i]->issave;
		FilesFiltered[cnt]->isdatabin = FilesFull[i]->isdatabin;
		FilesFiltered[cnt]->isNotInstalled = FilesFull[i]->isNotInstalled;
	}

	AvailableSearchChars.push_back(L'\0');

	if (FilesFiltered.size() < 2)
		AvailableSearchChars.clear();
	
	if (AvailableSearchChars.size() > 1)
		std::sort(AvailableSearchChars.begin(), AvailableSearchChars.end(), WCharSortCallback);

	ResetFull();
	
	return FilesFiltered.size();
}
