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
 * SaveBrowser.h
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#ifndef _SAVEBROWSER_H_
#define _SAVEBROWSER_H_

#include <unistd.h>
#include <gccore.h>

#include "../Files/Browser.hpp"

typedef struct _SaveBrowserStruct
{
	std::string name; //! title name
	std::string subname; //! title subname
	std::string ID; //! title ID
	u64 tid; //! titleid of the save
	float blocksize; //! size of the save
	
} SaveBrowserStruct;

class SaveBrowser : public Browser
{
	public:
		SaveBrowser();
		~SaveBrowser();
		int GetSaveList();
		void SetSelectedIndex(int ind) { browserSelIndex = ind; };
		void SetPageIndex(int ind) { browserPageIndex = ind; };
		int GetSaveListCount() {return saveBrowser.size(); };
		int GetPageIndex() { return browserPageIndex; };
		int GetSelIndex() { return browserSelIndex; };
		std::string GetItemSaveName(int ind) { return saveBrowser.at(ind)->name; };
		std::string GetItemSaveSubname(int ind) { return saveBrowser.at(ind)->subname; };
		std::string GetItemSaveID(int ind) { return saveBrowser.at(ind)->ID; };
		u64 GetItemSaveTid(int ind) { return saveBrowser.at(ind)->tid; };
		float GetItemSaveBlocks(int ind) { return saveBrowser.at(ind)->blocksize; };
		bool IsGettingList() { return gettingList; };
		
	private:
		void ResetSaveList();
		void AddEntry();
		
		int browserSelIndex; // currently selected index of browserList
		int browserPageIndex; // starting index of browserList page display
		std::vector<SaveBrowserStruct *> saveBrowser;
		
		
		bool gettingList;
};

#endif
