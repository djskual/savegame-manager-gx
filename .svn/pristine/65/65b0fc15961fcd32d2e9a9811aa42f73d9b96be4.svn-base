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
#ifndef WIITDB_HPP_
#define WIITDB_HPP_

#include <vector>
#include <string>

using namespace std;

typedef struct _GameXMLInfo
{
	string GameID;
	string Title;
	
} GameXMLInfo;

typedef struct _GameOffsets
{
    char gameID[5];
    unsigned int gamenode;
    unsigned int nodesize;
} __attribute__((__packed__)) GameOffsets;

class WiiTDB
{
    public:
        
		enum {
			WII,
			DEVICE
		};
		
		//! Constructor
        WiiTDB();
        //! Constructor
        //! If filepath is passed the xml file is opened and the node offsets are loaded
        WiiTDB(const char * filepath);
        //! Destructor
        ~WiiTDB();
        //! If filepath is passed the xml file is opened and the node offsets are loaded
        bool OpenFile(const char * filepath);
        //! Closes the WiiTDB xml file
        void CloseFile();
        //! Set the language code which should be use to find the appropriate language
        //! If the language code is not found, the language code defaults to EN
        void SetLanguageCode(const char * code) { if(code) LangCode = code; };
        //! Get the current set language code
        const char * GetLanguageCode() { return LangCode.c_str(); };
        //! Get the title of a specific game id in the language defined in LangCode
        bool GetTitle(const char * id, string & title);
        //! Get the version of the wiitdb xml database
        unsigned long long GetWiiTDBVersion();
        //! Get the entry count in the xml database
        inline size_t GetEntryCount() { return OffsetMap.size(); };
    private:
        bool ParseFile();
        bool LoadGameOffsets(const char * path);
        bool SaveGameOffsets(const char * path);
        inline size_t GetData(char * data, int offset, int size);
        inline char * LoadGameNode(const char * id);
        inline char * GetGameNode(const char * id);
        inline GameOffsets * GetGameOffset(const char * id);
        inline char * SeekLang(char * text, const char * langcode);
        inline char * GetNodeText(char * data, const char * nodestart, const char * nodeend);
		
        vector<GameOffsets> OffsetMap;
        FILE * file;
        string LangCode;
        char * GameNodeCache;
        char GameIDCache[5];
};

#endif
