/****************************************************************************
 *
 * menu_tools.h
 *
 ***************************************************************************/

#ifndef _MENU_TOOLS_H_
#define _MENU_TOOLS_H_

#define CFG_LANG_JAPANESE	0
#define CFG_LANG_ENGLISH	1
#define CFG_LANG_GERMAN		2
#define CFG_LANG_FRENCH		3
#define CFG_LANG_SPANISH	4
#define CFG_LANG_ITALIAN	5
#define CFG_LANG_DUTCH		6
#define CFG_LANG_S_CHINESE	7
#define CFG_LANG_T_CHINESE	8
#define CFG_LANG_KOREAN		9

// typedef returning info about current navigation state
typedef struct
{
	bool firstboot;//used at start up
	bool firstdevice;//used when load a path in browser
	bool switchdevice;//used when switch between devices
	bool refresh;//used when refresh brwser after extract or other operation
	char CurrentDevice [300];//root of current browsed device
	char LastBrowsedSdPath [300];//last path browsed on sd before switsh on other device
	char LastBrowsedUsbPath [300];//last path browsed on usb before switsh on other device
	char LastBrowsedPath [300];//last path browsed on device before needed for something
	int LastWiiSaveSelected;//last save selected in WiiSave list
	int LastWiiSaveFirstItem;//last first save displayed in WiiSave list
	int LastDeviceItemSelected;//last item selected in Device browser
	int LastDeviceFirstItem;//last first item displayed in Device browser
	int LastMiiSelected;//last save selected in WiiSave list
	int LastMiiFirstItem;//last first save displayed in WiiSave list
	
} NAVINFO;

void ResetNavInfo();
int InitApp();
int GetPathToBrowse();

void GetLanguageToLangCode(short langid, char *langcode);
void GetLanguageOptionDisplayname(short langid, char *langname);
void WiiTDBLanguageReload();
void LoadTitlesFromWiiTDB(const char * path, short source);

#endif
