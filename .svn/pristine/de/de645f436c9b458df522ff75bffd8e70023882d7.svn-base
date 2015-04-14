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
 * filecustombrowser.h
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#ifndef _FILECUSTOMBROWSER_H_
#define _FILECUSTOMBROWSER_H_

#include <unistd.h>
#include <gccore.h>
#include <sys/dirent.h>

#define FILTER_DIRECTORIES	0x01
#define FILTER_FILES		0x02

#include "Browser.hpp"
#include "../Text/wstring.hpp"
#include "CFileList.hpp"

typedef struct _CustomEntryStruct
{
	std::string filename; // full filename
	std::string displayname; // filename displayed in browser
	bool isdir; // 0 - file, 1 - directory
	bool ismii; // 0 - file, 1 - SaveFolder
	bool issave; // 0 - dir, 1 - SaveFolder
	bool isdatabin; // 0 - dir, 1 - SaveFolder
	bool isNotInstalled; // 0 - installed, 1 - not installed
	
} CustomEntryStruct;

class FileCustomBrowser : public Browser
{
	public:
		FileCustomBrowser();
		~FileCustomBrowser();
		int BrowsePath(std::string path);
		int BrowseDevice(int device);
		void SetPageIndex(int ind);
		void SetSelectedIndex(int ind);
		int GetEntrieCount() { return browserList.size(); };
		int GetPageIndex() { return BrowserPageIndex; };
		int GetSelIndex() { return BrowserSelIndex; };
		bool IsDir(int ind) { return browserList.at(ind)->isdir; };
		bool IsSave(int ind) { return browserList.at(ind)->issave; };
		bool IsDataBin(int ind) { return browserList.at(ind)->isdatabin; };
		bool IsMii(int ind) { return browserList.at(ind)->ismii; };
		bool IsNotInstalled(int ind) { return browserList.at(ind)->isNotInstalled; };
		std::string GetItemFileName(int ind) { return (!(browserList.at(ind)->filename).empty() ? browserList.at(ind)->filename : ""); };
		std::string GetItemDisplayName(int ind) { return (!(browserList.at(ind)->displayname).empty() ? browserList.at(ind)->displayname : ""); };
		std::string GetCurrentFilename() { return (!(browserList.at(BrowserSelIndex)->filename).empty() ? (browserList.at(BrowserSelIndex)->filename) : ""); };
		std::string GetCurrentDisplayname() { return (!(browserList.at(BrowserSelIndex)->displayname).empty() ? (browserList.at(BrowserSelIndex)->displayname) : ""); };
		std::string GetRootDir() { return BrowserRootDir; };
		std::string GetDir() { return BrowserDir; };
		std::string GetCurrentPath();
		std::string GetCurrentSelectedFilepath();
		ItemStruct * GetItemStruct(int pos);
		int ChangeDirectory();
		int BackInDirectory() { return LeaveCurDir(); };
		bool IsGettingList() { return gettingList; };
		bool ListChanged() { bool ret = listChanged; listChanged = false; return ret; };
		void Refresh();
		void SetFilter(u8 filtermode) { Filter = filtermode; };
		void SetFilterCustom(short Type);
		
	private:
		std::string GetMiiDeviceName(const char * miipath);
		void ParseDirEntries(bool ResetPosition);
		int ParseDirectory(bool ResetPosition = true);
		int UpdateDirName();
		void ResetBrowser();
		int EnterSelDir();
		int LeaveCurDir();
		void FilterList();
		void AddEntry();
		
		bool FilterLang;
		bool FilterTheme;
		bool FilterFont;
		bool FilterSound;
		
		std::string BrowserDir; //! directory path of browserList
		std::string BrowserRootDir; //! root path of browserList
		int BrowserSelIndex; //! currently selected index of browserList
		int BrowserPageIndex; //! starting index of browserList page display
		std::vector<CustomEntryStruct *> browserList;
		
		u8 Filter;
		DIR * dir;
		std::string currentpath;
		bool gettingList;
		bool listChanged;
};

#endif
