 /***************************************************************************
 * Copyright (C) 2009
 * by Dimok
 * modified by Dj Skual
 * for SaveGame ManagerGX 2011
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
 * for WiiXplorer 2009
 ***************************************************************************/
#include <ogcsys.h>
#include <stdio.h>
#include <string.h>

#include "../Prompts/PromptWindows.h"
#include "../Prompts/ProgressWindow.h"
#include "../Language/UpdateLanguage.h"
#include "../System/sys.h"
#include "../ArchiveOperations/ZipFile.h"
#include "../FileOperations/DirList.h"
#include "../FileOperations/fileops.h"
#include "../Settings/CSettings.h"
#include "../Language/gettext.h"
#include "../ManageAll/CErrorLog.h"
#include "FileDownloader.h"
#include "networkops.h"
#include "ChangeLog.h"
#include "URL_List.h"
#include "update.h"
#include "svnrev.h"
#include "http.h"
#include "../menu.h"

static const char Meta_Url[MAXPATHLEN] = "http://savegame-manager-gx.googlecode.com/svn/branches/Updates/meta.xml";
static const char LanguageFilesURL[MAXPATHLEN] = "http://savegame-manager-gx.googlecode.com/svn/trunk/Languages/";
static const char GameTDB_Url[MAXPATHLEN] = "http://www.gametdb.com/wiitdb.zip?LANG=TITLESONLY&WIIWARE=TRUE";
//static const char GameTDB_Url[MAXPATHLEN] = "http://www.gametdb.com/wiitdb.zip?LANG=TITLESONLY&GAMECUBE=TRUE&WIIWARE=TRUE";

/****************************************************************************
 * UpdateApp from a given url. The dol is downloaded and overwrites the old one.
 ***************************************************************************/
int UpdateApp(const char *url)
{
	if(!url)
	{
		WindowPrompt(tr("URL is empty."), 0, tr("OK"));
		return -1;
	}

	char dest[strlen(Settings.UpdatePath)+10];

	snprintf(dest, sizeof(dest), "%s/boot.tmp", Settings.UpdatePath);

	CreateSubfolder(Settings.UpdatePath);

	int res = DownloadFileToPath(url, dest, false);
	if(res < 1)
	{
		RemoveFile(dest);
		WindowPrompt(tr("Update failed"), tr("Could not download file."), tr("OK"));
		return -1;
	}
	else
	{
		char realdest[strlen(dest)+1];
		snprintf(realdest, sizeof(realdest), "%s/boot.dol", Settings.UpdatePath);
		RemoveFile(realdest);
		rename(dest, realdest);
		
		UpdateFiles();
	}

	return 1;
}

/****************************************************************************
 * Checking if an Update is available
 ***************************************************************************/
int CheckForUpdate()
{
	if(!NetworkInitPrompt())
		return -1;
	
	std::string DownloadURL;
    int newrev = 0;
	int currentrev = GetIntRev();

	struct block file = downloadfile("http://savegame-manager-gx.googlecode.com/svn/branches/Updates/update.txt");
	if (file.data != NULL) 
	{ 
		//! first line of the text file is the revision
        newrev = atoi((char *) file.data);
		
		//! 2nd line of the text file is the url
        char *ptr = strchr((char *)file.data, '\n');
        while(ptr && (*ptr == '\r' || *ptr == '\n' || *ptr == ' '))
            ptr++;
        while(ptr && *ptr != '\0' && *ptr != '\r' && *ptr != '\n')
        {
            DownloadURL.push_back(*ptr);
            ptr++;
        }
		
		free(file.data);
	}
	
	if (newrev > currentrev)
	{
		char text[100];
		sprintf(text, tr("Update to Rev%i available"), newrev);
		
		mainWindow->SetState(STATE_DISABLED);
		
		int choice = 1;
		while(choice)
		{
			choice = WindowPrompt(text, tr("Do you want to update now ?"), (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_UPDATES)) ? tr("Yes") : NULL, tr("Show Changelog"), tr("Cancel"));
			
			if(choice == 1 && (!Settings.GodMode && (Settings.ParentalBlocks & BLOCK_UPDATES)))
				choice = 2;
			
			if(choice == 1)
			{
				if(UpdateApp(DownloadURL.c_str()) >= 0)
				{
					if(WindowPrompt(tr("Update successfully finished"), tr("Do you want to reboot now ?"), tr("Yes"), tr("No")))
						RebootApp();
				}
				break;
			}
			else if (choice == 2)
			{
				ChangeLog Changelog;
				Changelog.DownloadChangeLog(newrev-5, newrev);
				if(!Changelog.Show())
					WindowPrompt(tr("Failed to get the Changelog."), 0, tr("OK"));
			}
		}
		mainWindow->SetState(STATE_DEFAULT);
	}
	else
	{
		newrev = 0;
	}

	return newrev;
}

/****************************************************************************
 * Update files selected in settings
 ***************************************************************************/
 bool UpdateFiles()
{
	ErrorLog.Reset();
	
	ShowProgress(tr("Downloading..."), "meta.xml", 0, 0, 1, false, false, UPDATE);
	if(!UpdateMetaXml())
		ErrorLog.AddToLog("meta.xml", tr("Update failed"));
	usleep(1300000);
	
	if(Settings.UpdateIcon > 0)
	{
		ShowProgress(tr("Downloading..."), "icon.png", 0, 0, 1, false, false, UPDATE);
		if(!UpdateIconPNG())
			ErrorLog.AddToLog("icon.png", tr("Update failed"));
		usleep(1000000);
	}
	if(Settings.UpdateLanguage > 0)
	{
		if (Settings.UpdateLanguage == LANG_UPDATE_ALL)
		{
			ShowProgress(tr("Downloading..."), tr("All Languages Files"), 0, 0, 1, false, false, UPDATE);
			if(!DownloadAllLanguages())
				ErrorLog.AddToLog(tr("All Languages Files"), tr("Update failed"));
		
		}
		else if (Settings.UpdateLanguage == LANG_UPDATE_INSTALLED)
		{
			ShowProgress(tr("Downloading..."), tr("Installed Languages Files"), 0, 0, 1, false, false, UPDATE);
			if(!UpdateLanguages())
				ErrorLog.AddToLog(tr("Installed Languages Files"), tr("Update failed"));
		}
	}
	if(Settings.UpdateGameTDB)
	{
		std::string text = tr("Downloading from");
		text += " gametdb.com";
		ShowProgress(text.c_str(), "wiitdb.zip", 0, 0, 1, false, false, UPDATE);
		if(!UpdateGameTDB())
			ErrorLog.AddToLog("wiitdb.zip", tr("Update failed"));
	}
	
	ProgressStop();
	
	if(ErrorLog.GetCount() > 0)
	{
		if(WindowPrompt(tr("Error"), tr("Show error log?"), tr("OK"), tr("Cancel")))
			ErrorLog.Show();
	}
	
	return ((ErrorLog.GetCount() > 0) ? false : true);
}

/****************************************************************************
 * Update the Meta.xml file
 ***************************************************************************/
bool UpdateMetaXml()
{
	struct block file = downloadfile(Meta_Url);
	if(!file.data)	
		return false;

	CreateSubfolder(Settings.UpdatePath);

	char path[MAXPATHLEN];
	snprintf(path, sizeof(path), "%s/meta.xml", Settings.UpdatePath);
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
bool UpdateIconPNG()
{
	char icon_Url[MAXPATHLEN];
	sprintf(icon_Url, "http://savegame-manager-gx.googlecode.com/svn/trunk/HBC/icon%d.png", Settings.UpdateIcon);
	
	struct block file = downloadfile(icon_Url);
	if(!file.data)
		return false;

	CreateSubfolder(Settings.UpdatePath);

	char path[MAXPATHLEN];
	snprintf(path, sizeof(path), "%s/icon.png", Settings.UpdatePath);
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
bool UpdateGameTDB()
{
	struct block file = downloadfile(GameTDB_Url);
	if(!file.data)
		return false;

	CreateSubfolder(Settings.GameTDBPath);

	char zippath[MAXPATHLEN];
	snprintf(zippath, sizeof(zippath), "%s/wiitdb.zip", Settings.GameTDBPath);
	
	FILE * pFile = fopen(zippath, "wb");
	if(!pFile)
		return false;

	fwrite(file.data, 1, file.size, pFile);
	fclose(pFile);
	
	//! load zipped XML file
	ZipFile *zipfile = new ZipFile(zippath);

	if( zipfile->ExtractAll(Settings.GameTDBPath, true) < 0)
		return false;
	
	delete zipfile;
	
	RemoveFile(zippath);

	return true;
}

/****************************************************************************
 * Download/Update all Languages Files found on SVN
 ***************************************************************************/
bool DownloadAllLanguages()
{
	if(!CreateSubfolder(Settings.LanguagePath))
		return false;

	char fullURL[300];

	URL_List LinkList(LanguageFilesURL);
	int listsize = LinkList.GetURLCount();
	int i = 0;
	
	for (i = 0; i < listsize; i++)
	{
		const char * filename = strrchr(LinkList.GetURL(i), '/');
		if(filename) filename++;
		else filename = LinkList.GetURL(i);
		
		if(!filename)
			continue;
		
		const char * FileExt = strrchr(filename, '.');
		if (!FileExt || (strcasecmp(FileExt, ".lang") != 0))
			continue;
		
		snprintf(fullURL, sizeof(fullURL), "%s%s", LanguageFilesURL, filename);
		
		struct block file = downloadfile(fullURL);
		if (file.data)
		{
			char filepath[300];
			snprintf(filepath, sizeof(filepath), "%s/%s", Settings.LanguagePath, filename);
			FILE * pfile = fopen(filepath, "wb");
			if(pfile)
			{
				fwrite(file.data, 1, file.size, pfile);
				fclose(pfile);
			}
			free(file.data);
		}
	}

	return true;
}

/****************************************************************************
 * Update the Languages Files found in LanguagePath
 ***************************************************************************/
bool UpdateLanguages()
{
	if(!CreateSubfolder(Settings.LanguagePath))
		return false;

	DirList Dir(Settings.LanguagePath, ".lang");

	if (Dir.GetFilecount() == 0) return false;

	char savepath[150];
	char codeurl[200];

	for(int i = 0; i < Dir.GetFilecount(); ++i)
	{
		snprintf(codeurl, sizeof(codeurl), "%s%s", LanguageFilesURL, Dir.GetFilename(i));
		snprintf(savepath, sizeof(savepath), "%s/%s", Settings.LanguagePath, Dir.GetFilename(i));
		
		struct block file = downloadfile(codeurl);
		
		if (file.data != NULL)
		{
			FILE * pfile;
			pfile = fopen(savepath, "wb");
			if (pfile != NULL)
			{
				fwrite(file.data, 1, file.size, pfile);
				fclose(pfile);
			}
			free(file.data);
		}
	}

	return true;
}