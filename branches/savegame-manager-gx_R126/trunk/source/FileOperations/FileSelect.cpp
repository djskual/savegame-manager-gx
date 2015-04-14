/****************************************************************************
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
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include <gctypes.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <malloc.h>

#include "../Prompts/PromptWindows.h"
#include "../Prompts/ProgressWindow.h"
#include "../Prompts/SelectBrowser.h"
#include "../Settings/CSettings.h"
#include "../Themes/CTheme.h"
#include "../Language/gettext.h"
#include "FileSelect.h"
#include "../menu.h"

int FileSelect(const char *filepath, short type)
{
	if(!filepath)
		return -1;

	char *fileext = strrchr(filepath, '.');
	char *filename = strrchr(filepath, '/')+1;

	if(Settings.FileExtensions.CompareLanguageFiles(fileext) == 0 && type == LANGBROWSER)
	{
		if(WindowPrompt(tr("Do you want to load this language ?"), filename, tr("Yes"), tr("Cancel")) == 1)
		{
			Settings.LoadLanguage(filepath);
			return EXIT_SUCCES;
		}
	}
	else if(Settings.FileExtensions.CompareAudio(fileext) == 0 && type == SOUNDBROWSER)
	{
		strcpy(Settings.BgMusicPath, filepath);
		return SOUND_SUCCES;
	}
	else if(Settings.FileExtensions.CompareFont(fileext) == 0 && type == FONTBROWSER)
	{
		int choice = WindowPrompt(tr("Do you want to change the font?"), filename, tr("Yes"), tr("No"));
		if(choice)
		{
			Settings.UseSystemFont = OFF;
			
			snprintf(Settings.CustomFontPath, sizeof(Settings.CustomFontPath), "%s", filepath);
			
			HaltGui();
			Theme::LoadFont(Settings.CustomFontPath);
			ResumeGui();
			
			WindowPrompt(tr("Fontpath changed."), tr("The new font is loaded."), tr("OK"));
			
			return EXIT_SUCCES;
		}
	}
	
	return 0;
}
