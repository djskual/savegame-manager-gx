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
 * SaveAllTools.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <malloc.h>

#include "SaveAllTools.hpp"
#include "CErrorLog.h"
#include "../EmuNand/CEmuList.hpp"
#include "../Saves/CSaveList.hpp"
#include "../Saves/SaveTools.h"
#include "../FileOperations/fileops.h"
#include "../FileOperations/DirList.h"
#include "../FileOperations/Nand.hpp"
#include "../Text/TextTools.hpp"
#include "../Tools/StringTools.h"
#include "../Tools/tools.h"
#include "../Language/gettext.h"
#include "../Settings/CSettings.h"


#include "../Tools/gecko.h"



static bool AbortRequested = false;

static void AbortCallback(void)
{
	AbortRequested = true;
}

SaveAllTools::SaveAllTools()
{
	AbortRequested = false;
	firstuse = true;
	returnVal = 0;
	
	isfsPath.clear();
	devicePath.clear();
	
	SavesList.clear();
	
	InitSaveAllProgressThread();
	SaveAllProgressSetAbortCallback(AbortCallback);
}

SaveAllTools::~SaveAllTools()
{
	isfsPath.clear();
	devicePath.clear();
	
	SavesList.clear();
	
	ExitSaveAllProgressThread();
}

void SaveAllTools::Reset()
{
	AbortRequested = false;
	firstuse = true;
	returnVal = 0;
	
	isfsPath.clear();
	devicePath.clear();
	
	SavesList.clear();
}

std::string SaveAllTools::FixFileName(std::string filename, short mode)
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

void SaveAllTools::ExtractAll_Saves_Nand_Dev(std::string destPath)
{
	int result = 0;
	int done = 0;
	char* buffer = (char*)memalign(32, ISFS_MAXPATH);
	std::string nandPath;
	std::string devPath = destPath;
	
	for (int cnt = 0; cnt < SaveList.GetFullCount(); cnt++)
	{
		ShowSaveAllProgress(SaveList.GetFullName(cnt).c_str(), SaveList.GetFullSubname(cnt).c_str(), done, SaveList.GetFullCount(), SaveList.GetFullTid(cnt));
		
		devPath += fmt("/%016llx", SaveList.GetFullTid(cnt));
		
		//! Set title UID
		if (ES_SetUID(SaveList.GetFullTid(cnt)) < 0)
		{
			ErrorLog.AddToLog(SaveList.GetFullName(cnt).c_str(), tr("Unable to SetUID"));
			
			goto next;
		}
		
		//! Get NAND path
		if (ES_GetDataDir(SaveList.GetFullTid(cnt), buffer) < 0)
		{
			ErrorLog.AddToLog(SaveList.GetFullName(cnt).c_str(), tr("Unable to get NAND path"));
			
			goto next;
		}
		nandPath = buffer;
		
		result = Internal_ExtractAll_Saves_Nand_Dev(nandPath, devPath);
		if (result < 0)
		{
			devPath += "/";
			RemoveDirectory(devPath.c_str());
		}
		if (result == -10)
			goto out;
		else if (result < 0 && result != -10)
			ErrorLog.AddToLog(SaveList.GetFullName(cnt).c_str(), tr("Unable to copy datas"));
		
	  next:
		
		done++;
		devPath = destPath;
	}
	
  out:
	
	free(buffer);
	
	SaveAllProgressStop();
	
	(result == -10) ? returnVal = -10 : (ErrorLog.GetCount() ? returnVal = -1 : returnVal = 1);
}

int SaveAllTools::Internal_ExtractAll_Saves_Nand_Dev(std::string nandpath, std::string devpath)
{
	if(!Nand::ExtractDir(nandpath, devpath))
		return -1;
	
	if (AbortRequested)
		return -10;
	
	return 1;
}

void SaveAllTools::CreateSavesList(std::string devPath)
{
	SavesList.clear();
	
	std::string path = devPath;
	while(path[path.size()-1] == '/')
		path.erase(path.size()-1, 1);
	
	DirList * dirList = new DirList(path, NULL, DirList::Dirs);
	
	for(int i = 0; i < dirList->GetFilecount(); i++)
	{
		if(CheckSave(dirList->GetFilepath(i)))
		{
			Savelist * save = new Savelist();
			
			save->Tid = StrToHex64(dirList->GetFilename(i));
			save->Path = dirList->GetFilepath(i);
			save->Name = GetDeviceName((char*)dirList->GetFilepath(i));
			save->Subname = GetDeviceSubname((char*)dirList->GetFilepath(i));
			
			SavesList.push_back(save);
		}
	}
	delete dirList;
}

void SaveAllTools::CreateEmuSavesList()
{
	std::string titlePath = fmt("%s/title",Settings.EmuNandPath);
	
	DirList * dirList = new DirList(titlePath, NULL, DirList::Dirs);
	for(int i = 0; i < dirList->GetFilecount(); i++)
	{
		std::string dirName = dirList->GetFilename(i);
		if(!dirName.compare("00000001") || !dirName.compare("00010002") || !dirName.compare("00010005") || !dirName.compare("00010008"))
			continue;
		
		DirList * emuList = new DirList(dirList->GetFilepath(i), ".bin", DirList::Dirs | DirList::Files | DirList::CheckSubfolders);
		for(int j = 0; j < emuList->GetFilecount(); j++)
		{
			std::string banner = emuList->GetFilename(j);
			if(!banner.compare("banner.bin"))
			{
				Savelist * save = new Savelist();
				
				//! Set title name & subname
				std::string savepath = GetParentDir(emuList->GetFilepath(j));
				save->Path = savepath;
				save->Name.assign(GetDeviceName(savepath));
				save->Subname.assign(GetDeviceSubname(savepath));
				
				//! Set titleid
				std::string str_tid = savepath;
				str_tid.erase(str_tid.find_last_of('/'));
				str_tid.erase(0, str_tid.find_last_of('/')+1);
				str_tid.insert(0, dirName);
				save->Tid = StrToHex64(str_tid.c_str());
				
				SavesList.push_back(save);
			}
		}
		delete emuList;
	}
	delete dirList;
}

void SaveAllTools::CreateAllPathsList(std::string devPath, std::string nandPath)
{
	isfsPath.clear();
	devicePath.clear();
	
	DirList * dirList = new DirList(devPath.c_str(), ".sgmgx_index", DirList::Files | DirList::CheckSubfolders);
	for(int i = 0; i < dirList->GetFilecount(); i++)
	{
		RemoveFile(dirList->GetFilepath(i));
	}
	delete dirList;
	
	dirList = new DirList(devPath.c_str(), NULL, DirList::Files | DirList::Dirs | DirList::CheckSubfolders);
	
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

void SaveAllTools::DeleteAllUselessNandFiles(std::string path)
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
				DeleteAllUselessNandFiles(nandPath);
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

void SaveAllTools::CreateAllMissingNandFiles()
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

void SaveAllTools::InstallAll_Saves_Nand_Emu()
{
	int result = 0;
	int done = 0;
	int total = SaveList.GetFullCount();
	char* buffer = (char*)memalign(32, ISFS_MAXPATH);
	std::string nandPath;
	std::string emuPath;
	
	for (int cnt = 0; cnt < SaveList.GetFullCount(); cnt++)
	{
		u64 tid = SaveList.GetFullTid(cnt);
		ShowSaveAllProgress(SaveList.GetFullName(cnt).c_str(), SaveList.GetFullSubname(cnt).c_str(), done, total, tid);
		
		emuPath = fmt("%s/title/%08x/%08x/data/", Settings.EmuNandPath, TITLE_UPPER(tid), TITLE_LOWER(tid));
		
		//! Set title UID
		if (ES_SetUID(tid) < 0)
		{
			ErrorLog.AddToLog(SaveList.GetFullName(cnt).c_str(), tr("Unable to SetUID"));
			
			goto next;
		}
		
		//! Get NAND path
		if (ES_GetDataDir(tid, buffer) < 0)
		{
			ErrorLog.AddToLog(SaveList.GetFullName(cnt).c_str(), tr("Unable to get NAND path"));
			
			goto next;
		}
		nandPath = buffer;
		
		result = Internal_ExtractAll_Saves_Nand_Dev(nandPath, emuPath);
		if (result < 0)
		{
			emuPath += "/";
			RemoveDirectory(emuPath.c_str());
		}
		if (result == -10)
			goto out;
		else if (result < 0 && result != -10)
			ErrorLog.AddToLog(SaveList.GetFullName(cnt).c_str(), tr("Unable to copy datas"));
		
	  next:
		
		done++;
	}
	
  out:
	
	free(buffer);
	
	SaveAllProgressStop();
	
	(result == -10) ? returnVal = -10 : (ErrorLog.GetCount() ? returnVal = -1 : returnVal = 1);
}

void SaveAllTools::InstallAll_Saves_Dev_Nand(std::string devPath)
{
	CreateSavesList(devPath);
	
	if (SavesList.empty())
	{
		ErrorLog.AddToLog(tr( "No Save Found in this directory" ), " ");
		returnVal = -1;
		return;
	}
	
	int result = 0;
	int done = 0;
	char* buffer = (char*)memalign(32, ISFS_MAXPATH);
	std::string nandPath;
	
	for(u32 i = 0; i < SavesList.size(); i++)
	{
		ShowSaveAllProgress(SavesList.at(i)->Name.c_str(), SavesList.at(i)->Subname.c_str(), done, SavesList.size(), 0, SavesList.at(i)->Path);
		
		//! Set title UID
		if (ES_SetUID(SavesList.at(i)->Tid) < 0)
		{
			ErrorLog.AddToLog(SavesList.at(i)->Name.c_str(), tr("Unable to SetUID"));
			goto next;
		}
		
		//! Get NAND path
		if (ES_GetDataDir(SavesList.at(i)->Tid, buffer) < 0)
		{
			ErrorLog.AddToLog(SavesList.at(i)->Name.c_str(), tr("Unable to get NAND path"));
			goto next;
		}
		nandPath = buffer;
		
		result = Internal_InstallAll_Saves_Dev_Nand(SavesList.at(i)->Path, nandPath);
		if (result == -10)
			goto out;
		else if (result < 0 && result != -10)
			ErrorLog.AddToLog(SavesList.at(i)->Name.c_str(), tr("Unable to copy datas"));
		
	  next:
		
		done++;
	}
	
  out:
	
	if(buffer) free(buffer);
	
	SaveAllProgressStop();
	
	(result == -10) ? returnVal = -10 : (ErrorLog.GetCount() ? returnVal = -1 : returnVal = 1);
}

int SaveAllTools::Internal_InstallAll_Saves_Dev_Nand(std::string devPath, std::string nandPath)
{
	int result = 1;
	
	CreateAllPathsList(devPath, nandPath);
	DeleteAllUselessNandFiles(nandPath);
	CreateAllMissingNandFiles();
	
	for(u32 i = 0; (i < isfsPath.size()) && (result == 1); i++)
	{
		result = -1;
		
		if(!CheckFile(devicePath.at(i)))
			RestoreSaveFile(devicePath.at(i));
		
		u8* buffer = NULL;
		u64 size = 0;
		if(LoadFileToMem(devicePath.at(i).c_str(), &buffer, &size) > 0)
		{
			if(Nand::Write(isfsPath.at(i), buffer, (u32)size) > 0)
				result = 1;
		}
	}
	
	if(AbortRequested)
		result = -10;
	
	return result;
}

void SaveAllTools::InstallAll_Saves_Dev_Emu(std::string devPath)
{
	CreateSavesList(devPath);
	
	if (SavesList.empty())
	{
		ErrorLog.AddToLog(tr( "No Save Found in this directory" ), " ");
		returnVal = -1;
		return;
	}
	
	int result = 0;
	int done = 0;
	std::string emul_path;
	
	for(u32 i = 0; i < SavesList.size(); i++)
	{
		ShowSaveAllProgress(SavesList.at(i)->Name.c_str(), SavesList.at(i)->Subname.c_str(), done, SavesList.size(), 0, SavesList.at(i)->Path);
		
		result = Internal_InstallAll_Saves_Dev_Emu(SavesList.at(i)->Path, SavesList.at(i)->Tid);
		if (result == -10)
			goto out;
		else if (result < 0 && result != -10)
			ErrorLog.AddToLog(SavesList.at(i)->Name.c_str(), tr("Unable to copy datas"));
		
		done++;
	}
	
  out:
	
	SaveAllProgressStop();
	
	returnVal = ((result == -10) ? -10 : (ErrorLog.GetCount() ? -1 : 1));
}

int SaveAllTools::Internal_InstallAll_Saves_Dev_Emu(std::string devpath, u64 tid)
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
	
	return result;
}

void SaveAllTools::DeleteAll_Saves_Dev(std::string devPath)
{
	CreateSavesList(devPath);
	
	if (SavesList.empty())
	{
		ErrorLog.AddToLog(tr( "No Save Found in this directory" ), " ");
		returnVal = -1;
		return;
	}
	
	int result = 0;
	
	for(u32 i = 0; i < SavesList.size(); i++)
	{
		ShowSaveAllProgress(SavesList.at(i)->Name.c_str(), SavesList.at(i)->Subname.c_str(), i, SavesList.size(), 0, SavesList.at(i)->Path);
		usleep(700000);
		
		result = Internal_DeleteAll_Saves_Dev(SavesList.at(i)->Path);
		if (result == -10)
			goto out;
		else if (result < 0 && result != -10)
			ErrorLog.AddToLog(SavesList.at(i)->Name.c_str(), tr("Unable to delete datas"));
	}
	
  out:
	
	SaveAllProgressStop();
	
	(result == -10) ? returnVal = -10 : (ErrorLog.GetCount() ? returnVal = -1 : returnVal = 1);
}

int SaveAllTools::Internal_DeleteAll_Saves_Dev(std::string savePath)
{
	savePath += "/";
	
	int result = RemoveDirectory(savePath.c_str());
	
	if (result <= 0)
		return -1;
	
	if(AbortRequested)
		result =  -10;
	
	return result;
}

void SaveAllTools::ExtractAll_Saves_Emu_Dev(std::string destPath)
{
	int result = 0;
	int done = 0;
	int total = EmuList.GetFullCount();
	std::string emuPath;
	std::string devPath = destPath;
	
	for (int cnt = 0; cnt < total; cnt++)
	{
		u64 tid = EmuList.GetFullTid(cnt);
		emuPath = fmt("%s/title/%08x/%08x/data/", Settings.EmuNandPath, TITLE_UPPER(tid), TITLE_LOWER(tid));
		devPath += fmt("/%016llx", tid);
		
		ShowSaveAllProgress(EmuList.GetFullName(cnt).c_str(), EmuList.GetFullSubname(cnt).c_str(), done, total, tid);
		
		DirList * dirList = new DirList(emuPath, NULL, DirList::Files | DirList::Dirs | DirList::CheckSubfolders);
		if(dirList)
		{
			result = 1;
			for(int i = 0; (i < dirList->GetFilecount()) && (result > 0); i++)
			{
				if(!dirList->IsDir(i))
				{
					std::string tmp_dev = dirList->GetFilepath(i);
					tmp_dev.erase(0, emuPath.size()-1);
					tmp_dev.insert(0, devPath);
					
					CreateSubfolder(GetParentDir(tmp_dev));
					result = CopyFile(dirList->GetFilepath(i), tmp_dev.c_str());
					
					if(AbortRequested)
					{
						result = -10;
						delete dirList;
						devPath += "/";
						RemoveDirectory(devPath);
						goto out;
					}
					else if (result <= 0)
					{
						result = -1;
						ErrorLog.AddToLog(SaveList.GetFullName(cnt).c_str(), tr("Unable to copy datas"));
						devPath += "/";
						RemoveDirectory(devPath);
					}
				}
			}
			
			delete dirList;
		}
		
		done++;
		devPath = destPath;
	}

  out:
	
	SaveAllProgressStop();
	
	returnVal = result;
}

void SaveAllTools::InstallAll_Saves_Emu_Nand()
{
	CreateEmuSavesList();
	
	if (SavesList.empty())
	{
		ErrorLog.AddToLog(tr( "No Save Found in this directory" ), " ");
		returnVal = -1;
		return;
	}
	
	int result = 0;
	int done = 0;
	char* buffer = (char*)memalign(32, ISFS_MAXPATH);
	std::string nandPath;
	
	for(u32 i = 0; i < SavesList.size(); i++)
	{
		ShowSaveAllProgress(SavesList.at(i)->Name.c_str(), SavesList.at(i)->Subname.c_str(), done, SavesList.size(), 0, SavesList.at(i)->Path);
		
		if (!CheckIfInstalled(SavesList.at(i)->Tid))
		{
			ErrorLog.AddToLog(SavesList.at(i)->Name.c_str(), tr("Not already installed in Wii"));
			goto next;
		}
		
		//! Set title UID
		if (ES_SetUID(SavesList.at(i)->Tid) < 0)
		{
			ErrorLog.AddToLog(SavesList.at(i)->Name.c_str(), tr("Unable to SetUID"));
			goto next;
		}
		
		//! Get NAND path
		if (ES_GetDataDir(SavesList.at(i)->Tid, buffer) < 0)
		{
			ErrorLog.AddToLog(SavesList.at(i)->Name.c_str(), tr("Unable to get NAND path"));
			goto next;
		}
		nandPath = buffer;
		
		result = Internal_InstallAll_Saves_Dev_Nand(SavesList.at(i)->Path, nandPath);
		if (result == -10)
			goto out;
		else if (result < 0 && result != -10)
			ErrorLog.AddToLog(SavesList.at(i)->Name.c_str(), tr("Unable to copy datas"));
		
	  next:
		
		done++;
	}
	
  out:
	
	if(buffer) free(buffer);
	
	SaveAllProgressStop();
	
	(result == -10) ? returnVal = -10 : (ErrorLog.GetCount() ? returnVal = -1 : returnVal = 1);
}

void SaveAllTools::DeleteAll_Saves_Emu()
{
	CreateEmuSavesList();
	
	if (SavesList.empty())
	{
		ErrorLog.AddToLog(tr( "No Save Found in this directory" ), " ");
		returnVal = -1;
		return;
	}
	
	int result = 0;
	
	for(u32 i = 0; i < SavesList.size(); i++)
	{
		ShowSaveAllProgress(SavesList.at(i)->Name.c_str(), SavesList.at(i)->Subname.c_str(), i, SavesList.size(), 0, SavesList.at(i)->Path);
		usleep(700000);
		
		result = Internal_DeleteAll_Saves_Emu(SavesList.at(i)->Tid);
		if (result == -10)
			goto out;
		else if (result < 0 && result != -10)
			ErrorLog.AddToLog(SavesList.at(i)->Name.c_str(), tr("Unable to delete datas"));
	}
	
  out:
	
	SaveAllProgressStop();
	
	(result == -10) ? returnVal = -10 : (ErrorLog.GetCount() ? returnVal = -1 : returnVal = 1);
}

int SaveAllTools::Internal_DeleteAll_Saves_Emu(u64 tid)
{
	std::string emulPath = fmt("%s/title/%08x/%08x/data/", Settings.EmuNandPath, TITLE_UPPER(tid), TITLE_LOWER(tid));
	
	int result = RemoveDirectory(emulPath);
	
	if (result <= 0)
		return -1;
	
	if(AbortRequested)
		result =  -10;
	
	return result;
}
