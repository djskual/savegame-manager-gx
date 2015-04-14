#ifndef DATABINTOOLS_H
#define DATABINTOOLS_H

#include <gctypes.h>
#include <vector>
#include <string>

#include "../../FileOperations/fileops.h"

#define RU(x,n)		(-(-(x) & -(n)))

#define NAND_FILE	1
#define NAND_DIR	2

#define NAND_ATTR_TYPE( attr ) ( attr & 3 )

#define SD_KEY		{ 0xab, 0x01, 0xb9, 0xd8, 0xe1, 0x62, 0x2b, 0x08, 0xaf, 0xba, 0xd8, 0x4d, 0xbf, 0xc2, 0xa5, 0x5d };
#define SD_IV		{ 0x21, 0x67, 0x12, 0xe6, 0xaa, 0x1f, 0x68, 0x9f, 0x95, 0xc5, 0xa2, 0x23, 0x24, 0xdc, 0x6a, 0x98 };
#define MD5_BLANKER { 0x0e, 0x65, 0x37, 0x81, 0x99, 0xbe, 0x45, 0x17, 0xab, 0x06, 0xec, 0x22, 0x45, 0x1a, 0x57, 0x93 };

struct SaveGame //!struct to hold save data
{
	u64 tid;
	std::string name;
	std::string subname;
	u32 decryptedSize;
	char ID[5];
	std::vector<std::string>		  entries;
	std::vector<u8>					  perm;
	//std::vector<u8>					  attr;
	std::vector<std::pair<u8*, u32> > data;
};

struct SaveInfos //!struct to hold save infos
{
	u64 tid;
	std::string name;
	std::string subname;
	char ID[5];
};

u32 be32(const u8 *p);
u64 be64(const u8 *p);
void wbe16(u8 *p, u16 x);
void wbe32(u8 *p, u32 x);
void wbe64(u8 *p, u64 x);

u8* GetSha1( u8 *stuff, u32 stuff_size );

bool IsValidSave( const SaveGame &save );
bool WriteFile(std::string FilePath, u8* data, u32 data_size);
bool CopyDataBin(std::string ID, std::string SrcFilePath, std::string DestPath);
SaveInfos * GetSaveInfos(std::string DataBinPath);

#endif // TOOLS_H
