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
 * CFileList.cpp
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
#include "CFileList.hpp"

CFileList FileList;

void CFileList::AddFullFile()
{
	FileStruct * newFile = new FileStruct;
	newFile->filename = "";
	newFile->displayname = "";
	newFile->isdir = false;
	newFile->ismii = false;
	newFile->issave = false;
	newFile->isdatabin = false;
	newFile->isNotInstalled = false;

	FilesFull.push_back(newFile);
}

void CFileList::AddFilteredFile()
{
	FileStruct * newFile = new FileStruct;
	newFile->filename = "";
	newFile->displayname = "";
	newFile->isdir = false;
	newFile->ismii = false;
	newFile->issave = false;
	newFile->isdatabin = false;
	newFile->isNotInstalled = false;

	FilesFiltered.push_back(newFile);
}

void CFileList::RemoveEntry(int ind)
{
	if(ind < 0 || ind >= (int) FilesFiltered.size())
		return;
	
	FilesFiltered.erase(FilesFiltered.begin() + ind);
}

std::string CFileList::GetFilename(int ind)
{
	if(ind < 0 || ind >= (int) FilesFiltered.size())
		return NULL;

	return FilesFiltered.at(ind)->filename;
}

std::string CFileList::GetDisplayname(int ind)
{
	if(ind < 0 || ind >= (int) FilesFiltered.size())
		return NULL;

	return FilesFiltered.at(ind)->displayname;
}

bool CFileList::GetIsDir(int ind)
{
	if(ind < 0 || ind >= (int) FilesFiltered.size())
		return 0;

	return FilesFiltered.at(ind)->isdir;
}

bool CFileList::GetIsMii(int ind)
{
	if(ind < 0 || ind >= (int) FilesFiltered.size())
		return 0;

	return FilesFiltered.at(ind)->ismii;
}

bool CFileList::GetIsSave(int ind)
{
	if(ind < 0 || ind >= (int) FilesFiltered.size())
		return 0;

	return FilesFiltered.at(ind)->issave;
}

bool CFileList::GetIsDataBin(int ind)
{
	if(ind < 0 || ind >= (int) FilesFiltered.size())
		return 0;

	return FilesFiltered.at(ind)->isdatabin;
}

bool CFileList::GetIsNotInstalled(int ind)
{
	if(ind < 0 || ind >= (int) FilesFiltered.size())
		return 0;

	return FilesFiltered.at(ind)->isNotInstalled;
}


void CFileList::ResetFull()
{
	for(u32 i = 0; i < FilesFull.size(); i++)
		delete FilesFull.at(i);

	FilesFull.clear();
}

void CFileList::ResetFiltered()
{
	for(u32 i = 0; i < FilesFiltered.size(); i++)
		delete FilesFiltered.at(i);

	FilesFiltered.clear();
}

int CFileList::SetFile (int i,
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
	
	(FilesFull.at(cnt)->filename).assign(filename);
	(FilesFull.at(cnt)->displayname).assign(displayname);
	FilesFull.at(cnt)->isdir = isdir;
	FilesFull.at(cnt)->ismii = ismii;
	FilesFull.at(cnt)->issave = issave;
	FilesFull.at(cnt)->isdatabin = isdatabin;
	FilesFull.at(cnt)->isNotInstalled = isNotInstalled;
	
	return FilesFull.size();
}

int CFileList::SetFilteredFile (int i,
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
	
	(FilesFiltered.at(cnt)->filename).assign(filename);
	(FilesFiltered.at(cnt)->displayname).assign(displayname);
	FilesFiltered.at(cnt)->isdir = isdir;
	FilesFiltered.at(cnt)->ismii = ismii;
	FilesFiltered.at(cnt)->issave = issave;
	FilesFiltered.at(cnt)->isdatabin = isdatabin;
	FilesFiltered.at(cnt)->isNotInstalled = isNotInstalled;
	
	return FilesFiltered.size();
}

void CFileList::LoadTitlesFromGameTDB()
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
		if(FilesFull.at(ind)->issave)
		{
			u64 tid = StrToHex64((FilesFull.at(ind)->filename).c_str());
			std::string ID = titleText((u32)(tid>> 32),(u32)(tid & 0xFFFFFFFF));
			
			if(XML_DB->GetTitle(ID.c_str(), Title))
				(FilesFull.at(ind)->displayname).assign(Title);
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

int CFileList::FilterList(const wchar_t * fileFilter)
{
	int cnt = 0;
	
	ResetFiltered();
	
	if (FilesFull.empty()) return 0;
	if (fileFilter) FileFilter.assign(fileFilter);

	FilesFiltered.clear();
	AvailableSearchChars.clear();

	for (u32 i = 0; i < FilesFull.size(); ++i)
	{
		wchar_t *Filename = charToWideChar((FilesFull.at(i)->displayname).c_str());
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
		
		(FilesFiltered.at(cnt)->filename).assign(FilesFull.at(i)->filename);
		(FilesFiltered.at(cnt)->displayname).assign(FilesFull.at(i)->displayname);
		FilesFiltered.at(cnt)->isdir = FilesFull.at(i)->isdir;
		FilesFiltered.at(cnt)->ismii = FilesFull.at(i)->ismii;
		FilesFiltered.at(cnt)->issave = FilesFull.at(i)->issave;
		FilesFiltered.at(cnt)->isdatabin = FilesFull.at(i)->isdatabin;
		FilesFiltered.at(cnt)->isNotInstalled = FilesFull.at(i)->isNotInstalled;
	}

	AvailableSearchChars.push_back(L'\0');

	if (FilesFiltered.size() < 2)
		AvailableSearchChars.clear();
	
	if (AvailableSearchChars.size() > 1)
		std::sort(AvailableSearchChars.begin(), AvailableSearchChars.end(), WCharSortCallback);

	ResetFull();
	
	return FilesFiltered.size();
}
