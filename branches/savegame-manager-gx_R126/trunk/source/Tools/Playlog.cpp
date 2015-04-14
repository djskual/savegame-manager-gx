/*********************************************************************************
 *	Playlog.cpp
 *	This code allows to modify play_rec.dat in order to store the
 *	game time in Wii's log correctly.
 *
 *	by Marc
 *	Thanks to tueidj for giving me some hints on how to do it :)
 *	Most of the code was taken from here:
 *	http://forum.wiibrew.org/read.php?27,22130
 *
 *	Modified by Dj_Skual
 *  for SaveGame Manager GX 2012
 ********************************************************************************/
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "../FileOperations/Nand.hpp"
#include "../Tools/tools.h"
#include "Playlog.h"

#define SECONDS_TO_2000 946684800LL
#define TICKS_PER_SECOND 60750000LL

//! Should be 32 byte aligned
static const char PLAYRECPATH[] ATTRIBUTE_ALIGN(32) = "/title/00000001/00000002/data/play_rec.dat";

//! Thanks to Dr. Clipper
static u64 getWiiTime(void)
{
	time_t uTime = time(NULL);
	return TICKS_PER_SECOND * (uTime - SECONDS_TO_2000);
}

int Playlog::Create()
{
	if(Nand::CheckFile(PLAYRECPATH))
		return 0; //!exists
	
	//!In case the play_rec.dat wasn´t found create one and try again
	int ret = Nand::CreateFile(PLAYRECPATH, 0, 3, 3, 3);
	if(ret >= 0)
		ISFS_SetAttr(PLAYRECPATH, 0x1000, 1, 0, 3, 3, 3);

	return ret;
}

int Playlog::Update(const char * ID, const u16 * title)
{
	if(!ID || !title)
		return -1;
	
	int res = -1;
	
	//!Open play_rec.dat
	int fd = Nand::OpenReadWrite(PLAYRECPATH);
	if(fd == -106)
	{
		//!In case the play_rec.dat wasn´t found create one and try again
		int ret = Playlog::Create();
		if(ret < 0)
			return ret;
		fd = Nand::OpenReadWrite(PLAYRECPATH);
	}
	
	if(fd < 0)
		return res;
	
	PlayRec * playrec_buf = (PlayRec *)Tools::AllocateMemory_32(sizeof(PlayRec)); //! Should be 32 byte aligned
	if(playrec_buf)
	{
		memset(playrec_buf, 0, sizeof(PlayRec));
		
		u64 stime = getWiiTime();
		playrec_buf->ticks_boot = stime;
		playrec_buf->ticks_last = stime;
		
		//!Update channel name and ID
		memcpy(playrec_buf->name, title, 84);
		memcpy(playrec_buf->title_id, ID, 6);
		
		//!Calculate and update checksum
		u32 sum = 0;
		for(u8 i = 0; i < 31; i++)
			sum += playrec_buf->data[i];
		
		playrec_buf->checksum = sum;
		
		//!Write play_rec.dat
		if(Nand::Write(fd, (u8*)playrec_buf, sizeof(PlayRec)) == sizeof(PlayRec))
			res = 0;
		
		free(playrec_buf);
	}
	Nand::Close(fd);
	
	return res;
}

int Playlog::Delete()
{
	int res = -1;
	
	//!Open play_rec.dat
	int fd = Nand::OpenReadWrite(PLAYRECPATH);
	if(fd >= 0)
	{
		PlayRec * playrec_buf = (PlayRec *)Tools::AllocateMemory_32(sizeof(PlayRec));
		if(playrec_buf)
		{
			//!Read play_rec.dat
			if(Nand::Read(fd, (u8*)playrec_buf, sizeof(PlayRec)) == sizeof(PlayRec))
			{
				if(Nand::Seek(fd, 0, 0) >= 0)
				{
					//! invalidate checksum
					playrec_buf->checksum = 0;
					
					if(Nand::Write(fd, (u8*)playrec_buf, sizeof(PlayRec)) == sizeof(PlayRec))
						res = 0;
				}
			}
			free(playrec_buf);
		}
		Nand::Close(fd);
	}
	
	return res;
}
