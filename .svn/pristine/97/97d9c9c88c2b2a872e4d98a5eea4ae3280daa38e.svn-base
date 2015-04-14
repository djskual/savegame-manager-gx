/****************************************************************************
 * Copyright (C) 2011
 * by Dj_Skual
 * Original sources from "WiiQt" by Giantpune
 * you can look at official google project page here:
 * http://code.google.com/p/wiiqt/
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
 * SaveDataBin.h
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/

#ifndef SAVEDATABIN_H
#define SAVEDATABIN_H

#include <gctypes.h>

#include "DataBinTools.h"

// class for dealing with a wii save (data.bin)

//! basic usage: read a data.bin into a bytearray and pass it to the first constructor.
//! if IsOk() returns true, then everything is good.  all the decrypted data and filenames
//! can be gotten from SaveStruct()

//! the static should take care of most of what you need to do - converting to/from/extracting a file
class SaveDataBin
{
public:
	SaveDataBin( std::string DataBinPath);
	SaveDataBin( u64 tid );
	~SaveDataBin() { ResetSave(); }
	bool IsOk(){ return _ok; }

	int GetEntriesCount(){ return sg.entries.size(); }
	std::string GetName(){ return sg.name; }
	std::string GetSubname(){ return sg.subname; }
	std::string GetID(){ return sg.ID; }
	std::string GetEntrie(int ind){ return sg.entries.at(ind); }
	u64 GetDecryptedSize(){ return sg.decryptedSize; }
	u64 GetTid(){ return sg.tid; }
	u8* GetData(int ind){ return sg.data.at(ind).first; }
	u32 GetDataSize(int ind){ return sg.data.at(ind).second; }
	
	const SaveGame SaveStruct(){ return sg; }
	bool WriteDecryptedDataBin(std::string DestPath);
	bool InstallDecryptedDataBin();
	bool EmuInstallDecryptedDataBin();
	bool StructToDataBin(std::string DestPath);
	
private:
	void LoadDataBin(u8* stuff = 0 , u64 stuff_size = 0);
	void ResetSave();
	void DeleteUselessNandFiles(std::string nandPath);
	void CreateMissingNandFiles(std::string nandPath);
	std::string FixDataBinFileName(std::string filename);
	
	bool GetWiiKeys();
	void CreateEntryList(std::string nandPath);
	std::pair<u8*, u32> DataFromSave(std::string path);
	u8 AttrFromSave(std::string path);
	bool StructFromNand();
	
	bool _ok;
	std::vector<std::string> isfsPath;
	SaveGame sg;
	u32 ngID;
	u32 ngKeyID;
	u8 ngSig[0x3c];
	u8 ngMac[6];
	u8 ngPriv[30];
};

#endif // SAVEDATABIN_H
