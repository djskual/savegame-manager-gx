#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>

#include "DataBinTools.h"
#include "../../Tools/gecko.h"
#include "../../Text/wstring.hpp"
#include "Crypto/aes.h"
#include "Crypto/sha1.h"
#include "Crypto/md5.h"

u32 be32(const u8 *p)
{
	return (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
}

u64 be64(const u8 *p)
{
	return ((u64)be32(p) << 32) | be32(p + 4);
}

void wbe16(u8 *p, u16 x)
{
	p[0] = x >> 8;
	p[1] = x;
}

void wbe32(u8 *p, u32 x)
{
	wbe16(p, x >> 16);
	wbe16(p + 2, x);
}

void wbe64(u8 *p, u64 x)
{
	wbe32(p, x >> 32);
	wbe32(p + 4, x);
}

u8* GetSha1( u8 * stuff, u32 stuff_size )
{
	SHA1Context sha;
	SHA1Reset( &sha );
	SHA1Input( &sha, (const unsigned char*)stuff, stuff_size );
	if( !SHA1Result( &sha ) )
	{
		gprintf("\n\tGetSha1 -> sha error");
		return 0;
	}
	u8* ret = (u8*)malloc(20);
	memset(ret, '\0', 20);
	
	for( int i = 0; i < 5 ; i++ )
		memcpy( (char*)ret + ( i * 4 ), &sha.Message_Digest[ i ], 4 );
	
	return ret;
}

bool IsValidSave( const SaveGame &save )
{
	u16 cnt = save.entries.size();
	
	if( !save.tid || cnt != save.perm.size() /*|| cnt != save.attr.size() */|| cnt != save.data.size())
	{
		gprintf("\n\tIsValidSave :bad sizes");
		return false;
	}
	
	bool banner_ok = false;
	for(u32 i = 0; i < cnt; i++)
	{
		if( !save.entries.at( i ).compare("/banner.bin") )
		{
			banner_ok = true;
			break;
		}
	}
	if( !banner_ok )
	{
		gprintf("\n\tIsValidSave :save doesnt contain a banner.bin");
		return false;
	}

	for( u16 i = 0; i < cnt; i++ )
	{
		if( save.entries.at( i ).size() > 52 )
		{
			gprintf("\n\tIsValidSave :name is too long");
			return false;
		}
		u8 perm = save.perm.at( i );
		if( NAND_ATTR_TYPE( perm ) == NAND_FILE )
		{
		}
		else if( NAND_ATTR_TYPE( perm ) == NAND_DIR )
		{
		}
		else
		{
			gprintf("\n\tIsValidSave :unknown attribute %u", perm);
			return false;
		}
	}
	
	return true;
}

bool WriteFile(std::string FilePath, u8* data, u32 data_size)
{
	FILE * outfp = fopen(FilePath.c_str(), "wb");
	if (outfp)
	{
		if((int)fwrite(data, 1, data_size, outfp) >= 0)
		{
			fclose(outfp);
			return true;
		}
		fclose(outfp);
	}
	return false;
}

bool CopyDataBin(std::string ID, std::string SrcFilePath, std::string DestPath)
{
	std::string destPath = DestPath;
	if(destPath[destPath.size()-1] != '/')
		destPath += "/";
	destPath += ID;
	
	if(!CreateSubfolder(destPath.c_str()))
	{
		gprintf("\n\tMoveDataBin -> Can't create folder %s", destPath.c_str());
		return false;
	}
	
	destPath += SrcFilePath.substr(SrcFilePath.rfind("/"));
	
	if(CopyFile(SrcFilePath.c_str(), destPath.c_str()) <= 0)
	{
		gprintf("\n\tMoveDataBin -> Can't copy file %s", SrcFilePath.c_str());
		return false;
	}
	
	sleep(1);
	
	return true;
}

SaveInfos * GetSaveInfos(std::string DataBinPath)
{
	SaveInfos * infos = new SaveInfos();
	
	u64 stuff_size = 0;
	u8 *stuff = NULL;
	LoadFileToMem(DataBinPath.c_str(), &stuff, &stuff_size);
	
	if( stuff_size < 0xf140 ) //!header + backup header
	{
		//gprintf("\n\tGetSaveInfos -> size is too small");
		delete infos;
		return NULL;
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
	{
		//gprintf("\n\tGetSaveInfos -> md5 mismatch");
		delete infos;
		return NULL;
	}
	
	infos->tid = be64(header);
	
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
	
	infos->name = ws_data->toUTF8();
	infos->subname = ws_subdata->toUTF8();
	
	delete ws_data;
	delete ws_subdata;
	
	u8 bk_header[0x80];
	memcpy(bk_header, stuff+size, 0x80);
	
	if (be32(bk_header) != 0x70)
	{
		//gprintf("\n\tGetSaveInfos -> bad hdr size");
		delete infos;
		return NULL;
	}
	if (be32(bk_header + 4) != 0x426b0001)
	{
		//gprintf("\n\tGetSaveInfos -> bad hdr magic");
		delete infos;
		return NULL;
	}
	memcpy(infos->ID, bk_header + 100, 4);
	
	free(stuff);
	
	return infos;
}
