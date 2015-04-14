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
#include <wctype.h>

#include "filebrowser.h"
#include "../DeviceControls/DeviceHandler.hpp"
#include "../TextOperations/FreeTypeGX.h"

/****************************************************************************
 * FileBrowser Class to parse directories on the fly
 ***************************************************************************/
FileBrowser::FileBrowser()
	:Browser()
{
	browserList.clear();
	FileFilter.clear();
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
	FileFilter.clear();
}

void FileBrowser::AddEntry()
{
	BrowserEntryStruct * newEntry = new BrowserEntryStruct;
	newEntry->filename = "";
	newEntry->displayname = L"";
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
	
	FileFilter.clear();
	
	browserRootDir.assign(path);
	browserRootDir.erase(browserRootDir.find_first_of("/")+1);
	
	browserDir.assign(path);
	while(browserDir[browserDir.size()-1] == '/')
		browserDir.erase(browserDir.size()-1, 1);
	browserDir.erase(0, browserDir.find_first_of("/")+1);
	
	return ParseDirectory();
}

/****************************************************************************
 * BrowseDevice
 * Displays a list of files on the selected device
 ***************************************************************************/
int FileBrowser::BrowseDevice(int device)
{
	if(device < 0 || device >= MAXDEVICES)
		return -1;
	
	FileFilter.clear();
	
	browserDir.assign("");
	browserRootDir.assign(fmt("%s:/", DeviceName[device]));
	
	return ParseDirectory();
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
	if(browserPageIndex == ind)
		return;
	
	browserPageIndex = ind;
}

/****************************************************************************
 * SetSelectedIndex
 * not inline for later mutex purpose
 ***************************************************************************/
void FileBrowser::SetSelectedIndex(int ind)
{
	if(browserSelIndex == ind)
		return;
	
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
/*ItemStruct * FileBrowser::GetItemStruct(int pos)
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
}*/

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
static bool /*FileBrowser::*/FileSortCallback(const BrowserEntryStruct *f1, const BrowserEntryStruct *f2)
{
	//! Special case for implicit directories
	if(((BrowserEntryStruct *)f1)->displayname[0] == '.' || ((BrowserEntryStruct *)f2)->displayname[0] == '.')
	{
		if(((BrowserEntryStruct *)f1)->displayname.compare(L".") == 0) { return true; }
		if(((BrowserEntryStruct *)f2)->displayname.compare(L".") == 0) { return false; }
		if(((BrowserEntryStruct *)f1)->displayname.compare(L"..") == 0) { return true; }
		if(((BrowserEntryStruct *)f2)->displayname.compare(L"..") == 0) { return false; }
	}

	//! If one is a file and one is a directory the directory is first
	if(((BrowserEntryStruct *)f1)->isdir && !(((BrowserEntryStruct *)f2)->isdir)) return true;
	if(!(((BrowserEntryStruct *)f1)->isdir) && ((BrowserEntryStruct *)f2)->isdir) return false;

	return (stricmp((wstr2str(((BrowserEntryStruct *)f1)->displayname)).c_str(), (wstr2str(((BrowserEntryStruct *)f2)->displayname)).c_str()) < 0);
}

/****************************************************************************
 * ParseDirEntries
 *
 * Update current directory and set new entry list and entrynum
 ***************************************************************************/
void FileBrowser::ParseDirEntries()
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
		
		char *fileext = strrchr(filename, '.');
		
		if(i >= (int)browserList.size())
			AddEntry();
		
		(browserList.at(i)->filename).assign(filename);
		(browserList.at(i)->displayname).assign(charToWideChar(filename));
		browserList.at(i)->ismii = (!(filestat.st_mode & S_IFDIR) && !(Settings.FileExtensions.CompareMiiFiles(fileext))) ? true : false;
		browserList.at(i)->issave = false;
		browserList.at(i)->isdatabin = false;
		browserList.at(i)->isNotInstalled = false;
		browserList.at(i)->isdir = (filestat.st_mode & S_IFDIR) ? true : false; //! flag this as a dir
		
		i++;
	}
	
	closedir(dir); //! close directory
	dir = NULL;
	
	std::sort(browserList.begin(), browserList.end(), FileSortCallback);
}

/***************************************************************************
 * Browse subdirectories
 **************************************************************************/
void FileBrowser::Refresh()
{
	ParseDirectory();
}

/***************************************************************************
 * Browse subdirectories
 **************************************************************************/
int FileBrowser::ParseDirectory(bool filteringList)
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
		{
			gettingList = false;
			return -1;
		}
	}
	
	currentpath = fulldir;
	
	ParseDirEntries();
	
	if(!filteringList) gettingList = false;
	
	return browserList.size();
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

int FileBrowser::FilterList(std::wstring fileFilter, bool resetBrowser)
{
	if(fileFilter.empty() || resetBrowser) ParseDirectory(true);
	
	if(browserList.empty())
	{
		gettingList = false;
		return 0;
	}
	
	if(!fileFilter.empty()) FileFilter = fileFilter;
	else FileFilter.clear();

	SearchChars.clear();
	
	gettingList = true;
	
	browserSelIndex = 0;
	browserPageIndex = 0;
	
	for (u32 i = 0; i < browserList.size(); ++i)
	{
		std::wstring Filename = browserList.at(i)->displayname;
		if (Filename.empty())
		{
			browserList.erase(browserList.begin()+i);
			i--;
			continue;
		}
		
		if(Settings.SearchMode == SEARCH_BEGINNING)
		{
			if (FileFilter.size() > 0 && wcsncasecmp(Filename.c_str(), FileFilter.c_str(), FileFilter.size()) != 0)
			{
				browserList.erase(browserList.begin()+i);
				i--;
				continue;
			}
			
			if (Filename.size() > FileFilter.size() &&
				SearchChars.find(towupper(Filename[FileFilter.size()])) == SearchChars.end() &&
				SearchChars.find(towlower(Filename[FileFilter.size()])) == SearchChars.end())
			{
				SearchChars.insert(Filename[FileFilter.size()]);
			}
		}
		else if(Settings.SearchMode == SEARCH_CONTENT)
		{
			if(!FileFilter.empty())
			{
				if (wcscasestr(Filename.c_str(), FileFilter.c_str()) == 0)
				{
					browserList.erase(browserList.begin()+i);
					i--;
					continue;
				}
				
				const wchar_t *found = Filename.c_str();
				while((found = wcscasestr(found, FileFilter.c_str())) != 0)
				{
					found += FileFilter.size();
					wchar_t ch = towupper(*found);
					if(ch)
						SearchChars.insert(ch);
				}
			}
			else
			{
				for(const wchar_t *wPtr = Filename.c_str(); *wPtr != 0; wPtr++)
				{
					wchar_t ch = towupper(*wPtr);
					if(ch > L'@')
						SearchChars.insert(ch);
				}
			}
		}
	}

	if (browserList.size() < 2)
		SearchChars.clear();
	
	gettingList = false;
	listChanged = true;
	
	return browserList.size();
}
