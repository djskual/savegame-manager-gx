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
 * EmuBrowser.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include <malloc.h>

#include "../Prompts/PromptWindows.h"
#include "../Language/gettext.h"
#include "CEmuList.hpp"
#include "EmuBrowser.h"

/****************************************************************************
 * EmuBrowser Class
 * Create a list of saves installed in the emulate nand
 ***************************************************************************/
EmuBrowser::EmuBrowser()
	:Browser()
{
	emuBrowser.clear();
	gettingList = false;
	
	//!Reset and prepare browser
	ResetEmuList();
}

/****************************************************************************
 * Destructor
 ***************************************************************************/
EmuBrowser::~EmuBrowser()
{
	for(u32 i = 0; i < emuBrowser.size(); ++i)
		delete emuBrowser.at(i);
	
	emuBrowser.clear();
}

void EmuBrowser::AddEntry()
{
	EmuBrowserStruct * newEntry = new EmuBrowserStruct;
	newEntry->name = " ";
	newEntry->subname = " ";
	newEntry->ID = " ";
	newEntry->tid = 0;
	newEntry->blocksize = 0;
	
	emuBrowser.push_back(newEntry);
}

/****************************************************************************
 * ResetEmuList()
 *
 * Clears the emulated nand browser memory, and allocates one initial entry
 ***************************************************************************/
void EmuBrowser::ResetEmuList()
{
	for(u32 i = 0; i < emuBrowser.size(); ++i)
		delete emuBrowser.at(i);
	
	emuBrowser.clear();
	
	browserSelIndex = 0;
	browserPageIndex = 0;
}

/***************************************************************************
 * Refresh()
 *
 * Browse subdirectories
 **************************************************************************/
void EmuBrowser::Refresh()
{
	GetEmuList();
}

/****************************************************************************
 * Create
 *
 * Create a list of saves installed in the emulated nand
 ***************************************************************************/
int EmuBrowser::GetEmuList()
{
	gettingList = true;
	
	ResetEmuList();
	
	if (EmuList.GetFilteredCount() == 0)
	{
		gettingList = false;
		return 0;
	}
	
	for (int i = 0; i < EmuList.GetFilteredCount(); i++)
	{
		if(i >= (int)emuBrowser.size())
			AddEntry();
		
		//! Set title tid
		emuBrowser.at(i)->tid = EmuList.GetTid(i);
		
		//! Set title name & subname
		(emuBrowser.at(i)->name).assign(EmuList.GetName(i));
		(emuBrowser.at(i)->subname).assign(EmuList.GetSubname(i));
		
		//! Set title blockSize
		emuBrowser.at(i)->blocksize = EmuList.GetBlocks(i);
		
		//! Set title ID[4]
		(emuBrowser.at(i)->ID).assign(EmuList.GetID(i));
	}
	
	gettingList = false;
	
	return emuBrowser.size();
}
