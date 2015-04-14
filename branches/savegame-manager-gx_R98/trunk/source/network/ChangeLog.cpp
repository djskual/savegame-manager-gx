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

#include "prompts/promptwindows.h"
#include "prompts/ProgressWindow.h"
#include "network/HTML_Stream.h"
#include "file/fileops.h"
#include "text/TextViewer.h"
#include "main.h"
#include "network/ChangeLog.h"
#include "network/svnrev.h"

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

    //for '\0'
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

    StartProgress(tr("Getting Changelog..."), THROBBER, 0);

    while(RevCounter != StopRev)
    {
        char ProgressText[50];
        snprintf(ProgressText, sizeof(ProgressText), "Revision %i", RevCounter);
        ShowProgress(0, 1, ProgressText);

        char * RevChangelog = this->GetChanges(RevCounter);
        if(!RevChangelog)
            continue;

        logsize += strlen(RevChangelog)+2;

        char * tmpMem = (char*) realloc(changelog, logsize);
        if(!tmpMem)
        {
            StopProgress();
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
    }

    StopProgress();

    if(strlen(changelog) < 5)
    {
        //something went wrong
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

    //for '\0'
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
        StopProgress();
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
        //something went wrong
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
    snprintf(writepath, sizeof(writepath), "%sSaveGame_Manager_GX.log", cfg.UpdatePath);
    CreateSubfolder(cfg.UpdatePath);

    FILE * f = fopen(writepath, "wb");
    if(!f)
        return false;

    fwrite(Changelog, 1, strlen(Changelog), f);
    fclose(f);
    free(Changelog);

    return true;
}

int ChangeLog::GetSavedChangeLogRev()
{
    char changelogpath[MAXPATHLEN];
    snprintf(changelogpath, sizeof(changelogpath), "%sSaveGame_Manager_GX.log", cfg.UpdatePath);

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

bool ChangeLog::Show()
{
    char changelogpath[MAXPATHLEN];
    snprintf(changelogpath, sizeof(changelogpath), "%sSaveGame_Manager_GX.log", cfg.UpdatePath);

    if(GetSavedChangeLogRev() < atoi(SvnRev()))
    {
        if(!DownloadChangeLog(atoi(SvnRev())-5, atoi(SvnRev())))
            return false;
    }

    if(!CheckFile(changelogpath))
        return false;

    TextViewer(changelogpath);

    return true;
}
