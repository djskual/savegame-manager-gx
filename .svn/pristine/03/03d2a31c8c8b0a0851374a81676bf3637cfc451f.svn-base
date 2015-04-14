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
 * CTitleList.hpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#ifndef CTITLELIST_HPP_
#define CTITLELIST_HPP_

#include <unistd.h>
#include <gccore.h>
#include <vector>
#include <string>

typedef struct _TitleStruct
{
	std::string ID;
	std::string name;	
	u64 tid;
	
} TitleStruct;


class CTitleList
{
	public:
		CTitleList() { Get(); };
		~CTitleList() { Reset(); };
		
		int Get();
		void Reset();
		void AddTitle();
		int GetCount() { return TitleList.size(); };
		std::string GetID(int ind);
		std::string GetName(int ind);
		u64 GetTid(int ind);
		
	protected:
		
		std::vector<TitleStruct *> TitleList;
};

#endif
