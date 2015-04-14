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
#ifndef _CSETTINGS_H_
#define _CSETTINGS_H_

#include <string>
#include <stdio.h>
#include <gctypes.h>

#include "SettingsEnums.h"
#include "../FileOperations/FileExtensions.h"
#include "../Controls/AppControls.hpp"

class CSettings
{
	public:
		//!Constructor
		CSettings();
		//!Destructor
		~CSettings();
		//!Set Default Settings
		void SetDefault();
		//!Load Settings
		bool Load();
		//!Save Settings
		bool Save();
		//!Reset Settings
		bool Reset();
		//!Load a languagefile
		//!\param language
		bool LoadLanguage(const char *path, int language = -1);

		/** Navigation Infos **/
		std::string LastUsedPath;
		/*std::string TmpPath;
		short LastBrowser;*/
		
		/** Variables **/
		char BootDevice[10];
		char ConfigPath[80];
		bool updateChecked;
		
		//!BrowserMode.
		short BrowserMode;
		//!Search Mode.
		short SearchMode;
		//!Rumble Off=0, on.
		short Rumble;
		//!Clock Off=0, 12h, 24h.
		short Clock;
		//!Tooltips Off=0, on.
		short Tooltips;
		//!Time before tooltip visible
		int TooltipDelay;
		//!Font scale factor.
		f32 FontScaleFactor;
		//!Clock Font scale factor.
		f32 ClockFontScaleFactor;
		//!Speed factor of wii pointer.
		f32 PointerSpeed;
		//!Scrolling speed
		short ScrollSpeed;
		//!Keyboard delete delay
		short KeyboardDeleteDelay;
		//!SoundblockCount
		short SoundblockCount;
		//!SoundblockSize
		int SoundblockSize;
		//!USB Port to mount
		short USBPort;
		//!Screensaver Off=0, 1min, 3mins, 5mins, 10mins, 20mins, 30mins, 60mins.
		short Screensaver;
		//!HomeMenu type.
		short HomeMenu;
		//!Force use of wii system font.
		short UseSystemFont;
		//!Path to where Themes are located
		char ThemePath[250];
		char Theme[100];
		//!Path to where boot.dol are located
		char UpdatePath[250];
		//!Path to where SaveGames are located
		char SaveGamePath[250];
		
		//!Path to where standard custom background musics are located
		char MusicPath[250];
		//!Music Volume.
		int MusicVolume;
		//!SFX Volume.
		int SFXVolume;
		//!Credits music Volume.
		int CreditsVolume;
		//!BGM Loop Mode
		short BGMLoopMode;
		
		
		//!Parental settings
		short GodMode;
		char unlockCode[20];
		short RememberUnlock;
		u32 ParentalBlocks;
		
		//!GameTDB force language: "" no force, "EN" english, "FR" french,...
		char GameTDBLanguageCode[20];
		
		//!AutoConnect Off=0, on.
		short AutoConnect;
		//!UpdateIcon Off=0, icon.png(standard), icon2.png(Black.Pearl).
		short UpdateIcon;
		//!Update languages Off=0, all, installed.
		short UpdateLanguage;
		//!UpdateGameTDB Off=0, on.
		short UpdateGameTDB;
		//!Wiiload Off=0, on.
		short Wiiload;
		
		/*
		//!BootMethod sd/usb=0, Wii, Mii.
		short BootBrowser;
		//!BrowserMode icon=0, list.
		short BrowserMode;
		//! Keyboard's type
		short keyset;
		//!wsprompt (fix display size) 4/3=no=0, 16/9=yes=1.
		short wsprompt;
		//!Path to where DataBin are located
		char DataBinPath[250];
		//!Path to where SaveMiis are located
		char SaveMiisPath[250];
		//!Path to the Homebrew folder
		char HomebrewPath[250];
		//!Path to where Emulated Nand is located
		char EmuNandPath[250];
		short PlaylogUpdate;
		//!GameTdb off=0, on.
		short GameTDBTitles;
		//!GameTDB force language: 0 no force, "2" english, "4" french,...
		short GameTDBLanguage;
		//!Force use of ios 58
		short UseIOS58;
		//!app loaded under sneek or not
		int Sneek;
		//!Emulated Nand browse
		short EmuNand;
		*/
		//!Languagefile path
		char LanguageFile[250];
		//!Languagefolder path
		char LanguagePath[250];
		//!Path to the customfont file
		char CustomFontPath[250];
		//!Path to where GameTDB database is located
		char GameTDBPath[250];
		//!Path to where the screenshots are written
		char ScreenshotPath[250];
		//!Screenshot image format
		short ScreenshotFormat;
		//!cIOS used by the app.
		short cIOS;
		//!Ios used when app loaded
		u8 EntryIOS;
		
		//!Widescreen Auto detected by the app(display size).
		//!So if Widescreen detected and wspronpt on yes, size are fixed
		short Widescreen;
		bool PAL50;
		//bool NTSC;

		
		
		//!Internal FileExtensions
		CFilesExtensions FileExtensions;
		//!The controls of the App
		AppControls Controls;
		
	protected:
		bool ValidVersion(FILE * file);
		bool SetSetting(char *name, char *value);
		//!Find the config file in the default paths
		bool FindConfig();

		void ParseLine(char *line);
		void TrimLine(char *dest, char *src, int size);
};

extern CSettings Settings;

#define WiiControls		 Settings.Controls.WiiControls
#define ClassicControls	 Settings.Controls.ClassicControls
#define GCControls		  Settings.Controls.GCControls

#endif
