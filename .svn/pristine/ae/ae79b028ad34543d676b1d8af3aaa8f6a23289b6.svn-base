#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <unistd.h>
#include <stdio.h>

#define UPDATEPATH         	"apps/SaveGame_Manager_GX/"
#define CONFIGPATH          "config/SaveGame_Manager_GX/"
#define CONFIGNAME          "SaveGame_Manager_GX.cfg"
#define LANGPATH      	    "config/SaveGame_Manager_GX/Languages/"
#define SAVEPATH      	    "savegames/"
#define MIIPATH      	    "savemiis/"
#define MUSICPATH      	    "music/"

enum {
	WiiTdbTitles_off=0,
	WiiTdbTitles_on,
	settings_WiiTdbTitles_max
};

enum {
	WiiTdbLanguage_Console=0,
	WiiTdbLanguage_Japanese,
	WiiTdbLanguage_English,
	WiiTdbLanguage_German,
	WiiTdbLanguage_French,
	WiiTdbLanguage_Spanish,
	WiiTdbLanguage_Italian,
	WiiTdbLanguage_Dutch,
	WiiTdbLanguage_Schinese,
	WiiTdbLanguage_Tchinese,
	WiiTdbLanguage_Korean,
	settings_WiiTdbLanguage_max
};

enum {
	WiiTdb_JPtoEN_off=0,
	WiiTdb_JPtoEN_on,
	settings_WiiTdb_JPtoEN_max
};

enum {
	Widescreen_off=0,
	Widescreen_on,
	settings_Widescreen_max
};

enum {
	sd_usb=0,
	isfs,
	mii,
	settings_BootMethod_max
};

enum {
	RumbleOff=0,
	RumbleOn,
	settings_Rumble_max
};

enum {
	TooltipsOff=0,
	TooltipsOn,
	settings_tooltips_max
};
	
enum {
	hr12=0,
	hr24,
	OFF,
	settings_clock_max
};

enum {
	min1=1,
	min3,
	min5,
	min10,
	min20,
	min30,
	min60,
	settings_screensaver_max
};

enum {
    APP_DEFAULT = 0,
    CONSOLE_DEFAULT,
    JAPANESE,
    ENGLISH,
    GERMAN,
    FRENCH,
    SPANISH,
    ITALIAN,
    DUTCH,
    S_CHINESE,
    T_CHINESE,
    KOREAN,
    MAX_LANGUAGE
};

typedef struct
{
	//!Settings variables:
        //!BootMethod sd/usb=0, isfs.
        short BootMethod;
        //!Music Volume.
        short MusicVolume;
        //!Credits music Volume.
        short CreditsVolume;
        //!SFX Volume.
        short SFXVolume;
        //!Clock 12h=0, 24h, Off.
        short Clock;
        //!Tooltips Off=0, on.
        short Tooltips;
        //!InfoWindow Off=0, on.
        short InfoWindow;
        //!Rumble Off=0, on.
        short Rumble;
        //!Screensaver Off=0, 1min, 3mins, 5mins, 10mins, 20mins, 30mins, 60mins.
        short Screensaver;
        //!Widescreen 4/3=0, 16/9.
        short Widescreen;
        //!WiiTdb off=0, on.
        short WiiTdbTitles;
        //!WiiTdb force language: 0 no force, "2" english, "4" french,...
        short WiiTdbLanguage;
		//!WiiTdb force language: "" no force, "EN" english, "FR" french,...
        char  WiiTdbLanguageCode[20];
		//!WiiTdb force japanese titles to english titles off=0, on.
        short WiiTdb_JPtoEN;
		//!Path to where WiiTdb database is located
		char  WiiTdbPath[MAXPATHLEN];
		//!AutoConnect Off=0, on.
        short AutoConnect;
		//!UpdateMeta Off=0, on.
        short UpdateMeta;
        //!UpdateIcon Off=0, on.
        short UpdateIcon;
        //!UpdateWiiTdb Off=0, icon1.png(standard), icon2.png(Black.Pearl).
        short UpdateWiiTdb;
        //!BgMusicLoop once=0, loop, random_bgm, dir_loop.
        short BgMusicLoop;
        //!Languagefolder path
        char  LanguagePath[MAXPATHLEN];
		//!Languagefile path
        char  LanguageFile[50];
        //!Path to the customfont file
        char  CustomFont[MAXPATHLEN];
		//!Path to where SaveGames are located
		char  SaveGamePath[MAXPATHLEN];
		//!Path to where SaveMiis are located
		char  SaveMiisPath[MAXPATHLEN];
		//!Path to where boot.dol are located
		char  UpdatePath[MAXPATHLEN];
		//!Path to where standard custom background musics are located
		char  BgMusicPath[MAXPATHLEN];
} CFG;

extern CFG cfg;


class Settings
{
    public:
		//!Constructor
        Settings();
		//!Destructor
		~Settings();
		//!Set Default Settings
		void SetDefault();
		//!Load Settings
		bool Load();
		//!Save Settings
        bool Save();
		//!Reset Settings
        bool Reset();
		//!Initialise Settings
        bool Init();
		//!Set a Setting
		//!\param name Settingname
		//!\param value Settingvalue
        bool SetSetting(char * name, char * value);
		//!Load a languagefile
		//!\param language
        bool LoadLanguage(const char *path, int language = -1);
		
    protected:
        void ParseLine(char *line);
        void TrimLine(char *dest, char *src, int size);
        FILE * file;
};

#endif
