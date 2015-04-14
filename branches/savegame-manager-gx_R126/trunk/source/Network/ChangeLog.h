/***************************************************************************
 * Copyright (C) 2009
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
 *
 * ChangeLog.h
 *
 * for WiiXplorer 2010
 ***************************************************************************/
#ifndef _CHANGELOG_H_
#define _CHANGELOG_H_

class ChangeLog
{
	public:
		ChangeLog();
		~ChangeLog();
		//!Does all the checks and downloads
		//!opens the ChangeLog.txt with TextViewer afterwards
		bool Show();
		int GetSavedChangeLogRev();
		bool DownloadChangeLog(int fromRev, int tillRev, bool backwards = true);
		//!Allocates memory that has to be freed afterwards
		char * GetChangeLogRange(int fromRev, int tillRev, bool backwards = true);
		char * GetChanges(int Rev);
	
	private:
		std::string FixChangeLogText(std::string text);
};

#endif
