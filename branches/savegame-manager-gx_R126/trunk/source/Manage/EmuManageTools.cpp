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
 * EmuManageTools.cpp
 *
 * for SaveGame Manager GX 2012
 ***************************************************************************/
#include "EmuManageTools.hpp"
#include "ManageButtons.hpp"
#include "../FileOperations/Nand.hpp"
#include "../FileOperations/fileops.h"
#include "../FileOperations/DirList.h"
#include "../Settings/CSettings.h"
#include "../Text/TextTools.hpp"
#include "../Tools/StringTools.h"
#include "../Language/gettext.h"
#include "../Saves/WiiSave/WiiSaveDownload.hpp"
#include "../Saves/SaveTools.h"
#include "../menu.h"

static bool AbortRequested = false;

static void AbortCallback(void)
{
	AbortRequested = true;
}

EmuManageTools::EmuManageTools(const char * title, const char * msg)
{
	Reset();
	
	InitManageProgressThread();
	ManageProgressSetAbortCallback(AbortCallback);
	StartManageProgress(title, msg);
}

EmuManageTools::~EmuManageTools()
{
	isfsPath.clear();
	emuPath.clear();
	
	ExitManageProgressThread();
}

void EmuManageTools::Reset()
{
	firstuse = true;
	returnVal = 0;
	
	isfsPath.clear();
	emuPath.clear();;
}

std::string EmuManageTools::FixFileName(std::string filename)
{
	filename.assign(StringReplace(filename, "&cl;", ":"));
	filename.assign(StringReplace(filename, "&st;", "*"));
	filename.assign(StringReplace(filename, "&qt;", "\""));
	filename.assign(StringReplace(filename, "&qm;", "?"));
	filename.assign(StringReplace(filename, "&vb;", "|"));
	filename.assign(StringReplace(filename, "&lt;", "<"));
	filename.assign(StringReplace(filename, "&gt;", ">"));
	
	return filename;
}

void EmuManageTools::ExtractSave_Emu_Dev(u64 tid, std::string devpath)
{
	std::string emul_path = fmt("%s/title/%08x/%08x/data/", Settings.EmuNandPath, TITLE_UPPER(tid), TITLE_LOWER(tid));
	int result = -1;
	
	DirList * dirList = new DirList(emul_path, NULL, DirList::Files | DirList::Dirs | DirList::CheckSubfolders);
	if(dirList)
	{
		result = 1;
		for(int i = 0; (i < dirList->GetFilecount()) && (result > 0); i++)
		{
			if(!dirList->IsDir(i))
			{
				std::string tmp_dev = dirList->GetFilepath(i);
				tmp_dev.erase(0, emul_path.size()-1);
				tmp_dev.insert(0, devpath);
				
				CreateSubfolder(GetParentDir(tmp_dev));
				result = CopyFile(dirList->GetFilepath(i), tmp_dev.c_str());
			}
		}
		
		delete dirList;
	}
	
	if (result <= 0)
		result = -1;
	
	if(AbortRequested && result > 0)
		result =  -10;
	
	returnVal = result;
}

void EmuManageTools::CreatePathsList(u64 tid, std::string nandPath)
{
	isfsPath.clear();
	emuPath.clear();
	
	std::string emulPath = fmt("%s/title/%08x/%08x/data/", Settings.EmuNandPath, TITLE_UPPER(tid), TITLE_LOWER(tid));
	
	DirList * dirList = new DirList(emulPath, NULL, DirList::Files | DirList::Dirs | DirList::CheckSubfolders);
	if(dirList)
	{
		for(int i = 0; i < dirList->GetFilecount(); i++)
		{
			if(!dirList->IsDir(i))
			{
				emuPath.push_back(dirList->GetFilepath(i));
				
				std::string tmpNand = dirList->GetFilepath(i);
				tmpNand = FixFileName(tmpNand);
				tmpNand.erase(0, emulPath.size()-1);
				tmpNand.insert(0, nandPath);
				
				isfsPath.push_back(tmpNand);
			}
		}
		delete dirList;
	}
}

void EmuManageTools::DeleteUselessNandFiles(std::string path)
{
	std::vector<std::string> List;
	if( Nand::ReadDir(path, List) == ISFS_OK )
	{
		for (int i = 0; (u32)i < List.size(); i++)
		{
			std::string nandPath = path;
			if(nandPath[nandPath.size()-1] != '/')
				nandPath += "/";
			nandPath += List.at(i);
			
			int fd = Nand::OpenRead(nandPath);
			if(fd < 0)
				DeleteUselessNandFiles(nandPath);
			else
			{
				Nand::Close(fd);
				bool need_file = false;
				for(u32 cur = 0; (cur < isfsPath.size()) && (!need_file); cur++)
				{
					if(!nandPath.compare(isfsPath.at(cur)))
						need_file = true;
				}
				if(!need_file)
					Nand::Delete(nandPath);
			}
		}
	}
}

void EmuManageTools::CreateMissingNandFiles()
{
	for(u32 i = 0; i < isfsPath.size(); i++)
	{
		int fd = Nand::OpenRead(isfsPath.at(i));
		if(fd < 0)
			Nand::CreateFile(isfsPath.at(i), 0, 3, 3, 3);
		else
			Nand::Close(fd);
	}
}

void EmuManageTools::InstallSave_Emu_Nand(u64 tid, std::string nandPath)
{
	int result = 1;
	
	CreatePathsList(tid, nandPath);
	DeleteUselessNandFiles(nandPath);
	CreateMissingNandFiles();
	
	for(u32 i = 0; (i < isfsPath.size()) && (result == 1); i++)
	{
		result = -1;
		
		u8* buffer = NULL;
		u64 size = 0;
		if(LoadFileToMem(emuPath.at(i), &buffer, &size) > 0)
		{
			if(Nand::Write(isfsPath.at(i), buffer, (u32)size) > 0)
				result = 1;
		}
	}
	
	if(AbortRequested && (result > 0))
		result = -10;
	
	returnVal = result;
}

void EmuManageTools::DeleteSave_Emu(u64 tid)
{
	std::string emulPath = fmt("%s/title/%08x/%08x/data/", Settings.EmuNandPath, TITLE_UPPER(tid), TITLE_LOWER(tid));
	
	int result = RemoveDirectory(emulPath);
	
	if (result <= 0)
	{
		returnVal = -1;
		return;
	}
	
	if(AbortRequested)
		result =  -10;
	
	returnVal = result;
}

void EmuManageTools::DownloadSave(std::string ID)
{
	returnVal = WiiSave::Download(ID);	
}
