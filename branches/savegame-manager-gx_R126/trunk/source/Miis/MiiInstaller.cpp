/****************************************************************************
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
 * MiiInstaller.cpp
 *
 * for SaveGame Manager GX 2012
 ***************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "MiiInstaller.h"
#include "CMiiList.hpp"
#include "../EmuNand/CEmuMiiList.hpp"
#include "../Language/gettext.h"
#include "../FileOperations/Nand.hpp"
#include "../FileOperations/fileops.h"
#include "../Prompts/PromptWindows.h"
#include "../Network/FileDownloader.h"
#include "../Network/networkops.h"
#include "../Network/http.h"
#include "../Settings/CSettings.h"
#include "../System/nandtitle.h"
#include "../Saves/SaveTools.h"
#include "../Tools/timer.h"

static const char mii_db_Url[60] = "http://savegame-manager-gx.googlecode.com/files/mii_db.bin";

void Mii::Fix_DB()
{
	//! Install or fix the mii channel datas
	//! only if channel installed
	NandTitles.Get();
	u64 tid = StrToHex64("0001000248414341");
	if(NandTitles.Exists(tid))
	{
		std::string NChPath = "/title/00010002/48414341/data/NigaoeCh.dat";
		if(!Nand::CheckFile(NChPath))
		{
			if (WindowPrompt(tr("Mii Channel installed"), tr("but datas are missing."), tr("Install"), tr("Cancel")))
			{
				Nand::CreateDir("/title/00010002/48414341/data/noerase");
				Nand::CreateFile(NChPath, 0, 3, 3, 3);
				Mii::Fix_NigaoeCh();
			}
		}
		else if(!Mii::NigaoeCh_IsValid())
		{
			if (WindowPrompt(tr("Mii Channel installed"), tr("but datas are corrupted."), tr("Repair"), tr("Cancel")))
				Mii::Fix_NigaoeCh();
		}
	}
	
	//! Create blank mii_db if file not present or corrupted
	bool facelib_found = Nand::CheckFile(MII_FILE);
	if(!facelib_found || !Mii::Db_IsValid())
	{
		int fix_db = 0;
		if(!facelib_found)
		{
			fix_db = WindowPrompt(tr("FaceLib database is missing."), 0, tr("Install"), tr("Cancel"));
			if(fix_db)
			{
				Nand::CreateDir("/shared2/menu/FaceLib");
				Nand::CreateFile(MII_FILE, 0, 3, 3, 3);
			}
		}
		else
			fix_db = WindowPrompt(tr("FaceLib database is corrupted."), 0, tr("Repair"), tr("Cancel"));
		
		if(fix_db)
		{
			u8* miibuf = NULL;
			u64 size = 0;
			
			std::string miidb_path = Settings.ConfigPath;
			miidb_path += "/mii_db.bin";
			
			if(CheckFile(miidb_path))
				LoadFileToMem(miidb_path.c_str(), &miibuf, &size);
			else
				DownloadDB(&miibuf, &size);
			
			if(miibuf && size)
			{
				Nand::Write(MII_FILE, miibuf, size);
				free(miibuf);
			}
			
			MiiList.Get();
		}
	}
	
	Mii::Fix_EmuDB();
}

void Mii::Fix_EmuDB()
{
	if(Settings.Sneek || !Settings.EmuNand)
		return;
	
	std::string EmuMiiDBPath = Settings.EmuNandPath;
	EmuMiiDBPath += MII_FILE;
	
	bool facelib_found = CheckFile(EmuMiiDBPath);
	if (!facelib_found || !EmuDb_IsValid())
	{
		int fix_db = 0;
		if(!facelib_found)
		{
			fix_db = WindowPrompt(tr("FaceLib database is missing."), tr("on emulated nand"), tr("Install"), tr("Cancel"));
			if(fix_db)
				CreateSubfolder(GetParentDir(EmuMiiDBPath));
		}
		else
			fix_db = WindowPrompt(tr("FaceLib database is corrupted."), tr("on emulated nand"), tr("Repair"), tr("Cancel"));
		
		if(fix_db)
		{
			u8* miibuf = NULL;
			u64 size = 0;
			
			std::string miidb_path = Settings.ConfigPath;
			miidb_path += "/mii_db.bin";
			
			if(CheckFile(miidb_path))
				LoadFileToMem(miidb_path.c_str(), &miibuf, &size);
			else
				DownloadDB(&miibuf, &size);
			
			if(miibuf && size)
			{
				FILE * miidb_file = fopen(EmuMiiDBPath.c_str(), "wb");
				if(!miidb_file)
				{
					free(miibuf);
					return;
				}
				
				fwrite(miibuf, 1, size, miidb_file);
				fclose(miidb_file);
				free(miibuf);
			}
			
			EmuMiiList.Get();
		}
	}
}

bool Mii::NigaoeCh_IsValid()
{
	bool result = false;
	
	u8* buffer = NULL;
	u32 size = 0;
	Nand::LoadFile("/title/00010002/48414341/data/NigaoeCh.dat", &buffer, &size);
	if(buffer)
	{
		if( (buffer[0x42] >= 2) && (buffer[0x43] >= 2) )
			result = true;
		
		free(buffer);
	}
	
	return result;
}

void Mii::Fix_NigaoeCh()
{
	u8 NigaoeCh[0x60];
	memset(NigaoeCh, 0, 0x60);
	memset(NigaoeCh+0x42, 2, 2);
	if (WindowPrompt(tr("3DS Mii transfer function"), 0, tr("ON"), tr("OFF")))
		memset(NigaoeCh+0x43, 3, 1);
	
	Nand::Write("/title/00010002/48414341/data/NigaoeCh.dat", NigaoeCh, 0x60);
}

bool Mii::Db_IsValid()
{
	//! Read Miis container
	int miiFile = Nand::OpenRead(MII_FILE);
	if (miiFile < 0)
		return false;
	
	static struct mii_db db ATTRIBUTE_ALIGN(32);
	if ( Nand::Read(miiFile, (u8*)&db, (u32)sizeof(db)) != sizeof(db) )
	{
		Nand::Close(miiFile);
		return false;
	}
	
	Nand::Close(miiFile);
	
	//! compare crc
	return (db.crc == Mii::Crc16((u8*)&db, sizeof(db) - sizeof(db.crc)));
}

bool Mii::EmuDb_IsValid()
{
	std::string EmuMiiDBPath = Settings.EmuNandPath;
	EmuMiiDBPath += MII_FILE;
	
	//! Read Miis container
	FILE * miiFile = fopen(EmuMiiDBPath.c_str(), "rb");
	if (!miiFile)
		return false;
	
	static struct mii_db db ATTRIBUTE_ALIGN(32);
	if ( fread((u8*)&db, 1, sizeof(db), miiFile) != sizeof(db) )
	{
		fclose(miiFile);
		return false;
	}
	
	fclose(miiFile);
	
	//! compare crc
	return (db.crc == Mii::Crc16((u8*)&db, sizeof(db) - sizeof(db.crc)));
}

void Mii::DownloadDB(u8 **miibuf, u64 *size)
{
	if (Settings.AutoConnect || IsNetworkInit())
	{
		if(!IsNetworkInit())
		{
			Timer timer;
			timer.reset();
			while(!IsNetworkInit() && timer.elapsed() < 10) usleep(50);
		}
		
		if(IsNetworkInit())
		{
			struct block file = downloadfile(mii_db_Url);
			if(file.data)
			{
				*miibuf = file.data;
				*size = file.size;
			}
		}
	}
}

int Mii::InstallNand(std::string SrcPath, int position)
{
	return Mii::InternalNandInstall(Mii::INSTALL, position, SrcPath.c_str());
}

int Mii::InstallEmu(std::string SrcPath, int position)
{
	return Mii::InternalEmuInstall(Mii::INSTALL, position, SrcPath.c_str());
}

int Mii::DeleteNand(int position)
{
	return Mii::InternalNandInstall(Mii::DELETE, position);
}

int Mii::DeleteEmu(int position)
{
	return Mii::InternalEmuInstall(Mii::DELETE, position);
}

int Mii::DeleteAllNand(int position)
{
	return Mii::InternalNandInstall(Mii::DELETE_ALL, position);
}

int Mii::DeleteAllEmu(int position)
{
	return Mii::InternalEmuInstall(Mii::DELETE_ALL, position);
}

int Mii::InternalNandInstall(short type, int position, const char * SrcPath)
{
	//! Read Miis container
	int miiFile = Nand::OpenRead(MII_FILE);
	if (miiFile < 0)
		return -1;
	
	static struct mii_db database ATTRIBUTE_ALIGN(32);
	int ret = Nand::Read(miiFile, (u8*)&database, (u32)sizeof(database));
	if (ret != sizeof(database))
	{
		Nand::Close(miiFile);
		return -1;
	}
	
	Nand::Close(miiFile);
	
	//! Read Mii
	struct mii_t buffer;
	memset(&buffer, 0, sizeof(buffer));
	
	if( type == Mii::INSTALL )
	{
		FILE * File = fopen(SrcPath, "rb");
		if (File)
		{
			fread(&buffer, 1, MII_SIZE, File);
			fclose(File);
		}
	}
	
	fflush(stdout);
	
	//! Copy Mii to Miis database
	if( type == Mii::DELETE_ALL )
	{
		for(int i = 0; i < MII_MAX; i++)
			memcpy(&database.mii[i], &buffer, MII_SIZE);
	}
	else
		memcpy(&database.mii[position], &buffer, MII_SIZE);

	//! Calculate new CRC
	database.crc = Mii::Crc16((u8*)&database, sizeof(database) - sizeof(database.crc));

	//! Write MiiDatabase
	miiFile = Nand::OpenReadWrite(MII_FILE);
	if (miiFile < 0)
		return -1;

	ret = Nand::Write(miiFile, (u8*)&database, sizeof(database));
	if (ret != sizeof(database))
	{
		Nand::Close(miiFile);
		return -1;
	}
	
	Nand::Close(miiFile);
	
	return 1;
}

int Mii::InternalEmuInstall(short type, int position, const char * SrcPath)
{
	//! Read Miis container
	std::string EmuMiiDBPath = Settings.EmuNandPath;
	EmuMiiDBPath += MII_FILE;
	
	FILE * miiFile = fopen(EmuMiiDBPath.c_str(), "rb");
	if (!miiFile)
		return -1;
	
	static struct mii_db database ATTRIBUTE_ALIGN(32);
	int ret = fread((u8*)&database, 1, sizeof(database), miiFile);
	if (ret != sizeof(database))
	{
		fclose(miiFile);
		return -1;
	}
	
	fclose(miiFile);
	
	//! Read Mii
	struct mii_t buffer;
	memset(&buffer, 0, sizeof(buffer));
	
	if( type == Mii::INSTALL )
	{
		FILE * File = fopen(SrcPath, "rb");
		if (File)
		{
			fread(&buffer, 1, MII_SIZE, File);
			fclose(File);
		}
	}
	
	fflush(stdout);
	
	//! Copy Mii to Miis database
	if( type == Mii::DELETE_ALL )
	{
		for(int i = 0; i < MII_MAX; i++)
			memcpy(&database.mii[i], &buffer, MII_SIZE);
	}
	else
		memcpy(&database.mii[position], &buffer, MII_SIZE);

	//! Calculate new CRC
	database.crc = Mii::Crc16((u8*)&database, sizeof(database) - sizeof(database.crc));

	//! Write MiiDatabase
	miiFile = fopen(EmuMiiDBPath.c_str(), "wb");
	if (!miiFile)
		return -1;

	ret = fwrite((u8*)&database, 1, sizeof(database), miiFile);
	if (ret != sizeof(database))
	{
		fclose(miiFile);
		return -1;
	}
	
	fclose(miiFile);
	
	return 1;
}

int Mii::NandToEmu(int nandPos, int emuPos)
{
	//! Read Miis container
	std::string EmuMiiDBPath = Settings.EmuNandPath;
	EmuMiiDBPath += MII_FILE;
	
	FILE * miiFile = fopen(EmuMiiDBPath.c_str(), "rb");
	if (!miiFile)
		return -1;
	
	static struct mii_db database ATTRIBUTE_ALIGN(32);
	int ret = fread((u8*)&database, 1, sizeof(database), miiFile);
	if (ret != sizeof(database))
	{
		fclose(miiFile);
		return -1;
	}
	
	fclose(miiFile);
	
	//! Read Mii
	u8* memory = (u8*)Tools::AllocateMemory_32(MII_SIZE);
	if(!memory)
		return -1;
	
	int miisrc = Nand::OpenRead(MII_FILE);
	if (miisrc >= 0)
	{
		Nand::Seek(miisrc, MII_HEADER+(nandPos*MII_SIZE), 0);
		
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
	}
	
	fflush(stdout);
	
	//! Copy Mii to Miis database
	memcpy(&database.mii[emuPos], memory, MII_SIZE);
	free(memory);
	memory = NULL;
	
	//! Calculate new CRC
	database.crc = Mii::Crc16((u8*)&database, sizeof(database) - sizeof(database.crc));

	//! Write MiiDatabase
	miiFile = fopen(EmuMiiDBPath.c_str(), "wb");
	if (!miiFile)
		return -1;

	ret = fwrite((u8*)&database, 1, sizeof(database), miiFile);
	if (ret != sizeof(database))
	{
		fclose(miiFile);
		return -1;
	}
	
	fclose(miiFile);
	
	return 1;
}

int Mii::EmuToNand(int emuPos, int nandPos)
{
	std::string EmuMiiDBPath = Settings.EmuNandPath;
	EmuMiiDBPath += MII_FILE;
	
	//! Read Miis container
	int miiFile = Nand::OpenRead(MII_FILE);
	if (miiFile < 0)
		return -1;
	
	static struct mii_db database ATTRIBUTE_ALIGN(32);
	int ret = Nand::Read(miiFile, (u8*)&database, (u32)sizeof(database));
	if (ret != sizeof(database))
	{
		Nand::Close(miiFile);
		return -1;
	}
	
	Nand::Close(miiFile);
	
	//! Read Mii
	u8* memory = (u8*)memalign(32, MII_SIZE);
	if(!memory)
		return -1;
	
	FILE * miisrc = fopen(EmuMiiDBPath.c_str(), "rb");
	if (miisrc)
	{
		fseek(miisrc, MII_HEADER+(emuPos*MII_SIZE), SEEK_SET);
		
		if (fread(memory, 1, MII_SIZE, miisrc) != MII_SIZE)
		{
			fclose(miisrc);
			free(memory);
			memory = NULL;
			return -1;
		}
		
		fclose(miisrc);
	}
	
	fflush(stdout);
	
	//! Copy Mii to Miis database
	memcpy(&database.mii[nandPos], memory, MII_SIZE);
	free(memory);
	memory = NULL;
	
	//! Calculate new CRC
	database.crc = Mii::Crc16((u8*)&database, sizeof(database) - sizeof(database.crc));

	//! Write MiiDatabase
	miiFile = Nand::OpenReadWrite(MII_FILE);
	if (miiFile < 0)
		return -1;

	ret = Nand::Write(miiFile, (u8*)&database, sizeof(database));
	if (ret != sizeof(database))
	{
		Nand::Close(miiFile);
		return -1;
	}
	
	Nand::Close(miiFile);
	
	return 1;
}

u16 Mii::Crc16(const u8 *buf, u32 len)
{
	u16 crc = 0x0000;
	int byte_idx, bit_idx, cnt;

	for (byte_idx = 0; byte_idx < (int)len; byte_idx++)
	{
		for (bit_idx = 7; bit_idx >= 0; bit_idx--)
			crc = (((crc << 1) | ((buf[byte_idx] >> bit_idx) & 0x1)) ^ (((crc & 0x8000) != 0) ? 0x1021 : 0));
	}

	for (cnt = 16; cnt > 0; cnt--)
		crc = ((crc << 1) ^ (((crc & 0x8000) != 0) ? 0x1021 : 0));

	return (crc & 0xFFFF);
}
