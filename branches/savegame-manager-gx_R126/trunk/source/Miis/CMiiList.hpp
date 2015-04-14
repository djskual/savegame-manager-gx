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
 * CMiiList.hpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#ifndef _CMIILIST_HPP_
#define _CMIILIST_HPP_

#include <vector>

#include "../Text/wstring.hpp"

typedef struct _MiiStruct
{
	std::string name;
	std::string creator;
	int position;
	int female;
	int month;
	int day;
	int favColor;	
	
} MiiStruct;


class CMiiList
{
	public:
		CMiiList() { };
		~CMiiList() { ResetFull(); ResetFiltered(); };
		
		int Get();
		int FilterList(const wchar_t * saveFilter = NULL);
		void ResetFull();
		void ResetFiltered();
		void AddFullMii();
		void AddFilteredMii();
		void RemoveEntry(int ind);
		int GetFullCount() { return MiisFull.size(); };
		int GetFilteredCount() { return MiisFiltered.size(); };
		std::string GetFullName(int ind);
		std::string GetName(int ind);
		std::string GetFullCreator(int ind);
		std::string GetCreator(int ind);
		int GetFullPosition(int ind);
		int GetPosition(int ind);
		bool GetFullFemale(int ind);
		bool GetFemale(int ind);
		int GetFullMonth(int ind);
		int GetMonth(int ind);
		int GetFullDay(int ind);
		int GetDay(int ind);
		int GetFullFavColor(int ind);
		int GetFavColor(int ind);
		
		void SetRefresh() { need_refresh = true; };
		const wchar_t * GetCurrentFilter() const { return MiiFilter.c_str(); }
		const wchar_t * GetAvailableSearchChars() const { return AvailableSearchChars.c_str(); }
		
	protected:
		std::string GetMiiNandName(int position);
		std::string GetMiiNandCreator(int position);
		void GetMiiNandInfos(int start, char * data, int m);
		
		bool need_refresh;
		wString AvailableSearchChars;
		wString MiiFilter;
		
		std::vector<MiiStruct *> MiisFull;
		std::vector<MiiStruct *> MiisFiltered;
};

extern CMiiList MiiList;

#endif
