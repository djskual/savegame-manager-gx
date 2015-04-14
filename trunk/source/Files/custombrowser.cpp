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

#include "custombrowser.h"
#include "../TextOperations/FreeTypeGX.h"

CustomBrowser::CustomBrowser()
	:FileBrowser()
{
	
}

int CustomBrowser::BrowsePath(std::string path)
{
	if(path.empty())
		return -1;
	
	browserRootDir.assign(path);
	browserRootDir.erase(browserRootDir.find_first_of("/")+1);
	
	browserDir.assign(path);
	while(browserDir[browserDir.size()-1] == '/')
		browserDir.erase(browserDir.size()-1, 1);
	browserDir.erase(0, browserDir.find_first_of("/")+1);
	
	return ParseDirectory();
}

static bool FileSortCallback(const BrowserEntryStruct *f1, const BrowserEntryStruct *f2)
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

void CustomBrowser::ParseDirEntries()
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
		
		//! Filter Files
		if((Filter & FILTER_PATH) && !(filestat.st_mode & S_IFDIR))
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
		//! Other Filters
		if((Filter & FILTER_FONT) && !(filestat.st_mode & S_IFDIR) && (Settings.FileExtensions.CompareFont(fileext) != 0))
			continue;
		if((Filter & FILTER_LANG) && !(filestat.st_mode & S_IFDIR) && (Settings.FileExtensions.CompareLanguageFiles(fileext) != 0))
			continue;
		
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
void CustomBrowser::Refresh()
{
	ParseDirectory();
}

/***************************************************************************
 * Browse subdirectories
 **************************************************************************/
int CustomBrowser::ParseDirectory(bool filteringList UNUSED)
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
	
	gettingList = false;
	
	return browserList.size();
}

/****************************************************************************
 * ChangeDirectory
 *
 * Update current directory and set new entry list if directory has changed
 ***************************************************************************/
int CustomBrowser::ChangeDirectory()
{
	if(!UpdateDirName())
		return -1;

	ParseDirectory();
	
	return browserList.size();
}
