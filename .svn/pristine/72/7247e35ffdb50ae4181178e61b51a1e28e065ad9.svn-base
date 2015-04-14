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
 * MiiManageTools.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include <stdio.h>
#include <malloc.h>

#include "MiiManageTools.hpp"
#include "../Settings/CSettings.h"
#include "../FileOperations/fileops.h"
#include "../FileOperations/Nand.hpp"
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

MiiManageTools::MiiManageTools(const char * title)
{
	returnVal = 0;
	
	InitManageProgressThread();
	ManageProgressSetAbortCallback(AbortCallback);
	StartManageProgress(title);
}

MiiManageTools::~MiiManageTools()
{
	ExitManageProgressThread();
}

void MiiManageTools::Reset()
{
	returnVal = 0;
}

void MiiManageTools::StopProgress()
{
	ManageProgressStop();
}

std::string MiiManageTools::FixMiiName(std::string filename)
{
	filename.assign(StringReplace(filename, ":", "&cl;"));
	filename.assign(StringReplace(filename, "*", "&st;"));
	filename.assign(StringReplace(filename, "\"", "&qt;"));
	filename.assign(StringReplace(filename, "?", "&qm;"));
	filename.assign(StringReplace(filename, "|", "&vb;"));
	filename.assign(StringReplace(filename, "<", "&lt;"));
	filename.assign(StringReplace(filename, ">", "&gt;"));
	
	return filename;
}

int MiiManageTools::FindFirstFree(bool Nand)
{
	int position = 0;
	int listCnt = 0;
	int pos = 0;
	
	if(Nand)
		listCnt = MiiList.GetFullCount();
	else
		listCnt = EmuMiiList.GetFullCount();
	
	for(position = 0; position < MII_MAX; position++)
	{
		bool freeSpaceOk = true;
		
		for(int n = 0; n < listCnt; n++)
		{
			if(Nand)
				pos = MiiList.GetFullPosition(n);
			else
				pos = EmuMiiList.GetFullPosition(n);
			
			if(pos == position)
			{
				freeSpaceOk = false;
				break;
			}
		}
		if(freeSpaceOk == true)
			break;
	}
	
	return position;
}

void MiiManageTools::ExtractMii_Nand_Dev(std::string devpath, int selected)
{
	CreateSubfolder(devpath);

	//! Get extract file name
	if (devpath[devpath.size()-1] != '/')
		devpath += "/";
	devpath += FixMiiName(MiiList.GetName(selected));
	devpath += ".miigx";
	
	int miisrc = Nand::OpenRead(MII_FILE);
	if (miisrc >= 0)
	{
		
		int pos = MiiList.GetPosition(selected);
		Nand::Seek(miisrc, MII_HEADER+(pos*MII_SIZE), 0);
		
		u8* memory = (u8*)Tools::AllocateMemory_32(MII_SIZE);
		if(!memory)
		{
			Nand::Close(miisrc);
			returnVal = -1;
			return;
		}
		
		//! Read Mii
		int ret = Nand::Read(miisrc, memory, MII_SIZE);
		if (ret != MII_SIZE)
		{
			Nand::Close(miisrc);
			free(memory);
			memory = NULL;
			returnVal = -1;
			return;
		}
		
		Nand::Close(miisrc);
		
		//! Open destination file
		FILE * miidest = fopen(devpath.c_str(), "wb");
		if(!miidest) {
			free(memory);
			memory = NULL;
			returnVal = -1;
			return;
		}
		
		//! Write file
		fwrite(memory, 1, MII_SIZE, miidest);
		fclose(miidest);
		free(memory);
		memory = NULL;
		
		if (AbortRequested)
			returnVal = -10;
		else
			returnVal = 1;
		
		return;
		
	}
	else
		returnVal = -1;
	
	return;
}

void MiiManageTools::InstallMii_Nand_Emu(int nandPos, int emuPos)
{
	returnVal = Mii::NandToEmu(nandPos, emuPos);
}

void MiiManageTools::DeleteMii_Nand(int position)
{
	int result = Mii::DeleteNand(position);
	
	if(AbortRequested)
		result =  -10;
	
	returnVal = result;
}

void MiiManageTools::InstallMii_Dev_Nand(std::string miipath, int position)
{
	returnVal = Mii::InstallNand(miipath, position);
}

void MiiManageTools::InstallMii_Dev_Emu(std::string miipath, int position)
{
	returnVal = Mii::InstallEmu(miipath, position);
}

void MiiManageTools::DeleteMii_Dev(std::string miiPath)
{
	bool result = RemoveFile(miiPath.c_str());
	
	if(AbortRequested)
	{
		returnVal =  -10;
		return;
	}
	
	if (result == false)
		returnVal = -1;
	else
		returnVal = 1;
}

void MiiManageTools::MoveMii(std::string srcFile, std::string destFile)
{
	int result = MoveFile(srcFile.c_str(), (char*)destFile.c_str());
	
	if(AbortRequested)
		result =  -10;
	
	returnVal = result;
}

void MiiManageTools::CopyMii(std::string srcFile, std::string destFile)
{
	int result = CopyFile(srcFile.c_str(), destFile.c_str());
	
	if(AbortRequested)
		result =  -10;
	
	returnVal = result;
}

void MiiManageTools::ExtractMii_Emu_Dev(std::string devpath, int selected)
{
	CreateSubfolder(devpath);

	//! Get extract file name
	if (devpath[devpath.size()-1] != '/')
		devpath += "/";
	devpath += FixMiiName(EmuMiiList.GetName(selected));
	devpath += ".miigx";
	
	std::string EmuMiiDBPath = Settings.EmuNandPath;
	EmuMiiDBPath += MII_FILE;
	
	FILE * miisrc = fopen(EmuMiiDBPath.c_str(), "rb");
	if (miisrc)
	{
		
		int pos = EmuMiiList.GetPosition(selected);
		fseek(miisrc, MII_HEADER+(pos*MII_SIZE), SEEK_SET);
		
		u8* memory = (u8*)memalign(32, MII_SIZE);
		if(!memory)
		{
			fclose(miisrc);
			returnVal = -1;
			return;
		}
		
		//! Read Mii
		if (fread(memory, 1, MII_SIZE, miisrc) != MII_SIZE)
		{
			fclose(miisrc);
			free(memory);
			memory = NULL;
			returnVal = -1;
			return;
		}
		
		fclose(miisrc);
		
		//! Open destination file
		FILE * miidest = fopen(devpath.c_str(), "wb");
		if(!miidest)
		{
			free(memory);
			memory = NULL;
			returnVal = -1;
			return;
		}
		
		//! Write file
		fwrite(memory, 1, MII_SIZE, miidest);
		fclose(miidest);
		free(memory);
		memory = NULL;
		
		if (AbortRequested)
			returnVal = -10;
		else
			returnVal = 1;
		
		return;
		
	}
	else
		returnVal = -1;
	
	return;
}

void MiiManageTools::InstallMii_Emu_Nand(int emuPos, int nandPos)
{
	returnVal = Mii::EmuToNand(emuPos, nandPos);
}

void MiiManageTools::DeleteMii_Emu(int position)
{
	int result = Mii::DeleteEmu(position);
	
	if(AbortRequested)
		result =  -10;
	
	returnVal = result;
}
