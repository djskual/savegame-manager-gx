/****************************************************************************
 * 
 * menu_tools.cpp
 *
 * Menu flow routines - handles all menu logic
 ***************************************************************************/

#include <gccore.h>
#include <ogcsys.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "libwiigui/gui.h"
#include "menu/menu.h"
#include "main.h"
#include "saves/WiiTDB.hpp"
#include "saves/wii_list.h"
#include "prompts/browsers/browsers.h"
#include "prompts/promptwindows.h"
#include "prompts/ProgressWindow.h"
#include "mii/mii_list.h"

FreeTypeGX *fontSystem=0;
FreeTypeGX *fontClock=0;

void ResetNavInfo()
{
	navinfo.firstboot = true;
	navinfo.firstdevice = true;
	navinfo.switchdevice = false;
	navinfo.refresh = false;
	navinfo.LastWiiSaveSelected = 0;
	navinfo.LastWiiSaveFirstItem = 0;
	navinfo.LastDeviceItemSelected = 0;
	navinfo.LastDeviceFirstItem = 0;
	navinfo.LastMiiSelected = 0;
	navinfo.LastMiiFirstItem = 0;
}

int InitApp()
{
	Settings Settings;
	
	SDCard_Init();
	USBDevice_Init();
	ISFS_Initialize();
	
	int result;
	
	if (Settings.Init())
	{
		if(strcmp(cfg.LanguageFile, "NotSet") != 0) {
			char path [300];
			snprintf(path, sizeof(path), "%s%s", cfg.LanguagePath, cfg.LanguageFile);
			Settings.LoadLanguage(path);
		}
		else
			Settings.LoadLanguage(NULL, CONSOLE_DEFAULT);
		
		GetLanguageToLangCode(cfg.WiiTdbLanguage, cfg.WiiTdbLanguageCode);
		
		result = 1;
	}
	else
		result = 0;
		
	fontSystem = new FreeTypeGX();
    fontSystem->loadFont(cfg.CustomFont, font_ttf, font_ttf_size, 0);
    fontSystem->setCompatibilityMode(FTGX_COMPATIBILITY_DEFAULT_TEVOP_GX_PASSCLR | FTGX_COMPATIBILITY_DEFAULT_VTXDESC_GX_NONE);

    fontClock = new FreeTypeGX();
    fontClock->loadFont(NULL, clock_ttf, clock_ttf_size, 0);
    fontClock->setCompatibilityMode(FTGX_COMPATIBILITY_DEFAULT_TEVOP_GX_PASSCLR | FTGX_COMPATIBILITY_DEFAULT_VTXDESC_GX_NONE);
	
	//InitGUIThreads(); // Initialize GUI
	
	return result;
}

int GetPathToBrowse()
{
	Settings Settings;

	if (navinfo.firstboot == true) {
		if (cfg.BootMethod == 0) {
			device.BrowsedDevice = device.ConfigDevice;
			device.LastBrowsedDevice = device.BrowsedDevice;
		}
		else if (cfg.BootMethod == 1) {
			device.BrowsedDevice = 3;
			device.LastBrowsedDevice = device.ConfigDevice;
		}
		else {
			device.BrowsedDevice = 4;
			device.LastBrowsedDevice = device.ConfigDevice;
		}
		
		// populate initial directory listing
		if (device.BrowsedDevice == 4)
			snprintf(navinfo.CurrentDevice, sizeof(navinfo.CurrentDevice), cfg.SaveMiisPath);
		if (device.BrowsedDevice == 3)
			snprintf(navinfo.CurrentDevice, sizeof(navinfo.CurrentDevice), cfg.SaveGamePath);
		if (device.BrowsedDevice == 2)
			snprintf(navinfo.CurrentDevice, sizeof(navinfo.CurrentDevice), "usb:/");
		if (device.BrowsedDevice == 1)
			snprintf(navinfo.CurrentDevice, sizeof(navinfo.CurrentDevice), "sd:/");
		if (device.BrowsedDevice == device.ConfigDevice)
			snprintf(navinfo.CurrentDevice, sizeof(navinfo.CurrentDevice), cfg.SaveGamePath);
	
		if(BrowseDevice(navinfo.CurrentDevice) <= 0)
		{
			int choice = WindowPrompt(
			tr("Error"),
			tr("Unable to find device."),
			tr("Retry"),
			tr("Exit"));

			if(choice == 1)
			{
				SDCard_Init();
				USBDevice_Init();
				ISFS_Initialize();
				
				if (Settings.Init())
				{
					if(strcmp(cfg.LanguageFile, "NotSet") != 0) {
						char path [300];
						snprintf(path, sizeof(path), "%s%s", cfg.LanguagePath, cfg.LanguageFile);
						Settings.LoadLanguage(path);
					}
					else
						Settings.LoadLanguage(NULL, CONSOLE_DEFAULT);
				}
				return 1;//retry to load
			}
			else
				return -1;//exit app
		}
		
		
		if (device.BrowsedDevice == 4) {
			snprintf(navinfo.LastBrowsedSdPath, sizeof(navinfo.LastBrowsedSdPath), cfg.SaveMiisPath);
			snprintf(navinfo.LastBrowsedUsbPath, sizeof(navinfo.LastBrowsedUsbPath), cfg.SaveMiisPath);
		}
		if (device.BrowsedDevice == 3) {
			snprintf(navinfo.LastBrowsedSdPath, sizeof(navinfo.LastBrowsedSdPath), cfg.SaveGamePath);
			snprintf(navinfo.LastBrowsedUsbPath, sizeof(navinfo.LastBrowsedUsbPath), cfg.SaveGamePath);
		}
		if (device.BrowsedDevice == 2) {
			snprintf(navinfo.CurrentDevice, sizeof(navinfo.CurrentDevice), "usb:/");
			snprintf(navinfo.LastBrowsedSdPath, sizeof(navinfo.LastBrowsedSdPath), "sd:/");
			snprintf(navinfo.LastBrowsedUsbPath, sizeof(navinfo.LastBrowsedUsbPath), cfg.SaveGamePath);
		}
		if (device.BrowsedDevice == 1) {
			snprintf(navinfo.CurrentDevice, sizeof(navinfo.CurrentDevice), "sd:/");
			snprintf(navinfo.LastBrowsedSdPath, sizeof(navinfo.LastBrowsedSdPath), cfg.SaveGamePath);
			snprintf(navinfo.LastBrowsedUsbPath, sizeof(navinfo.LastBrowsedUsbPath), "usb:/");
		}
		
		navinfo.firstboot = false;
		return 0;//succes
	}
	else if(navinfo.switchdevice == true)
	{
		if (device.BrowsedDevice == 2)
			snprintf(navinfo.CurrentDevice, sizeof(navinfo.CurrentDevice), "usb:/");
		if (device.BrowsedDevice == 1)
			snprintf(navinfo.CurrentDevice, sizeof(navinfo.CurrentDevice), "sd:/");
		
		if (device.BrowsedDevice == 1)
			BrowseDevice(navinfo.LastBrowsedSdPath);
		if (device.BrowsedDevice == 2)
			BrowseDevice(navinfo.LastBrowsedUsbPath);
		
		navinfo.switchdevice = false;
		return 0;//succes
	}
	else if(navinfo.refresh == true)
	{
		navinfo.firstdevice = true;
		BrowseDevice(navinfo.LastBrowsedPath);
		CreateMiiList();
		navinfo.refresh = false;
		return 0;//succes
	}
	else
		return 0;//no special request
}

void GetLanguageToLangCode(short langid, char *langcode) {
// map language id (or Wii language settting if langid is set to -1) to language code. CONF_LANG_JAPANESE = 0, not 1

	if (langid < 0)
		langid = CONF_GetLanguage();

    switch (langid) {
    case CONF_LANG_JAPANESE:
        sprintf(langcode, "JA");
        break;
    case CONF_LANG_ENGLISH:
        sprintf(langcode, "EN");
        break;
    case CONF_LANG_GERMAN:
        sprintf(langcode, "DE");
        break;
    case CONF_LANG_FRENCH:
        sprintf(langcode, "FR");
        break;
    case CONF_LANG_SPANISH:
        sprintf(langcode, "ES");
        break;
    case CONF_LANG_ITALIAN:
        sprintf(langcode, "IT");
        break;
    case CONF_LANG_DUTCH:
        sprintf(langcode, "NL");
        break;
    case CONF_LANG_SIMP_CHINESE:
        sprintf(langcode, "ZHCN"); // People's Republic of China
        break;
    case CONF_LANG_TRAD_CHINESE:
        sprintf(langcode, "ZHTW"); // Taiwan
        break;
    case CONF_LANG_KOREAN:
        sprintf(langcode, "KO");
        break;
    }
	
	cfg.WiiTdbLanguage = langid;
}

void GetLanguageOptionDisplayname(short langid, char *langname) {

	switch (langid) {
    case CONF_LANG_JAPANESE:
        sprintf(langname, tr("Japanese"));
        break;
    case CONF_LANG_ENGLISH:
        sprintf(langname, tr("English"));
        break;
    case CONF_LANG_GERMAN:
        sprintf(langname, tr("German"));
        break;
    case CONF_LANG_FRENCH:
        sprintf(langname, tr("French"));
        break;
    case CONF_LANG_SPANISH:
        sprintf(langname, tr("Spanish"));
        break;
    case CONF_LANG_ITALIAN:
        sprintf(langname, tr("Italian"));
        break;
    case CONF_LANG_DUTCH:
        sprintf(langname, tr("Dutch"));
        break;
    case CONF_LANG_SIMP_CHINESE:
        sprintf(langname, tr("Simple Chinese")); // People's Republic of China
        break;
    case CONF_LANG_TRAD_CHINESE:
        sprintf(langname, tr("Trad Chinese")); // Taiwan
        break;
    case CONF_LANG_KOREAN:
        sprintf(langname, tr("Korean"));
        break;
    }
}

void WiiTDBLanguageReload()
{
	if(!cfg.WiiTdbTitles)
        return;

    if(!strcmp(cfg.LanguageFile, "asturian.lang"))
		cfg.WiiTdbLanguage = 4;
    else if(!strcmp(cfg.LanguageFile, "danish.lang"))
		cfg.WiiTdbLanguage = 1;
    else if(!strcmp(cfg.LanguageFile, "dutch.lang"))
		cfg.WiiTdbLanguage = 6;
    else if(!strcmp(cfg.LanguageFile, "english.lang"))
		cfg.WiiTdbLanguage = 1;
    else if(!strcmp(cfg.LanguageFile, "finnish.lang"))
		cfg.WiiTdbLanguage = 1;
    else if(!strcmp(cfg.LanguageFile, "french.lang"))
		cfg.WiiTdbLanguage = 3;
    else if(!strcmp(cfg.LanguageFile, "german.lang"))
		cfg.WiiTdbLanguage = 2;
    else if(!strcmp(cfg.LanguageFile, "italian.lang"))
		cfg.WiiTdbLanguage = 5;
    else if(!strcmp(cfg.LanguageFile, "japanese.lang"))
		cfg.WiiTdbLanguage = 0;
    else if(!strcmp(cfg.LanguageFile, "schinese.lang"))
		cfg.WiiTdbLanguage = 7;
    else if(!strcmp(cfg.LanguageFile, "spanish.lang"))
		cfg.WiiTdbLanguage = 4;
    else if(!strcmp(cfg.LanguageFile, "tchinese.lang"))
		cfg.WiiTdbLanguage = 8;
    
	GetLanguageToLangCode(cfg.WiiTdbLanguage, cfg.WiiTdbLanguageCode);
	
	Settings Settings;
	Settings.Save();
}

void LoadTitlesFromWiiTDB(const char * path, short source)
{
    if(!path || !cfg.WiiTdbTitles)
        return;

    std::string Title;
    std::string Filepath = path;
    if(path[strlen(path)-1] != '/')
        Filepath += '/';
    Filepath += "wiitdb.xml";

    WiiTDB XML_DB(Filepath.c_str());
	XML_DB.SetLanguageCode(cfg.WiiTdbLanguageCode);

    if(source == WiiTDB::WII)
	{
		for(int i = 0; i < wiiSaveInfo.Cnt; ++i)
		{
			if(XML_DB.GetTitle((const char *) wiiSaveList[i].ID, Title))
				strcpy(wiiSaveList[i].name, Title.c_str());
		}
	}
	else
	{
		for(int i = 0; i < browserD.numEntries; i++)
		{
			if(browserDevice[i].isSave == 1)
			{
				if(XML_DB.GetTitle((const char *) browserDevice[i].ID, Title))
					strcpy(browserDevice[i].displayname, Title.c_str());
			}
		}
	}
}
