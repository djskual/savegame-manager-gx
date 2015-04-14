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
 * EmuMiiBrowser.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include <algorithm>
#include <malloc.h>

#include "../Prompts/PromptWindows.h"
#include "../Language/gettext.h"
#include "../Tools/tools.h"
#include "EmuMiiBrowser.h"
#include "CEmuMiiList.hpp"

/****************************************************************************
 * EmuMiiBrowser Class
 * Create a list of miis installed in the wii
 ***************************************************************************/
EmuMiiBrowser::EmuMiiBrowser()
	:Browser()
{
	emuMiiBrowser.clear();
	gettingList = false;
	
	//!Reset and prepare browser
	ResetEmuMiiList();
}

/****************************************************************************
 * Destructor
 ***************************************************************************/
EmuMiiBrowser::~EmuMiiBrowser()
{
	for(u32 i = 0; i < emuMiiBrowser.size(); ++i)
		delete emuMiiBrowser.at(i);
	
	emuMiiBrowser.clear();
}

void EmuMiiBrowser::AddEntry()
{
	EmuMiiBrowserStruct * newEntry = new EmuMiiBrowserStruct;
	newEntry->name = " ";
	newEntry->creator = " ";
	newEntry->position = 0;
	newEntry->female = 0;
	newEntry->month = 0;
	newEntry->day = 0;
	newEntry->favColor = 0;
	
	emuMiiBrowser.push_back(newEntry);
}

/****************************************************************************
 * ResetEmuMiiList()
 *
 * Clears the file browser memory, and allocates one initial entry
 ***************************************************************************/
void EmuMiiBrowser::ResetEmuMiiList()
{
	for(u32 i = 0; i < emuMiiBrowser.size(); ++i)
		delete emuMiiBrowser.at(i);
	
	emuMiiBrowser.clear();
	
	BrowserSelIndex = 0;
	BrowserPageIndex = 0;
}

/****************************************************************************
 * SortCallback
 *
 * Quick sort callback to sort saves entries
 ***************************************************************************/
static bool MiiSortCallback(const EmuMiiBrowserStruct *f1, const EmuMiiBrowserStruct *f2)
{
	return (stricmp(((EmuMiiBrowserStruct *)f1)->name.c_str(), ((EmuMiiBrowserStruct *)f2)->name.c_str()) < 0);
}

/****************************************************************************
 * Refresh
 *
 * Refresh the list of miis installed in the wii
 ***************************************************************************/
void EmuMiiBrowser::Refresh()
{
	GetEmuMiiList();
}

/****************************************************************************
 * GetEmuMiiList
 *
 * Create a list of miis installed in the wii
 ***************************************************************************/
int EmuMiiBrowser::GetEmuMiiList()
{
	gettingList = true;
	
	ResetEmuMiiList();
	
	if (EmuMiiList.GetFilteredCount() == 0)
	{
		gettingList = false;
		return 0;
	}
	
	for (int i = 0; i < EmuMiiList.GetFilteredCount(); i++)
	{
		if(i >= (int)emuMiiBrowser.size())
			AddEntry();
		
		(emuMiiBrowser.at(i)->name).assign(EmuMiiList.GetName(i));
		(emuMiiBrowser.at(i)->creator).assign(EmuMiiList.GetCreator(i));
		emuMiiBrowser.at(i)->position = EmuMiiList.GetPosition(i);
		emuMiiBrowser.at(i)->female = EmuMiiList.GetFemale(i);
		emuMiiBrowser.at(i)->month = EmuMiiList.GetMonth(i);
		emuMiiBrowser.at(i)->day = EmuMiiList.GetDay(i);
		emuMiiBrowser.at(i)->favColor = EmuMiiList.GetFavColor(i);
	}
	
	std::sort(emuMiiBrowser.begin(), emuMiiBrowser.end(), MiiSortCallback);
	
	gettingList = false;
	
	return emuMiiBrowser.size();
}
