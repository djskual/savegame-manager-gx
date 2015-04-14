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
 * CErrorLog.cpp
 *
 * for SaveGame Manager GX 2011
 *
 ***************************************************************************/
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "../Text/TextViewer.h"
#include "CErrorLog.h"
#include "../FileOperations/fileops.h"
#include "../Settings/CSettings.h"

CErrorLog ErrorLog;

CErrorLog::CErrorLog()
{
	Reset();
}

CErrorLog::~CErrorLog()
{
	if(errorlog != NULL)
		free(errorlog);
}

bool CErrorLog::Reset()
{
	cnt = 0;
	
	snprintf(errorlogpath, sizeof(errorlogpath), "%s/error.log", Settings.UpdatePath);
	
	logsize = 1;
	
	if(errorlog != NULL)
		free(errorlog);
	
	errorlog = (char*) malloc(32);
	if(!errorlog)
		return false;
	
	strcpy(errorlog, "");

	return true;
	
}

void CErrorLog::AddToLog(const char *name, const char *errorTxt)
{
	char errorchar[200];
	snprintf(errorchar, sizeof(errorchar), "* %s:\n    %s\n", name, errorTxt);
	
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

bool CErrorLog::SaveErrorFile()
{
	CreateSubfolder(Settings.UpdatePath);
	
	FILE * file;
	file = fopen(errorlogpath, "wb");

	if(!file)
		return false;
	
	if((int)fwrite((char*)errorlog, 1, logsize, file) < 0) {
		fclose(file);
		RemoveFile(errorlogpath);
		return false;
	}
	
	fclose(file);
	
	return true;
}

bool CErrorLog::Show()
{
	SaveErrorFile();
	
	if(!CheckFile(errorlogpath))
		return false;

	TextViewer(errorlogpath);

	RemoveFile(errorlogpath);
	
	return true;
}
