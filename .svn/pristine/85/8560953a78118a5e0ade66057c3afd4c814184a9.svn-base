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
 * filecustombrowser.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include <algorithm>
#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiiuse/wpad.h>
#include <malloc.h>

#include "filecustombrowser.h"
#include "CCustomList.hpp"
#include "../DeviceControls/DeviceHandler.hpp"
#include "../Prompts/PromptWindows.h"
#include "../Prompts/SelectBrowser.h"
#include "../Language/gettext.h"
#include "../FileOperations/fileops.h"
#include "../Settings/CSettings.h"
#include "../Saves/SaveTools.h"
#include "../Saves/DataBin/DataBinTools.h"
#include "../Text/wstring.hpp"
#include "../Tools/StringTools.h"
#include "../XML/GameTDB.hpp"
#include "../menu.h"

/****************************************************************************
 * FileCustomBrowser Class to parse directories on the fly
 ***************************************************************************/
FileCustomBrowser::FileCustomBrowser()
	:Browser()
{
	Filter = 0;
	browserList.clear();
	gettingList = false;
	listChanged = false;
	dir = NULL;
	
	FilterLang = false;
	FilterTheme = false;
	FilterFont = false;
	FilterSound = false;
	
	//!Reset and prepare browser
	ResetBrowser();
}

/****************************************************************************
 * Destructor
 ***************************************************************************/
FileCustomBrowser::~FileCustomBrowser()
{
	for(u32 i = 0; i < browserList.size(); ++i)
		delete browserList.at(i);
	
	browserList.clear();
}

void FileCustomBrowser::AddEntry()
{
	CustomEntryStruct * newEntry = new CustomEntryStruct;
	newEntry->filename = " ";
	newEntry->displayname = " ";
	newEntry->isdir = false;
	newEntry->ismii = false;
	newEntry->issave = false;
	newEntry->isdatabin = false;
	newEntry->isNotInstalled = false;

	browserList.push_back(newEntry);
}

/****************************************************************************
 * BrowsePath
 * Displays a list of files on the selected path
 ***************************************************************************/
int FileCustomBrowser::BrowsePath(std::string path)
{
	if(path.empty())
		return -1;

	BrowserRootDir.assign(path);
	BrowserRootDir.erase(BrowserRootDir.find_first_of("/")+1);
	
	BrowserDir.assign(path);
	while(BrowserDir[BrowserDir.size()-1] == '/')
		BrowserDir.erase(BrowserDir.size()-1, 1);
	BrowserDir.erase(0, BrowserDir.find_first_of("/")+1);
	
	ParseDirectory(); //! Parse root directory
	
	return browserList.size();
}

/****************************************************************************
 * BrowseDevice
 * Displays a list of files on the selected device
 ***************************************************************************/
int FileCustomBrowser::BrowseDevice(int device)
{
	if(device < 0 || device >= MAXDEVICES)
		return -1;

	BrowserDir.clear();
	BrowserRootDir.assign(fmt("%s:/", DeviceName[device]));

	ParseDirectory(); //! Parse root directory
	
	return browserList.size();
}

/****************************************************************************
 * Enter the current selected directory
 ***************************************************************************/
int FileCustomBrowser::EnterSelDir()
{
	if((browserList.at(BrowserSelIndex)->filename).empty())
		return -1;

	int dirlength = BrowserDir.size();
	int filelength = (browserList.at(BrowserSelIndex)->filename).size();
	if((dirlength+filelength+1) > MAXPATHLEN)
		return -1;

	if(dirlength == 0)
		BrowserDir.assign(browserList.at(BrowserSelIndex)->filename);
	else
		BrowserDir.assign(fmt("%s/%s", BrowserDir.c_str(), (browserList.at(BrowserSelIndex)->filename).c_str()));

	listChanged = true;
	
	return 1;
}

/****************************************************************************
 * Leave the current directory
 ***************************************************************************/
int FileCustomBrowser::LeaveCurDir()
{
	int pos = BrowserDir.find_last_of("/");
	if(pos != (int) std::string::npos)
		BrowserDir.erase(pos);
	else
		BrowserDir.clear();
	
	listChanged = true;
	
	return 1;
}

/****************************************************************************
 * UpdateDirName()
 * Update curent directory name for file browser
 ***************************************************************************/
int FileCustomBrowser::UpdateDirName()
{
	if((browserList.at(BrowserSelIndex)->filename).empty())
		return -1;

	if(!(browserList.at(BrowserSelIndex)->filename).compare(".."))
	{
		return LeaveCurDir();
	}

	return EnterSelDir();
}

/****************************************************************************
 * SetPageIndex
 * not inline for later mutex purpose
 ***************************************************************************/
void FileCustomBrowser::SetPageIndex(int ind)
{
	BrowserPageIndex = ind;
}
/****************************************************************************
 * SetSelectedIndex
 * not inline for later mutex purpose
 ***************************************************************************/
void FileCustomBrowser::SetSelectedIndex(int ind)
{
	BrowserSelIndex = ind;
}
/****************************************************************************
 * Get the current full path
 ***************************************************************************/
std::string FileCustomBrowser::GetCurrentPath()
{
	currentpath.assign(BrowserRootDir);
	currentpath += BrowserDir;

	return currentpath;
}
/****************************************************************************
 * Get the current full path with filename
 ***************************************************************************/
std::string FileCustomBrowser::GetCurrentSelectedFilepath()
{
	currentpath.clear();
	
	if((browserList.at(BrowserSelIndex)->filename).size())
	{
		currentpath.assign(BrowserRootDir);
		currentpath += BrowserDir;
		currentpath += "/";
		currentpath += browserList.at(BrowserSelIndex)->filename;
	}
	
	return currentpath;
}

/****************************************************************************
 * Get the current item structure
 ***************************************************************************/
ItemStruct * FileCustomBrowser::GetItemStruct(int pos)
{
	if(pos < 0 || pos >= (int) browserList.size() || (browserList.at(pos)->filename).empty())
		return NULL;

	ItemStruct * Item = new ItemStruct;
	memset(Item, 0, sizeof(ItemStruct));

	Item->itempath = (char *) malloc(BrowserRootDir.size()+BrowserDir.size()+(browserList.at(pos)->filename).size()+2);
	if(Item->itempath)
		sprintf(Item->itempath, "%s%s/%s", BrowserRootDir.c_str(), BrowserDir.c_str(), (browserList.at(pos)->filename).c_str());
	Item->isdir = browserList.at(pos)->isdir;
	Item->itemindex = pos;

	return Item;
}

/****************************************************************************
 * ResetBrowser()
 * Clears the file browser memory, and allocates one initial entry
 ***************************************************************************/
void FileCustomBrowser::ResetBrowser()
{
	for(u32 i = 0; i < browserList.size(); ++i)
		delete browserList.at(i);
	
	browserList.clear();

	BrowserSelIndex = 0;
	BrowserPageIndex = 0;
}

/****************************************************************************
 * FileSortCallback
 *
 * Quick sort callback to sort file entries with the following order:
 *   .
 *   ..
 *   <dirs>
 *   <files>
 ***************************************************************************/
static bool FileSortCallback(const CustomEntryStruct *f1, const CustomEntryStruct *f2)
{
	//! Special case for implicit directories
	if(((CustomEntryStruct *)f1)->displayname[0] == '.' || ((CustomEntryStruct *)f2)->displayname[0] == '.')
	{
		if(((CustomEntryStruct *)f1)->displayname.compare(".") == 0) { return true; }
		if(((CustomEntryStruct *)f2)->displayname.compare(".") == 0) { return false; }
		if(((CustomEntryStruct *)f1)->displayname.compare("..") == 0) { return true; }
		if(((CustomEntryStruct *)f2)->displayname.compare("..") == 0) { return false; }
	}

	//! If one is a file and one is a directory the directory is first
	if(((CustomEntryStruct *)f1)->isdir && !(((CustomEntryStruct *)f2)->isdir)) return true;
	if(!(((CustomEntryStruct *)f1)->isdir) && ((CustomEntryStruct *)f2)->isdir) return false;

	return (stricmp(((CustomEntryStruct *)f1)->displayname.c_str(), ((CustomEntryStruct *)f2)->displayname.c_str()) < 0);
}

/****************************************************************************
 * SetFilterCustom()
 * Set filters for custum browsing
 ***************************************************************************/
void FileCustomBrowser::SetFilterCustom(short Type)
{
	if(Type == LANGBROWSER)
		FilterLang = true;
	if(Type == THEMEPATHBROWSER)
		FilterTheme = true;
	if(Type == FONTBROWSER)
		FilterFont = true;
	if(Type == SOUNDBROWSER)
		FilterSound = true;
}

/***************************************************************************
 * Get Mii Name
 **************************************************************************/
std::string FileCustomBrowser::GetMiiDeviceName(const char * miipath)
{
	std::string name;
	
	FILE *fp = fopen(miipath,"rb");

	if(fp == NULL)
		return "";
	
	fseek(fp , 2, SEEK_SET);
	u16* namedata = (u16*)memalign(32, 20);
	memset(namedata, 0, sizeof(namedata));
	
	fread(namedata,1,20,fp);
	
	fclose(fp);
	
	wchar_t wname[20];
	memset(wname, 0, 20);
	
	u32 i = 0;
	for (i = 0; i < 20; i++){
		wname[i] = (wchar_t)namedata[i];
	}
	
	free(namedata);
	namedata = NULL;
	
	wString *ws_name = new wString(wname);
	
	name.assign(ws_name->toUTF8());
	
	delete ws_name;
	
	return name;
}

/****************************************************************************
 * ParseDirEntries
 *
 * Update current directory and set new entry list and entrynum
 ***************************************************************************/
void FileCustomBrowser::ParseDirEntries(bool ResetPosition)
{
	if(!dir)
		return;
	
	char filename[MAXPATHLEN];
	struct stat filestat;
	struct dirent * dirent = NULL;
	
	int i = 0;
	
	std::string Path = currentpath;
	if(Path[Path.size()-1] == '/')
		Path.erase(Path.size()-1, 1);
	
	while ((dirent = readdir(dir)) != 0)
	{
		snprintf(filename, sizeof(filename), "%s/%s", Path.c_str(), dirent->d_name);
		
		if(stat(filename, &filestat) != 0)
			continue;
 		
		snprintf(filename, sizeof(filename), dirent->d_name);
		
		if(strcmp(filename,".") == 0)
			continue;
		
		//! Hide System Files
		if(filename[0] == '.' && strcmp(filename,"..") != 0)
			continue;
		else if(filename[0] == '$')
			continue;
		else if(strcasecmp(filename,"thumb.db") == 0)
			continue;
		
		//! Filters
		if((Filter & FILTER_DIRECTORIES) && (filestat.st_mode & S_IFDIR))
			continue;
		if((Filter & FILTER_FILES) && !(filestat.st_mode & S_IFDIR))
			continue;
		
		char *fileext = strrchr(filename, '.');
		if (FilterLang && !(filestat.st_mode & S_IFDIR) && (Settings.FileExtensions.CompareLanguageFiles(fileext) != 0))
			continue;
		if (FilterFont && !(filestat.st_mode & S_IFDIR) && (Settings.FileExtensions.CompareFont(fileext) != 0))
			continue;
		if (FilterSound && !(filestat.st_mode & S_IFDIR) && (Settings.FileExtensions.CompareAudio(fileext) != 0))
			continue;
		if (FilterTheme && (filestat.st_mode & S_IFDIR) && (strcasecmp(filename,"tmp") == 0))
			continue;
		
		if(i >= (int)browserList.size())
			AddEntry();
		
		(browserList.at(i)->filename).assign(filename);
		(browserList.at(i)->displayname).assign(filename);
		browserList.at(i)->ismii = (!(filestat.st_mode & S_IFDIR) && !(Settings.FileExtensions.CompareMiiFiles(fileext))) ? true : false;
		browserList.at(i)->issave = false;
		browserList.at(i)->isNotInstalled = false;
		browserList.at(i)->isdir = (filestat.st_mode & S_IFDIR) ? true : false;
		
		//! Check if dir is a decompressed save
		if( browserList.at(i)->isdir == true &&
			browserList.at(i)->filename.size() == 16)
		{
			char savepath[MAXPATHLEN];
			strcpy(savepath, fmt("%s%s/%s", BrowserRootDir.c_str(), BrowserDir.c_str(), (browserList.at(i)->filename).c_str()));
			
			//! Get save infos
			if( CheckSave(savepath) == true )
			{
				//! flag this as a save
				browserList.at(i)->isdir = false;
				browserList.at(i)->issave = true;
				
				//! Check if the save is already installed
				browserList.at(i)->isNotInstalled = !CheckIfInstalled(StrToHex64(browserList.at(i)->filename.c_str()));
				
				//! Get name of the save
				(browserList.at(i)->displayname).assign(GetDeviceName(savepath));
			}
		}
		//! Check if .bin is a compressed save
		else if (!browserList.at(i)->isdir &&
				 !Settings.FileExtensions.CompareBinaryFiles(fileext))
		{
			char databinpath[MAXPATHLEN];
			strcpy(databinpath, fmt("%s%s/%s", BrowserRootDir.c_str(), BrowserDir.c_str(), (browserList.at(i)->filename).c_str()));
			
			SaveInfos * infos = GetSaveInfos(databinpath);
			if(infos)
			{
				browserList.at(i)->isdatabin = true;
				browserList.at(i)->displayname.assign(infos->name);
				browserList.at(i)->isNotInstalled = !CheckIfInstalled(infos->tid);
				
				if(Settings.GameTDBTitles)
				{
					std::string Title;
					std::string Filepath = Settings.GameTDBPath;
					if(Filepath[Filepath.size()-1] != '/')
						Filepath += '/';
					Filepath += "wiitdb.xml";
					
					GameTDB * XML_DB = new GameTDB(Filepath.c_str());
					XML_DB->SetLanguageCode(Settings.GameTDBLanguageCode);
					
					if(XML_DB->GetTitle(infos->ID, Title))
						browserList.at(i)->displayname.assign(Title);
					
					delete XML_DB;
				}
				delete infos;
			}
		}
		
		//! Get Mii infos
		if(browserList.at(i)->ismii == true)
		{
			char miipath[MAXPATHLEN];
			strcpy(miipath, fmt("%s%s/%s", BrowserRootDir.c_str(), BrowserDir.c_str(), (browserList.at(i)->filename).c_str()));
			
			(browserList.at(i)->displayname).assign(GetMiiDeviceName(miipath));
		}
		i++;
	}
	
	closedir(dir); //! close directory
	dir = NULL;
	
	if(!browserList.empty())
	{
		for(u32 cnt = 0; cnt < browserList.size(); cnt++)
		{
			CustomList.SetFile (cnt,
								browserList.at(cnt)->filename,
								browserList.at(cnt)->displayname,
								browserList.at(cnt)->isdir,
								browserList.at(cnt)->ismii,
								browserList.at(cnt)->issave,
								browserList.at(cnt)->isdatabin,
								browserList.at(cnt)->isNotInstalled);
		}
		
		CustomList.LoadTitlesFromGameTDB();
	}
	
	u32 currPageIndex = BrowserPageIndex;
	u32 currSelIndex = BrowserSelIndex;

	ResetBrowser();

	if(!ResetPosition)
	{
		BrowserPageIndex = currPageIndex;
		BrowserSelIndex = currSelIndex;
	}
}

/***************************************************************************
 * Refresh Browser
 **************************************************************************/
void FileCustomBrowser::Refresh()
{
	ParseDirectory(false);
}

/***************************************************************************
 * Browse subdirectories
 **************************************************************************/
int FileCustomBrowser::ParseDirectory(bool ResetPosition)
{
	gettingList = true;
	
	ResetBrowser();

	std::string fulldir(BrowserRootDir);
	fulldir += BrowserDir;
	
	//! open the directory
	dir = opendir(fulldir.c_str());
	if(dir == NULL)
	{
		//! if we can't open the dir, try opening the root dir
		BrowserDir.clear();
		fulldir = BrowserRootDir;
		
		dir = opendir(fulldir.c_str());
		if(dir == NULL)
			return -1;
	}
	
	currentpath = fulldir;
	
	ParseDirEntries(ResetPosition); 
	
	FilterList();
	
	gettingList = false;
	
	return browserList.size();
}

void FileCustomBrowser::FilterList()
{
	wString oldFilter(CustomList.GetCurrentFilter());
	CustomList.FilterList(oldFilter.c_str());
	
	for(int i=0; i < CustomList.GetFilteredCount(); i++)
	{
		if(i >= (int)browserList.size())
			AddEntry();
		
		(browserList.at(i)->filename).assign(CustomList.GetFilename(i));
		(browserList.at(i)->displayname).assign(CustomList.GetDisplayname(i));
		browserList.at(i)->isdir = CustomList.GetIsDir(i);
		browserList.at(i)->ismii = CustomList.GetIsMii(i);
		browserList.at(i)->issave = CustomList.GetIsSave(i);
		browserList.at(i)->isdatabin = CustomList.GetIsDataBin(i);
		browserList.at(i)->isNotInstalled = CustomList.GetIsNotInstalled(i);
	}
	
	CustomList.ResetFiltered();
	
	std::sort(browserList.begin(), browserList.end(), FileSortCallback);
	
	for(u32 cnt = 0; cnt < browserList.size(); cnt++)
	{
		CustomList.SetFilteredFile(cnt,
								   browserList.at(cnt)->filename,
								   browserList.at(cnt)->displayname,
								   browserList.at(cnt)->isdir,
								   browserList.at(cnt)->ismii,
								   browserList.at(cnt)->issave,
								   browserList.at(cnt)->isdatabin,
								   browserList.at(cnt)->isNotInstalled);
	}
}

/****************************************************************************
 * ChangeDirectory
 *
 * Update current directory and set new entry list if directory has changed
 ***************************************************************************/
int FileCustomBrowser::ChangeDirectory()
{
	if(!UpdateDirName())
		return -1;

	ParseDirectory();

	return browserList.size();
}
