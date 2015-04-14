/*-------------------------------------------------------------
 from any title deleter and wad manager 1.4
title.h --

Copyright (C) 2008 tona and/or waninkoko
-------------------------------------------------------------*/
#include <gccore.h>
#include <ogcsys.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>

#ifndef _TITLE_H_
#define _TITLE_H_

#ifdef __cplusplus
extern "C" {
#endif
    /* Constants */
#define BLOCK_SIZE			1024
#define MAX_TITLES			256
#define TITLE_ID(x,y)		(((u64)(x) << 32) | (y))
#define TITLE_UPPER(x)          ((u32)((x) >> 32))
#define TITLE_LOWER(x)          ((u32)(x))

// Get the number of titles on the Wii of a given type
    s32 getTitles_TypeCount(u32 type, u32 *count);

// Get the list of titles of this type
    s32 getTitles_Type(u32 type, u32 *titles, u32 count);

//boot HBC in either HAXX or JODI locations
	s32 WII_BootHBC();
	
//fonction from savegame-manager
	s32 Title_GetList(u64 **, u32 *);
	s32 Title_GetTicketViews(u64, tikview **, u32 *);
	s32 Title_GetTMD(u64, signed_blob **, u32 *);
	s32 Title_GetVersion(u64, u16 *);
	s32 Title_GetSysVersion(u64, u64 *);
	s32 Title_GetSize(u64, u32 *);

// Get The GameID[4] based on titleid
	char *titleText(u32 kind, u32 title);

#ifdef __cplusplus
}
#endif

#endif
