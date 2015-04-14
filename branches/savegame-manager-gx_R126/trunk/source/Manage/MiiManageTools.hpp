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
 * MiiManageTools.hpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#ifndef _MIIMANAGETOOLS_HPP_
#define _MIIMANAGETOOLS_HPP_

#include <string>

#include "ManageProgress.h"

class MiiManageTools
{
	public:
		MiiManageTools(const char * title = NULL);
		~MiiManageTools();
		
		void Reset();
		void StopProgress();
		int GetResult() { return returnVal; };
		int FindFirstFree(bool Nand = true);
		
		//! Nand
		void ExtractMii_Nand_Dev(std::string devpath, int selected);
		void InstallMii_Nand_Emu(int nandPos, int emuPos);
		void DeleteMii_Nand(int position);
		
		//! Device
		void InstallMii_Dev_Nand(std::string miipath, int position);
		void InstallMii_Dev_Emu(std::string miipath, int position);
		void DeleteMii_Dev(std::string miiPath);
		void MoveMii(std::string srcFile, std::string destFile);
		void CopyMii(std::string srcFile, std::string destFile);
		
		//! Emu
		void ExtractMii_Emu_Dev(std::string devpath, int selected);
		void InstallMii_Emu_Nand(int emuPos, int nandPos);
		void DeleteMii_Emu(int position);
		
		
	protected:
		
		std::string FixMiiName(std::string filename);
		
		int returnVal;
};

#endif
