/****************************************************************************
 * SaveGame Manager GX
 * Dj_Skual 2010
 *
 * langbrowser.h
 *
 ****************************************************************************/

#ifndef _LANGBROWSER_H_
#define _LANGBROWSER_H_

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
} BROWSERLANGINFO;

typedef struct
{
	unsigned int length; // file length
	char isdir; // 0 - file, 1 - directory
	char filename[MAXJOLIET + 1]; // title full filename
	char displayname[MAXDISPLAY + 1]; // title name for browser display
} BROWSERLANGENTRY;

extern BROWSERLANGINFO browserl;
extern BROWSERLANGENTRY * browserLang;

int UpdateLangDirName(int method);
int LangSortCallback(const void *f1, const void *f2);
void ResetLangBrowser();
int ParseLangDirectory(const char * Path);
int BrowserLangChangeFolder();
int BrowseLangDevice();
void ParseLang(char *Path);
void ParseLangFile(char *Path);
void TrimLang(char *dest, char *src, int size);

#endif
