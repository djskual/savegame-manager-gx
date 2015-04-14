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
 * SysKeys.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <sys/dirent.h>
#include <network.h>
//#include <ogc/es.h>
#include <seeprom/seeprom.h>
#include <otp.h>

#include "SysKeys.hpp"
#include "../Saves/DataBin/DataBinTools.h"

SysKeys::SysKeys()
{
	FindKeys();
}

SysKeys::~SysKeys()
{
	
}

void SysKeys::FindKeys()
{
	_ok = false;
	ngID = 0;
	ngKeyID = 0;
	memset(ngSig, 0, 60);
	memset(ngMac, 0, 6);
	memset(ngPriv, 0, 32);
	
	if( NgMac() && Otp() && Seeprom() )
	{
		_ok = true;
	}
}

/*bool SysKeys::NgID()
{
	return (ES_GetDeviceID(&ngID) >= 0);
}*/

bool SysKeys::NgMac()
{
	return (net_get_mac_address(ngMac) >= 0);
}

bool SysKeys::Otp()
{
	if(OTP_Mount())
	{
		struct dirent *dirent = NULL;
		
		DIR *dir = opendir("otp:/");
		if (!dir)
			return false;
		
		char * filepath = new (std::nothrow) char[OTP_MAXPATHLEN];
		struct stat *st = new (std::nothrow) struct stat;
		if(!filepath || !st)
		{
			delete [] filepath;
			delete st;
			closedir(dir);
			return false;
		}
		
		int count = 0;
		while ((dirent = readdir(dir)) != 0)
		{
			if(!dirent->d_name)
			continue;
			
			snprintf(filepath, OTP_MAXPATHLEN, "otp:/%s", dirent->d_name);
			
			if(stat(filepath, st) != 0)
				continue;
			
			u64 size = 0;
			u8 *stuff = NULL;
			
			if(!strncmp(dirent->d_name, "ng_id.bin", 10))
			{
				LoadFileToMem(filepath, &stuff, &size);
				ngID = be32(stuff);
				count++;
			}
			else if(!strncmp(dirent->d_name, "ng_private_key.bin", 19))
			{
				LoadFileToMem(filepath, &stuff, &size);
				memcpy(ngPriv, stuff, 32);
				count++;
			}
			
			if(stuff) free(stuff);
			
			if(count == 2)
			{
				delete [] filepath;
				delete st;
				closedir(dir);
				OTP_Unmount();
				return true;
			}
		}
		
		delete [] filepath;
		delete st;
		closedir(dir);
		OTP_Unmount();
	}
	
	return false;
}

bool SysKeys::Seeprom()
{
	if(SEEPROM_Mount())
	{
		struct dirent *dirent = NULL;
		
		DIR *dir = opendir("seeprom:/");
		if (!dir)
			return false;
		
		char * filepath = new (std::nothrow) char[SEEPROM_MAXPATHLEN];
		struct stat *st = new (std::nothrow) struct stat;
		if(!filepath || !st)
		{
			delete [] filepath;
			delete st;
			closedir(dir);
			return false;
		}
		
		int count = 0;
		while ((dirent = readdir(dir)) != 0)
		{
			if(!dirent->d_name)
			continue;
			
			snprintf(filepath, SEEPROM_MAXPATHLEN, "seeprom:/%s", dirent->d_name);
			
			if(stat(filepath, st) != 0)
				continue;
			
			u64 size = 0;
			u8 *stuff = NULL;
			
			if(!strncmp(dirent->d_name, "ng_key_id", 10))
			{
				LoadFileToMem(filepath, &stuff, &size);
				ngKeyID = be32(stuff);
				count++;
			}
			else if(!strncmp(dirent->d_name, "ng_sig", 7))
			{
				LoadFileToMem(filepath, &stuff, &size);
				memcpy(ngSig, stuff, 60);
				count++;
			}
			
			if(stuff) free(stuff);
			
			if(count == 2)
			{
				delete [] filepath;
				delete st;
				closedir(dir);
				SEEPROM_Unmount();
				return true;
			}
		}
		
		delete [] filepath;
		delete st;
		closedir(dir);
		SEEPROM_Unmount();
	}
	
	return false;
}
