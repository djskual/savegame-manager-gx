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
 * SaveDataBin.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include <ogcsys.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "SaveDataBin.h"
#include "../../FileOperations/Nand.hpp"
#include "../../Text/wstring.hpp"
#include "../../Text/TextTools.hpp"
#include "../../System/SysKeys.hpp"
#include "../../Tools/tools.h"
#include "../../Tools/gecko.h"
#include "../../Tools/StringTools.h"
#include "../../Settings/CSettings.h"
#include "../SaveTools.h"
#include "DataBinTools.h"
#include "Crypto/aes.h"
#include "Crypto/md5.h"
#include "Crypto/ec.h"

SaveDataBin::SaveDataBin(std::string DataBinPath)
{
	u64 size = 0;
	u8 *stuff = NULL;
	LoadFileToMem(DataBinPath.c_str(), &stuff, &size);
	
	LoadDataBin(stuff, size);
	
	if(stuff) free(stuff);	
}

SaveDataBin::SaveDataBin( u64 tid )
{
	_ok = false;
	
	ResetSave();
	
	sg.tid = tid;
	
	if(!StructFromNand())
		return;
	
	_ok = IsValidSave( sg );
}

void SaveDataBin::ResetSave()
{
	isfsPath.clear();
	sg.entries.clear();
	//sg.attr.clear();
	sg.perm.clear();
	sg.data.clear();
}

void SaveDataBin::LoadDataBin( u8* stuff, u64 stuff_size )
{
	_ok = false;
	ngID = 0;
	ngKeyID = 0;
	sg.tid = 0;
	if( stuff_size < 0xf140 ) //!header + backup header
	{
		gprintf("\n\tSaveDataBin::SaveDataBin -> size is too small");
		return;
	}
	u32 size = 0xf0c0;
	u8 header[size];
	memset(header, '\0', size);
	
	//!decrypt the header
	u8 iv[ 16 ] = SD_IV;
	u8 sdkey[ 16 ] = SD_KEY;
	aes_set_key( sdkey );
	aes_decrypt( iv, stuff, header, size );
	
	//!check MD5
	u8 md5blanker[ 16 ] = MD5_BLANKER;
	u8 expected[ 16 ];
	u8 tmp_header[size];
	
	memcpy(expected, header + 0x0e, 16);
	memcpy(tmp_header, header, size);
	memcpy(tmp_header + 0x0e, md5blanker, 16);
	
	MD5 hash;
	hash.update( tmp_header, sizeof(tmp_header) );
	hash.finalize();
	u8 actual[ 16 ];
	memcpy(actual, hash.hexdigestChar(), 16);
	if (memcmp(actual, expected, 0x10))
		gprintf("\n\tSaveDataBin::SaveDataBin -> md5 mismatch");
	
	//!read the tid & banner.bin size
	u32 bnrSize;
	u8 bnrPerm;
	bnrSize = be32(header + 8);
	if (bnrSize < 0x72a0 || bnrSize > 0xf0a0 || (bnrSize - 0x60a0) % 0x1200 != 0)
	{
		gprintf("\n\tSaveDataBin::SaveDataBin -> bad file header size");
		return;
	}
	sg.tid = be64(header);
	
	u16 name_data[0x40];
	u16 subname_data[0x40];
	memcpy(name_data, (u8*)header+0x40, 0x40);
	memcpy(subname_data, (u8*)header+0x80, 0x40);
	
	wchar_t wdata[0x40];
	for(int i = 0; i < 64; i++)
		wdata[i] = name_data[i];
	wchar_t wsubdata[0x40];
	for(int i = 0; i < 64; i++)
		wsubdata[i] = subname_data[i];
	
	wString *ws_data = new wString(wdata);
	wString *ws_subdata = new wString(wsubdata);
	
	sg.name = ws_data->toUTF8();
	sg.subname = ws_subdata->toUTF8();
	
	delete ws_data;
	delete ws_subdata;
	
	bnrPerm = header[0x0c];
	
	//!add the entry for banner.bin in the save struct
	u8 perm = ( bnrPerm << 2 ) | NAND_FILE;
	
	sg.perm.push_back(perm);
	sg.entries.push_back("/banner.bin");
	u8* banner = (u8*)memalign(32, bnrSize);
	memcpy(banner, header + 0x20, bnrSize);
	sg.data.push_back(std::pair<u8*, u32>(banner, bnrSize));
	
	//!read the Bk header
	u32 cnt;
	u32 fSize;
	u32 tSize;
	
	u8 bk_header[0x80];
	memcpy(bk_header, stuff+size, 0x80);
	
	if (be32(bk_header) != 0x70)
	{
		gprintf("\n\tSaveDataBin::SaveDataBin -> bad hdr size");
		return;
	}
	if (be32(bk_header + 4) != 0x426b0001)
	{
		gprintf("\n\tSaveDataBin::SaveDataBin -> bad magic");
		return;
	}
	ngID = be32(bk_header + 8);
	cnt = be32(bk_header + 12);
	fSize = be32(bk_header + 16);
	tSize = be32(bk_header + 28);
	memcpy(sg.ID, bk_header + 100, 4);
	
	if( stuff_size < fSize + 0xf140 )
	{
		gprintf("\n\tSaveDataBin::SaveDataBin -> buffer size is less than expected");
		return;
	}

	memcpy(ngMac, bk_header + 104, 6 );
	
	//!read all the files and folders
	u32 pos = 0xf140;
	sg.decryptedSize = bnrSize;
	for( u32 i = 0; i < cnt; i++ )
	{
		u32 size;
		u8 perm;
		//u8 attr;
		u8 type;
		u8 iv[ 0x10 ];
		char name[0x45];
		
		if (be32(stuff+pos) != 0x03adf17e)
		{
			gprintf("\n\tSaveDataBin::SaveDataBin -> bad file magic");
			return;
		}
		size = be32(stuff+pos + 4);
		sg.decryptedSize += size;
		perm = stuff[pos+8];
		//attr = stuff[pos+9];
		type = stuff[pos+10];
		memcpy(iv, stuff+(pos+0x50), 0x10);
		memcpy(name, stuff+pos+11, 0x45);
		std::string Name = "/";
		Name += name;
		
		perm = ( perm << 2 ) | type;
		
		sg.entries.push_back(Name);
		sg.perm.push_back(perm);
		//sg.attr.push_back(attr);
		
		switch( type )
		{
		case NAND_FILE:
			{
				u8* encData = (u8*)malloc(RU(size, 0x40));
				memcpy(encData, stuff+(pos+0x80), RU(size, 0x40));
				
				u8* decData = (u8*)malloc(RU(size, 0x40));
				memset(decData, 0, RU(size, 0x40));
				
				aes_set_key( sdkey );
				aes_decrypt( iv, encData, decData, RU(size, 0x40));
				free(encData);
				
				u8* tmpData = (u8*)malloc(size);
				memcpy(tmpData, decData, size);
				sg.data.push_back(std::pair<u8*, u32>(tmpData, size));
				free(decData);
			}
			break;
		case NAND_DIR:
			{
				sg.data.push_back(std::pair<u8*, u32>(0, 0));
			}
			break;
		default:
			gprintf("\n\tSaveDataBin::SaveDataBin -> unknown type");
			return;
			break;
		}
		
		//!seek to beginning of next file
		pos = ( pos + 0x80 + RU(size, 0x40) );
		
	}

	//!get a couple keys useful for repacking
	ngKeyID = be32(stuff+(pos+0x144));
	memcpy(ngSig, stuff+(pos+0x44), 0x3c);
	
	//!check the cert mumbojombo
	u32 data_size = tSize - 0x340;
	u8* data = (u8*)malloc(data_size);
	memcpy(data, stuff + 0xf0c0, data_size);
	
	u8* sha1h = GetSha1( data, data_size );
	u8* sha1H = GetSha1( sha1h, 20 );
	free(data);
	free(sha1h);
	
	u8 sig[0x40];
	u8 ng_cert[0x180];
	u8 ap_cert[0x180];
	memcpy(sig, stuff+pos, 0x40); 
	memcpy(ng_cert, stuff+(pos+0x40), 0x180); 
	memcpy(ap_cert, stuff+(pos+0x1c0), 0x180); 
	
	_ok = check_ec(ng_cert, ap_cert, sig, sha1H);
	
	free(sha1H);
}

std::string SaveDataBin::FixDataBinFileName(std::string filename)
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

bool SaveDataBin::WriteDecryptedDataBin(std::string DestPath)
{
	std::string SavePath = DestPath;
	SavePath += "/";
	char dirname[17];
	memset(dirname, '\0', 17);
	snprintf(dirname, 17, "%016llx", sg.tid);
	SavePath += dirname;
	
	if(!CreateSubfolder(SavePath.c_str()))
	{
		gprintf("\n\tSaveDataBin::WriteDecryptedDataBin -> Can't create folder:\n\t%s", SavePath.c_str());
		return false;
	}
	
	for(u32 i = 0; i < sg.entries.size(); i++)
	{
		std::string filePath = SavePath;
		filePath += FixDataBinFileName(sg.entries.at(i));
		
		if(sg.data.at(i).second > 0) //! file
		{
			if( !WriteFile(filePath, sg.data.at(i).first, sg.data.at(i).second) )
			{
				gprintf("\n\tSaveDataBin::WriteDecryptedDataBin -> Can't write file:\n\t%s", filePath.c_str());
				return false;
			}
		}
		else //! folder
		{
			if(!CreateSubfolder(filePath.c_str()))
			{
				gprintf("\n\tSaveDataBin::WriteDecryptedDataBin -> Can't create folder:\n\t%s", filePath.c_str());
				return false;
			}
		}
	}
	
	return true;
}

void SaveDataBin::DeleteUselessNandFiles(std::string path)
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
				for(u32 cur = 0; (cur < sg.entries.size()) && (!need_file); cur++)
				{
					std::string Path = path;
					Path += sg.entries.at(cur);
					if(!Path.compare(nandPath))
						need_file = true;
				}
				if(!need_file)
					Nand::Delete(nandPath);
			}
		}
	}
}

void SaveDataBin::CreateMissingNandFiles(std::string nandPath)
{
	for(u32 i = 0; i < sg.entries.size(); i++)
	{
		std::string nandpath = nandPath;
		nandpath += sg.entries.at(i);
		
		int fd = Nand::OpenRead(nandpath);
		if(fd < 0)
			Nand::CreateFile(nandpath, 0, 3, 3, 3);
		else
			Nand::Close(fd);
	}
}

bool SaveDataBin::EmuInstallDecryptedDataBin()
{
	std::string emulPath = fmt("%s/title/%08x/%08x/data/", Settings.EmuNandPath, TITLE_UPPER(sg.tid), TITLE_LOWER(sg.tid));
	
	if(!CreateSubfolder(emulPath))
	{
		gprintf("\n\tSaveDataBin::EmuInstallDecryptedDataBin -> Can't create folder:\n\t%s", emulPath.c_str());
		return false;
	}
	
	for(u32 i = 0; i < sg.entries.size(); i++)
	{
		std::string filePath = emulPath;
		filePath += FixDataBinFileName(sg.entries.at(i));
		
		if(sg.data.at(i).second > 0) //! file
		{
			if( !WriteFile(filePath, sg.data.at(i).first, sg.data.at(i).second) )
			{
				gprintf("\n\tSaveDataBin::EmuInstallDecryptedDataBin -> Can't write file:\n\t%s", filePath.c_str());
				return false;
			}
		}
		else //! folder
		{
			if(!CreateSubfolder(filePath.c_str()))
			{
				gprintf("\n\tSaveDataBin::EmuInstallDecryptedDataBin -> Can't create folder:\n\t%s", filePath.c_str());
				return false;
			}
		}
	}
	
	return true;
}

bool SaveDataBin::InstallDecryptedDataBin()
{
	char* buffer = (char*)memalign(32, ISFS_MAXPATH);
	
	//! Set title UID
	if (ES_SetUID(sg.tid) < 0)
	{
		gprintf("\n\tSaveDataBin::InstallDecryptedDataBin -> Unable to SetUID()");
		return false;
	}

	//! Get NAND path
	if (ES_GetDataDir(sg.tid, buffer) < 0)
	{
		gprintf("\n\tSaveDataBin::InstallDecryptedDataBin -> Unable to get NandPath");
		return false;
	}
	std::string nandPath = buffer;
	free(buffer);
	
	DeleteUselessNandFiles(nandPath);
	CreateMissingNandFiles(nandPath);
	
	bool result = true;
	for(u32 i = 0; (i < sg.entries.size()) && result; i++)
	{
		result = false;
		
		std::string nandpath = nandPath;
		nandpath += sg.entries.at(i);
		
		int outfp = Nand::OpenReadWrite(nandpath);
		if (outfp >= 0)
		{
			if(Nand::Write(outfp, sg.data.at(i).first, sg.data.at(i).second) >= 0)
				result = true;
			
			Nand::Close(outfp);
		}
		
		if(!sg.data.at(i).second)
			result = true;
	}
	
	return result;
}

bool SaveDataBin::GetWiiKeys()
{
	bool result = false;
	
	SysKeys * keys = new SysKeys();
	if(keys->IsOK())
	{
		ngID = keys->GetNgID();
		ngKeyID = keys->GetNgKeyID();
		memcpy(ngSig, keys->GetNgSig(), 60);
		memcpy(ngMac, keys->GetNgMac(), 6);
		memcpy(ngPriv, keys->GetNgPriv(), 30);
		
		result = true;
	}
	delete keys;
	
	return result;
}

void SaveDataBin::CreateEntryList(std::string nandPath)
{
	std::vector<std::string> List;
	if( Nand::ReadDir(nandPath, List) == ISFS_OK )
	{
		for (int i = 0; (u32)i < List.size(); i++)
		{
			std::string Path = nandPath;
			Path += "/";
			Path += List.at(i);
			
			isfsPath.push_back(Path);
			
			int fd = Nand::OpenRead(Path);
			if(fd < 0)
				CreateEntryList(Path);
			else
				Nand::Close(fd);
		}
	}
}

std::pair<u8*, u32> SaveDataBin::DataFromSave(std::string path)
{
	for(int i = 0; i < (int)sg.entries.size(); i++)
	{
		if(!path.compare(sg.entries.at(i)))
			return std::pair<u8*, u32> (sg.data.at(i).first, sg.data.at(i).second);
	}
	
	return std::pair<u8*, u32> (0, 0);
}

u8 SaveDataBin::AttrFromSave(std::string path)
{
	for(int i = 0; i < (int)sg.entries.size(); i++)
	{
		if(!path.compare(sg.entries.at(i)))
			return sg.perm.at(i);
	}
	
	return 0;
}

bool SaveDataBin::StructFromNand()
{
	//! Get NAND path
	char* buffer = (char*)memalign(32, ISFS_MAXPATH);
	if (ES_GetDataDir(sg.tid, buffer) < 0)
	{
		if(buffer) free(buffer);
		return false;
	}
	std::string nandPath = buffer;
	free(buffer);
	
	//! create entries list
	CreateEntryList(nandPath);
	if(isfsPath.empty())
		return false;
	
	for(int i = 0; i < (int)isfsPath.size(); i++)
	{
		//! get perm
		u32 ownerID;
		u16 groupID;
		u8  attributes;
		u8  ownerperm;
		u8  groupperm;
		u8  otherperm;
		
		ISFS_GetAttr(isfsPath.at(i).c_str(), &ownerID, &groupID, &attributes, &ownerperm, &groupperm, &otherperm);
		
		u8 perm = ownerperm;
		perm = ( perm << 2 ) | groupperm;
		perm = ( perm << 2 ) | otherperm;
		
		//! determine file or folder
		int fd = Nand::OpenRead(isfsPath.at(i));
		if(fd < 0)
		{
			perm = ( perm << 2 ) | NAND_DIR;
			sg.data.push_back(std::pair<u8*, u32>(0, 0));
		}
		else
		{
			perm = ( perm << 2 ) | NAND_FILE;
			
			//! get data & size
			u32 size = Nand::GetFileSize(fd);
			bool result = false;
			if ((int)size >= 0)
			{
				u8* Data = NULL;
				if ((int)size > 0)
				{
					Data = (u8*)memalign(32, RU(size, 32));
					memset(Data, 0, RU(size, 32));
				}
				if (Data)
				{
					if (Nand::Read(fd, Data, size) == (int)size)
					{
						sg.data.push_back(std::pair<u8*, u32>(Data, size));
						result = true;
					}
				}
			}
			Nand::Close(fd);
			
			if(result == false)
				return false;
		}
		
		//! store entrie and perm
		sg.perm.push_back(perm);
		//sg.attr.push_back(attr);
		sg.entries.push_back(isfsPath.at(i).erase(0, 29));
	}
	
	return true;
}

bool SaveDataBin::StructToDataBin(std::string DestPath)
{
	//! create data
	std::pair<u8*, u32> bnr = DataFromSave("/banner.bin");
	if(!bnr.second)
	{
		gprintf("\n\tSaveDataBin::StructToDataBin -> no banner.bin found in the save");
		return false;
	}
	
	if(bnr.first[0] != 'W' || bnr.first[1] != 'I' || bnr.first[2] != 'B' || bnr.first[3] != 'N')
	{
		gprintf("\n\tSaveDataBin::StructToDataBin -> banner.bin bad magic WIBN (%c%c%c%c)", bnr.first[0], bnr.first[1], bnr.first[2], bnr.first[3]);
		return false;
	}
	
	//!make the different byteArrays that make up this beast and assemble them all at the end
	u8 mainHeader[0xf0c0];
	memset(mainHeader, '\0', sizeof(mainHeader));
	
	wbe64(mainHeader, sg.tid);
	
	wbe32(mainHeader+8, bnr.second);
	
	u8 tmp8 = AttrFromSave("/banner.bin");
	mainHeader[0xc] = (tmp8 >> 2);
	tmp8 = 0;
	mainHeader[0xd] = tmp8;
	
	u8 md5blanker[16] = MD5_BLANKER;
	memcpy(mainHeader+0xe, md5blanker, 16);
	memcpy(mainHeader+0x20, bnr.first, bnr.second);
	
	u8 tmpHeader[0xf0c0];
	memset(tmpHeader, '\0', sizeof(tmpHeader));
	memcpy(tmpHeader, mainHeader, sizeof(tmpHeader));
	
	MD5 hash;
	hash.update(mainHeader, sizeof(mainHeader));
	hash.finalize();
	memcpy(tmpHeader+0x0e, hash.hexdigestChar(), 16);
	
	//!encrypt the header
	memset(mainHeader, '\0', sizeof(mainHeader));
	u8 iv[16] = SD_IV;
	u8 sdkey[16] = SD_KEY;
	aes_set_key(sdkey);
	aes_encrypt(iv, tmpHeader, mainHeader, 0xf0c0);
	
	//!do the file stuff
	u32 cnt = sg.perm.size();
	u8* files = NULL; 
	u32 fSize = 0;
	for(u32 i = 0; i < cnt; i++)
	{
		if(!sg.entries.at(i).compare("/banner.bin"))
			continue;
		
		u32 size = sg.data.at(i).second;
		u8 perm = sg.perm.at(i);
		u8 type = NAND_ATTR_TYPE(perm);
		perm = perm >> 2;
		
		u8 fHeader[0x80];
		memset(fHeader, 0, 0x80);
		wbe32(fHeader, (u32)0x03adf17e);			//!magic1
		wbe32(fHeader+4, size);						//!size
		fHeader[8] = perm;							//!permissions
		fHeader[9] = 0;//attr;						//!attributes
		fHeader[0x0a] = type;						//!file or dir
		
		std::string name = sg.entries.at(i);				//!path
		name.erase(0, 1);									//!remove leading "/"
		memcpy(fHeader + 0x0b, name.c_str(), name.size());	//!name (null terminated)
		
		files = (u8*)realloc(files, fSize+0x80);
		memcpy(files+fSize, fHeader, 0x80);
		fSize += 0x80;
		
		if( type == NAND_FILE )
		{
			u32 roundedSize = RU(size, 0x40);
			
			u8* dec_data = (u8*)malloc(roundedSize);
			memcpy(dec_data, sg.data.at(i).first, size);
			memset(dec_data+size, 0, roundedSize-size);
			
			u8* enc_data = (u8*)malloc(roundedSize);
			memset(enc_data, 0, roundedSize);
			
			u8 iv[0x10];
			memcpy(iv, fHeader+0x50, 0x10);
			aes_set_key(sdkey);
			aes_encrypt(iv, dec_data, enc_data, roundedSize);
			free(dec_data);
			
			files = (u8*)realloc(files, fSize+roundedSize);
			memcpy(files+fSize, enc_data, roundedSize);
			free(enc_data);
			
			fSize += roundedSize;
		}
	}
	
	u8 bkHeader[0x80];
	memset(bkHeader, 0, 0x80);

	if(!GetWiiKeys())
		return false;
	
	wbe32(bkHeader, 0x70);
	wbe32(bkHeader + 4, 0x426b0001);
	wbe32(bkHeader + 8, ngID);
	wbe32(bkHeader + 0x0c, cnt-1);
	wbe32(bkHeader + 0x10, fSize);
	wbe32(bkHeader + 0x1c, fSize + 0x3c0);

	wbe64(bkHeader + 0x60, sg.tid);
	memcpy(bkHeader + 0x68, ngMac, 6);

	//!make the magic cert stuff
	char signer[64];
	char name[64];
	u8 sig[0x40];
	u8 ng_cert[0x180];
	u8 ap_cert[0x180];
	u8 ap_priv[30];
	u8 ap_sig[60];
	
	sprintf(signer, "Root-CA00000001-MS00000002");
	sprintf(name, "NG%08x", ngID);
	make_ec_cert(ng_cert, ngSig, signer, name, ngPriv, ngKeyID);
	
	memset(ap_priv, 0, sizeof(ap_priv));
	ap_priv[10] = 1;
	
	memset(ap_sig, 81, sizeof(ap_sig));
	
	sprintf(signer, "Root-CA00000001-MS00000002-NG%08x", ngID);
	sprintf(name, "AP%08x%08x", 1, 2);
	make_ec_cert( ap_cert, ap_sig, signer, name, ap_priv, 0 );
	
	u8* sha = GetSha1(ap_cert + 0x80, 0x100);
	generate_ecdsa(ap_sig, ap_sig + 30, ngPriv, sha);
	make_ec_cert(ap_cert, ap_sig, signer, name, ap_priv, 0 );
	
	u32 data_size = fSize + 0x80;
	
	u8* stuffToHash = (u8*)malloc(data_size);
	memcpy(stuffToHash, bkHeader, 0x80);
	memcpy(stuffToHash + 0x80, files, fSize);
	sha = GetSha1( stuffToHash, data_size );
	sha = GetSha1( sha, 20 );
	generate_ecdsa(sig, sig + 30, ap_priv, sha);
	wbe32(sig + 60, 0x2f536969);
	free(stuffToHash);
	free(sha);
	
	//! Write data.bin to storage device
	u32 totalSize = 0xf0c0 + fSize + 0x3c0;
	
	u8* stuffToWrite = (u8*)malloc(totalSize);
	if(!stuffToWrite)
	{
		if(files) free(files);
		return false;
	}
	
	memcpy(stuffToWrite, mainHeader, 0xf0c0);
	int offset = 0xf0c0;
	memcpy(stuffToWrite + offset, bkHeader, 0x80);
	offset += 0x80;
	memcpy(stuffToWrite + offset, files, fSize);
	offset += fSize;
	memcpy(stuffToWrite + offset, sig, 0x40);
	offset += 0x40;
	memcpy(stuffToWrite + offset, ng_cert, 0x180);
	offset += 0x180;
	memcpy(stuffToWrite + offset, ap_cert, 0x180);
	
	DestPath += "/data.bin";
	if(!WriteFile(DestPath, stuffToWrite, totalSize))
	{
		if(files) free(files);
		if(stuffToWrite) free(stuffToWrite);
		return false;
	}
	
	if(files) free(files);
	if(stuffToWrite) free(stuffToWrite);
	
	return true;
}
