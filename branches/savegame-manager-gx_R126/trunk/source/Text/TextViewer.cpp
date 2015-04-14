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
 * TextViewer.cpp
 *
 * for WiiXplorer 2009
 ***************************************************************************/
#include <gctypes.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "../LibWiiGui/gui.h"
#include "../Tools/ShowError.h"
#include "../Tools/tools.h"
#include "../Prompts/PromptWindows.h"
#include "../Language/gettext.h"
#include "../menu.h"
#include "../FileOperations/fileops.h"
#include "../System/sys.h"
#include "TextEditor.h"
#include "wstring.hpp"


/****************************************************************************
* TextViewer
***************************************************************************/
void TextViewer(const char *filepath)
{
	bool exitwindow = false;

	u8 *file = NULL;
	u64 filesize = 0;

	int ret = LoadFileToMemWithProgress(tr("Loading file:"), filepath, &file, &filesize);
	if(ret < 0)
	{
		ShowError(tr("Could not load text file."));
		return;
	}
	else if(filesize > (u32) (4.5*MB_SIZE))
	{
		free(file);
		ShowError(tr("File is too big."));
		return;
	}

	u8 * tmp = (u8 *) realloc(file, filesize+1);
	if(!tmp)
	{
		free(file);
		ShowError(tr("Not enough memory."));
		return;
	}
	file = tmp;
	file[filesize] = 0;
	filesize++;

	wString * filetext = NULL;

	//To check if text is UTF8 or not
	if(utf8Len((char*) file) > 0)
	{
		filetext = new (std::nothrow) wString();
		if(!filetext)
		{
			free(file);
			file = NULL;
			ShowError(tr("Not enough memory."));
			return;
		}

		filetext->fromUTF8((char*) file);
		free(file);
		file = NULL;
	}
	else
	{
		wchar_t * tmptext = charToWideChar((char*) file);

		free(file);
		file = NULL;

		if(!tmptext)
		{
			ShowError(tr("Not enough memory."));
			return;
		}

		filetext = new (std::nothrow) wString(tmptext);

		delete [] tmptext;

		if(!filetext)
		{
			ShowError(tr("Not enough memory."));
			return;
		}
	}

	TextEditor * Editor = new TextEditor(filetext->c_str(), 9, filepath);
	Editor->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	Editor->SetPosition(0, 0);

	delete filetext;
	filetext = NULL;

	mainWindow->SetState(STATE_DISABLED);
	mainWindow->Append(Editor);

	while(!exitwindow)
	{
		usleep(100);

		if(Editor->GetState() == STATE_CLOSED)
			exitwindow = true;
	}

	delete Editor;
	Editor = NULL;

	mainWindow->SetState(STATE_DEFAULT);
	ResumeGui();
}
