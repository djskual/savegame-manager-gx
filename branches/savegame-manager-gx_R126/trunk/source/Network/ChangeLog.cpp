/***************************************************************************
 * Copyright (C) 2010
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
#include <unistd.h>

#include "../Prompts/PromptWindows.h"
#include "../Prompts/ProgressWindow.h"
#include "../FileOperations/fileops.h"
#include "../Text/TextViewer.h"
#include "../Language/gettext.h"
#include "../Settings/CSettings.h"
#include "../Text/TextTools.hpp"
#include "HTML_Stream.h"
#include "ChangeLog.h"
#include "svnrev.h"

ChangeLog::ChangeLog()
{
}

ChangeLog::~ChangeLog()
{
}

char * ChangeLog::GetChangeLogRange(int fromRev, int tillRev, bool backwards)
{
	char * changelog = (char*) malloc(32);
	if(!changelog)
		return NULL;

	strcpy(changelog, "");

	//!for '\0'
	int logsize = 1;
	int RevCounter;
	int StopRev;

	if(backwards)
	{
		RevCounter = tillRev;
		StopRev = fromRev-1;
	}
	else
	{
		RevCounter = fromRev;
		StopRev = tillRev+1;
	}

	int done = 0;
	while(RevCounter != StopRev)
	{
		char ProgressText[50];
		snprintf(ProgressText, sizeof(ProgressText), "Revision %i", RevCounter);
		ShowProgress(tr("Getting Changelog..."), 0, ProgressText, done, 6, false, false);

		char * RevChangelog = this->GetChanges(RevCounter);
		if(!RevChangelog)
			continue;

		logsize += strlen(RevChangelog)+2;

		char * tmpMem = (char*) realloc(changelog, logsize);
		if(!tmpMem)
		{
			ProgressStop();
			free(tmpMem);
			tmpMem = NULL;
			free(changelog);
			free(RevChangelog);
			return NULL;
		}

		changelog = tmpMem;

		strcat(changelog, RevChangelog);
		strcat(changelog, "\n\n");

		free(RevChangelog);

		if(backwards)
			RevCounter--;
		else
			RevCounter++;
		
		done++;
	}

	ProgressStop();

	if(strlen(changelog) < 5)
	{
		//!something went wrong
		free(changelog);
		changelog = NULL;
	}

	return changelog;
}

char * ChangeLog::GetChanges(int Rev)
{
	HTML_Stream html;

	char * changelog = (char*) malloc(32);
	if(!changelog)
		return NULL;

	strcpy(changelog, "");

	//!for '\0'
	int logsize = 1;
	char RevTxt[50];
	char URL[80];

	sprintf(URL, "http://code.google.com/p/savegame-manager-gx/source/detail?r=%i", Rev);
	if(!html.LoadLink(URL))
	{
		free(changelog);
		return NULL;;
	}

	html.FindStringEnd("<pre class=\"wrap\" style=\"margin-left:1em\">");
	char * RevChangelog = html.CopyString("</pre>");

	snprintf(RevTxt, sizeof(RevTxt), "Revision: %i\n", Rev);
	logsize += strlen(RevChangelog)+strlen(RevTxt);
	char * tmpMem = (char*) realloc(changelog, logsize);
	if(!tmpMem)
	{
		free(tmpMem);
		tmpMem = NULL;
		free(changelog);
		changelog = NULL;
		free(RevChangelog);
		return NULL;
	}

	changelog = tmpMem;
	strcat(changelog, RevTxt);
	strcat(changelog, RevChangelog);
	free(RevChangelog);

	if(strlen(changelog) < 5)
	{
		//!something went wrong
		free(changelog);
		changelog = NULL;
	}

	return changelog;
}


bool ChangeLog::DownloadChangeLog(int fromRev, int tillRev, bool backwards)
{
	char * Changelog = GetChangeLogRange(fromRev, tillRev, backwards);

	if(!Changelog)
		return false;

	char writepath[MAXPATHLEN];
	snprintf(writepath, sizeof(writepath), "%s/SaveGame_Manager_GX.log", Settings.UpdatePath);
	CreateSubfolder(Settings.UpdatePath);

	FILE * f = fopen(writepath, "wb");
	if(!f)
	{
		free(Changelog);
		return false;
	}

	std::string ChangeLog = FixChangeLogText(Changelog);
	fwrite(ChangeLog.c_str(), 1, ChangeLog.size(), f);
	
	fclose(f);
	free(Changelog);

	return true;
}

int ChangeLog::GetSavedChangeLogRev()
{
	char changelogpath[MAXPATHLEN];
	snprintf(changelogpath, sizeof(changelogpath), "%s/SaveGame_Manager_GX.log", Settings.UpdatePath);

	if(!CheckFile(changelogpath))
		return -1;

	FILE * f = fopen(changelogpath, "rb");
	if(!f)
		return -1;

	char ChangelogRev[20];
	fseek(f, 10, SEEK_SET);
	fread(&ChangelogRev, 1, 4, f);
	fclose(f);

	return atoi(ChangelogRev);
}

std::string ChangeLog::FixChangeLogText(std::string text)
{
	text.assign(StringReplace(text, "&gt;", ">"));
	text.assign(StringReplace(text, "&lt;", "<"));
	text.assign(StringReplace(text, "&quot;", "\""));
	text.assign(StringReplace(text, "&apos;", "\'"));
	text.assign(StringReplace(text, "&amp;", "&"));
	
	return text;
}

bool ChangeLog::Show()
{
	char changelogpath[MAXPATHLEN];
	snprintf(changelogpath, sizeof(changelogpath), "%s/SaveGame_Manager_GX.log", Settings.UpdatePath);

	if(GetSavedChangeLogRev() < atoi(GetRev()))
	{
		if(!DownloadChangeLog(atoi(GetRev())-5, atoi(GetRev())))
			return false;
	}

	if(!CheckFile(changelogpath))
		return false;

	TextViewer(changelogpath);

	return true;
}
