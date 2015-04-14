/****************************************************************************
 * SaveGame Manager GX
 * Dj_Skual 2010
 *
 * customPathBrowser.h
 *
 ****************************************************************************/

#ifndef _CUSTOMPATHBROWSER_H_
#define _CUSTOMPATHBROWSER_H_

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
} BROWSERCUSTOMPATHINFO;

typedef struct
{
	unsigned int length; // file length
	char isdir; // 0 - file, 1 - directory
	char filename[MAXJOLIET + 1]; // title full filename
	char displayname[MAXDISPLAY + 1]; // title name for browser display
} BROWSERCUSTOMPATHENTRY;

extern BROWSERCUSTOMPATHINFO browserCP;
extern BROWSERCUSTOMPATHENTRY * browserCustomPath;

int UpdateCustomDirName(int method);
int CustomSortCallback(const void *f1, const void *f2);
void ResetCustomBrowser();
int ParseCustomDirectory(const char * Path);
int BrowserCustomChangeFolder();
int BrowseCustom(char * Path, char * customPath);
void ParseCustom(char *Path);
void ParseCustomPath(char *Path);
void TrimCustom(char *dest, char *src, int size);

#endif
