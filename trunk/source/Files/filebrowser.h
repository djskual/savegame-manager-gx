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
#ifndef _FILEBROWSER_H_
#define _FILEBROWSER_H_

#include <unistd.h>
#include <gccore.h>
#include <sys/dirent.h>
#include <vector>

#include "Browser.hpp"
#include "../TextOperations/wString.hpp"

typedef struct _BrowserEntryStruct
{
	std::string filename; //! full filename
	std::wstring displayname; //! filename displayed in browser
	bool isdir; //! 0 - file, 1 - directory
	bool ismii; //! 0 - file, 1 - mii
	bool issave; //! 0 - dir, 1 - SaveFolder
	bool isdatabin; //! 0 - file, 1 - databin
	bool isNotInstalled; //! 0 - installed, 1 - not installed
	
} BrowserEntryStruct;

class FileBrowser : public Browser
{
	public:
		FileBrowser();
		~FileBrowser();
		int BrowsePath(std::string path);
		int BrowseDevice(int device);
		void SetPageIndex(int ind);
		void SetSelectedIndex(int ind);
		int GetEntrieCount() { return browserList.size(); }
		int GetPageIndex() { return browserPageIndex; }
		int GetSelIndex() { return browserSelIndex; }
		bool IsDir(int ind) { return browserList.at(ind)->isdir; }
		bool IsSave(int ind) { return browserList.at(ind)->issave; }
		bool IsDataBin(int ind) { return browserList.at(ind)->isdatabin; }
		bool IsMii(int ind) { return browserList.at(ind)->ismii; }
		bool IsNotInstalled(int ind) { return browserList.at(ind)->isNotInstalled; }
		std::string GetItemName(int ind) { return (!(browserList.at(ind)->filename).empty() ? browserList.at(ind)->filename : ""); }
		std::wstring GetItemDisplayName(int ind) { return (!(browserList.at(ind)->displayname).empty() ? browserList.at(ind)->displayname : L""); }
		std::string GetCurrentFilename() { return (!(browserList.at(browserSelIndex)->filename).empty() ? (browserList.at(browserSelIndex)->filename) : ""); }
		std::wstring GetCurrentDisplayname() { return (!(browserList.at(browserSelIndex)->displayname).empty() ? (browserList.at(browserSelIndex)->displayname) : L""); }
		std::string GetRootDir() { return browserRootDir; }
		std::string GetDir() { return browserDir; }
		std::string GetCurrentPath();
		std::string GetCurrentSelectedFilepath();
		//ItemStruct * GetItemStruct(int pos);
		int ChangeDirectory();
		int BackInDirectory() { return LeaveCurDir(); }
		bool IsGettingList() { return gettingList; }
		bool ListChanged() { bool ret = listChanged; listChanged = false; return ret; }
		void Refresh();
		
		int FilterList(std::wstring fileFilter = L"", bool resetBrowser = false);
		std::wstring GetCurrentFilter() { return FileFilter; }
		std::set<wchar_t> GetSearchChars() { return SearchChars; }
		
	protected:
		void ParseDirEntries();
		int ParseDirectory(bool filteringList = false);
		int UpdateDirName();
		void ResetBrowser();
		int EnterSelDir();
		int LeaveCurDir();
		void AddEntry();
		
		std::set<wchar_t> SearchChars;
		std::wstring FileFilter;
		
		//std::string GetMiiDeviceName(const char * miipath);
		std::string browserRootDir; //! root path of browserList
		std::string browserDir; //! directory path of browserList
		int browserSelIndex; //! currently selected index of browserList
		int browserPageIndex; //! starting index of browserList page display
		std::vector<BrowserEntryStruct *> browserList;
		
		DIR * dir;
		std::string currentpath;
		bool gettingList;
		bool listChanged;
};

#endif
