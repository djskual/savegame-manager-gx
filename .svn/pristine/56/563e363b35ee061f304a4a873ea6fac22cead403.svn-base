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
 * CSaveList.hpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#ifndef _CSAVELIST_HPP_
#define _CSAVELIST_HPP_

#include <unistd.h>
#include <gccore.h>
#include <vector>

#include "../Text/wstring.hpp"

#define MAXNAME 64

typedef struct _SaveStruct
{
	std::string name;
	std::string subname;
	std::string ID;
	u64 tid;
	float blocksize;
	
} SaveStruct;


class CSaveList
{
	public:
		CSaveList() { };
		~CSaveList() { ResetFull(); };
		
		int Get();
		int FilterList(const wchar_t * saveFilter = NULL);
		void ResetFull();
		void ResetFiltered();
		void AddFullSave();
		void AddFilteredSave();
		int GetFullCount() { return SavesFull.size(); };
		int GetFilteredCount() { return SavesFiltered.size(); };
		std::string GetFullName(int ind);
		std::string GetName(int ind);
		std::string GetFullSubname(int ind);
		std::string GetSubname(int ind);
		std::string GetID(int ind);
		u64 GetFullTid(int ind);
		u64 GetTid(int ind);
		float GetBlocks(int ind);
		
		const wchar_t * GetCurrentFilter() const { return SaveFilter.c_str(); }
		const wchar_t * GetAvailableSearchChars() const { return AvailableSearchChars.c_str(); }
		
	protected:
		void LoadTitlesFromGameTDB(const char * path);
		
		wString AvailableSearchChars;
		wString SaveFilter;
		
		std::vector<SaveStruct *> SavesFull;
		std::vector<SaveStruct *> SavesFiltered;
};

extern CSaveList SaveList;

#endif
