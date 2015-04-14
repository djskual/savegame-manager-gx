/****************************************************************************
 * Copyright (C) 2009
 * by Dimok
 * modified by Dj_Skual
 * for SaveGame Manager Gx 2011
 *
 * Original Filebrowser by Tantric for libwiigui
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
 *
 * FileBrowser
 *
 * Directory parsing Class
 * for WiiXplorer 2009
 ***************************************************************************/
#ifndef _CUSTOMBROWSER_H_
#define _CUSTOMBROWSER_H_

#include "filebrowser.h"

#define FILTER_PATH		0x01
#define FILTER_FONT		0x02
#define FILTER_LANG		0x04

class CustomBrowser : public FileBrowser
{
	public:
		CustomBrowser();
		void SetFilter(u8 filtermode) { Filter = filtermode; }
		int BrowsePath(std::string path);
		int ChangeDirectory();
		void Refresh();
		
	private:
		void ParseDirEntries();
		int ParseDirectory(bool filteringList = false);
		
		u8 Filter;
};

#endif
