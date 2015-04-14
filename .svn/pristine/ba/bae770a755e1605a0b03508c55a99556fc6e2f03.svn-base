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
 * CTitleList.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include <malloc.h>

#include "../Language/gettext.h"
#include "../Settings/CSettings.h"
#include "../System/nandtitle.h"
#include "../XML/GameTDB.hpp"
#include "CTitleList.hpp"

void CTitleList::AddTitle()
{
	TitleStruct * newTitle = new TitleStruct;
	newTitle->ID = " ";
	newTitle->name = " ";
	newTitle->tid = 0;
	
	TitleList.push_back(newTitle);
}

std::string CTitleList::GetID(int ind)
{
	if(ind < 0 || ind >= (int) TitleList.size())
		return "";

	return TitleList.at(ind)->ID;
}

std::string CTitleList::GetName(int ind)
{
	if(ind < 0 || ind >= (int) TitleList.size())
		return "";

	return TitleList.at(ind)->name;
}

u64 CTitleList::GetTid(int ind)
{
	if(ind < 0 || ind >= (int) TitleList.size())
		return 0;

	return TitleList.at(ind)->tid;
}

void CTitleList::Reset()
{
	for(u32 i = 0; i < TitleList.size(); i++)
		delete TitleList.at(i);

	TitleList.clear();
}

int CTitleList::Get()
{
	Reset();
	
	std::string Filepath = Settings.GameTDBPath;
	Filepath += "wiitdb.xml";

	GameTDB * XML_DB = new GameTDB(Filepath.c_str());
	XML_DB->SetLanguageCode(Settings.GameTDBLanguageCode);
	
	NandTitles.Get();
	u32 n = NandTitles.SetType(0x10001);
	u32 cnt = 0;
	
	for (u32 i = 0; i < n; i++)
	{
		u64 tid = NandTitles.Next();
		if (!tid)
			break;
		
		if (!NandTitles.Exists(tid))
			continue;
		
		if(cnt >= TitleList.size())
			AddTitle();
		
		char id[5];
		NandTitles.AsciiTID(tid, id);
		TitleList.at(cnt)->ID = id;
		TitleList.at(cnt)->tid = tid;
		
		const char* name = NULL;
		std::string TitleName;
		
		if(XML_DB->GetTitle(id, TitleName))
			name = TitleName.c_str();
		else
			name = NandTitles.NameOf(tid);
		
		TitleList.at(cnt)->name = (name ? name :  tr( "Unknown" ));
		
		cnt++;
	}

	//! Get count of system titles
	n = NandTitles.SetType(0x10002);
	for (u32 i = 0; i < n; i++)
	{
		u64 tid = NandTitles.Next();
		if (!tid)
			break;
		
		//!these can't be booted anyways
		if (TITLE_LOWER( tid ) == 0x48414741 || TITLE_LOWER( tid ) == 0x48414141 || TITLE_LOWER( tid ) == 0x48414641)
			continue;

		//!these aren't installed on the nand
		if (!NandTitles.Exists(tid))
			continue;
		
		if(cnt >= TitleList.size())
			AddTitle();
		
		char id[5];
		NandTitles.AsciiTID(tid, id);
		TitleList.at(cnt)->ID = id;
		TitleList.at(cnt)->tid = tid;
		
		const char* name = NULL;
		std::string TitleName;
		
		if(XML_DB->GetTitle(id, TitleName))
			name = TitleName.c_str();
		else
			name = NandTitles.NameOf(tid);
		
		TitleList.at(cnt)->name = (name ? name :  tr( "Unknown" ));
		
		cnt++;
	}
	delete XML_DB;
	XML_DB = NULL;

	if(cnt >= TitleList.size())
		AddTitle();
	
	TitleList.at(cnt)->tid = 0;
	(TitleList.at(cnt)->ID).assign(tr( "Wii Settings" ));
	(TitleList.at(cnt)->name).assign(" ");
	
	return TitleList.size();
}
