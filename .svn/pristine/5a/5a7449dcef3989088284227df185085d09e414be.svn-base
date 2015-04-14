/****************************************************************************
 * libwiigui Template
 * Tantric 2009
 *
 * devicebrowser.h
 *
 * Generic file routines - reading, writing, browsing
 ****************************************************************************/

#ifndef _DEVICEBROWSER_H_
#define _DEVICEBROWSER_H_

#include <unistd.h>
#include <gccore.h>

#define MAXJOLIET 255
#define MAXDISPLAY 64

typedef struct
{
	char dir[MAXPATHLEN]; // directory path of browserList
	int numEntries; // # of entries in browserList
	int selIndex; // currently selected index of browserList
	int pageIndex; // starting index of browserList page display
} BROWSERDEVICEINFO;

typedef struct
{
	u64 offset; // DVD offset
	unsigned int length; // file length
	float blockSize; // size of the save
	u64 tid;//titleid of the save
	char isdir; // 0 - file, 1 - directory
	char isSave; // 0 - file, 1 - save
	char isNotInstalled; // 0 - installed, 1 - not installed
	char isNotSupported; // 0 - supported, 1 - not supported
	char isLog; // 0 - file, 1 - changeLog file
	char isMii; // 0 - file, 1 - Mii file
	char filename[MAXJOLIET + 1]; // title full filename
	char displayname[MAXDISPLAY + 1]; // title name for browser display
	char subname[MAXDISPLAY + 1]; // title subname
	char ID[5]; // title ID
	char miiCreator[31]; // mii's creator name
	int miiDay; // day of mii's birthday
	int miiMonth; // month of mii's birthday
	int miiFemale; // 0 - male, 1 - female
	int miiFavColor; // favorite mii color
	
} BROWSERDEVICEENTRY;

extern BROWSERDEVICEINFO browserD;
extern BROWSERDEVICEENTRY * browserDevice;

int UpdateDeviceDirName(int method);
int DeviceSortCallback(const void *f1, const void *f2);
void ResetDeviceBrowser();
int ParseDeviceDirectory(const char * Path);
int BrowserDeviceChangeFolder();
int BrowseDevice(char * Path);
void ParseDevice(char *Path);
void ParseDevicePath(char *Path);
void TrimDevice(char *dest, char *src, int size);

#endif
