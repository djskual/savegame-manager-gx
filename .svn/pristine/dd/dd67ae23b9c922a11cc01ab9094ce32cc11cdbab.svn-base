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
 * SaveManageTools.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include "SaveManageTools.hpp"
#include "ManageButtons.hpp"
#include "../FileOperations/Nand.hpp"
#include "../FileOperations/fileops.h"
#include "../FileOperations/DirList.h"
#include "../Settings/CSettings.h"
#include "../Text/TextTools.hpp"
#include "../Tools/tools.h"
#include "../Tools/StringTools.h"
#include "../Language/gettext.h"
#include "../Saves/DataBin/SaveDataBin.h"
#include "../Saves/WiiSave/WiiSaveDownload.hpp"
#include "../Saves/SaveTools.h"
#include "../menu.h"

static bool AbortRequested = false;

static void AbortCallback(void)
{
	AbortRequested = true;
}

SaveManageTools::SaveManageTools(const char * title, const char * msg)
{
	Reset();
	
	InitManageProgressThread();
	ManageProgressSetAbortCallback(AbortCallback);
	StartManageProgress(title, msg);
}

SaveManageTools::~SaveManageTools()
{
	isfsPath.clear();
	devicePath.clear();
	
	ExitManageProgressThread();
}

void SaveManageTools::Reset()
{
	firstuse = true;
	returnVal = 0;
	
	isfsPath.clear();
	devicePath.clear();
}

std::string SaveManageTools::FixFileName(std::string filename, short mode)
{
	if(mode == FIX_RESTORE)
	{
		filename.assign(StringReplace(filename, "(COLON)", "&cl;"));
		filename.assign(StringReplace(filename, "(STAR)", "&st;"));
		filename.assign(StringReplace(filename, "(QUOTE)", "&qt;"));
		filename.assign(StringReplace(filename, "(INTER)", "&qm;"));
		filename.assign(StringReplace(filename, "(VERTICAL)", "&vb;"));
		filename.assign(StringReplace(filename, "(INF)", "&lt;"));
		filename.assign(StringReplace(filename, "(SUP)", "&gt;"));
	}
	else //! FIX_INSTALL
	{
		filename.assign(StringReplace(filename, "&cl;", ":"));
		filename.assign(StringReplace(filename, "&st;", "*"));
		filename.assign(StringReplace(filename, "&qt;", "\""));
		filename.assign(StringReplace(filename, "&qm;", "?"));
		filename.assign(StringReplace(filename, "&vb;", "|"));
		filename.assign(StringReplace(filename, "&lt;", "<"));
		filename.assign(StringReplace(filename, "&gt;", ">"));
	}
	
	return filename;
}

void SaveManageTools::InstallSave_Nand_Emu(u64 tid)
{
	//! Set title UID
	if (ES_SetUID(tid) < 0)
	{
		returnVal = -1;
		return;
	}
	
	//! Get NAND path
	char* buffer = (char*)memalign(32, ISFS_MAXPATH);
	if (ES_GetDataDir(tid, buffer) < 0)
	{
		returnVal = -1;
		return;
	}
	std::string nandPath = buffer;
	free(buffer);
	
	Settings.TmpPath = fmt("%s/title/%08x/%08x/data/", Settings.EmuNandPath, TITLE_UPPER(tid), TITLE_LOWER(tid));
	ExtractSave_Nand_Dev(nandPath, Settings.TmpPath);
}

void SaveManageTools::InstallSave_Dev_Emu(std::string devpath, u64 tid)
{
	std::string emulPath = fmt("%s/title/%08x/%08x/data", Settings.EmuNandPath, TITLE_UPPER(tid), TITLE_LOWER(tid));
	
	int result = -1;
	
	DirList * dirList = new DirList(devpath, NULL, DirList::Files | DirList::Dirs | DirList::CheckSubfolders);
	if(dirList)
	{
		result = 1;
		for(int i = 0; (i < dirList->GetFilecount()) && (result > 0); i++)
		{
			if(!dirList->IsDir(i))
			{
				std::string tmp_emul = dirList->GetFilepath(i);
				tmp_emul.erase(0, devpath.size());
				tmp_emul.insert(0, emulPath);
				
				CreateSubfolder(GetParentDir(tmp_emul));
				result = CopyFile(dirList->GetFilepath(i), tmp_emul.c_str());
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

void SaveManageTools::ExtractSave_Nand_Dev(std::string nandpath, std::string devpath)
{
	if(!Nand::ExtractDir(nandpath, devpath))
	{
		returnVal = -1;
		return;
	}
	
	if (AbortRequested)
	{
		returnVal = -10;
		return;
	}
	
	returnVal = 1;
}

void SaveManageTools::CreatePathsList(std::string devPath, std::string nandPath)
{
	isfsPath.clear();
	devicePath.clear();
	
	DirList * dirList = new DirList(devPath, ".sgmgx_index", DirList::Files | DirList::CheckSubfolders);
	for(int i = 0; i < dirList->GetFilecount(); i++)
	{
		RemoveFile(dirList->GetFilepath(i));
	}
	delete dirList;
	
	dirList = new DirList(devPath, NULL, DirList::Files | DirList::Dirs | DirList::CheckSubfolders);
	
	for(int i = 0; i < dirList->GetFilecount(); i++)
	{
		if((!dirList->IsDir(i)) && (strcasecmp(dirList->GetFilename(i), "index.sgmgx")))
		{
			std::string tmpName = dirList->GetFilepath(i);
			tmpName.erase(0, devPath.size());
			tmpName = FixFileName(tmpName, FIX_RESTORE);
			std::string tmpPath = devPath;
			if(tmpPath.at(tmpPath.size()-1) != '/')
				tmpPath += "/";
			tmpPath += tmpName;
			devicePath.push_back(tmpPath);
			
			std::string tmpNand = dirList->GetFilepath(i);
			tmpNand.erase(0, devPath.size());
			tmpNand.insert(0, nandPath);
			tmpNand = FixFileName(tmpNand, FIX_INSTALL);
			
			isfsPath.push_back(tmpNand);
		}
		else if(strcasecmp(dirList->GetFilename(i), "index.sgmgx"))
		{
			RemoveFile(dirList->GetFilepath(i));
		}
	}
	delete dirList;
}

void SaveManageTools::DeleteUselessNandFiles(std::string path)
{
	std::vector<std::string> List;
	if( Nand::ReadDir(path, List) == ISFS_OK )
	{
		for (int i = 0; (u32)i < List.size(); i++)
		{
			std::string nandPath = path;
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

void SaveManageTools::CreateMissingNandFiles()
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

void SaveManageTools::InstallSave_Dev_Nand(std::string devPath, std::string nandPath)
{
	int result = 1;
	
	CreatePathsList(devPath, nandPath);
	DeleteUselessNandFiles(nandPath);
	CreateMissingNandFiles();
	
	for(u32 i = 0; (i < isfsPath.size()) && (result == 1); i++)
	{
		result = -1;
		
		if(!CheckFile(devicePath.at(i)))
			RestoreSaveFile(devicePath.at(i));
		
		u8* buffer = NULL;
		u64 size = 0;
		if(LoadFileToMem(devicePath.at(i), &buffer, &size) > 0)
		{
			if(Nand::Write(isfsPath.at(i), buffer, (u32)size) > 0)
				result = 1;
		}
	}
	
	if(AbortRequested)
		result = -10;
	
	returnVal = result;
}

void SaveManageTools::DeleteSave_Dev(std::string savePath)
{
	int result = RemoveDirectory(savePath);
	
	if (result <= 0)
	{
		returnVal = -1;
		return;
	}
	
	if(AbortRequested)
		result =  -10;
	
	returnVal = result;
}

void SaveManageTools::CompressSave_Nand_Dev(u64 tid, std::string destPath)
{
	//! Set title UID
	if (ES_SetUID(tid) < 0)
	{
		returnVal = -1;
		return;
	}
	//! create final dest path
	destPath += "/";
	destPath += titleText((u32)(tid >> 32),(u32)(tid & 0xFFFFFFFF));
	if(!CreateSubfolder(destPath))
	{
		returnVal = -2;
		return;
	}
	
	//! create new databin struct from nand files
	SaveDataBin * databin = new SaveDataBin(tid);
	if (!databin->IsOk())
	{
		delete databin;
		returnVal = -3;
		return;
	}
	
	//! create data.bin file from struct
	if (!databin->StructToDataBin(destPath))
	{
		delete databin;
		returnVal = -4;
		return;
	}
	
	delete databin;
	
	returnVal = 1;
}

void SaveManageTools::MoveSave(std::string savePath, std::string destPath)
{
	int result = MoveDirectory((char*)savePath.c_str(), destPath.c_str());
	
	if (result <= 0)
	{
		returnVal = -1;
		return;
	}
	
	if(AbortRequested)
		result =  -10;
	
	returnVal = result;
}

void SaveManageTools::CopySave(std::string savePath, std::string destPath)
{
	int result = CopyDirectory(savePath.c_str(), destPath.c_str());
	
	if (result <= 0)
	{
		returnVal = -1;
		return;
	}
	
	if(AbortRequested)
		result =  -10;
	
	returnVal = result;
}

void SaveManageTools::DownloadSave(std::string ID)
{
	returnVal = WiiSave::Download(ID);	
}
