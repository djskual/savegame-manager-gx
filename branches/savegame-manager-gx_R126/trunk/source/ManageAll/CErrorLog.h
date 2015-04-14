/***************************************************************************
 * Copyright (C) 2011
 * by Dj_Skual
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
 * CErrorLog.h
 *
 * for SaveGame Manager GX 2011
 *
 ***************************************************************************/
#ifndef _CERRORLOG_H_
#define _CERRORLOG_H_

#include <unistd.h>

class CErrorLog
{
	public:
		CErrorLog();
		~CErrorLog();
		
		void AddToLog(const char * name, const char * errorTxt);
		int GetCount() { return cnt; };
		bool Show();
		bool Reset();
		
	private:
		bool SaveErrorFile();
		
		int cnt; //number of error
		char errorlogpath[MAXPATHLEN]; // path of error log file
		char * errorlog; //error buffer
		int logsize; //size of error buffer
};

extern CErrorLog ErrorLog;

#endif
