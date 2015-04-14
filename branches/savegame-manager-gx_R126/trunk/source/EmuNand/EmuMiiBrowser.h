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
 * EmuMiiBrowser.h
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#ifndef _EMUMIIBROWSER_H_
#define _EMUMIIBROWSER_H_

#include <unistd.h>
#include <gccore.h>

#include "../Files/Browser.hpp"

typedef struct _EmuMiiBrowserStruct
{
	std::string name; //! mii name
	std::string creator; //! mii creator name
	int position; //! reference to the place in wii miiFile
	int female;
	int month;
	int day;
	int favColor;	
	
} EmuMiiBrowserStruct;

class EmuMiiBrowser : public Browser
{
	public:
		EmuMiiBrowser();
		~EmuMiiBrowser();
		int GetEmuMiiList();
		void SetSelectedIndex(int ind) { BrowserSelIndex = ind; };
		void SetPageIndex(int ind) { BrowserPageIndex = ind; };
		int GetEmuMiiCount() {return emuMiiBrowser.size(); };
		int GetPageIndex() { return BrowserPageIndex; };
		int GetSelIndex() { return BrowserSelIndex; };
		std::string GetItemEmuMiiName(int ind) { return emuMiiBrowser.at(ind)->name; };
		std::string GetItemEmuMiiCreator(int ind) { return emuMiiBrowser.at(ind)->creator; };
		int GetItemEmuMiiFemale(int ind) { return emuMiiBrowser.at(ind)->female; };
		int GetItemEmuMiiMonth(int ind) { return emuMiiBrowser.at(ind)->month; };
		int GetItemEmuMiiDay(int ind) { return emuMiiBrowser.at(ind)->day; };
		int GetItemEmuMiiFavColor(int ind) { return emuMiiBrowser.at(ind)->favColor; };
		bool IsGettingList() { return gettingList; };
		void Refresh();
		
	private:
		void ResetEmuMiiList();
		void AddEntry();
		
		int BrowserSelIndex; //! currently selected index of browserList
		int BrowserPageIndex; //! starting index of browserList page display
		std::vector<EmuMiiBrowserStruct *> emuMiiBrowser;
		
		bool gettingList;
};

#endif
