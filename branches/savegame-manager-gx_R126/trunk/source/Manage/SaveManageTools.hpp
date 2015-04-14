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
 * SaveManageTools.hpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#ifndef _SAVEMANAGETOOLS_HPP_
#define _SAVEMANAGETOOLS_HPP_

#include <string>
#include <vector>

#include "ManageProgress.h"
#include "../ArchiveOperations/Archive.h"

class SaveManageTools
{
	public:
		SaveManageTools(const char * title = NULL, const char * msg = NULL);
		~SaveManageTools();
		
		void Reset();
		void StopProgress() { ManageProgressStop(); };
		int GetResult() { return returnVal; };
		
		//! Nand
		void ExtractSave_Nand_Dev(std::string nandpath, std::string devpath);
		void InstallSave_Nand_Emu(u64 tid);
		void CompressSave_Nand_Dev(u64 tid, std::string destPath);
		
		//! Device
		void InstallSave_Dev_Nand(std::string devpath, std::string nandpath);
		void InstallSave_Dev_Emu(std::string devpath, u64 tid);
		void DeleteSave_Dev(std::string savePath);
		void MoveSave(std::string savePath, std::string destPath);
		void CopySave(std::string savePath, std::string destPath);
		void DownloadSave(std::string ID);
		
		enum
		{
			FIX_RESTORE = 0,
			FIX_INSTALL,
		};
		
	protected:
		
		std::string FixFileName(std::string filename, short mode);
		
		void CreatePathsList(std::string devPath, std::string nandPath);
		void DeleteUselessNandFiles(std::string path);
		void CreateMissingNandFiles();
		
		bool firstuse;
		int returnVal;
		
		std::vector <std::string> isfsPath;
		std::vector <std::string> devicePath;
};

#endif
