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
 * update.cpp
 *
 * Update operations
 * for SaveGame Manager GX 2010
 ***************************************************************************/
#include <ogcsys.h>
#include <stdio.h>
#include <string.h>
#include <zip/unzip.h>

#include "libwiigui/gui.h"
#include "prompts/promptwindows.h"
#include "prompts/ProgressWindow.h"
#include "file/fileops.h"
#include "network/http.h"
#include "network/ChangeLog.h"
#include "network/networkops.h"
#include "network/update.h"
#include "network/svnrev.h"
#include "main.h"
#include "menu/MainWindow.h"
#include "system/sys.h"
#include "network/HTML_Stream.h"

bool refreshMainMenu = false;
extern bool boothomebrew;

static const char meta_Url[MAXPATHLEN] = "http://savegame-manager-gx.googlecode.com/svn/trunk/HBC/meta.xml";
static const char wiiTDB_Url[MAXPATHLEN] = "http://wiitdb.com/wiitdb.zip?LANG=TITLESONLY";

/****************************************************************************
 * UpdateApp from a given url. The dol is downloaded and overwrites the old one.
 ***************************************************************************/
int UpdateApp(const char *url) {
    Settings Settings;
	
	if(!url)
    {
        WindowPrompt(tr("URL is empty."), 0, tr("OK"), 0, 0, 0, false);
        return -1;
    }

    char dest[strlen(cfg.UpdatePath)+10];

    snprintf(dest, sizeof(dest), "%sboot.tmp", cfg.UpdatePath);

    CreateSubfolder(cfg.UpdatePath);

    int res = DownloadFileToPath(url, dest, false);
    if(res < 1)
    {
        RemoveFile(dest);
        WindowPrompt(tr("Update failed"), tr("Could not download file."), tr("OK"), 0, 0, 0, false);
        return -1;
    }
    else
    {
        char realdest[strlen(dest)+1];
        snprintf(realdest, sizeof(realdest), "%sboot.dol", cfg.UpdatePath);
        RemoveFile(realdest);
        rename(dest, realdest);
        
		StartProgress(tr("Downloading file..."), THROBBER, false);
		
		if(cfg.UpdateMeta) {
            ShowProgress(0, 1, "meta.xml");
			UpdateMetaXml();
			usleep(1300000);}
        if(cfg.UpdateIcon > 0) {
			ShowProgress(0, 1, "icon.png");
            UpdateIconPNG();
			usleep(1000000);}
		if(cfg.UpdateWiiTdb) {
			ShowProgress(0, 1, "wiitdb.zip");
            UpdateWiiTdb();}
		
		StopProgress();
    }

    return 1;
}

/****************************************************************************
 * Checking if an Update is available
 ***************************************************************************/
int CheckForUpdate() {
	if(!IsNetworkInit()) {
		int net = NetworkInitPrompt();
		if (net == 0) {
			return -1;
		}
	}
	
    int revnumber = 0;
    int currentrev = atoi(SvnRev());

    HTML_Stream HTML("http://code.google.com/p/savegame-manager-gx/downloads/list");

    const char * HTML_Pos = NULL;
    char DownloadLink[300];
    memset(DownloadLink, 0, sizeof(DownloadLink));

    do
    {
        HTML_Pos = HTML.FindStringEnd("href=\"");
        char * tmpLink = HTML.CopyString("\"");
        if(tmpLink)
        {
            char *fileext = strrchr(tmpLink, '.');
            if(fileext)
            {
                if(strcasecmp(fileext, ".dol") == 0)
                {
                    char revtxt[80];
                    char *filename = strrchr(tmpLink, '/')+2;
                    u8 n = 0;
                    for (n = 0; n < strlen(filename)-2; n++)
                        revtxt[n] = filename[n];
                    revtxt[n] = 0;
                    int fileRev = atoi(revtxt);

                    if(fileRev > revnumber)
                    {
                        revnumber = fileRev;
                        snprintf(DownloadLink, sizeof(DownloadLink), "%s", tmpLink);
                    }
                }
            }
			free(tmpLink);
        }
    }while(HTML_Pos != NULL);

    if (revnumber > currentrev)
    {
        char text[100];
        sprintf(text, tr("Update to Rev%i available"), revnumber);
        
		MainWindow::Instance()->SetState(STATE_DISABLED);
		
		int choice = 1;
        while(choice)
        {
            choice = WindowPrompt(text, tr("Do you want to update now ?"), tr("Yes"), tr("Show Changelog"), tr("Cancel"), 0, false);
			
            if(choice == 1)
            {
                if(UpdateApp(DownloadLink) >= 0)
                {
                    int ret = WindowPrompt(tr("Update successfully finished"), tr("Do you want to reboot now ?"), tr("Yes"), tr("No"), 0, 0, false);
                    if(ret) {
                        RebootApp();
					}
                }
                break;
            }
            else if(choice == 2)
            {
                ChangeLog Changelog;
                Changelog.DownloadChangeLog(revnumber-5, revnumber);
                if(!Changelog.Show())
                    WindowPrompt(tr("Failed to get the Changelog."), 0, tr("OK"), 0, 0, 0, false);
            }
        }
		refreshMainMenu = true;
		MainWindow::Instance()->SetState(STATE_DEFAULT);
    }
    else
    {
        revnumber = 0;
    }

    return revnumber;
}

/****************************************************************************
 * Update the Meta.xml file
 ***************************************************************************/
bool UpdateMetaXml() {
	if(!IsNetworkInit()) {
		int net = NetworkInitPrompt();
		if (net == 0) {
			return 0;
		}
	}
	
    struct block file = downloadfile(meta_Url);
    if(!file.data)	
        return false;

    CreateSubfolder(cfg.UpdatePath);

    char path[MAXPATHLEN];
    snprintf(path, sizeof(path), "%smeta.xml", cfg.UpdatePath);
    FILE * pFile = fopen(path, "wb");
    if(!pFile)
        return false;

    fwrite(file.data, 1, file.size, pFile);
    fclose(pFile);

    return true;
}

/****************************************************************************
 * Update the Icon.png file
 ***************************************************************************/
bool UpdateIconPNG() {
	if(!IsNetworkInit()) {
		int net = NetworkInitPrompt();
		if (net == 0) {
			return false;
		}
	}
	
	char icon_Url[MAXPATHLEN];
	sprintf(icon_Url, "http://savegame-manager-gx.googlecode.com/svn/trunk/HBC/icon%d.png", cfg.UpdateIcon);
	
	struct block file = downloadfile(icon_Url);
    if(!file.data)
        return false;

    CreateSubfolder(cfg.UpdatePath);

    char path[MAXPATHLEN];
    snprintf(path, sizeof(path), "%sicon.png", cfg.UpdatePath);
    FILE * pFile = fopen(path, "wb");
    if(!pFile)
        return false;

    fwrite(file.data, 1, file.size, pFile);
    fclose(pFile);

    return true;
}

/****************************************************************************
 * Update the wiitdb.zip file
 ***************************************************************************/
bool UpdateWiiTdb()
{
	if(!IsNetworkInit()) {
		int net = NetworkInitPrompt();
		if (net == 0) {
			return false;
		}
	}
	
	struct block file = downloadfile(wiiTDB_Url);
    if(!file.data)
        return false;

    CreateSubfolder(cfg.WiiTdbPath);

    char zippath[MAXPATHLEN];
    char xmlpath[MAXPATHLEN];
    snprintf(zippath, sizeof(zippath), "%swiitdb.zip", cfg.WiiTdbPath);
    snprintf(xmlpath, sizeof(xmlpath), "%swiitdb.xml", cfg.WiiTdbPath);
    
	FILE * pFile = fopen(zippath, "wb");
    if(!pFile)
        return false;

    fwrite(file.data, 1, file.size, pFile);
    fclose(pFile);
	
	/* load zipped XML file */
    int	xmlmaxsize = 4194304 ; // 4Mo
	
	unzFile unzfile = unzOpen(zippath);
	if (unzfile == NULL)
		return false;
	unzOpenCurrentFile(unzfile);

	unz_file_info zipfileinfo;
	unzGetCurrentFileInfo(unzfile, &zipfileinfo, NULL, 0, NULL, 0, NULL, 0);
	int zipfilebuffersize = zipfileinfo.uncompressed_size;
	if (zipfilebuffersize >= xmlmaxsize) {
		unzCloseCurrentFile(unzfile);
		unzClose(unzfile);
		return false;
	}

	char * zipfilebuffer = (char*)malloc(zipfilebuffersize);
	memset(zipfilebuffer, 0, zipfilebuffersize);
	if (zipfilebuffer == NULL) {
		unzCloseCurrentFile(unzfile);
		unzClose(unzfile);
		return false;
	}

	unzReadCurrentFile(unzfile, zipfilebuffer, zipfilebuffersize);
	unzCloseCurrentFile(unzfile);
	unzClose(unzfile);

	/* save unzipped XML file */
    pFile = fopen(xmlpath, "wb");
    if(!pFile)
        return false;

    fwrite(zipfilebuffer, 1, zipfilebuffersize, pFile);
    fclose(pFile);
	
	free(zipfilebuffer);
	RemoveFile(zippath);

    return true;
}
