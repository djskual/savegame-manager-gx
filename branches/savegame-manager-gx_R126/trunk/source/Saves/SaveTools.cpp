/****************************************************************************
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
 * SaveTools.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <malloc.h>
#include <sys/stat.h>

#include "SaveTools.h"
#include "../Tools/tools.h"
#include "../Text/wstring.hpp"
#include "../Text/TextTools.hpp"
#include "../FileOperations/fileops.h"
#include "../FileOperations/Nand.hpp"

bool CheckSave(std::string path)
{
	std::string bannerpath = path;
	bannerpath += "/banner.bin";
	
	struct stat filestat;

	if (stat(bannerpath.c_str(), &filestat) < 0)
		return false;
	
	return true;
}

bool CheckIfInstalled(u64 tid)
{
	char file[ISFS_MAXPATH];
	sprintf(file, "/title/%08x/%08x/data/banner.bin", (u32)(tid >> 32), (u32)(tid & 0xFFFFFFFF));
	
	s32 fh = Nand::OpenRead(file);
	if (fh < 0)
		return false;
	
	Nand::Close(fh);
	
	return true;
}

bool CheckTitle(std::string path)
{
	std::string bannerpath = path;
	bannerpath += "/banner.bin";
	
	int fd = Nand::OpenRead(bannerpath);
	if(fd < 0)
		return false;
	
	Nand::Close(fd);
	
	return true;
}

bool GetNandPath(u64 tid, char *outbuf)
{
	static char buffer[1024] ATTRIBUTE_ALIGN(32);
	
	if (ES_GetDataDir(tid, buffer) < 0)
		return false;

	strcpy(outbuf, buffer);
	
	return true;
}

char *titleText(u32 kind, u32 title)
{
	static char text[10];
	
	if (kind == 1) {
		//! If we're dealing with System Titles, use custom names
		switch (title) {
		case 1:
			strcpy(text, "BOOT2");
			break;
		case 2:
			strcpy(text, "SYSMENU");
			break;
		case 0x100:
			strcpy(text, "BC");
			break;
		case 0x101:
			strcpy(text, "MIOS");
			break;
		default:
			sprintf(text, "IOS%u", title);
			break;
		}
	} else {
		// Otherwise, just convert the title to ASCII
		int i =32, j = 0;
		do {
			u8 temp;
			i -= 8;
			temp = (title >> i) & 0x000000FF;
			if (temp < 32 || temp > 126)
				text[j] = '.';
			else
				text[j] = temp;
			j++;
		} while (i > 0);
		text[4] = 0;
	}
	return text;
}

u64 StrToHex64(const char *str)
{
	u64 val = 0;
	u32 cnt, len;
	
	len = strlen(str);
	
	for (cnt = 0; cnt < len; cnt++) {
		u32  idx = len - (cnt + 1);
		char c   = toupper((int)str[idx]);
		
		u64 n = (isdigit((int)c)) ? c - '0' : (c - 'A') + 0xA;
		u64 m = 1;
		
		for (idx = 0; idx < cnt; idx++)
			m *= 16;
		
		val += n * m;
	}

	return val;
} 

/****************************************************************************
 * utf16fix()
 *
 * fix utf-16 name & subname of save
 ***************************************************************************/
std::string utf16fix(u16* data)
{
	wchar_t wdata[0x40];
	
	for (u32 i = 0; i < 64; i++)
		wdata[i] = data[i];
	
	free(data);
	data = NULL;
	
	wString *ws_data = new wString(wdata);
	
	std::string utf8 = ws_data->toUTF8();
	
	delete ws_data;
	
	return utf8;
}

/****************************************************************************
 * GetNandNames()
 *
 * Get the utf-16 name & subname of the save
 ***************************************************************************/
std::string GetNandName(u64 tid)
{
	std::string name;
	
	char file[ISFS_MAXPATH];
	sprintf(file, "/title/%08x/%08x/data/banner.bin", (u32)(tid >> 32), (u32)(tid & 0xFFFFFFFF));
	
	if (ES_SetUID(tid) < 0)
		return "";
	
	int fh = Nand::OpenRead(file);
	if (fh < 0)
		return "";
	
	Nand::Seek(fh, 0x20, 0);
	u16* data = NULL;
	data = (u16*)memalign(32, 0x40);
	
	if (data)
	{
		if (Nand::Read(fh, (u8*)data, 0x40) < 0)
		{
			Nand::Close(fh);
			free(data);
			return "";
		}
	}
	
	Nand::Close(fh);
	
	name.assign(utf16fix(data));
	
	return name;
}

std::string GetNandSubname(u64 tid)
{
	std::string subname;
	
	char file[ISFS_MAXPATH];
	sprintf(file, "/title/%08x/%08x/data/banner.bin", (u32)(tid >> 32), (u32)(tid & 0xFFFFFFFF));
	
	if (ES_SetUID(tid) < 0)
		return "";
	
	int fh = Nand::OpenRead(file);
	if (fh < 0)
		return "";
	
	Nand::Seek(fh, 0x60, 0);
	u16* data = NULL;
	data = (u16*)memalign(32, 0x40);
	
	if (data)
	{
		if (Nand::Read(fh, (u8*)data, 0x40) < 0)
		{
			Nand::Close(fh);
			free(data);
			return "";
		}
	}
	
	Nand::Close(fh);
	
	subname.assign(utf16fix(data));
	
	return subname;
}

/****************************************************************************
 * GetDeviceNames()
 *
 * Get the utf-16 name & subname of the save on storage device
 ***************************************************************************/
std::string GetDeviceName(std::string savepath)
{
	std::string name;
	
	char bannerpath[128];
	sprintf(bannerpath, "%s/banner.bin", savepath.c_str());
	
	FILE *fp = fopen(bannerpath,"rb");
	fseek(fp , 0x20, SEEK_SET);
	
	u16* data = NULL;
	data = (u16*)memalign(32, 0x40);
	
	fread(data, 1, 0x40, fp);
	fclose(fp);
	
	name.assign(utf16fix(data));
	
	return name;
}

std::string GetDeviceSubname(std::string savepath)
{
	std::string subname;
	
	char bannerpath[128];
	sprintf(bannerpath, "%s/banner.bin", savepath.c_str());
	
	FILE *fp = fopen(bannerpath,"rb");
	fseek(fp , 0x60, SEEK_SET);
	
	u16* data = NULL;
	data = (u16*)memalign(32, 0x40);
	
	fread(data, 1, 0x40, fp);
	fclose(fp);
	
	subname.assign(utf16fix(data));
	
	return subname;
}

/****************************************************************************
 * RestoreSaveFile()
 *
 * Restore savefile name with new unvalid fat characters fix
 ***************************************************************************/
static std::string FixOldFileName(std::string filename)
{
	filename.assign(StringReplace(filename, "&cl;", "(COLON)")); //! :
	filename.assign(StringReplace(filename, "&st;", "(STAR)")); //! *
	filename.assign(StringReplace(filename, "&qt;", "(QUOTE)")); //! "
	filename.assign(StringReplace(filename, "&qm;", "(INTER)")); //! ?
	filename.assign(StringReplace(filename, "&vb;", "(VERTICAL)")); //! |
	filename.assign(StringReplace(filename, "&lt;", "(INF)")); //! <
	filename.assign(StringReplace(filename, "&gt;", "(SUP)")); //! >
	
	return filename;
}

void RestoreSaveFile(std::string EntryPath)
{
	if(EntryPath.size())
	{
		size_t found = EntryPath.rfind("/");
		if(found != std::string::npos)
		{
			std::string Filename(EntryPath);
			std::string Path(EntryPath);
			Filename = Filename.substr(found+1);
			Path.erase(found+1);
			Path += FixOldFileName(Filename);
			
			if(CheckFile(Path.c_str()))
				RenameFile(Path.c_str(), EntryPath.c_str());
		}
	}
}
