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
#include <wiiuse/wpad.h>
#include <sys/dir.h>
#include <malloc.h>

#include "saves/savegame.h"
#include "saves/wii_list.h"
#include "saves/WiiTDB.hpp"
#include "saves/title.h"
#include "saves/utils.h"
#include "file/fileops.h"
#include "text/wstring.hpp"
#include "prompts/browsers/devicebrowser.h"
#include "menu/menu.h"
#include "main.h"

extern bool sizegainrunning;

BROWSERDEVICEINFO browserD;
BROWSERDEVICEENTRY * browserDevice = NULL; // list of files/folders in browser

char * read(char * path){
	FILE * File;
	char * buffer;
	File = fopen(path, "rb");
	if (File==NULL) {
		return NULL;
	}
	buffer = (char*) malloc (74);
	fread(buffer,1,74,File);
	fclose (File);
	return buffer;
}

/****************************************************************************
 * ResetDeviceBrowser()
 * Clears the file browser memory, and allocates one initial entry
 ***************************************************************************/
void ResetDeviceBrowser()
{
	browserD.numEntries = 0;
	browserD.selIndex = 0;
	browserD.pageIndex = 0;

	// Clear any existing values
	if(browserDevice != NULL)
	{
		free(browserDevice);
		browserDevice = NULL;
	}
	// set aside space for 1 entry
	browserDevice = (BROWSERDEVICEENTRY *)malloc(sizeof(BROWSERDEVICEENTRY));
	memset(browserDevice, 0, sizeof(BROWSERDEVICEENTRY));
}

/****************************************************************************
 * UpdateDeviceDirName()
 * Update curent directory name for file browser
 ***************************************************************************/
int UpdateDeviceDirName()
{
	int size=0;
	char * test;
	char temp[1024];

	/* current directory doesn't change */
	if (strcmp(browserDevice[browserD.selIndex].filename,".") == 0)
	{
		return 0;
	}
	/* go up to parent directory */
	else if (strcmp(browserDevice[browserD.selIndex].filename,"..") == 0)
	{
		/* determine last subdirectory namelength */
		sprintf(temp,"%s",browserD.dir);
		test = strtok(temp,"/");
		while (test != NULL)
		{
			size = strlen(test);
			test = strtok(NULL,"/");
		}

		/* remove last subdirectory name */
		size = strlen(browserD.dir) - size - 1;
		browserD.dir[size] = 0;

		return 1;
	}
	/* Open a directory */
	else
	{
		/* test new directory namelength */
		if ((strlen(browserD.dir)+1+strlen(browserDevice[browserD.selIndex].filename)) < MAXPATHLEN)
		{
			/* update current directory name */
			sprintf(browserD.dir, "%s%s/",browserD.dir, browserDevice[browserD.selIndex].filename);
			return 1;
		}
		else
		{
			return -1;
		}
	}
}

/****************************************************************************
 * DeviceSortCallback
 *
 * Quick sort callback to sort file entries with the following order:
 *   .
 *   ..
 *   <dirs>
 *   <files>
 ***************************************************************************/
int DeviceSortCallback(const void *f1, const void *f2)
{
	/* Special case for implicit directories */
	if(((BROWSERDEVICEENTRY *)f1)->filename[0] == '.' || ((BROWSERDEVICEENTRY *)f2)->filename[0] == '.')
	{
		if(strcmp(((BROWSERDEVICEENTRY *)f1)->filename, ".") == 0) { return -1; }
		if(strcmp(((BROWSERDEVICEENTRY *)f2)->filename, ".") == 0) { return 1; }
		if(strcmp(((BROWSERDEVICEENTRY *)f1)->filename, "..") == 0) { return -1; }
		if(strcmp(((BROWSERDEVICEENTRY *)f2)->filename, "..") == 0) { return 1; }
	}

	/* If one is a file and one is a directory the directory is first. */
	if(((BROWSERDEVICEENTRY *)f1)->isdir && !(((BROWSERDEVICEENTRY *)f2)->isdir)) return -1;
	if(!(((BROWSERDEVICEENTRY *)f1)->isdir) && ((BROWSERDEVICEENTRY *)f2)->isdir) return 1;

	/* If one is a file/folder and one is a save the directory is first. */
	if(((BROWSERDEVICEENTRY *)f1)->isSave && !(((BROWSERDEVICEENTRY *)f2)->isSave)) return 1;
	if(!(((BROWSERDEVICEENTRY *)f1)->isSave) && ((BROWSERDEVICEENTRY *)f2)->isSave) return -1;
	
	/* Sort Saves */
	if(((BROWSERDEVICEENTRY *)f1)->isSave && ((BROWSERDEVICEENTRY *)f2)->isSave)
		return stricmp(((BROWSERDEVICEENTRY *)f1)->displayname, ((BROWSERDEVICEENTRY *)f2)->displayname);

	/* Sort files*/
	return stricmp(((BROWSERDEVICEENTRY *)f1)->filename, ((BROWSERDEVICEENTRY *)f2)->filename);
}

/***************************************************************************
 * Get Mii Name
 **************************************************************************/
int GetMiiDeviceName(char * name, const char * miipath)
{
	memset(name, 0, sizeof(name));
	
    FILE *fp = fopen(miipath,"rb");

	if(fp == NULL) {
		return 0;
	}
	
	fseek(fp , 2, SEEK_SET);
	u16* namedata = (u16*)memalign(32, 20);
	memset(namedata, 0, sizeof(namedata));
	
	fread(namedata,1,20,fp);
	
	fclose(fp);
	
	wchar_t wname[20];
	memset(wname, 0, 20);
	
	u32 i = 0;
	for (i = 0; i < 20; i++){
		wname[i] = (wchar_t)namedata[i];
	}
	
	free(namedata);
	namedata = NULL;
	
	wString *ws_name = new wString(wname);
	
	snprintf(name, 31, "%s", (ws_name->toUTF8()).c_str());
	
	delete ws_name;
	
	return 1;
}

/***************************************************************************
 * Get Mii Creator name
 **************************************************************************/
int GetMiiDeviceCreator(char * creator, const char * miipath)
{
	memset(creator, 0, sizeof(creator));
	
    FILE *fp = fopen(miipath,"rb");

	if(fp == NULL) {
		return 0;
	}
	
	fseek(fp , 0x36, SEEK_SET);
	u16* creatordata = (u16*)memalign(32, 20);
	memset(creatordata, 0, sizeof(creatordata));
	
	fread(creatordata,1,20,fp);
	
	fclose(fp);
	
	wchar_t wcreator[20];
	memset(wcreator, 0, 20);
	
	u32 i = 0;
	for (i = 0; i < 20; i++){
		wcreator[i] = (wchar_t)creatordata[i];
	}
	
	free(creatordata);
	creatordata = NULL;
	
	wString *ws_creator = new wString(wcreator);
	
	snprintf(creator, 31, "%s", (ws_creator->toUTF8()).c_str());
	
	delete ws_creator;
	
	return 1;
}

/***************************************************************************
 * Get Mii Infos
 **************************************************************************/
int GetMiiInfos(char * fulldir, int entryNum)
{
	char * data;
	data = read(fulldir);
	if (!data)
		return 0;
	
	int day = 0;
	if (data[0x00] & 1) day += 8;
	if (data[0x00] & 2) day += 16;
	if (data[0x01] & 32) day += 1;
	if (data[0x01] & 64) day += 2;
	if (data[0x01] & 128) day += 4;
	browserDevice[entryNum].miiDay = day;
	
	int month = 0;
	if (data[0x00] & 4) month += 1;
	if (data[0x00] & 8) month += 2;
	if (data[0x00] & 16) month += 4;
	if (data[0x00] & 32) month += 8;
	browserDevice[entryNum].miiMonth = month;
	
	browserDevice[entryNum].miiFemale = data[0x00] & 64;
	
	int favColor = 0;
	if (data[0x01] & 2) favColor += 1;
	if (data[0x01] & 4) favColor += 2;
	if (data[0x01] & 8) favColor += 4;
	if (data[0x01] & 16) favColor += 8;
	browserDevice[entryNum].miiFavColor = favColor;
	
	free(data);
	data = NULL;
	
	return 1;
}

/***************************************************************************
 * Browse subdirectories
 **************************************************************************/
int
ParseDeviceDirectory(const char * Path)
{
	DIR_ITER *dir = NULL;
	char fulldir[MAXPATHLEN];
	char filename[MAXPATHLEN];
	struct stat filestat;
	
	// reset browser
	ResetDeviceBrowser();
	
	// open the directory
	dir = diropen(Path);
	snprintf(navinfo.LastBrowsedPath, sizeof(navinfo.LastBrowsedPath), "%s", Path);
	if (dir == NULL)
	{
		sprintf(fulldir, "%s%s", navinfo.CurrentDevice, browserD.dir); // add currentDevice to path
		dir = diropen(fulldir);
		snprintf(navinfo.LastBrowsedPath, sizeof(navinfo.LastBrowsedPath), "%s", fulldir);
	}

	// if we can't open the dir, try opening the root dir
	if (dir == NULL)
	{
		sprintf(browserD.dir,"/");
		dir = diropen(navinfo.CurrentDevice);
		if (dir == NULL)
		{
			return -1;
		}
		snprintf(navinfo.LastBrowsedPath, sizeof(navinfo.LastBrowsedPath), "%s", navinfo.CurrentDevice);
	}
	
	
	// index files/folders
	int entryNum = 0;

	while(dirnext(dir,filename,&filestat) == 0)
	{
		if(strcmp(filename,".") != 0)
		{
			BROWSERDEVICEENTRY * newBrowserDevice = (BROWSERDEVICEENTRY *)realloc(browserDevice, (entryNum+1) * sizeof(BROWSERDEVICEENTRY));

			if(!newBrowserDevice) // failed to allocate required memory
			{
				ResetDeviceBrowser();
				entryNum = -1;
				break;
			}
			else
			{
				browserDevice = newBrowserDevice;
			}
			memset(&(browserDevice[entryNum]), 0, sizeof(BROWSERDEVICEENTRY)); // clear the new entry

			strncpy(browserDevice[entryNum].filename, filename, MAXJOLIET);

			if(strcmp(filename,"..") == 0)
			{
				sprintf(browserDevice[entryNum].displayname, "..");
			}
			else
			{
				strncpy(browserDevice[entryNum].displayname, filename, MAXDISPLAY);	// crop name for display
			}

			browserDevice[entryNum].length = filestat.st_size;
			browserDevice[entryNum].isdir = (filestat.st_mode & _IFDIR) == 0 ? 0 : 1; // flag this as a dir
			
			sprintf(fulldir, "%s%s", navinfo.LastBrowsedPath, browserDevice[entryNum].filename);
			if(VerifyFileType(fulldir, "log"))
				browserDevice[entryNum].isLog = 1;
			if(VerifyFileType(fulldir, "miigx"))
			{
				browserDevice[entryNum].isMii = 1;
				
				GetMiiDeviceName(browserDevice[entryNum].displayname, fulldir);
				GetMiiDeviceCreator(browserDevice[entryNum].miiCreator, fulldir);
				
				GetMiiInfos(fulldir, entryNum);
				
			}
			
			if(browserDevice[entryNum].isdir)
			{
				char savepath[128];
				sprintf(savepath, "%s%s", navinfo.LastBrowsedPath, browserDevice[entryNum].filename);
				
				int ret = Savegame_CheckTitle(savepath, ManageWindow::DEVICE);
				if(!ret)
				{
					browserDevice[entryNum].isdir = 0;
					browserDevice[entryNum].isSave = 1;
					
					browserDevice[entryNum].tid = StrToHex64(browserDevice[entryNum].filename);
					
					// Get name, Subname, and check if the save is already installed
					ret = GetNandNames(browserDevice[entryNum].displayname, browserDevice[entryNum].subname, browserDevice[entryNum].tid);
					if(ret<0) {
						browserDevice[entryNum].isNotInstalled = 1;
						
						GetDeviceNames(browserDevice[entryNum].displayname, browserDevice[entryNum].subname, savepath);
					}
					if(!supportedFormat(savepath))
						browserDevice[entryNum].isNotSupported = 1;
					
					// Get title ID[4]
					snprintf(browserDevice[entryNum].ID, sizeof(browserDevice[entryNum].ID), "%s", titleText((u32)(browserDevice[entryNum].tid>> 32),(u32)(browserDevice[entryNum].tid & 0xFFFFFFFF)));
					
					// Get the BlockSize
					char temp[MAXPATHLEN];
					snprintf(temp, sizeof(temp), "%s/", savepath);
					StartGetFolderSizeThread(temp);
					while(sizegainrunning) usleep(100);
					browserDevice[entryNum].blockSize = ((float)((float)GetCurrentFolderSize()/(float)NAND_BLOCK_SIZE));
					StopSizeGain();
				}
			}
			
			if(browserDevice[entryNum].isdir ||
			   browserDevice[entryNum].isSave ||
			   browserDevice[entryNum].isLog ||
			   browserDevice[entryNum].isMii)
				entryNum++;
		}
	}

	// close directory
	dirclose(dir);
	
	browserD.numEntries = entryNum;
	
	if(cfg.WiiTdbTitles)
		LoadTitlesFromWiiTDB(cfg.WiiTdbPath, WiiTDB::DEVICE);
	
	// Sort the file list
	qsort(browserDevice, browserD.numEntries, sizeof(BROWSERDEVICEENTRY), DeviceSortCallback);

	return browserD.numEntries;
}

/****************************************************************************
 * BrowserDeviceChangeFolder
 *
 * Update current directory and set new entry list if directory has changed
 ***************************************************************************/
int BrowserDeviceChangeFolder()
{
	if(navinfo.firstdevice == false && navinfo.switchdevice == false)
	{
		if(!UpdateDeviceDirName())
			return -1;
	}

	char fullpath[200];
	snprintf(fullpath, sizeof(fullpath), "%s%s", navinfo.CurrentDevice, browserD.dir);
	ParseDeviceDirectory(fullpath);
	
	return browserD.numEntries;
}

void ParseDevice(char *Path)
{
    char temp[200], name[200], value[200];

    memcpy (temp,Path,strlen(Path)+1);

    char * eq = strchr(temp, '/');

    if(!eq) return;

    *eq = 0;

    TrimDevice(name, temp, sizeof(name));
    TrimDevice(value, eq+1, sizeof(value));

	sprintf(navinfo.CurrentDevice, "%s/", name);
	sprintf(browserD.dir, value);
	
	return;
}

void ParseDevicePath(char *Path)
{
    char temp[200], name[200], value[200];

    memcpy (temp,Path,strlen(Path)+1);

    char * eq = strrchr(temp, '/');

    if(!eq) return;

    *eq = 0;

    TrimDevice(name, temp, sizeof(name));
    TrimDevice(value, eq+1, sizeof(value));

	sprintf(browserD.dir, "%s/", name);
	//sprintf(filename, value);
	
	return;
}

void TrimDevice(char *dest, char *src, int size)
{
	int len;
	while (*src == ' ') src++;
	len = strlen(src);
	while (len > 0 && strchr(" \r\n", src[len-1])) len--;
	if (len >= size) len = size-1;
	strncpy(dest, src, len);
	dest[len] = 0;
}

/****************************************************************************
 * BrowseDevice
 * Displays a list of files on the selected device
 ***************************************************************************/
int BrowseDevice(char * Path)
{
	ResetDeviceBrowser();
	
	ParseDevice(Path);
	ParseDeviceDirectory(navinfo.CurrentDevice);
	
	ParseDevicePath(browserD.dir);
	BrowserDeviceChangeFolder();

	navinfo.firstdevice = false;
	
	return browserD.numEntries;
	
	
}
