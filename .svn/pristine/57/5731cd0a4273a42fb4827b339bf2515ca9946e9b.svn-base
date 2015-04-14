 /***************************************************************************
 * Copyright (C) 2009
 * by Dimok
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
 * foldersize.cpp
 *
 * FolderSizeThread
 * for Wii-FileXplorer 2009
 ***************************************************************************/
#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "file/fileops.h"
#include "saves/utils.h"

/*** Variables used only in this file ***/
static lwp_t foldersizethread = LWP_THREAD_NULL;
static char folderpath[1024];
static u64 foldersize = 0;
static u32 filecount = 0;
bool sizegainrunning = false;

/****************************************************************************
 * GetCurrentFolderSize
 ***************************************************************************/
u64 GetCurrentFolderSize()
{
    return foldersize;
}
/****************************************************************************
 * GetFilenumber
 ***************************************************************************/
u32 GetFilecount()
{
    return filecount;
}

/****************************************************************************
 * GetSize
 ***************************************************************************/
static void GetSize()
{
    if(!sizegainrunning)
        return;

    GetFolderSize(folderpath, foldersize, filecount);

    sizegainrunning = false;
}

/****************************************************************************
 * FolderSizeThread
  ***************************************************************************/

static void * FolderSizeThread(void *arg UNUSED)
{
	while(1)
	{
		if(!sizegainrunning)
			LWP_SuspendThread(foldersizethread);

        GetSize();

        usleep(100);
	}
	return NULL;
}
/****************************************************************************
 * StartGetFolderSizeThread
 *
 * Update the size value
 ***************************************************************************/
void StartGetFolderSizeThread(const char * path)
{
    strncpy(folderpath, path, sizeof(folderpath));
    foldersize = 0;
    filecount = 0;
    sizegainrunning = true;
    LWP_ResumeThread(foldersizethread);
}
/****************************************************************************
 * StopSizeGain
 ***************************************************************************/
void StopSizeGain()
{
    sizegainrunning = false;
}

/****************************************************************************
 * InitGetSizeThread
 *
 * Startup FolderSizeThread in idle prio
 ***************************************************************************/
void InitGetSizeThread() {
	LWP_CreateThread(&foldersizethread, FolderSizeThread, NULL, NULL, 0, 60);
}

/****************************************************************************
 * ExitGetSizeThread
 *
 * Shutdown FolderSizeThread
 ***************************************************************************/
void ExitGetSizeThread() {
	LWP_JoinThread(foldersizethread, NULL);
	foldersizethread = LWP_THREAD_NULL;
}
