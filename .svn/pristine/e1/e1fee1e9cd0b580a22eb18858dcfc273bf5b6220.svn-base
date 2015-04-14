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
 * SaveAllTools.hpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#ifndef _SAVEALLTOOLS_HPP_
#define _SAVEALLTOOLS_HPP_

#include <string>
#include <vector>

#include "SaveAllProgress.h"

struct Savelist
{
	u64 Tid;
	std::string Path;
	std::string Name;
	std::string Subname;
};

class SaveAllTools
{
	public:
		SaveAllTools();
		~SaveAllTools();
		
		void Reset();
		int GetResult() { return returnVal; };
		void ExtractAll_Saves_Nand_Dev(std::string destPath);
		void InstallAll_Saves_Nand_Emu();
		void InstallAll_Saves_Dev_Nand(std::string devPath);
		void InstallAll_Saves_Dev_Emu(std::string devPath);
		void DeleteAll_Saves_Dev(std::string devPath);
		void ExtractAll_Saves_Emu_Dev(std::string devPath);
		void InstallAll_Saves_Emu_Nand();
		void DeleteAll_Saves_Emu();
		
		enum
		{
			FIX_RESTORE = 0,
			FIX_INSTALL,
		};
		
	protected:
		
		int Internal_ExtractAll_Saves_Nand_Dev(std::string nandpath, std::string devpath);
		int Internal_InstallAll_Saves_Dev_Nand(std::string devpath, std::string nandpath);
		int Internal_InstallAll_Saves_Dev_Emu(std::string devpath, u64 tid);
		int Internal_DeleteAll_Saves_Dev(std::string savePath);
		int Internal_DeleteAll_Saves_Emu(u64 tid);
		
		std::string FixFileName(std::string filename, short mode);
		
		void CreateEmuSavesList();
		void CreateSavesList(std::string destPath);
		void CreateAllPathsList(std::string devPath, std::string nandPath);
		void DeleteAllUselessNandFiles(std::string path);
		void CreateAllMissingNandFiles();
		
		bool firstuse;
		int returnVal;
		
		std::vector <std::string> isfsPath;
		std::vector <std::string> devicePath;
		
		std::vector <struct Savelist*> SavesList;
};

#endif
