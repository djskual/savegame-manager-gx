/****************************************************************************
 * Copyright (C) 2010
 * by Dimok
 * Modified by Dj_Skual
 * for SaveGame Manager GX
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
#include <stdio.h>
#include <ogcsys.h>
#include <stdlib.h>
#include <string>
#include <cstring>

#include "GameTDB.hpp"
#include "../Settings/CSettings.h"
#include "../Language/gettext.h"

#define NAME_OFFSET_DB  "wiitdb_offsets.bin"
#define MAXREADSIZE     1024*1024   //! Cache size only for parsing the offsets: 1MB

typedef struct _ReplaceStruct
{
	const char * orig;
	char replace;
	short size;
} ReplaceStruct;

//! More replacements can be added if needed
static const ReplaceStruct Replacements[] =
{
	{ "&gt;", '>', 4 },
	{ "&lt;", '<', 4 },
	{ "&quot;", '\"', 6 },
	{ "&apos;", '\'', 6 },
	{ "&amp;", '&', 5 },
	{ NULL, '\0', 0 }
};

GameTDB::GameTDB()
	: file(0), LangCode("EN"), GameNodeCache(0)
{
}

GameTDB::GameTDB(const char * filepath)
	: file(0), LangCode("EN"), GameNodeCache(0)
{
	OpenFile(filepath);
}

GameTDB::~GameTDB()
{
	CloseFile();
}

bool GameTDB::OpenFile(const char * filepath)
{
	if(!filepath)
		return false;

	file = fopen(filepath, "rb");
	if(file)
	{
		int pos;
		std::string OffsetsPath = filepath;
		if((pos = OffsetsPath.find_last_of('/')) != (int) std::string::npos)
			OffsetsPath[pos] = '\0';
		else
			OffsetsPath.clear(); //! Relative path
		
		LoadGameOffsets(OffsetsPath.c_str());
	}

	return (file != NULL);
}

void GameTDB::CloseFile()
{
	OffsetMap.clear();
	std::vector<GameOffsets>().swap(OffsetMap);

	if(GameNodeCache)
		delete [] GameNodeCache;
	GameNodeCache = NULL;

	if(file)
		fclose(file);
	file = NULL;
}

bool GameTDB::LoadGameOffsets(const char * path)
{
	if(!path)
		return false;

	std::string OffsetDBPath = path;
	if(strlen(path) > 0 && path[strlen(path)-1] != '/')
		OffsetDBPath += '/';
	OffsetDBPath += NAME_OFFSET_DB;

	FILE * fp = fopen(OffsetDBPath.c_str(), "rb");
	if(!fp)
	{
		bool result = ParseFile();
		if(result)
			SaveGameOffsets(OffsetDBPath.c_str());
		
		return result;
	}

	unsigned long long ExistingVersion = GetGameTDBVersion();
	unsigned long long Version = 0;
	unsigned int NodeCount = 0;

	fread(&Version, 1, sizeof(Version), fp);

	if(ExistingVersion != Version)
	{
		fclose(fp);
		bool result = ParseFile();
		if(result)
			SaveGameOffsets(OffsetDBPath.c_str());
		
		return result;
	}

	fread(&NodeCount, 1, sizeof(NodeCount), fp);

	if(NodeCount == 0)
	{
		fclose(fp);
		bool result = ParseFile();
		if(result)
			SaveGameOffsets(OffsetDBPath.c_str());
		
		return result;
	}

	OffsetMap.resize(NodeCount);

	if(fread(&OffsetMap[0], 1, NodeCount*sizeof(GameOffsets), fp) != NodeCount*sizeof(GameOffsets))
	{
		fclose(fp);
		bool result = ParseFile();
		if(result)
			SaveGameOffsets(OffsetDBPath.c_str());
		
		return result;
	}

	fclose(fp);

	return true;
}

bool GameTDB::SaveGameOffsets(const char * path)
{
	if(OffsetMap.empty() || !path)
		return false;

	FILE * fp = fopen(path, "wb");
	if(!fp)
		return false;

	unsigned long long ExistingVersion = GetGameTDBVersion();
	unsigned int NodeCount = OffsetMap.size();

	if((int)fwrite(&ExistingVersion, 1, sizeof(ExistingVersion), fp) < 0)
	{
		fclose(fp);
		return false;
	}

	if((int)fwrite(&NodeCount, 1, sizeof(NodeCount), fp) < 0)
	{
		fclose(fp);
		return false;
	}

	if((int)fwrite(&OffsetMap[0], 1, NodeCount*sizeof(GameOffsets), fp) < 0)
	{
		fclose(fp);
		return false;
	}

	fclose(fp);

	return true;
}

unsigned long long GameTDB::GetGameTDBVersion()
{
	if(!file)
		return 0;

	char TmpText[1024];

	if((int)GetData(TmpText, 0, sizeof(TmpText)) < 0)
		return 0;

	char * VersionText = GetNodeText(TmpText, "<WiiTDB version=\"", "/>");
	if(!VersionText)
		return 0;

	return strtoull(VersionText, NULL, 10);
}

size_t GameTDB::GetData(char * data, int offset, int size)
{
	if(!file || !data)
		return -1;

	fseek(file, offset, SEEK_SET);

	return fread(data, 1, size, file);
}

char * GameTDB::LoadGameNode(const char * id)
{
	unsigned int read = 0;

	GameOffsets * offset = this->GetGameOffset(id);
	if(!offset)
		return NULL;

	char * data = new (std::nothrow) char[offset->nodesize+1];
	if(!data)
		return NULL;

	if((read = GetData(data, offset->gamenode, offset->nodesize)) != offset->nodesize)
	{
		delete [] data;
		return NULL;
	}

	data[read] = '\0';

	return data;
}

char * GameTDB::GetGameNode(const char * id)
{
	char * data = NULL;

	if(GameNodeCache != 0 && strncmp(id, GameIDCache, strlen(GameIDCache)) == 0)
	{
		data = new (std::nothrow) char[strlen(GameNodeCache)+1];
		if(data)
			strcpy(data, GameNodeCache);
	}
	else
	{
		if(GameNodeCache)
			delete [] GameNodeCache;

		GameNodeCache = LoadGameNode(id);

		if(GameNodeCache)
		{
			snprintf(GameIDCache, sizeof(GameIDCache), id);
			data = new (std::nothrow) char[strlen(GameNodeCache)+1];
			if(data)
				strcpy(data, GameNodeCache);
		}
	}

	return data;
}

GameOffsets * GameTDB::GetGameOffset(const char * gameID)
{
	for(unsigned int i = 0; i < OffsetMap.size(); ++i)
	{
		if(strncmp(gameID, OffsetMap[i].gameID, strlen(OffsetMap[i].gameID)) == 0)
			return &OffsetMap[i];
	}

	return 0;
}

static inline char * CleanText(char * in_text)
{
	if(!in_text)
		return NULL;

	const char * ptr = in_text;
	char * text = in_text;

	while(*ptr != '\0')
	{
		for(int i = 0; Replacements[i].orig != 0; ++i)
		{
			if(strncmp(ptr, Replacements[i].orig, Replacements[i].size) == 0)
			{
				ptr += Replacements[i].size;
				*text = Replacements[i].replace;
				++text;
				i = 0;
				continue;
			}
		}

		if(*ptr == '\r')
		{
			++ptr;
			continue;
		}

		*text = *ptr;
		++ptr;
		++text;
	}

	*text = '\0';

	return in_text;
}

char * GameTDB::GetNodeText(char * data, const char * nodestart, const char * nodeend)
{
	if(!data || !nodestart || !nodeend)
		return NULL;

	char * position = strstr(data, nodestart);
	if(!position)
		return NULL;

	position += strlen(nodestart);

	char * end = strstr(position, nodeend);
	if(!end)
		return NULL;

	*end = '\0';

	return CleanText(position);
}

char * GameTDB::SeekLang(char * text, const char * langcode)
{
	if(!text || !langcode) return NULL;

	char * ptr = text;
	while((ptr = strstr(ptr, "<locale lang=")) != NULL)
	{
		ptr += strlen("<locale lang=\"");

		if(strncmp(ptr, langcode, strlen(langcode)) == 0)
		{
			//! Cut off all the other languages
			char * end = strstr(ptr, "</locale>");
			if(!end)
				return NULL;

			end += strlen("</locale>");
			*end = '\0';

			return ptr;
		}
	}

	return NULL;
}

bool GameTDB::ParseFile()
{
	OffsetMap.clear();

	if(!file)
		return false;

	char * Line = new (std::nothrow) char[MAXREADSIZE+1];
	if(!Line)
		return false;

	bool readnew = false;
	int i, currentPos = 0;
	int read = 0;
	const char * gameNode = NULL;
	const char * idNode = NULL;
	const char * gameEndNode = NULL;

	while((read = GetData(Line, currentPos, MAXREADSIZE)) > 0)
	{
		gameNode = Line;
		readnew = false;

		//! Ensure the null termination at the end
		Line[read] = '\0';

		while((gameNode = strstr(gameNode, "<game name=\"")) != NULL)
		{
			idNode = strstr(gameNode, "<id>");
			gameEndNode = strstr(gameNode, "</game>");
			if(!idNode || !gameEndNode)
			{
				//! We are in the middle of the game node, reread complete node and more
				currentPos += (gameNode-Line);
				fseek(file, currentPos, SEEK_SET);
				readnew = true;
				break;
			}

			idNode += strlen("<id>");
			gameEndNode += strlen("</game>");

			int size = OffsetMap.size();
			OffsetMap.resize(size+1);

			for(i = 0; i < 4 && *idNode != '<'; ++i, ++idNode)
				OffsetMap[size].gameID[i] = *idNode;
			OffsetMap[size].gameID[i] = '\0';
			OffsetMap[size].gamenode = currentPos+(gameNode-Line);
			OffsetMap[size].nodesize = (gameEndNode-gameNode);
			gameNode = gameEndNode;
		}

		if(readnew)
			continue;

		currentPos += read;
	}

	delete [] Line;

	return true;
}

bool GameTDB::GetTitle(const char * id, std::string & title)
{
	if(!id)
		return false;

	char * data = GetGameNode(id);
	if(!data)
		return false;
	
	char * language = SeekLang(data, LangCode.c_str());
	if(!language)
	{
		language = SeekLang(data, "EN");
		if(!language)
		{
			delete [] data;
			return false;
		}
	}

	char * the_title = GetNodeText(language, "<title>", "</title>");
	if(!the_title)
	{
		delete [] data;
		return false;
	}

	title = the_title;

	delete [] data;

	return true;
}

void GetLanguageToLangCode(short langid, char *langcode)
{
	//! map language id (or Wii language settting if langid is set to -1) to language code. CONF_LANG_JAPANESE = 0, not 1

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
		sprintf(langcode, "ZHCN"); //! People's Republic of China
		break;
	case CONF_LANG_TRAD_CHINESE:
		sprintf(langcode, "ZHTW"); //! Taiwan
		break;
	case CONF_LANG_KOREAN:
		sprintf(langcode, "KO");
		break;
	}
	
	Settings.GameTDBLanguage = langid;
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
		sprintf(langname, tr("Simple Chinese")); //! People's Republic of China
		break;
	case CONF_LANG_TRAD_CHINESE:
		sprintf(langname, tr("Trad Chinese")); //! Taiwan
		break;
	case CONF_LANG_KOREAN:
		sprintf(langname, tr("Korean"));
		break;
	}
}
/*void GameTDBLanguageReload()
{
	if(!Settins.GameTDBTitles)
		return;

	if(!strcmp(Settings.LanguageFile, "asturian.lang"))
		Settings.GameTdbLanguage = 4;
	else if(!strcmp(Settings.LanguageFile, "danish.lang"))
		Settings.GameTdbLanguage = 1;
	else if(!strcmp(Settings.LanguageFile, "dutch.lang"))
		Settings.GameTdbLanguage = 6;
	else if(!strcmp(Settings.LanguageFile, "english.lang"))
		Settings.GameTdbLanguage = 1;
	else if(!strcmp(Settings.LanguageFile, "finnish.lang"))
		Settings.GameTdbLanguage = 1;
	else if(!strcmp(Settings.LanguageFile, "french.lang"))
		Settings.GameTdbLanguage = 3;
	else if(!strcmp(Settings.LanguageFile, "german.lang"))
		Settings.GameTdbLanguage = 2;
	else if(!strcmp(Settings.LanguageFile, "italian.lang"))
		Settings.GameTdbLanguage = 5;
	else if(!strcmp(Settings.LanguageFile, "japanese.lang"))
		Settings.GameTdbLanguage = 0;
	else if(!strcmp(Settings.LanguageFile, "schinese.lang"))
		Settings.GameTdbLanguage = 7;
	else if(!strcmp(Settings.LanguageFile, "spanish.lang"))
		Settings.GameTdbLanguage = 4;
	else if(!strcmp(Settings.LanguageFile, "tchinese.lang"))
		Settings.GameTdbLanguage = 8;
	else if(!strcmp(Settings.LanguageFile, "korean.lang"))
		Settings.GameTdbLanguage = 9;
	
	GetLanguageToLangCode(Settings.GameTdbLanguage, Settings.GameTdbLanguageCode);
	
	Settings.Save();
}*/
