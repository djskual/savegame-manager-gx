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
 * SaveBrowser.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include <malloc.h>

#include "../Prompts/PromptWindows.h"
#include "../Language/gettext.h"
#include "CSaveList.hpp"
#include "SaveBrowser.h"

/****************************************************************************
 * SaveBrowser Class
 * Create a list of saves installed in the wii
 ***************************************************************************/
SaveBrowser::SaveBrowser()
	:Browser()
{
	saveBrowser.clear();
	gettingList = false;
	
	//!Reset and prepare browser
	ResetSaveList();
}

/****************************************************************************
 * Destructor
 ***************************************************************************/
SaveBrowser::~SaveBrowser()
{
	for(u32 i = 0; i < saveBrowser.size(); ++i)
		delete saveBrowser.at(i);
	
	saveBrowser.clear();
}

void SaveBrowser::AddEntry()
{
	SaveBrowserStruct * newEntry = new SaveBrowserStruct;
	newEntry->name = " ";
	newEntry->subname = " ";
	newEntry->ID = " ";
	newEntry->tid = 0;
	newEntry->blocksize = 0;
	
	saveBrowser.push_back(newEntry);
}

/****************************************************************************
 * ResetSaveList()
 *
 * Clears the file browser memory, and allocates one initial entry
 ***************************************************************************/
void SaveBrowser::ResetSaveList()
{
	for(u32 i = 0; i < saveBrowser.size(); ++i)
		delete saveBrowser.at(i);
	
	saveBrowser.clear();
	
	browserSelIndex = 0;
	browserPageIndex = 0;
}

/****************************************************************************
 * Create
 *
 * Create a list of saves installed in the wii
 ***************************************************************************/
int SaveBrowser::GetSaveList()
{
	gettingList = true;
	
	ResetSaveList();
	
	if (SaveList.GetFilteredCount() == 0)
	{
		gettingList = false;
		return 0;
	}
	
	for (int i = 0; i < SaveList.GetFilteredCount(); i++)
	{
		if(i >= (int)saveBrowser.size())
			AddEntry();
		
		//! Set title tid
		saveBrowser.at(i)->tid = SaveList.GetTid(i);
		
		//! Set title name & subname
		(saveBrowser.at(i)->name).assign(SaveList.GetName(i));
		(saveBrowser.at(i)->subname).assign(SaveList.GetSubname(i));
		
		//! Set title blockSize
		saveBrowser.at(i)->blocksize = SaveList.GetBlocks(i);
		
		//! Set title ID[4]
		(saveBrowser.at(i)->ID).assign(SaveList.GetID(i));
	}
	
	gettingList = false;
	
	return saveBrowser.size();
}
