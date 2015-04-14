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
 * MiiAllTools.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "MiiAllTools.hpp"
#include "CErrorLog.h"
#include "../FileOperations/fileops.h"
#include "../FileOperations/DirList.h"
#include "../FileOperations/Nand.hpp"
#include "../Settings/CSettings.h"
#include "../EmuNand/CEmuMiiList.hpp"
#include "../Miis/CMiiList.hpp"
#include "../Miis/MiiInstaller.h"
#include "../Text/TextTools.hpp"
#include "../Tools/tools.h"
#include "../Language/gettext.h"

static bool AbortRequested = false;

static void AbortCallback(void)
{
	AbortRequested = true;
}

MiiAllTools::MiiAllTools()
{
	returnVal = 0;
	firstrun = true;
	
	miiPath.clear();
	miiName.clear();
	miiCreator.clear();
	miiPosition.clear();
	miiFemale.clear();
	miiDay.clear();
	miiMonth.clear();
	miiFavColor.clear();
	
	InitMiiAllProgressThread();
	MiiAllProgressSetAbortCallback(AbortCallback);
}

MiiAllTools::~MiiAllTools()
{
	miiPath.clear();
	miiName.clear();
	miiCreator.clear();
	miiPosition.clear();
	miiFemale.clear();
	miiDay.clear();
	miiMonth.clear();
	miiFavColor.clear();
	
	ExitMiiAllProgressThread();
}

void MiiAllTools::Reset()
{
	returnVal = 0;
	firstrun = true;
	
	miiPath.clear();
	miiName.clear();
	miiCreator.clear();
	miiPosition.clear();
	miiFemale.clear();
	miiDay.clear();
	miiMonth.clear();
	miiFavColor.clear();
}

std::string MiiAllTools::FixMiiName(std::string filename)
{
	filename.assign(StringReplace(filename, ":", "_"));
	filename.assign(StringReplace(filename, "?", "_"));
	
	return filename;
}

void MiiAllTools::ExtractAll_Miis_Nand_Dev(std::string destPath)
{
	MiiList.Get();
	
	int result = 0;
	int done = 0;
	
	for (int cnt = 0; cnt < MiiList.GetFullCount(); cnt++)
	{
		ShowMiiAllProgress(MiiList.GetFullName(cnt).c_str(),
						   done,
						   MiiList.GetFullCount(),
						   cnt);
		
		result = Internal_ExtractAll_Miis_Nand_Dev(destPath, cnt);
		if (result == -10)
			goto out;
		else if (result < 0 && result != -10)
			ErrorLog.AddToLog(MiiList.GetFullName(cnt).c_str(), tr("Unable to copy datas"));
		
		done++;
		usleep(800000);
	}
	
  out:
	
	MiiAllProgressStop();
	
	(result == -10) ? returnVal = -10 : (ErrorLog.GetCount() ? returnVal = -1 : returnVal = 1);
}

int MiiAllTools::Internal_ExtractAll_Miis_Nand_Dev(std::string devpath, int selected)
{
	if (firstrun)
	{
		MiiList.Get();
		CreateSubfolder(devpath.c_str());
		firstrun = false;
	}
	
	//! Get extract file name
	std::string destPath = devpath;
	if (destPath[destPath.size()-1] != '/')
		destPath += "/";
	destPath += FixMiiName(MiiList.GetFullName(selected));
	destPath += ".miigx";
	
	int miisrc = Nand::OpenRead(MII_FILE);
	if (miisrc >= 0)
	{
		int pos = MiiList.GetFullPosition(selected);
		Nand::Seek(miisrc, MII_HEADER+(pos*MII_SIZE), 0);
		
		u8*memory = (u8*)Tools::AllocateMemory_32(MII_SIZE);
		if(!memory)
		{
			Nand::Close(miisrc);
			return -1;
		}
		
		//! Read Mii
		int ret = Nand::Read(miisrc, memory, MII_SIZE);
		if (ret != MII_SIZE)
		{
			Nand::Close(miisrc);
			free(memory);
			memory = NULL;
			return -1;
		}
		Nand::Close(miisrc);
		
		//! Open destination file
		FILE * miidest = fopen(destPath.c_str(), "wb");
		if(!miidest)
		{
			free(memory);
			memory = NULL;
			return -1;
		}
		
		//! Write file
		fwrite(memory, 1, MII_SIZE, miidest);
		fclose(miidest);
		free(memory);
		memory = NULL;
		
		if (AbortRequested)
			return -10;
		else
			return 1;
		
	}
	
	return -1;
}

void MiiAllTools::ExtractAll_Miis_Emu_Dev(std::string destPath)
{
	EmuMiiList.Get();
	
	int result = 0;
	int done = 0;
	
	for (int cnt = 0; cnt < EmuMiiList.GetFullCount(); cnt++)
	{
		ShowMiiAllProgress(EmuMiiList.GetFullName(cnt).c_str(),
						   done,
						   EmuMiiList.GetFullCount(),
						   -1,
						   cnt);
		
		result = Internal_ExtractAll_Miis_Emu_Dev(destPath, cnt);
		if (result == -10)
			goto out;
		else if (result < 0 && result != -10)
			ErrorLog.AddToLog(EmuMiiList.GetFullName(cnt).c_str(), tr("Unable to copy datas"));
		
		done++;
		usleep(800000);
	}
	
  out:
	
	MiiAllProgressStop();
	
	(result == -10) ? returnVal = -10 : (ErrorLog.GetCount() ? returnVal = -1 : returnVal = 1);
}

int MiiAllTools::Internal_ExtractAll_Miis_Emu_Dev(std::string devpath, int selected)
{
	if (firstrun)
	{
		EmuMiiList.Get();
		CreateSubfolder(devpath.c_str());
		firstrun = false;
	}
	
	//! Get extract file name
	std::string destPath = devpath;
	if (destPath[destPath.size()-1] != '/')
		destPath += "/";
	destPath += FixMiiName(EmuMiiList.GetFullName(selected));
	destPath += ".miigx";
	
	std::string EmuMiiDBPath = Settings.EmuNandPath;
	EmuMiiDBPath += MII_FILE;
	
	//! Read Mii
	u8* memory = (u8*)memalign(32, MII_SIZE);
	if(memory)
	{
		FILE * miisrc = fopen(EmuMiiDBPath.c_str(), "rb");
		if (miisrc)
		{
			int pos = EmuMiiList.GetFullPosition(selected);
			fseek(miisrc, MII_HEADER+(pos*MII_SIZE), SEEK_SET);
			
			if (fread(memory, 1, MII_SIZE, miisrc) != MII_SIZE)
			{
				fclose(miisrc);
				free(memory);
				memory = NULL;
				return -1;
			}
			
			fclose(miisrc);
			
			//! Open destination file
			FILE * miidest = fopen(destPath.c_str(), "wb");
			if(!miidest)
			{
				free(memory);
				memory = NULL;
				return -1;
			}
			
			//! Write file
			fwrite(memory, 1, MII_SIZE, miidest);
			fclose(miidest);
			free(memory);
			memory = NULL;
			
			if (AbortRequested)
				return -10;
			else
				return 1;
			
		}
	}
	
	return -1;
}

void MiiAllTools::GetMiiName(std::string miipath)
{
	char name[31];
	memset(name, 0, sizeof(name));
	
	FILE *fp = fopen(miipath.c_str(),"rb");

	if(fp == NULL)
		return;
	
	fseek(fp , 2, SEEK_SET);
	u16* namedata = (u16*)Tools::AllocateMemory_32(20);
	memset(namedata, 0, sizeof(namedata));
	
	fread(namedata,1,20,fp);
	
	fclose(fp);
	
	wchar_t wname[20];
	memset(wname, 0, 20);
	
	u32 i = 0;
	for (i = 0; i < 20; i++)
		wname[i] = (wchar_t)namedata[i];
	
	free(namedata);
	namedata = NULL;
	
	wString *ws_name = new wString(wname);
	
	snprintf(name, 31, "%s", (ws_name->toUTF8()).c_str());
	
	delete ws_name;
	
	miiName.push_back(name);
}

void MiiAllTools::GetMiiCreator(std::string miipath)
{
	char creator[31];
	memset(creator, 0, sizeof(creator));
	
	FILE *fp = fopen(miipath.c_str(),"rb");

	if(fp == NULL)
		return;
	
	fseek(fp , 0x36, SEEK_SET);
	u16* creatordata = (u16*)Tools::AllocateMemory_32(20);
	memset(creatordata, 0, sizeof(creatordata));
	
	fread(creatordata,1,20,fp);
	
	fclose(fp);
	
	wchar_t wcreator[20];
	memset(wcreator, 0, 20);
	
	u32 i = 0;
	for (i = 0; i < 20; i++)
		wcreator[i] = (wchar_t)creatordata[i];
	
	free(creatordata);
	creatordata = NULL;
	
	wString *ws_creator = new wString(wcreator);
	
	snprintf(creator, 31, "%s", (ws_creator->toUTF8()).c_str());
	
	delete ws_creator;
	
	miiCreator.push_back(creator);
}

static char * readMii(std::string path)
{
	FILE * File = fopen(path.c_str(), "rb");
	if (File == NULL)
		return NULL;
	
	char * buffer = (char*)malloc(MII_SIZE);
	fread(buffer, 1, MII_SIZE, File);
	fclose(File);
	
	return buffer;
}

void MiiAllTools::GetMiiInfos(std::string miipath)
{
	char * data;
	data = readMii(miipath);
	if (!data)
		return;
	
	int day = 0;
	if (data[0x00] & 1) day += 8;
	if (data[0x00] & 2) day += 16;
	if (data[0x01] & 32) day += 1;
	if (data[0x01] & 64) day += 2;
	if (data[0x01] & 128) day += 4;
	miiDay.push_back(day);
	
	int month = 0;
	if (data[0x00] & 4) month += 1;
	if (data[0x00] & 8) month += 2;
	if (data[0x00] & 16) month += 4;
	if (data[0x00] & 32) month += 8;
	miiMonth.push_back(month);
	
	miiFemale.push_back(data[0x00] & 64);
	
	int favColor = 0;
	if (data[0x01] & 2) favColor += 1;
	if (data[0x01] & 4) favColor += 2;
	if (data[0x01] & 8) favColor += 4;
	if (data[0x01] & 16) favColor += 8;
	miiFavColor.push_back(favColor);
	
	free(data);
	data = NULL;
}

void MiiAllTools::CreateMiiList(std::string devPath)
{
	DirList * dirList = new DirList(devPath, ".miigx", DirList::Files);
	
	for(int i = 0; i < dirList->GetFilecount(); i++)
	{
		std::string miipath = dirList->GetFilepath(i);
		
		miiPath.push_back(miipath);
		
		GetMiiName(miipath);
		GetMiiCreator(miipath);
		GetMiiInfos(miipath);
	}
	delete dirList;
}

int MiiAllTools::GetFirstFree(bool emu)
{
	int pos = 0;
	
	for(pos = 0; pos < MII_MAX; pos++)
	{
		bool freeSpaceOk = true;
		int fullCount = (emu ? EmuMiiList.GetFullCount() : MiiList.GetFullCount());
		for(int n = 0; n < fullCount; n++)
		{
			if((emu ? EmuMiiList.GetFullPosition(n) : MiiList.GetFullPosition(n)) == pos)
			{
				freeSpaceOk = false;
				break;
			}
		}
		if(freeSpaceOk == true)
			break;
	}
	
	if(pos >= MII_MAX)
		return -1;
	
	miiPosition.push_back(pos);
	
	return 1;
}

int MiiAllTools::GetFirstDestFree(bool EmuToNand)
{
	int pos = 0;
	int fullCount = (EmuToNand ? MiiList.GetFullCount() : EmuMiiList.GetFullCount());
	
	for(pos = 0; pos < MII_MAX; pos++)
	{
		bool freeSpaceOk = true;
		for(int n = 0; n < fullCount; n++)
		{
			if((EmuToNand ? MiiList.GetFullPosition(n) : EmuMiiList.GetFullPosition(n)) == pos)
			{
				freeSpaceOk = false;
				break;
			}
		}
		if(freeSpaceOk == true)
			break;
	}
	
	if(pos >= MII_MAX)
		return -1;
	
	return pos;
}

int MiiAllTools::GetPosition(int ind, bool emu)
{
	if(emu)
	{
		for (int i = 0; i < EmuMiiList.GetFullCount(); i++)
		{
			if (!EmuMiiList.GetFullName(i).compare(miiName.at(ind)))
			{
				miiPosition.push_back(EmuMiiList.GetFullPosition(i));
				return 1;
			}
		}
	}
	else
	{
		for (int i = 0; i < MiiList.GetFullCount(); i++)
		{
			if (!MiiList.GetFullName(i).compare(miiName.at(ind)))
			{
				miiPosition.push_back(MiiList.GetFullPosition(i));
				return 1;
			}
		}
	}
	
	return GetFirstFree(emu);
}

int MiiAllTools::GetDestPosition(int ind, bool EmuToNand)
{
	if(EmuToNand)
	{
		for (int i = 0; i < MiiList.GetFullCount(); i++)
		{
			if (!MiiList.GetFullName(i).compare(EmuMiiList.GetFullName(ind)))
				return MiiList.GetFullPosition(i);
		}
	}
	else
	{
		for (int i = 0; i < EmuMiiList.GetFullCount(); i++)
		{
			if (!EmuMiiList.GetFullName(i).compare(MiiList.GetFullName(ind)))
				return EmuMiiList.GetFullPosition(i);
		}
	}
	
	return GetFirstDestFree(EmuToNand);
}

void MiiAllTools::InstallAll_Miis_Dev_Nand(std::string devPath)
{
	CreateMiiList(devPath);
	
	if (miiPath.empty())
	{
		ErrorLog.AddToLog(tr("No Mii Found in this path"), " ");
		returnVal = -1;
		return;
	}
	
	int result = 0;
	int done = 0;
	
	for (u32 i = 0; i < miiPath.size(); i++)
	{
		ShowMiiAllProgress(miiName.at(i).c_str(), done, miiPath.size(), -1, -1, miiCreator.at(i), miiFemale.at(i), miiDay.at(i), miiMonth.at(i), miiFavColor.at(i));
		
		if (GetPosition(i) < 0)
		{
			ErrorLog.AddToLog(tr( "Mii memory is full" ), tr( "100 miis are installed!!!" ));
			goto out;
		}
		
		result = Mii::InstallNand(miiPath.at(i), miiPosition.at(i));
		if (result == -10)
			goto out;
		else if (result < 0 && result != -10)
			ErrorLog.AddToLog(miiName.at(i).c_str(), tr("Unable to copy datas"));
		
		MiiList.Get();
		
		done++;
		usleep(700000);
	}
	
  out:
	
	MiiAllProgressStop();
	
	(result == -10) ? returnVal = -10 : (ErrorLog.GetCount() ? returnVal = -1 : returnVal = 1);
}

void MiiAllTools::InstallAll_Miis_Dev_Emu(std::string devPath)
{
	EmuMiiList.Get();
	CreateMiiList(devPath);
	
	if (miiPath.empty())
	{
		ErrorLog.AddToLog(tr("No Mii Found in this path"), " ");
		returnVal = -1;
		return;
	}
	
	int result = 0;
	int done = 0;
	
	for (u32 i = 0; i < miiPath.size(); i++)
	{
		ShowMiiAllProgress(miiName.at(i).c_str(), done, miiPath.size(), -1, -1, miiCreator.at(i), miiFemale.at(i), miiDay.at(i), miiMonth.at(i), miiFavColor.at(i));
		
		if (GetPosition(i, true) < 0)
		{
			ErrorLog.AddToLog(tr( "Mii memory is full" ), tr( "100 miis are installed!!!" ));
			goto out;
		}
		
		result = Mii::InstallEmu(miiPath.at(i), miiPosition.at(i));
		if (result == -10)
			goto out;
		else if (result < 0 && result != -10)
			ErrorLog.AddToLog(miiName.at(i).c_str(), tr("Unable to copy datas"));
		
		EmuMiiList.Get();
		
		done++;
		usleep(700000);
	}
	
  out:
	
	MiiAllProgressStop();
	
	(result == -10) ? returnVal = -10 : (ErrorLog.GetCount() ? returnVal = -1 : returnVal = 1);
}

void MiiAllTools::InstallAll_Miis_Nand_Emu()
{
	MiiList.Get();
	EmuMiiList.Get();
	
	if (!MiiList.GetFullCount())
	{
		ErrorLog.AddToLog(tr("No Mii Found in Wii Nand"), " ");
		returnVal = -1;
		return;
	}
	
	int result = 0;
	int done = 0;
	int emuPos = 0;
	
	for (int cnt = 0; cnt < MiiList.GetFullCount(); cnt++)
	{
		ShowMiiAllProgress(MiiList.GetFullName(cnt).c_str(),
						   done,
						   MiiList.GetFullCount(),
						   cnt);
		
		if ((emuPos = GetDestPosition(cnt, false)) < 0)
		{
			ErrorLog.AddToLog(tr( "Mii memory is full" ), tr( "100 miis are installed!!!" ));
			goto out;
		}
		
		result = Mii::NandToEmu(MiiList.GetFullPosition(cnt), emuPos);
		if (result == -10)
			goto out;
		else if (result < 0 && result != -10)
			ErrorLog.AddToLog(MiiList.GetFullName(cnt).c_str(), tr("Unable to copy datas"));
		
		EmuMiiList.Get();
		
		done++;
		usleep(700000);
	}
	
  out:
	
	MiiAllProgressStop();
	
	(result == -10) ? returnVal = -10 : (ErrorLog.GetCount() ? returnVal = -1 : returnVal = 1);
}

void MiiAllTools::InstallAll_Miis_Emu_Nand()
{
	MiiList.Get();
	EmuMiiList.Get();
	
	if (!EmuMiiList.GetFullCount())
	{
		ErrorLog.AddToLog(tr("No Mii Found in Emulated Nand"), " ");
		returnVal = -1;
		return;
	}
	
	int result = 0;
	int done = 0;
	int nandPos = 0;
	
	for (int cnt = 0; cnt < EmuMiiList.GetFullCount(); cnt++)
	{
		ShowMiiAllProgress(EmuMiiList.GetFullName(cnt).c_str(),
						   done,
						   EmuMiiList.GetFullCount(),
						   -1,
						   cnt);
		
		if ((nandPos = GetDestPosition(cnt, true)) < 0)
		{
			ErrorLog.AddToLog(tr( "Mii memory is full" ), tr( "100 miis are installed!!!" ));
			goto out;
		}
		
		result = Mii::EmuToNand(EmuMiiList.GetFullPosition(cnt), nandPos);
		if (result == -10)
			goto out;
		else if (result < 0 && result != -10)
			ErrorLog.AddToLog(EmuMiiList.GetFullName(cnt).c_str(), tr("Unable to copy datas"));
		
		MiiList.Get();
		
		done++;
		usleep(700000);
	}
	
  out:
	
	MiiAllProgressStop();
	
	(result == -10) ? returnVal = -10 : (ErrorLog.GetCount() ? returnVal = -1 : returnVal = 1);
}

void MiiAllTools::DeleteAll_Miis_Nand()
{
	MiiList.Get();
	
	int result = 0;
	int done = 0;
	
	for (int cnt = 0; cnt < MiiList.GetFullCount(); cnt++)
	{
		ShowMiiAllProgress(MiiList.GetFullName(cnt).c_str(), done, MiiList.GetFullCount(), cnt);
		
		result = Internal_DeleteAll_Miis_Nand(MiiList.GetFullPosition(cnt));
		if (result == -10)
			goto out;
		else if (result < 0 && result != -10)
			ErrorLog.AddToLog(MiiList.GetFullName(cnt).c_str(), tr("Unable to delete datas"));
		
		done++;
		usleep(800000);
	}
	
  out:
	
	MiiAllProgressStop();
	
	(result == -10) ? returnVal = -10 : (ErrorLog.GetCount() ? returnVal = -1 : returnVal = 1);
}

int MiiAllTools::Internal_DeleteAll_Miis_Nand(int position)
{
	int result =  Mii::DeleteNand(position);
	
	if(AbortRequested)
		result =  -10;
	
	return result;
}

void MiiAllTools::DeleteAll_Miis_Emu()
{
	EmuMiiList.Get();
	
	int result = 0;
	int done = 0;
	
	for (int cnt = 0; cnt < EmuMiiList.GetFullCount(); cnt++)
	{
		ShowMiiAllProgress(EmuMiiList.GetFullName(cnt).c_str(), done, EmuMiiList.GetFullCount(), -1, cnt);
		
		result = Internal_DeleteAll_Miis_Emu(EmuMiiList.GetFullPosition(cnt));
		if (result == -10)
			goto out;
		else if (result < 0 && result != -10)
			ErrorLog.AddToLog(EmuMiiList.GetFullName(cnt).c_str(), tr("Unable to delete datas"));
		
		done++;
		usleep(800000);
	}
	
  out:
	
	MiiAllProgressStop();
	
	(result == -10) ? returnVal = -10 : (ErrorLog.GetCount() ? returnVal = -1 : returnVal = 1);
}

int MiiAllTools::Internal_DeleteAll_Miis_Emu(int position)
{
	int result =  Mii::DeleteEmu(position);
	
	if(AbortRequested)
		result =  -10;
	
	return result;
}

void MiiAllTools::DeleteAll_Miis_Dev(std::string devPath)
{
	CreateMiiList(devPath);
	
	if (miiPath.empty())
	{
		ErrorLog.AddToLog(tr("No Mii Found in this path"), " ");
		returnVal = -1;
		return;
	}
	
	int result = 0;
	int done = 0;
	
	for (u32 i = 0; i < miiPath.size(); i++)
	{
		ShowMiiAllProgress(miiName.at(i).c_str(), done, miiPath.size(), -1, -1, miiCreator.at(i), miiFemale.at(i), miiDay.at(i), miiMonth.at(i), miiFavColor.at(i));
		
		result = Internal_DeleteAll_Miis_Dev(miiPath.at(i));
		if (result == -10)
			goto out;
		else if (result < 0 && result != -10)
			ErrorLog.AddToLog(miiName.at(i).c_str(), tr("Unable to delete datas"));
		
		done++;
		usleep(700000);
	}
	
  out:
	
	MiiAllProgressStop();
	
	(result == -10) ? returnVal = -10 : (ErrorLog.GetCount() ? returnVal = -1 : returnVal = 1);
}

int MiiAllTools::Internal_DeleteAll_Miis_Dev(std::string miiPath)
{
	bool result = RemoveFile(miiPath.c_str());
	
	if(AbortRequested)
		return -10;
	
	if (result == false)
		return -1;
	
	return 1;
}
