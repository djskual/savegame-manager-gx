/****************************************************************************
 * libwiigui Template
 * Tantric 2009
 *
 * devicebrowser.cpp
 *
 * Generic file routines - reading, writing, browsing
 ***************************************************************************/

#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "prompts/ProgressWindow.h"
#include "saves/savegame.h"
#include "saves/title.h"
#include "saves/utils.h"
#include "saves/wii_list.h"
#include "saves/WiiTDB.hpp"
#include "file/fileops.h"
#include "text/wstring.hpp"
#include "menu/menu.h"
#include "main.h"

WIILISTINFO wiiSaveInfo;
WIILISTENTRY * wiiSaveList = NULL;

/****************************************************************************
 * ResetWiiSaveList()
 *
 * Clears the file browser memory, and allocates one initial entry
 ***************************************************************************/
void ResetWiiSaveList()
{
	wiiSaveInfo.Cnt = 0;
	wiiSaveInfo.selIndex = 0;
	wiiSaveInfo.pageIndex = 0;
	
	// Clear any existing values
	if(wiiSaveList != NULL)
	{
		free(wiiSaveList);
		wiiSaveList = NULL;
	}
	// set aside space for 1 entry
	wiiSaveList = (WIILISTENTRY *)malloc(sizeof(WIILISTENTRY));
	memset(wiiSaveList, 0, sizeof(WIILISTENTRY));
}


/****************************************************************************
 * utf16fix()
 *
 * fix utf-16 name & subname
 ***************************************************************************/
int utf16fix(char* name, char* subname, u16* namedata, u16* subdata)
{
	// Get the name & subname of the title
    wchar_t wname[65] __attribute__ ((aligned (32)));
    wchar_t wsubname[65] __attribute__ ((aligned (32)));
    
	u32 i = 0;
    for (i = 0; i < 65; i++){
		wname[i] = namedata[i];
	}
	for (i = 0; i < 65; i++){
		wsubname[i] = subdata[i];
	}
	
	free(namedata);
	free(subdata);
	
	wString *ws_name = new wString(wname);
	wString *ws_subname = new wString(wsubname);
	
	snprintf(name, 65, "%s", (ws_name->toUTF8()).c_str());
    snprintf(subname, 65, "%s", (ws_subname->toUTF8()).c_str());
    
	delete ws_name;
	delete ws_subname;
	
	return 1;
}

/****************************************************************************
 * GetNandNames()
 *
 * Get the utf-16 name & subname of the save
 ***************************************************************************/
int GetNandNames(char* name, char* subname, u64 tid) {
    
	// Terminate the name string just in case the function exits prematurely
    name[0] = 0;
	subname[0] = 0;
	
    // Create a string containing the absolute filename
    char file[256] __attribute__ ((aligned (32)));
    sprintf(file, "/title/%08x/%08x/data/banner.bin", (u32)(tid >> 32), (u32)tid);

    // Bring the Wii into the title's userspace
    if (ES_SetUID(tid) < 0)
        return -1;

    // Try to open file
    s32 fh = ISFS_Open(file, ISFS_OPEN_READ);

    // If a title does not have a banner.bin bail out
    if (fh == -106)
        return -2;
	
	// Get the name data from the banner.bin
	ISFS_Seek(fh, 0x20, 0);
	u16* namedata = NULL;
	namedata = (u16*)memalign(32, 0x40);

	if (ISFS_Read(fh, (char*)namedata, 0x40) < 0)
	{
		ISFS_Close(fh);
		free(namedata);
		return -3;
		
	}
	
	// Get the subname data from the banner.bin
	ISFS_Seek(fh, 0x60, 0);
	u16* subdata = NULL;
	subdata = (u16*)memalign(32, 0x40);

	if (ISFS_Read(fh, (char*)subdata, 0x40) < 0)
	{
		ISFS_Close(fh);
		free(subdata);
		return -3;
		
	}
	
    // Close the banner.bin
    ISFS_Close(fh);

    // Get the name & subname of the title
    utf16fix(name, subname, namedata, subdata);
	
	// Job well done
    return 1;
}

/****************************************************************************
 * GetDeviceNames()
 *
 * Get the utf-16 name & subname of the save on storage device
 ***************************************************************************/
int GetDeviceNames(char* name, char* subname, char* savepath) {
    
	// Terminate the name string just in case the function exits prematurely
    name[0] = 0;
	subname[0] = 0;
	
    // Generate filepath //
	char bannerpath[128];
	sprintf(bannerpath, "%s/banner.bin", savepath);
	
	// Try to open file
    FILE *fp = fopen(bannerpath,"rb");

	// Get the name data from the banner.bin
	fseek(fp , 0x20, SEEK_SET);
	
	u16* namedata = NULL;
	namedata = (u16*)memalign(32, 0x40);
	
	fread(namedata, 1, 0x40, fp);
	rewind (fp);
	
	// Get the subname data from the banner.bin
	fseek(fp , 0x60, SEEK_SET);
	
	u16* subdata = NULL;
	subdata = (u16*)memalign(32, 0x40);

	fread(subdata, 1, 0x40, fp);
	
    // Close the banner.bin
    fclose(fp);

    // Get the name & subname of the title
    utf16fix(name, subname, namedata, subdata);
	
	// Job well done
    return 1;
}

/****************************************************************************
 * WiiListSortCallback
 *
 * Quick sort callback to sort saves entries
 ***************************************************************************/
int WiiListSortCallback(const void *f1, const void *f2)
{
	return stricmp(((WIILISTENTRY *)f1)->name, ((WIILISTENTRY *)f2)->name);
}

/****************************************************************************
 * CreateWiiSaveList
 *
 * Create a list of saves installed in the wii
 ***************************************************************************/
int CreateWiiSaveList()
{
	ResetWiiSaveList();
	
	u32 idx = 0;
	int ret = 0;
	int cntlist = 0;
	
	u64 *titleList = NULL;
	u32  titleCnt;

	// Get title list //
	ret = Title_GetList(&titleList, &titleCnt);
	if(ret < 0)
		return -1;
	if(titleCnt == 0) {
		wiiSaveInfo.Cnt = 0;
		return 0;}
	
	WIILISTENTRY * newWiiSaveList = (WIILISTENTRY *)realloc(wiiSaveList, (titleCnt+1) * sizeof(WIILISTENTRY));

	if(!newWiiSaveList) // failed to allocate required memory
	{
		ResetWiiSaveList();
		return -1;
	}
	else
	{
		wiiSaveList = newWiiSaveList;
	}
	
	for (idx = 0; idx < titleCnt; idx++) {
		
		memset(&(wiiSaveList[cntlist]), 0, sizeof(WIILISTENTRY)); // clear the new entry
		
		u64 tid = titleList[idx];
		char savepath[128];
		char Path[128];
		
		// Generate dirpath //
		Savegame_GetNandPath(tid, savepath);
		
		// Check for title savegame //
		ret = Savegame_CheckTitle(savepath, ManageWindow::WII);
		if (!ret) {
			
			u32 high = (u32)(tid >> 32);
			u32 low  = (u32)(tid & 0xFFFFFFFF);
			
			// Set title ID //
			wiiSaveList[cntlist].tid = tid;
			
			// Set title name & subname //
			GetNandNames(wiiSaveList[cntlist].name, wiiSaveList[cntlist].subname, wiiSaveList[cntlist].tid);
			
			// Set title blockSize //
			snprintf(Path, sizeof(Path),"/title/%08x/%08x/data", high, low);
			wiiSaveList[cntlist].blockSize = ((float)(get_nand_size_directory(Path)/(float)NAND_BLOCK_SIZE));
			
			// Set title ID[4] //
			snprintf(wiiSaveList[cntlist].ID, sizeof(wiiSaveList[cntlist].ID), "%s", titleText(high,low));
			
			cntlist++;
		}
	}
	
	// Free memory //
	if (titleList)
		free(titleList);
	
	wiiSaveInfo.Cnt = cntlist;
	
	// set WiiTdb Titles //
	LoadTitlesFromWiiTDB(cfg.WiiTdbPath, WiiTDB::WII);
	
	qsort(wiiSaveList, wiiSaveInfo.Cnt, sizeof(WIILISTENTRY), WiiListSortCallback);
	
	return wiiSaveInfo.Cnt;
}
