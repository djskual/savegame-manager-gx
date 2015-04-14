/****************************************************************************
 * Copyright (C) 2009
 * by Dimok
 * modified by Dj_Skual
 * for SaveGame Manager Gx 2011
 *
 * Original Filebrowser by Tantric for libwiigui
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
 * FileBrowser
 *
 * Directory parsing Class
 * for WiiXplorer 2009
 ***************************************************************************/
#include <algorithm>
#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiiuse/wpad.h>
#include <malloc.h>

#include "filebrowser.h"
#include "CFileList.hpp"
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
 * FileBrowser Class to parse directories on the fly
 ***************************************************************************/
FileBrowser::FileBrowser()
	:Browser()
{
	Filter = 0;
	browserList.clear();
	gettingList = false;
	listChanged = false;
	dir = NULL;
	
	//!Reset and prepare browser
	ResetBrowser();
}

/****************************************************************************
 * Destructor
 ***************************************************************************/
FileBrowser::~FileBrowser()
{
	for(u32 i = 0; i < browserList.size(); ++i)
		delete browserList.at(i);
	
	browserList.clear();
}

void FileBrowser::AddEntry()
{
	BrowserEntryStruct * newEntry = new BrowserEntryStruct;
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
int FileBrowser::BrowsePath(std::string path)
{
	if(path.empty())
		return -1;

	browserRootDir.assign(path);
	browserRootDir.erase(browserRootDir.find_first_of("/")+1);
	
	browserDir.assign(path);
	while(browserDir[browserDir.size()-1] == '/')
		browserDir.erase(browserDir.size()-1, 1);
	browserDir.erase(0, browserDir.find_first_of("/")+1);
	
	ParseDirectory(); //! Parse directory
	
	return browserList.size();
}

/****************************************************************************
 * BrowseDevice
 * Displays a list of files on the selected device
 ***************************************************************************/
int FileBrowser::BrowseDevice(int device)
{
	if(device < 0 || device >= MAXDEVICES)
		return -1;

	browserDir.clear();
	browserRootDir.assign(fmt("%s:/", DeviceName[device]));

	ParseDirectory(); //! Parse root directory
	return browserList.size();
}

/****************************************************************************
 * Enter the current selected directory
 ***************************************************************************/
int FileBrowser::EnterSelDir()
{
	if(browserList.at(browserSelIndex)->filename.empty())
		return -1;

	int dirlength = browserDir.size();
	int filelength = (browserList.at(browserSelIndex)->filename).size();
	if((dirlength+filelength+1) > MAXPATHLEN)
		return -1;

	if(dirlength == 0)
		browserDir.assign(browserList.at(browserSelIndex)->filename);
	else
		browserDir.assign(fmt("%s/%s", browserDir.c_str(), (browserList.at(browserSelIndex)->filename).c_str()));
	
	listChanged = true;
	
	return 1;
}

/****************************************************************************
 * Leave the current directory
 ***************************************************************************/
int FileBrowser::LeaveCurDir()
{
	int pos = browserDir.find_last_of("/");
	if(pos != (int) std::string::npos)
		browserDir.erase(pos);
	else
		browserDir.clear();
	
	listChanged = true;
	
	return 1;
}

/****************************************************************************
 * UpdateDirName()
 * Update curent directory name for file browser
 ***************************************************************************/
int FileBrowser::UpdateDirName()
{
	if((browserList.at(browserSelIndex)->filename).empty())
		return -1;

	if(!(browserList.at(browserSelIndex)->filename).compare(".."))
		return LeaveCurDir();

	return EnterSelDir();
}

/****************************************************************************
 * SetPageIndex
 * not inline for later mutex purpose
 ***************************************************************************/
void FileBrowser::SetPageIndex(int ind)
{
	browserPageIndex = ind;
}
/****************************************************************************
 * SetSelectedIndex
 * not inline for later mutex purpose
 ***************************************************************************/
void FileBrowser::SetSelectedIndex(int ind)
{
	browserSelIndex = ind;
}
/****************************************************************************
 * Get the current full path
 ***************************************************************************/
std::string FileBrowser::GetCurrentPath()
{
	currentpath.assign(browserRootDir);
	currentpath += browserDir;

	return currentpath;
}
/****************************************************************************
 * Get the current full path with filename
 ***************************************************************************/
std::string FileBrowser::GetCurrentSelectedFilepath()
{
	currentpath.clear();
	
	if((browserList.at(browserSelIndex)->filename).size())
	{
		currentpath.assign(browserRootDir);
		currentpath += browserDir;
		currentpath += "/";
		currentpath += browserList.at(browserSelIndex)->filename;
	}

	return currentpath;
}

/****************************************************************************
 * Get the current item structure
 ***************************************************************************/
ItemStruct * FileBrowser::GetItemStruct(int pos)
{
	if(pos < 0 || pos >= (int) browserList.size() || !(browserList.at(pos)->filename).empty())
		return NULL;

	ItemStruct * Item = new ItemStruct;
	memset(Item, 0, sizeof(ItemStruct));

	Item->itempath = (char *) malloc(browserRootDir.size()+browserDir.size()+(browserList.at(pos)->filename).size()+2);
	if(Item->itempath)
		sprintf(Item->itempath, "%s%s/%s", browserRootDir.c_str(), browserDir.c_str(), (browserList.at(pos)->filename).c_str());
	Item->isdir = browserList.at(pos)->isdir;
	Item->itemindex = pos;

	return Item;
}

/****************************************************************************
 * ResetBrowser()
 * Clears the file browser memory, and allocates one initial entry
 ***************************************************************************/
void FileBrowser::ResetBrowser()
{
	for(u32 i = 0; i < browserList.size(); ++i)
		delete browserList.at(i);
	
	browserList.clear();

	browserSelIndex = 0;
	browserPageIndex = 0;
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
static bool FileSortCallback(const BrowserEntryStruct *f1, const BrowserEntryStruct *f2)
{
	//! Special case for implicit directories
	if(((BrowserEntryStruct *)f1)->displayname[0] == '.' || ((BrowserEntryStruct *)f2)->displayname[0] == '.')
	{
		if(((BrowserEntryStruct *)f1)->displayname.compare(".") == 0) { return true; }
		if(((BrowserEntryStruct *)f2)->displayname.compare(".") == 0) { return false; }
		if(((BrowserEntryStruct *)f1)->displayname.compare("..") == 0) { return true; }
		if(((BrowserEntryStruct *)f2)->displayname.compare("..") == 0) { return false; }
	}

	//! If one is a file and one is a directory the directory is first
	if(((BrowserEntryStruct *)f1)->isdir && !(((BrowserEntryStruct *)f2)->isdir)) return true;
	if(!(((BrowserEntryStruct *)f1)->isdir) && ((BrowserEntryStruct *)f2)->isdir) return false;

	return (stricmp(((BrowserEntryStruct *)f1)->displayname.c_str(), ((BrowserEntryStruct *)f2)->displayname.c_str()) < 0);
}

/***************************************************************************
 * Get Mii Name
 **************************************************************************/
std::string FileBrowser::GetMiiDeviceName(const char * miipath)
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
	
	name = (ws_name->toUTF8()).c_str();
	
	delete ws_name;
	
	return name;
}

/****************************************************************************
 * ParseDirEntries
 *
 * Update current directory and set new entry list and entrynum
 ***************************************************************************/
void FileBrowser::ParseDirEntries(bool ResetPosition)
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
		
		if(i >= (int)browserList.size())
			AddEntry();
		
		(browserList.at(i)->filename).assign(filename);
		(browserList.at(i)->displayname).assign(filename);
		browserList.at(i)->ismii = (!(filestat.st_mode & S_IFDIR) && !(Settings.FileExtensions.CompareMiiFiles(fileext))) ? true : false;
		browserList.at(i)->issave = false;
		browserList.at(i)->isdatabin = false;
		browserList.at(i)->isNotInstalled = false;
		browserList.at(i)->isdir = (filestat.st_mode & S_IFDIR) ? true : false; // flag this as a dir
		
		//! Check if dir is a decompressed save
		if (browserList.at(i)->isdir == true &&
			browserList.at(i)->filename.size() == 16)
		{
			char savepath[MAXPATHLEN];
			strcpy(savepath, fmt("%s%s/%s", browserRootDir.c_str(), browserDir.c_str(), (browserList.at(i)->filename).c_str()));
			
			//! Get save infos
			if( CheckSave(savepath) == true )
			{
				//! flag this as a save
				browserList.at(i)->isdir = false;
				browserList.at(i)->issave = true;
				
				//! Check if the save is already installed
				browserList.at(i)->isNotInstalled = !CheckIfInstalled(StrToHex64((browserList.at(i)->filename).c_str()));
				
				//! Get name and Subname of the save
				(browserList.at(i)->displayname).assign(GetDeviceName(savepath));
			}
		}
		//! Check if .bin is a compressed save
		else if (!browserList.at(i)->isdir &&
				 !Settings.FileExtensions.CompareBinaryFiles(fileext))
		{
			char databinpath[MAXPATHLEN];
			strcpy(databinpath, fmt("%s%s/%s", browserRootDir.c_str(), browserDir.c_str(), (browserList.at(i)->filename).c_str()));
			
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
			strcpy(miipath, fmt("%s%s/%s", browserRootDir.c_str(), browserDir.c_str(), (browserList.at(i)->filename).c_str()));
			
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
			FileList.SetFile(cnt,
							 browserList.at(cnt)->filename,
							 browserList.at(cnt)->displayname,
							 browserList.at(cnt)->isdir,
							 browserList.at(cnt)->ismii,
							 browserList.at(cnt)->issave,
							 browserList.at(cnt)->isdatabin,
							 browserList.at(cnt)->isNotInstalled);
		}
		
		FileList.LoadTitlesFromGameTDB();
	}
	
	u32 currPageIndex = browserPageIndex;
	u32 currSelIndex = browserSelIndex;

	ResetBrowser();

	if(!ResetPosition)
	{
		browserPageIndex = currPageIndex;
		browserSelIndex = currSelIndex;
	}
}

/***************************************************************************
 * Browse subdirectories
 **************************************************************************/
void FileBrowser::Refresh()
{
	ParseDirectory(false);
}

/***************************************************************************
 * Browse subdirectories
 **************************************************************************/
int FileBrowser::ParseDirectory(bool ResetPosition)
{
	gettingList = true;
	
	ResetBrowser();

	std::string fulldir(browserRootDir);
	fulldir += browserDir;
	
	//! open the directory
	dir = opendir(fulldir.c_str());
	if(dir == NULL)
	{
		//! if we can't open the dir, try opening the root dir
		browserDir.clear();
		fulldir = browserRootDir;
		
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

void FileBrowser::FilterList()
{
	wString oldFilter(FileList.GetCurrentFilter());
	FileList.FilterList(oldFilter.c_str());
	
	for(int i = 0; i < FileList.GetFilteredCount(); i++)
	{
		if(i >= (int)browserList.size())
			AddEntry();
		
		(browserList.at(i)->filename).assign(FileList.GetFilename(i));
		(browserList.at(i)->displayname).assign(FileList.GetDisplayname(i));
		browserList.at(i)->isdir = FileList.GetIsDir(i);
		browserList.at(i)->ismii = FileList.GetIsMii(i);
		browserList.at(i)->issave = FileList.GetIsSave(i);
		browserList.at(i)->isdatabin = FileList.GetIsDataBin(i);
		browserList.at(i)->isNotInstalled = FileList.GetIsNotInstalled(i);
	}
	
	FileList.ResetFiltered();
	
	std::sort(browserList.begin(), browserList.end(), FileSortCallback);
	
	for(u32 cnt = 0; cnt < browserList.size(); cnt++)
	{
		FileList.SetFilteredFile(cnt,
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
int FileBrowser::ChangeDirectory()
{
	if(!UpdateDirName())
		return -1;

	ParseDirectory();
	
	return browserList.size();
}
