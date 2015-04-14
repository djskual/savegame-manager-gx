/****************************************************************************
 * Copyright (C) 2010
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
 ***************************************************************************/
#include <ogcsys.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CSettings.h"
#include "../DeviceControls/DeviceHandler.hpp"
#include "../Language/gettext.h"
#include "../Themes/CTheme.h"
#include "../FileOperations/fileops.h"
#include "../Tools/encrypt.h"
#include "../Network/svnrev.h"
#include "../ImageOperations/ImageWrite.h"

#define VALID_CONFIG_REV    127

CSettings Settings;
CFilesExtensions FilesExtensions;

CSettings::CSettings()
{
	CONF_Init();
	
	strcpy(BootDevice, "sd:");
	snprintf(ConfigPath, sizeof(ConfigPath), "%s/apps/SaveGame_Manager_GX", BootDevice);
	
	this->SetDefault();
}

CSettings::~CSettings()
{
}

void CSettings::SetDefault()
{
	BrowserMode = ICONBROWSER;
	SearchMode = SEARCH_BEGINNING;
	Rumble = ON;
	Clock = CLOCK_HR24;
	Tooltips = ON;
	TooltipDelay = 2;
	FontScaleFactor = 1.0f;
	ClockFontScaleFactor = 1.0f;  //! Scale of 1 to prevent misaligned clock.
	PointerSpeed = 0.180f;
	ScrollSpeed = 5;
	KeyboardDeleteDelay = 15;
	SoundblockCount = 32;
	SoundblockSize = 8192;
	USBPort = 0;
	cIOS = 249;
	Screensaver = SCREENSAVER_5_MIN;
	HomeMenu = HOME_MENU_DEFAULT;
	UseSystemFont = OFF;
	GodMode = 1;
	strcpy(unlockCode, "");
	RememberUnlock = ON;
	ParentalBlocks = BLOCK_ALL;
	
	snprintf(UpdatePath, sizeof(UpdatePath), "%s/apps/SaveGame_Manager_GX", BootDevice);
	snprintf(CustomFontPath, sizeof(CustomFontPath), "%s/font.ttf", ConfigPath);
	snprintf(SaveGamePath, sizeof(SaveGamePath), "%s/savegames", BootDevice);
	snprintf(GameTDBPath, sizeof(GameTDBPath), "%s", ConfigPath);
	
	snprintf(LanguagePath, sizeof(LanguagePath), "%s/Languages", ConfigPath);
	strcpy(LanguageFile, "");
	
	ScreenshotFormat = IMAGE_PNG;
	snprintf(ScreenshotPath, sizeof(ScreenshotPath), "%s/Screenshots", ConfigPath);
	
	strcpy(GameTDBLanguageCode, "EN");
	
	AutoConnect = OFF;
	UpdateIcon = OFF;
	UpdateLanguage = LANG_UPDATE_OFF;
	UpdateGameTDB = OFF;
	Wiiload = ON;
	
	MusicVolume = 80;
	SFXVolume = 80;
	CreditsVolume = 50;
	BGMLoopMode = LOOP;
	strcpy(MusicPath, "");
	
	snprintf(ThemePath, sizeof(ThemePath), "%s/Themes", ConfigPath);
	strcpy(Theme, "");
	
	LastUsedPath = SaveGamePath; 
	updateChecked = false;
	/*
	TmpPath = ""; 
	
	snprintf(SaveMiisPath, sizeof(SaveMiisPath), "%s/savemiis", BootDevice);
	snprintf(HomebrewPath, sizeof(HomebrewPath), "%s/apps", BootDevice);
	snprintf(EmuNandPath, sizeof(EmuNandPath), "%s/nand", BootDevice);
	
	if(DeviceHandler::Instance()->SD_Inserted())
		snprintf(DataBinPath, sizeof(DataBinPath), "sd:/private/wii/title");
	else
		snprintf(DataBinPath, sizeof(DataBinPath), "%s/private/wii/title", BootDevice);
	
	GameTDBTitles = OFF;
	GameTDBLanguage = 1;
	
	BootBrowser = BROWSE_SDUSB;
	keyset = KEYBOARD_QWERTY;
	
	PlaylogUpdate = ON;
	UseIOS58 = OFF;
	wsprompt = 0;
	emuNand = OFF;*/
	
	Widescreen = (CONF_GetAspectRatio() == CONF_ASPECT_16_9);
	PAL50 = (CONF_GetVideo() == CONF_VIDEO_PAL) && (CONF_GetEuRGB60() == 0);
	
	FilesExtensions.SetDefault();
	Controls.SetDefault();
}

bool CSettings::Load()
{
	FindConfig();
	
	//! Reset default path variables to the right device
	SetDefault();
	
	char filepath[300];
	snprintf(filepath, sizeof(filepath), "%s/SaveGame_Manager_GX.cfg", ConfigPath);
	
	FILE * file = fopen(filepath, "r");
	if (!file) return false;
	
	if(!ValidVersion(file))
	{
		fclose(file);
		Controls.Load(ConfigPath);
		return false;
	}
	
	char line[1024];
	
	while (fgets(line, sizeof(line), file))
	{
		if (line[0] == '#') continue;
		
		this->ParseLine(line);
	}
	fclose(file);
	
	Controls.Load(ConfigPath);
	
	return true;
}

bool CSettings::ValidVersion(FILE * file)
{
	if(!file) return false;

	char line[255];
	int revision = 0;

	while (fgets(line, sizeof(line), file))
	{
		const char * ptr = strcasestr(line, "SaveGame Manager GX R");
		if(ptr)
		{
			ptr += strlen("SaveGame Manager GX R");
			revision = atoi(ptr);
			break;
		}
	}

	rewind(file);

	return revision >= VALID_CONFIG_REV;
}

bool CSettings::Reset()
{
	this->SetDefault();

	if (this->Save()) return true;

	return false;
}

bool CSettings::Save()
{
	if (!FindConfig()) return false;

	char filedest[300];
	snprintf(filedest, sizeof(filedest), "%s/SaveGame_Manager_GX.cfg", ConfigPath);

	if(!CreateSubfolder(ConfigPath))	return false;

	FILE * file = fopen(filedest, "w");
	if (!file) return false;

	fprintf(file, "# SaveGame Manager GX R%s - Main settings file\n", GetRev());
	fprintf(file, "# Note: This file is automatically generated\n\n ");
	
	fprintf(file, "BrowserMode = %d\n ", BrowserMode);
	fprintf(file, "SearchMode = %d\n ", SearchMode);
    fprintf(file, "Rumble = %d\n ", Rumble);
	fprintf(file, "Clock = %d\n ", Clock);
	fprintf(file, "Tooltips = %d\n ", Tooltips);
	fprintf(file, "TooltipDelay = %d\n ", TooltipDelay);
	fprintf(file, "FontScaleFactor = %0.3f\n ", FontScaleFactor);
	fprintf(file, "ClockFontScaleFactor = %0.3f\n ", ClockFontScaleFactor);
	fprintf(file, "PointerSpeed = %0.3f\n ", PointerSpeed);
	fprintf(file, "ScrollSpeed = %d\n ", ScrollSpeed);
	fprintf(file, "KeyboardDeleteDelay = %d\n ", KeyboardDeleteDelay);
	fprintf(file, "SoundblockCount = %d\n ", SoundblockCount);
	fprintf(file, "SoundblockSize = %d\n ", SoundblockSize);
    fprintf(file, "LanguageFile = %s\n ", LanguageFile);
	fprintf(file, "LanguagePath = %s\n ", LanguagePath);
	fprintf(file, "ScreenshotFormat = %d\n ", ScreenshotFormat);
	fprintf(file, "ScreenshotPath = %s\n ", ScreenshotPath);
	fprintf(file, "cIOS = %d\n ", cIOS);
	fprintf(file, "Screensaver = %d\n ", Screensaver);
	fprintf(file, "HomeMenu = %d\n ", HomeMenu);
	fprintf(file, "UseSystemFont = %d\n ", UseSystemFont);
	fprintf(file, "GodMode = %d\n ", GodMode);
	char EncryptedTxt[50];
	EncryptString(unlockCode, EncryptedTxt);
	fprintf(file, "Password = %s\n ", EncryptedTxt);
	fprintf(file, "RememberUnlock = %d\n", RememberUnlock);
	fprintf(file, "ParentalBlocks = %08X\n ", ParentalBlocks);
	
	fprintf(file, "GameTDBPath = %s\n ", GameTDBPath);
	fprintf(file, "GameTDBLanguageCode = %s\n ", GameTDBLanguageCode);
	
	fprintf(file, "MusicPath = %s\n ", MusicPath);
	fprintf(file, "MusicVolume = %d\n ", MusicVolume);
	fprintf(file, "SFXVolume = %d\n ", SFXVolume);
	fprintf(file, "CreditsVolume = %d\n ", CreditsVolume);
	fprintf(file, "BGMLoopMode = %d\n ", BGMLoopMode);
	
	fprintf(file, "UpdatePath = %s\n ", UpdatePath);
	fprintf(file, "CustomFontPath = %s\n ", CustomFontPath);
	fprintf(file, "SaveGamePath = %s\n ", SaveGamePath);
	fprintf(file, "ThemePath = %s\n ", ThemePath);
	fprintf(file, "Theme = %s\n ", Theme);
	
	fprintf(file, "AutoConnect = %d\n ", AutoConnect);
	fprintf(file, "UpdateIcon = %d\n ", UpdateIcon);
	fprintf(file, "UpdateLanguage = %d\n ", UpdateLanguage);
	fprintf(file, "UpdateGameTDB = %d\n ", UpdateGameTDB);
	fprintf(file, "Wiiload = %d\n ", Wiiload);
	
	/*fprintf(file, "BootBrowser = %d\n ", BootBrowser);
	fprintf(file, "keyset = %d\n ", keyset);
	fprintf(file, "wsprompt = %d\n ", wsprompt);
	fprintf(file, "EmuNand = %d\n ", EmuNand);
	
	fprintf(file, "DataBinPath = %s\n ", DataBinPath);
	fprintf(file, "SaveMiisPath = %s\n ", SaveMiisPath);
	fprintf(file, "HomebrewPath = %s\n ", HomebrewPath);
	fprintf(file, "EmuNandPath = %s\n ", EmuNandPath);
	
	fprintf(file, "GameTDBTitles = %d\n ", GameTDBTitles);
	fprintf(file, "GameTDBLanguage = %d\n ", GameTDBLanguage);
	
	
	fprintf(file, "PlaylogUpdate = %d\n ", PlaylogUpdate);
	fprintf(file, "UseIOS58 = %d\n ", UseIOS58);*/
	fprintf(file, "USBPort = %d\n", USBPort);
	fclose(file);

	Controls.Save();

	return true;
}

bool CSettings::SetSetting(char *name, char *value)
{
	int i = 0;

	if (strcmp(name, "ScrollSpeed") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) ScrollSpeed = i;
		return true;
	}
	else if (strcmp(name, "PointerSpeed") == 0)
	{
		PointerSpeed = atof(value);
		return true;
	}
	else if (strcmp(name, "KeyboardDeleteDelay") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) KeyboardDeleteDelay = i;
		return true;
	}
	else if (strcmp(name, "SoundblockCount") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) SoundblockCount = i;
		return true;
	}
	else if (strcmp(name, "SoundblockSize") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) SoundblockSize = i;
		return true;
	}
	else if (strcmp(name, "USBPort") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) USBPort = i;
		return true;
	}
	else if (strcmp(name, "LanguageFile") == 0)
	{
		strcpy(LanguageFile, value);
		return true;
	}
	else if (strcmp(name, "LanguagePath") == 0)
	{
		strcpy(LanguagePath, value);
		return true;
	}
	else if (strcmp(name, "ScreenshotFormat") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) ScreenshotFormat = i;
		return true;
	}
	else if (strcmp(name, "ScreenshotPath") == 0)
	{
		strcpy(ScreenshotPath, value);
		return true;
	}
	else if (strcmp(name, "GameTDBLanguageCode") == 0)
	{
		strcpy(GameTDBLanguageCode, value);
		return true;
	}
	else if (strcmp(name, "MusicPath") == 0)
	{
		strcpy(MusicPath, value);
		return true;
	}
	else if (strcmp(name, "cIOS") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) cIOS = i;
		return true;
	}
	else if (strcmp(name, "Rumble") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) Rumble = i;
		return true;
	}
	else if (strcmp(name, "UpdatePath") == 0)
	{
		strcpy(UpdatePath, value);
		return true;
	}
	else if (strcmp(name, "MusicVolume") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) MusicVolume = i;
		return true;
	}
	else if (strcmp(name, "BGMLoopMode") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) BGMLoopMode = i;
		return true;
	}
	else if (strcmp(name, "CustomFontPath") == 0)
	{
		strcpy(CustomFontPath, value);
		return true;
	}
	else if (strcmp(name, "Screensaver") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) Screensaver = i;
		return true;
	}
	else if (strcmp(name, "SFXVolume") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) SFXVolume = i;
		return true;
	}
	else if (strcmp(name, "CreditsVolume") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) CreditsVolume = i;
		return true;
	}
	else if (strcmp(name, "UseSystemFont") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) UseSystemFont = i;
		return true;
	}
	else if (strcmp(name, "Theme") == 0)
	{
		strcpy(Theme, value);
		return true;
	}
	else if (strcmp(name, "HomeMenu") == 0)
	{
		HomeMenu = atoi(value);
		return true;
	}
	else if (strcmp(name, "SaveGamePath") == 0)
	{
		strcpy(SaveGamePath, value);
		return true;
	}
	if (strcmp(name, "BrowserMode") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) BrowserMode = i;
		return true;
	}
	else if(strcmp(name, "SearchMode") == 0)
	{
		SearchMode = atoi(value);
		return true;
	}
	else if (strcmp(name, "Clock") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) Clock = i;
		return true;
	}
	else if (strcmp(name, "Tooltips") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) Tooltips = i;
		return true;
	}
	else if (strcmp(name, "TooltipDelay") == 0)
	{
		TooltipDelay = atoi(value);
		return true;
	}
	else if (strcmp(name, "FontScaleFactor") == 0)
	{
		FontScaleFactor = atof(value);
		return true;
	}
	else if (strcmp(name, "ClockFontScaleFactor") == 0)
	{
		ClockFontScaleFactor = atof(value);
		return true;
	}
	else if (strcmp(name, "GodMode") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) GodMode = i;
		return true;
	}
	else if (strcmp(name, "RememberUnlock") == 0)
	{
		RememberUnlock = atoi(value);
		return true;
	}
	else if (strcmp(name, "Password") == 0)
	{
		char EncryptedTxt[50];
		strlcpy(EncryptedTxt, value, sizeof(EncryptedTxt));
		DecryptString(EncryptedTxt, unlockCode);
		
		if(!RememberUnlock && strlen(unlockCode) > 0 && strcmp(unlockCode, "not set") != 0)
			GodMode = OFF;
		return true;
	}
	else if (strcmp(name, "ParentalBlocks") == 0)
	{
		ParentalBlocks = strtoul(value, 0, 16);
		return true;
	}
	else if (strcmp(name, "ThemePath") == 0)
	{
		strcpy(ThemePath, value);
		return true;
	}
	else if (strcmp(name, "AutoConnect") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) AutoConnect = i;
		return true;
	}
	else if (strcmp(name, "UpdateIcon") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) UpdateIcon = i;
		return true;
	}
	else if (strcmp(name, "UpdateLanguage") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) UpdateLanguage = i;
		return true;
	}
	else if (strcmp(name, "UpdateGameTDB") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) UpdateGameTDB = i;
		return true;
	}
	else if (strcmp(name, "Wiiload") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) Wiiload = i;
		return true;
	}
	else if (strcmp(name, "GameTDBPath") == 0)
	{
		strcpy(GameTDBPath, value);
		return true;
	}
	/*else if (strcmp(name, "BootBrowser") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) BootBrowser = i;
		return true;
	}
	else if (strcmp(name, "keyset") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) keyset = i;
		return true;
	}
	else if (strcmp(name, "wsprompt") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) wsprompt = i;
		return true;
	}
	else if (strcmp(name, "EmuNand") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) EmuNand = i;
		return true;
	}
	else if (strcmp(name, "GameTDBTitles") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) GameTDBTitles = i;
		return true;
	}
	else if (strcmp(name, "GameTDBLanguage") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) GameTDBLanguage = i;
		return true;
	}
	else if (strcmp(name, "PlaylogUpdate") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) PlaylogUpdate = i;
		return true;
	}
	else if (strcmp(name, "UseIOS58") == 0)
	{
		if (sscanf(value, "%d", &i) == 1) UseIOS58 = i;
		return true;
	}
	else if (strcmp(name, "DataBinPath") == 0)
	{
		strcpy(DataBinPath, value);
		return true;
	}
	else if (strcmp(name, "SaveMiisPath") == 0)
	{
		strcpy(SaveMiisPath, value);
		return true;
	}
	else if (strcmp(name, "HomebrewPath") == 0)
	{
		strcpy(HomebrewPath, value);
		return true;
	}
	else if (strcmp(name, "EmuNandPath") == 0)
	{
		strcpy(EmuNandPath, value);
		return true;
	}*/
	
	return false;
}

bool CSettings::FindConfig()
{
	bool found = false;
	char CheckDevice[10];
	char CheckPath[300];

	for (int i = SD; i < MAXDEVICES; ++i)
	{
		sprintf(CheckDevice, "%s:", DeviceName[i]);
		
		if(!found)
		{
			strcpy(BootDevice, CheckDevice);
			snprintf(ConfigPath, sizeof(ConfigPath), "%s/apps/SaveGame_Manager_GX", BootDevice);
			snprintf(CheckPath, sizeof(CheckPath), "%s/SaveGame_Manager_GX.cfg", ConfigPath);
			found = CheckFile(CheckPath);
		}
		if(!found)
		{
			strcpy(BootDevice, CheckDevice);
			snprintf(ConfigPath, sizeof(ConfigPath), "%s/apps/savegame_gx", BootDevice);
			snprintf(CheckPath, sizeof(CheckPath), "%s/SaveGame_Manager_GX.cfg", ConfigPath);
			found = CheckFile(CheckPath);
		}
	}
	
	if (!found)
	{
		FILE * testFp = NULL;
		//! No existing config so try to find a place where we can write it too
		for (int i = SD; i < MAXDEVICES; ++i)
		{
			sprintf(CheckDevice, "%s:", DeviceName[i]);
			
			if (!found)
			{
				strcpy(BootDevice, CheckDevice);
				snprintf(ConfigPath, sizeof(ConfigPath), "%s/apps/SaveGame_Manager_GX", BootDevice);
				snprintf(CheckPath, sizeof(CheckPath), "%s/boot.dol", ConfigPath);
				if(CheckFile(CheckPath))
				{
					snprintf(CheckPath, sizeof(CheckPath), "%s/SaveGame_Manager_GX.cfg", ConfigPath);
					CreateSubfolder(ConfigPath);
					testFp = fopen(CheckPath, "wb");
					found = (testFp != NULL);
					fclose(testFp);
				}
			}
			if (!found)
			{
				strcpy(BootDevice, CheckDevice);
				snprintf(ConfigPath, sizeof(ConfigPath), "%s/apps/savegame_gx", BootDevice);
				snprintf(CheckPath, sizeof(CheckPath), "%s/boot.dol", ConfigPath);
				if(CheckFile(CheckPath))
				{
					snprintf(CheckPath, sizeof(CheckPath), "%s/SaveGame_Manager_GX.cfg", ConfigPath);
					CreateSubfolder(ConfigPath);
					testFp = fopen(CheckPath, "wb");
					found = (testFp != NULL);
					fclose(testFp);
				}
			}
		}
	}
	
	return found;
}

void CSettings::ParseLine(char *line)
{
	char temp[1024], name[1024], value[1024];

	snprintf(temp, sizeof(temp), line);

	char * eq = strchr(temp, '=');

	if (!eq) return;

	*eq = 0;

	this->TrimLine(name, temp, sizeof(name));
	this->TrimLine(value, eq + 1, sizeof(value));

	this->SetSetting(name, value);
}

void CSettings::TrimLine(char *dest, char *src, int size)
{
	int len;
	while (*src == ' ')
		src++;
	len = strlen(src);
	while (len > 0 && strchr(" \r\n", src[len - 1]))
		len--;
	if (len >= size) len = size - 1;
	strncpy(dest, src, len);
	dest[len] = 0;
}

//! Get language code from the selected language file
//! eg. german.lang = DE and default to EN
static inline const char * GetLangCode(const char * langpath)
{
	if(strcasestr(langpath, "japanese"))
		return "JA";

	else if(strcasestr(langpath, "german"))
		return "DE";

	else if(strcasestr(langpath, "french"))
		return "FR";

	else if(strcasestr(langpath, "spanish"))
		return "ES";

	else if(strcasestr(langpath, "italian"))
		return "IT";

	else if(strcasestr(langpath, "dutch"))
		return "NL";

	else if(strcasestr(langpath, "schinese"))
		return "ZHCN";

	else if(strcasestr(langpath, "tchinese"))
		return "ZHTW";

	else if(strcasestr(langpath, "korean"))
		return "KO";

	return "EN";
}

bool CSettings::LoadLanguage(const char *path, int lang)
{
	bool ret = false;

	if (path && strlen(path) > 3)
	{
		ret = gettextLoadLanguage(path);
		if (ret)
		{
			snprintf(LanguageFile, sizeof(LanguageFile), path);
			strcpy(GameTDBLanguageCode, GetLangCode(LanguageFile));
			
			std::string langpath = path;
			langpath = langpath.substr(0, langpath.find_last_of("/"));
			snprintf(LanguagePath, sizeof(LanguagePath), langpath.c_str());
		}
		else
			return LoadLanguage(NULL, CONSOLE_DEFAULT);
	}
	else if (lang >= 0)
	{
		std::string langfile;
		std::string langpath = LanguagePath;
		
		if (lang == CONSOLE_DEFAULT)
			return LoadLanguage(NULL, CONF_GetLanguage());
		
		else if (lang == JAPANESE)	langfile = langpath + "/japanese.lang";
		else if (lang == ENGLISH)	langfile = langpath + "/english.lang";
		else if (lang == GERMAN)	langfile = langpath + "/german.lang";
		else if (lang == FRENCH)	langfile = langpath + "/french.lang";
		else if (lang == SPANISH)	langfile = langpath + "/spanish.lang";
		else if (lang == ITALIAN)	langfile = langpath + "/italian.lang";
		else if (lang == DUTCH)		langfile = langpath + "/dutch.lang";
		else if (lang == S_CHINESE)	langfile = langpath + "/schinese.lang";
		else if (lang == T_CHINESE)	langfile = langpath + "/tchinese.lang";
		else if (lang == KOREAN)	langfile = langpath + "/korean.lang";
		
		strcpy(GameTDBLanguageCode, GetLangCode(langfile.c_str()));
		ret = gettextLoadLanguage(langfile.c_str());
		if (ret)
		{
			snprintf(LanguageFile, sizeof(LanguageFile), langfile.c_str());
			snprintf(LanguagePath, sizeof(LanguagePath), langpath.c_str());
		}
	}
	
	return ret;
}
