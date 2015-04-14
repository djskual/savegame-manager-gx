#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <gccore.h>
#include <ogcsys.h>

#include "mii/mii_installer.h"

extern bool progressmiicanceled;

u16 crc16(const u8 *buf, u32 len)
{
	u16 crc = 0x0000;
	s32 byte_idx, bit_idx, cnt;

	for (byte_idx = 0; byte_idx < (int)len; byte_idx++)
	for (bit_idx = 7; bit_idx >= 0; bit_idx--)
		crc = (((crc << 1) | ((buf[byte_idx] >> bit_idx) & 0x1)) ^ (((crc & 0x8000) != 0) ? 0x1021 : 0));

	for (cnt = 16; cnt > 0; cnt--)
		crc = ((crc << 1) ^ (((crc & 0x8000) != 0) ? 0x1021 : 0));

	return (crc & 0xFFFF);
}

s32 install_mii(const char * miiPath, int reference)
{
	static struct mii_db database ATTRIBUTE_ALIGN(32);

	s32 ret;
	s32 miiFile = -1;

	/* Read Miis container */
	miiFile = ISFS_Open(MII_FILE, ISFS_OPEN_READ);
	if (miiFile < 0)
		return 0;
	
	ret = ISFS_Read(miiFile, &database, sizeof(database));
	if (ret != sizeof(database)) {
		ISFS_Close(miiFile);
		return 0;
	}
	
	ISFS_Close(miiFile);	
	
	/* Read Mii */
	FILE * File;
	struct mii_t buffer;
	memset(&buffer, 0, sizeof(buffer));

	if(!strcmp(miiPath, "DELETE") || !strcmp(miiPath, "DELETE_ALL"))
		goto installMii;
	
	File = fopen(miiPath, "rb");
	if (File==NULL) {
		return 0;
	}
	fread(&buffer,1,74,File);
	fclose (File);
	
	fflush(stdout);
	
  installMii:

	/* Copy Mii to Miis database */
	if(!strcmp(miiPath, "DELETE_ALL")){
		int i=0;
		for(i=0; i<MII_MAX; i++) {
			memcpy(&database.mii[i], &buffer, 74);
		}
	}
	else
		memcpy(&database.mii[reference], &buffer, 74);

	/* Calculate new CRC */
	database.crc = crc16((u8 *)&database, sizeof(database) - sizeof(database.crc));

	/* Write Miis */
	miiFile = ISFS_Open(MII_FILE, ISFS_OPEN_WRITE);
	if (miiFile < 0)
		return 0;

	ret = ISFS_Write(miiFile, &database, sizeof(database));
	if (ret != sizeof(database)) {
		ISFS_Close(miiFile);
		return 0;
	}
	
	ISFS_Close(miiFile);
	
	if(progressmiicanceled == true)
		return -10;
	
	return 1;
}
