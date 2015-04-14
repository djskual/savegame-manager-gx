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
 * EmuBrowser.h
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#ifndef _EMUBROWSER_H_
#define _EMUBROWSER_H_

#include <unistd.h>
#include <gccore.h>

#include "../Files/Browser.hpp"

typedef struct _EmuBrowserStruct
{
	std::string name; //! title name
	std::string subname; //! title subname
	std::string ID; //! title ID
	u64 tid; //! titleid of the save
	float blocksize; //! size of the save
	
} EmuBrowserStruct;

class EmuBrowser : public Browser
{
	public:
		EmuBrowser();
		~EmuBrowser();
		int GetEmuList();
		void SetSelectedIndex(int ind) { browserSelIndex = ind; };
		void SetPageIndex(int ind) { browserPageIndex = ind; };
		int GetEmuListCount() {return emuBrowser.size(); };
		int GetPageIndex() { return browserPageIndex; };
		int GetSelIndex() { return browserSelIndex; };
		std::string GetItemEmuName(int ind) { return emuBrowser.at(ind)->name; };
		std::string GetItemEmuSubname(int ind) { return emuBrowser.at(ind)->subname; };
		std::string GetItemEmuID(int ind) { return emuBrowser.at(ind)->ID; };
		u64 GetItemEmuTid(int ind) { return emuBrowser.at(ind)->tid; };
		float GetItemEmuBlocks(int ind) { return emuBrowser.at(ind)->blocksize; };
		bool IsGettingList() { return gettingList; };
		void Refresh();
		
	private:
		void ResetEmuList();
		void AddEntry();
		
		int browserSelIndex; //! currently selected index of browserList
		int browserPageIndex; //! starting index of browserList page display
		std::vector<EmuBrowserStruct *> emuBrowser;
		
		bool gettingList;
};

#endif
