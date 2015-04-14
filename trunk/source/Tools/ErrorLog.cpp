/***************************************************************************
 * Copyright (C) 2012
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
 * for SaveGame Manager GX 2012
 ***************************************************************************/
#include <malloc.h>
#include <string.h>

#include "../TextOperations/TextEditor.h"
#include "ErrorLog.h"
#include "StringTools.h"
#include "../FileOperations/fileops.h"

ErrorLog *ErrorLog::instance = NULL;

void ErrorLog::Reset()
{
	errorLog.clear();
	errorCount = 0;
	errorLogPath.assign(strfmt("%s/Error.log", Settings.UpdatePath));
	
}

void ErrorLog::AddEntry(std::string name, std::string errorTxt)
{
	errorCount++;
	errorLog += strfmt("* %s:\n    %s\n", name.c_str(), errorTxt.c_str());
}

void ErrorLog::WriteFile()
{
	CreateSubfolder(Settings.UpdatePath);
	
	FILE * file;
	file = fopen(errorLogPath.c_str(), "wb");

	if(!file)
		return;
	
	if(fwrite((char*)errorLog.c_str(), 1, errorLog.size(), file) != errorLog.size())
	{
		fclose(file);
		RemoveFile(errorLogPath);
		return;
	}
	
	fclose(file);
	
	return;
}

void ErrorLog::Show()
{
	WriteFile();
	
	if(!CheckFile(errorLogPath))
		return;

	TextEditor::LoadFile(errorLogPath.c_str(), false);
	
	RemoveFile(errorLogPath);
}
