/***************************************************************************
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
 * MiiAllTools.hpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#ifndef _MIIALLTOOLS_HPP_
#define _MIIALLTOOLS_HPP_

#include <string>
#include <vector>

#include "MiiAllProgress.h"

class MiiAllTools
{
	public:
		MiiAllTools();
		~MiiAllTools();
		
		void Reset();
		int GetResult() { return returnVal; };
		void ExtractAll_Miis_Nand_Dev(std::string destPath);
		void ExtractAll_Miis_Emu_Dev(std::string destPath);
		void InstallAll_Miis_Dev_Nand(std::string devPath);
		void InstallAll_Miis_Dev_Emu(std::string devPath);
		void InstallAll_Miis_Nand_Emu();
		void InstallAll_Miis_Emu_Nand();
		void DeleteAll_Miis_Nand();
		void DeleteAll_Miis_Emu();
		void DeleteAll_Miis_Dev(std::string devPath);
		
	protected:
		
		int returnVal;
		bool firstrun;
		
		std::string FixMiiName(std::string filename);
		
		void GetMiiName(std::string miipath);
		void GetMiiCreator(std::string miipath);
		void GetMiiInfos(std::string miipath);
		void CreateMiiList(std::string devPath);
		int GetFirstFree(bool emu = false);
		int GetFirstDestFree(bool EmuToNand = false);
		int GetPosition(int ind, bool emu = false);
		int GetDestPosition(int ind, bool EmuToNand = false);
		
		int Internal_ExtractAll_Miis_Nand_Dev(std::string devpath, int selected);
		int Internal_ExtractAll_Miis_Emu_Dev(std::string devpath, int selected);
		int Internal_DeleteAll_Miis_Nand(int position);
		int Internal_DeleteAll_Miis_Emu(int position);
		int Internal_DeleteAll_Miis_Dev(std::string miiPath);
		
		std::vector <std::string> miiPath;
		std::vector <std::string> miiName;
		std::vector <std::string> miiCreator;
		std::vector <int> miiPosition;
		std::vector <int> miiFemale;
		std::vector <int> miiDay;
		std::vector <int> miiMonth;
		std::vector <int> miiFavColor;
};

#endif
