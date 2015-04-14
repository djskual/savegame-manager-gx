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
 * MiiBrowser.h
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#ifndef _MIIBROWSER_H_
#define _MIIBROWSER_H_

#include <unistd.h>
#include <gccore.h>

#include "../Files/Browser.hpp"

typedef struct _MiiBrowserStruct
{
	std::string name; //! mii name
	std::string creator; //! mii creator name
	int position; //! reference to the place in wii miiFile
	int female;
	int month;
	int day;
	int favColor;	
	
} MiiBrowserStruct;

class MiiBrowser : public Browser
{
	public:
		MiiBrowser();
		~MiiBrowser();
		int GetMiiList();
		void SetSelectedIndex(int ind) { BrowserSelIndex = ind; };
		void SetPageIndex(int ind) { BrowserPageIndex = ind; };
		int GetMiiCount() {return miiBrowser.size(); };
		int GetPageIndex() { return BrowserPageIndex; };
		int GetSelIndex() { return BrowserSelIndex; };
		std::string GetItemMiiName(int ind) { return miiBrowser.at(ind)->name; };
		std::string GetItemMiiCreator(int ind) { return miiBrowser.at(ind)->creator; };
		int GetItemMiiFemale(int ind) { return miiBrowser.at(ind)->female; };
		int GetItemMiiMonth(int ind) { return miiBrowser.at(ind)->month; };
		int GetItemMiiDay(int ind) { return miiBrowser.at(ind)->day; };
		int GetItemMiiFavColor(int ind) { return miiBrowser.at(ind)->favColor; };
		bool IsGettingList() { return gettingList; };
		void Refresh();
		
	private:
		void ResetMiiList();
		void AddEntry();
		
		int BrowserSelIndex; //! currently selected index of browserList
		int BrowserPageIndex; //! starting index of browserList page display
		std::vector<MiiBrowserStruct *> miiBrowser;
		
		bool gettingList;
};

#endif
