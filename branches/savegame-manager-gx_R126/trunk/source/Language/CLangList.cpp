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
 * CLangList.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include "CLangList.hpp"

CLangList LangList;

void CLangList::AddLang()
{
	LangStruct * newLang = new LangStruct;
	newLang->filename = (char *) malloc(MAXPATHLEN + 1);
	newLang->needed = false;
	
	LangFiles.push_back(newLang);
}

const char * CLangList::GetFilename(int ind)
{
	if(ind < 0 || ind >= (int) LangFiles.size())
		return NULL;

	return (const char *) LangFiles.at(ind)->filename;
}

bool CLangList::Needed(int ind)
{
	if(ind < 0 || ind >= (int) LangFiles.size())
		return NULL;

	return LangFiles.at(ind)->needed;
}

void CLangList::SetNeeded(int ind, bool b)
{
	if(ind < 0 || ind >= (int) LangFiles.size())
		return;

	LangFiles.at(ind)->needed = b;
}

void CLangList::Reset()
{
	for(u32 i = 0; i < LangFiles.size(); i++)
	{
		free(LangFiles.at(i)->filename);
		delete LangFiles.at(i);
	}

	LangFiles.clear();
}

int CLangList::Get(URL_List  * LinkList)
{
	Reset();
	
	u32 cnt = 0;
	
	for (int i = 0; i < LinkList->GetURLCount(); i++)
	{
		const char * filename = strrchr(LinkList->GetURL(i), '/');
		if(filename) filename++;
		else filename = LinkList->GetURL(i);

		if(!filename)
			continue;

		const char * FileExt = strrchr(filename, '.');
		if (!FileExt || strcasecmp(FileExt, ".lang") != 0)
			continue;

		if(strlen(filename) > 0)
		{
			if(cnt >= LangFiles.size())
				AddLang();
			
			snprintf(LangFiles[cnt]->filename, strlen(filename)+1, "%s", filename);
			LangFiles[cnt]->needed = false;
			
			cnt++;
		}
	}
	
	return LangFiles.size();
}
