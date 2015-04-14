/***************************************************************************
 * Copyright (C) 2012
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
 * EmuManageTools.hpp
 *
 * for SaveGame Manager GX 2012
 ***************************************************************************/
#ifndef _EMUMANAGETOOLS_HPP_
#define _EMUMANAGETOOLS_HPP_

#include <string>
#include <vector>

#include "ManageProgress.h"
#include "../ArchiveOperations/Archive.h"

class EmuManageTools
{
	public:
		EmuManageTools(const char * title = NULL, const char * msg = NULL);
		~EmuManageTools();
		
		void Reset();
		void StopProgress() { ManageProgressStop(); };
		int GetResult() { return returnVal; };
		
		void ExtractSave_Emu_Dev(u64 tid, std::string devpath);
		void InstallSave_Emu_Nand(u64 tid, std::string nandpath);
		void DeleteSave_Emu(u64 tid);
		
		void DownloadSave(std::string ID);
		
	protected:
		
		std::string FixFileName(std::string filename);
		
		void CreatePathsList(u64 tid, std::string nandPath);
		void DeleteUselessNandFiles(std::string path);
		void CreateMissingNandFiles();
		
		bool firstuse;
		int returnVal;
		
		std::vector <std::string> isfsPath;
		std::vector <std::string> emuPath;
};

#endif
