/****************************************************************************
 * SaveGame Manager GX 2010
 *
 * wii_list.h
 *
 * Generic save routine - read wii save list and get saves info;
 ****************************************************************************/

#ifndef _WII_LIST_H_
#define _WII_LIST_H_

#include <unistd.h>
#include <gccore.h>

#define MAXNAME 64

typedef struct
{
	int Cnt; // # of entries in wiiSaveList
	int selIndex; // currently selected index of browserList
	int pageIndex; // starting index of browserList page display
	
} WIILISTINFO;

typedef struct
{
	float blockSize; // size of the save
	u64 tid;//titleid of the save
	char name[MAXNAME + 1]; // title name
	char subname[MAXNAME + 1]; // title subname
	char ID[5]; // title ID
} WIILISTENTRY;

extern WIILISTINFO wiiSaveInfo;
extern WIILISTENTRY * wiiSaveList;

void ResetWiiSaveList();
int GetNandNames(char* name, char* subname, u64 tid);
int GetDeviceNames(char* name, char* subname, char* savepath);
int WiiListSortCallback(const void *f1, const void *f2);
int CreateWiiSaveList();



#endif
