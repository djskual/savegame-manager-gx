/***************************************************************************
 * Copyright (C) 2010
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
 * ErrorLog.cpp
 *
 * for SaveGame Manager GX 2010
 *
 ***************************************************************************/
#include <unistd.h>

#include "text/TextViewer.h"
#include "tools/ErrorLog.h"
#include "file/fileops.h"
#include "main.h"


ErrorLog::ErrorLog()
{
	Init();
}

ErrorLog::~ErrorLog()
{
	if(errorlog != NULL)
		free(errorlog);
}

bool ErrorLog::Init()
{
	cnt = 0;
	
	snprintf(errorlogpath, sizeof(errorlogpath), "%serror.log", cfg.UpdatePath);
	
	logsize = 1;
	
	errorlog = (char*) malloc(32);
    if(!errorlog)
        return false;
	strcpy(errorlog, "");

    return true;
    
}

void ErrorLog::AddToLog(const char *name, const char *error)
{
	char errorchar[200];
	snprintf(errorchar, sizeof(errorchar), "* %s:\n    %s\n", name, error);
	
	logsize += strlen(errorchar);
	
	char * tmpMem = (char*) realloc(errorlog, logsize);
	if(!tmpMem)
	{
		free(tmpMem);
		tmpMem = NULL;
	}

	errorlog = tmpMem;

	strcat(errorlog, errorchar);
	
	cnt++;
}

bool ErrorLog::SaveErrorFile()
{
	CreateSubfolder(cfg.UpdatePath);
	
    FILE * file;
	file = fopen(errorlogpath, "wb");

    if(!file) {
        fclose(file);
        return false;
    }
	
	if((int)fwrite((char*)errorlog, 1, logsize, file) < 0) {
		fclose(file);
		RemoveFile(errorlogpath);
		return false;
	}
	
	fclose(file);
	
	return true;
}

bool ErrorLog::DeleteErrorFile()
{
	return RemoveFile(errorlogpath);
}

int ErrorLog::GetCount()
{
	
	return cnt;
}

bool ErrorLog::Show()
{
    SaveErrorFile();
	
	if(!CheckFile(errorlogpath))
        return false;

    TextViewer(errorlogpath);

    DeleteErrorFile();
	
	return true;
}
