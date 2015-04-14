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
 * MiiInstaller.h
 *
 * for SaveGame Manager GX 2012
 ***************************************************************************/
#ifndef _MIIINSTALLER_H_
#define _MIIINSTALLER_H_

#include <gctypes.h>
#include <string>

#define MII_FILE	"/shared2/menu/FaceLib/RFL_DB.dat"
#define MII_MAX		100
#define MII_SIZE	74

struct mii_t
{
	u8 data1[2];
	u16 name[10];
	u8 data2[32];
	u16 creator[10];
};

struct mii_db
{
	u8 header[4];
	struct mii_t mii[MII_MAX];
	u8 fill[20];
	u8 db_data[0x1D4DE];
	u16 crc;
};

class Mii
{
	public:
		static void Fix_DB();
		static void Fix_EmuDB();
		static int InstallNand(std::string SrcPath, int position);
		static int InstallEmu(std::string SrcPath, int position);
		static int NandToEmu(int nandPos, int emuPos);
		static int EmuToNand(int emuPos, int nandPos);
		static int DeleteNand(int position);
		static int DeleteEmu(int position);
		static int DeleteAllNand(int position);
		static int DeleteAllEmu(int position);
		
		enum
		{
			INSTALL = 0,
			DELETE,
			DELETE_ALL,			
		};
	
	protected:
		static bool NigaoeCh_IsValid();
		static void Fix_NigaoeCh();
		static bool Db_IsValid();
		static bool EmuDb_IsValid();
		static void DownloadDB(u8 **miibuf, u64 *size);
		
		static int InternalNandInstall(short type, int position, const char * SrcPath = NULL);	
		static int InternalEmuInstall(short type, int position, const char * SrcPath = NULL);	
		static u16 Crc16(const u8 *buf, u32 len);
};

#endif
