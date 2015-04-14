/****************************************************************************
 * SaveGame Manager GX
 * Dj_Skual 2010
 *
 * customFilePathBrowser.h
 *
 ****************************************************************************/

#ifndef _CUSTOMFILEPATHBROWSER_H_
#define _CUSTOMFILEPATHBROWSER_H_

#include <unistd.h>
#include <gccore.h>

#define MAXJOLIET 255
#define MAXDISPLAY 64

typedef struct
{
	char dir[MAXPATHLEN]; // directory path of customList
	int numEntries; // # of entries in customList
	int selIndex; // currently selected index of customList
	int pageIndex; // starting index of customList page display
} BROWSERCUSTOMFILEPATHINFO;

typedef struct
{
	unsigned int length; // file length
	char isdir; // 0 - file, 1 - directory
	char filename[MAXJOLIET + 1]; // title full filename
	char displayname[MAXDISPLAY + 1]; // title name for browser display
} BROWSERCUSTOMFILEPATHENTRY;

extern BROWSERCUSTOMFILEPATHINFO browserCFP;
extern BROWSERCUSTOMFILEPATHENTRY * browserCustomFilePath;

int UpdateCustomFileDirName(int method);
int CustomFileSortCallback(const void *f1, const void *f2);
void ResetCustomFileBrowser();
int ParseCustomFileDirectory(const char * Path);
int FileCustomChangeFolder();
int BrowseCustomFile(char * Path, char * customPath);
void ParseCustomFile(char *Path);
void ParseCFP(char *Path);
void TrimCustomFile(char *dest, char *src, int size);

#endif
