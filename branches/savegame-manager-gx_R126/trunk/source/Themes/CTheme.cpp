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

#include "CTheme.h"
#include "../Tools/StringTools.h"
#include "../Settings/CSettings.h"
#include "../FileOperations/Nand.hpp"
#include "../FileOperations/fileops.h"
#include "../Text/FreeTypeGX.h"
#include "../Tools/gecko.h"
#include "../Menu/menus.h"

typedef struct map_entry
{
	char name[8];
	u8 hash[20];
} __attribute__((packed)) map_entry_t;

typedef struct _U8Header
{
	u32 fcc;
	u32 rootNodeOffset;
	u32 headerSize;
	u32 dataOffset;
	u8 zeroes[16];
} __attribute__((packed)) U8Header;

typedef struct _U8Entry
{
	struct
	{
		u32 fileType :8;
		u32 nameOffset :24;
	};
	u32 fileOffset;
	union
	{
		u32 fileLength;
		u32 numEntries;
	};
} __attribute__( ( packed ) ) U8Entry;


static inline const char * u8Filename(const U8Entry *fst, int i)
{
	return (char *) (fst + fst[0].numEntries) + fst[i].nameOffset;
}

static const u8 WIIFONT_HASH[]		= {0x32, 0xb3, 0x39, 0xcb, 0xbb, 0x50, 0x7d, 0x50, 0x27, 0x79, 0x25, 0x9a, 0x78, 0x66, 0x99, 0x5d, 0x03, 0x0b, 0x1d, 0x88};
static const u8 WIIFONT_HASH_KOR[]	= {0xb7, 0x15, 0x6d, 0xf0, 0xf4, 0xae, 0x07, 0x8f, 0xd1, 0x53, 0x58, 0x3e, 0x93, 0x6e, 0x07, 0xc0, 0x98, 0x77, 0x49, 0x0e};

FreeTypeGX * fontSystem = NULL;
static FT_Byte * systemFont = NULL;
static u32 systemFontSize = 0;
static FT_Byte * customFont = NULL;
static u32 customFontSize = 0; 

bool Theme::ShowTooltips = true;

void Theme::Reload()
{
	mainWindow->Remove(bgImg);
	delete bgImg;
	delete background;
	delete btnSoundClick;
	delete btnSoundOver;
	
	for(int i = 0; i < 4; ++i)
	{
		char image[50];
		snprintf(image, sizeof(image), "player%i_point.png", i+1);
		pointer[i]->SetImage(image);
	}
	
	btnSoundClick = new GuiSound(Resources::GetFile("button_click.wav"), Resources::GetFileSize("button_click.wav"), Settings.SFXVolume);
	btnSoundOver = new GuiSound(Resources::GetFile("button_over.wav"), Resources::GetFileSize("button_over.wav"), Settings.SFXVolume);
	background = Resources::GetImageData(Settings.Widescreen ? "menu_wbackground.png" : "menu_background.png");
	bgImg = new GuiImage(background);
	mainWindow->Append(bgImg);
}

void Theme::CleanUp()
{
	ThemeCleanUp();
	Resources::Clear();
	Theme::ClearFontData();
}

void Theme::SetDefault()
{
	ShowTooltips = true;
	Theme::CleanUp();
	strcpy(Settings.Theme, "");
	Theme::LoadFont(Settings.CustomFontPath);
}

bool Theme::Load(const char * theme_file_path)
{
	bool result = LoadTheme(theme_file_path);
	if(!result)
		return result;
	
	Theme::ShowTooltips = (thInt("1 - Enable tooltips: 0 for off and 1 for on") != 0);

	FILE * file = fopen(theme_file_path, "rb");
	if(!file)
		return false;

	char line[300];
	char * Foldername = NULL;

	while (fgets(line, sizeof(line), file))
	{
		char * ptr = strcasestr(line, "Image-Folder:");
		if(!ptr)
			continue;

		ptr += strlen("Image-Folder:");

		while(*ptr != '\0' && *ptr == ' ') ptr++;

		Foldername = ptr;

		while(*ptr != '\\' && *ptr != '"' && *ptr != '\0') ptr++;

		*ptr = '\0';
		break;
	}

	fclose(file);

	if(!Foldername)
		return result;

	char theme_path[300];
	snprintf(theme_path, sizeof(theme_path), theme_file_path);

	char * ptr = strrchr(theme_path, '/');
	if(ptr) *ptr = '\0';

	strcat(theme_path, fmt("/%s", Foldername));
	
	if(!Resources::LoadFiles(theme_path))
	{
		const char * ThemeFilename = strrchr(theme_file_path, '/')+1;
		char Filename[255];
		snprintf(Filename, sizeof(Filename), ThemeFilename);

		char * fileext = strrchr(Filename, '.');
		if(fileext) *fileext = 0;

		char * ptr = strrchr(theme_path, '/');
		*ptr = 0;
		
		strcat(theme_path, fmt("/%s", Filename));
		Resources::LoadFiles(theme_path);
	}

	//! Override font.ttf with the theme font.ttf if it exists in the image folder
	strcat(theme_path, "/font.ttf");
	
	if(CheckFile(theme_path))
		Theme::LoadFont(theme_path);

	return result;
}

bool Theme::loadSystemFont(bool korean)
{
	const char contentMapPath[] ATTRIBUTE_ALIGN(32) = "/shared1/content.map";
	u8 *contentMap = NULL;
	u32 mapsize = 0;

	Nand::LoadFile(contentMapPath, &contentMap, &mapsize);
	if(!contentMap)
		return false;

	int fileCount = mapsize / sizeof(map_entry_t);

	map_entry_t *mapEntryList = (map_entry_t *) contentMap;

	for (int i = 0; i < fileCount; i++)
	{
		if (memcmp(mapEntryList[i].hash, korean ? WIIFONT_HASH_KOR : WIIFONT_HASH, 20) != 0)
			continue;

		//! Name found, load it and unpack it
		char font_filename[32] ATTRIBUTE_ALIGN(32);
		snprintf(font_filename, sizeof(font_filename), "/shared1/%.8s.app", mapEntryList[i].name);

		u8 *fontArchive = NULL;
		u32 filesize = 0;

		Nand::LoadFile(font_filename, &fontArchive, &filesize);
		if(!fontArchive)
			continue;

		const U8Header *u8Archive = (U8Header *) fontArchive;
		const U8Entry *fst = (const U8Entry *) (((const u8 *) u8Archive) + u8Archive->rootNodeOffset);

		if(fst[0].numEntries < 1)
		{
			free(fontArchive);
			continue;
		}

		if(systemFont)
			delete [] systemFont;

		systemFontSize = fst[1].fileLength;
		systemFont = new (std::nothrow) FT_Byte[systemFontSize];
		if(!systemFont)
		{
			free(fontArchive);
			continue;
		}

		memcpy(systemFont, fontArchive + fst[1].fileOffset, systemFontSize);

		free(fontArchive);
		free(contentMap);
		gprintf("Loaded Wii System Font: %s\n", u8Filename(fst, 1));
		return true;
	}

	free(contentMap);

	return false;
}

bool Theme::LoadFont(const char *path)
{
	bool result = false;
	FILE *pfile = NULL;

	delete [] customFont;
	customFont = NULL; 
	
	pfile = fopen(path, "rb");

	if (pfile)
	{
		fseek(pfile, 0, SEEK_END);
		customFontSize = ftell(pfile);
		rewind(pfile);

		customFont = new (std::nothrow) FT_Byte[customFontSize];
		if (customFont)
		{
			fread(customFont, 1, customFontSize, pfile);
			result = true;
		}
		fclose(pfile);
	}

	FT_Byte * loadedFont = customFont;
	u32 loadedFontSize = customFontSize;
	
	if(!loadedFont && Settings.UseSystemFont)
	{
		if(!systemFont)
			loadSystemFont(CONF_GetLanguage() == CONF_LANG_KOREAN);
		if(!systemFont)
			loadSystemFont(CONF_GetLanguage() != CONF_LANG_KOREAN);
		
		//! Default to system font if no custom is loaded
		loadedFont = systemFont;
		loadedFontSize = systemFontSize;
	} 
	if(!loadedFont)
	{
		loadedFont = (FT_Byte *) Resources::GetFile("font.ttf");
		loadedFontSize = Resources::GetFileSize("font.ttf");
	}
	
	delete fontSystem;
	
	fontSystem = new FreeTypeGX(loadedFont, loadedFontSize, loadedFont == systemFont);

	return result;
}

void Theme::ClearFontData()
{
	if (fontSystem)
		delete fontSystem;
	fontSystem = NULL;

	if (customFont)
		delete [] customFont;
	customFont = NULL;
	
	if (systemFont)
		delete [] systemFont;
	systemFont = NULL;
}
