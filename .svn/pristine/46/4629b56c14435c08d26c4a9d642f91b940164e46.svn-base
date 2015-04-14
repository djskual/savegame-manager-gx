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
 * MiiBrowser.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include <algorithm>
#include <malloc.h>

#include "../Prompts/PromptWindows.h"
#include "../Language/gettext.h"
#include "../Tools/tools.h"
#include "MiiBrowser.h"
#include "CMiiList.hpp"

/****************************************************************************
 * MiiBrowser Class
 * Create a list of miis installed in the wii
 ***************************************************************************/
MiiBrowser::MiiBrowser()
	:Browser()
{
	miiBrowser.clear();
	gettingList = false;
	
	//!Reset and prepare browser
	ResetMiiList();
}

/****************************************************************************
 * Destructor
 ***************************************************************************/
MiiBrowser::~MiiBrowser()
{
	for(u32 i = 0; i < miiBrowser.size(); ++i)
		delete miiBrowser.at(i);
	
	miiBrowser.clear();
}

void MiiBrowser::AddEntry()
{
	MiiBrowserStruct * newEntry = new MiiBrowserStruct;
	newEntry->name = " ";
	newEntry->creator = " ";
	newEntry->position = 0;
	newEntry->female = 0;
	newEntry->month = 0;
	newEntry->day = 0;
	newEntry->favColor = 0;
	
	miiBrowser.push_back(newEntry);
}

/****************************************************************************
 * ResetMiiList()
 *
 * Clears the file browser memory, and allocates one initial entry
 ***************************************************************************/
void MiiBrowser::ResetMiiList()
{
	for(u32 i = 0; i < miiBrowser.size(); ++i)
		delete miiBrowser.at(i);
	
	miiBrowser.clear();
	
	BrowserSelIndex = 0;
	BrowserPageIndex = 0;
}

/****************************************************************************
 * SortCallback
 *
 * Quick sort callback to sort saves entries
 ***************************************************************************/
static bool MiiSortCallback(const MiiBrowserStruct *f1, const MiiBrowserStruct *f2)
{
	return (stricmp(((MiiBrowserStruct *)f1)->name.c_str(), ((MiiBrowserStruct *)f2)->name.c_str()) < 0);
}

/****************************************************************************
 * Refresh
 *
 * Refresh the list of miis installed in the wii
 ***************************************************************************/

void MiiBrowser::Refresh()
{
	GetMiiList();
}

/****************************************************************************
 * GetMiiList
 *
 * Create a list of miis installed in the wii
 ***************************************************************************/

int MiiBrowser::GetMiiList()
{
	gettingList = true;
	
	ResetMiiList();
	
	if (MiiList.GetFilteredCount() == 0)
	{
		gettingList = false;
		return 0;
	}
	
	for (int i = 0; i < MiiList.GetFilteredCount(); i++)
	{
		if(i >= (int)miiBrowser.size())
			AddEntry();
		
		(miiBrowser.at(i)->name).assign(MiiList.GetName(i));
		(miiBrowser.at(i)->creator).assign(MiiList.GetCreator(i));
		miiBrowser.at(i)->position = MiiList.GetPosition(i);
		miiBrowser.at(i)->female = MiiList.GetFemale(i);
		miiBrowser.at(i)->month = MiiList.GetMonth(i);
		miiBrowser.at(i)->day = MiiList.GetDay(i);
		miiBrowser.at(i)->favColor = MiiList.GetFavColor(i);
	}
	
	std::sort(miiBrowser.begin(), miiBrowser.end(), MiiSortCallback);
	
	gettingList = false;
	
	return miiBrowser.size();
}
