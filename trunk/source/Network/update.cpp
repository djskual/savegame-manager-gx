 /***************************************************************************
 * Copyright (C) 2009
 * by Dimok
 * modified by Dj Skual
 * for SaveGame Manager GX 2012
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
 ***************************************************************************/
#include "../Controls/Application.h"
#include "../System/sys.h"
#include "../ArchiveOperations/ZipFile.h"
#include "../FileOperations/DirList.h"
#include "../FileOperations/fileops.h"
#include "../Tools/ErrorLog.h"
#include "FileDownloader.h"
#include "ChangeLog.h"
#include "URL_List.h"
#include "update.h"
#include "svnrev.h"
#include "http.h"

static const char Meta_Url[MAXPATHLEN] = "http://savegame-manager-gx.googlecode.com/svn/branches/Updates/meta.xml";
static const char LanguageFilesURL[MAXPATHLEN] = "http://savegame-manager-gx.googlecode.com/svn/trunk/Languages/";
static const char GameTDB_Url[MAXPATHLEN] = "http://www.gametdb.com/wiitdb.zip?LANG=TITLESONLY&WIIWARE=TRUE";

/****************************************************************************
 * Checking if an Update is available
 ***************************************************************************/
int CheckForUpdate(UpdateWindow * updateWindow)
{
	UpdateWindow * update_window = updateWindow;
	bool newWindow = false;
	
	if(update_window)
	{
		if(!update_window->NetInitPrompt())
			return -1;
		
		update_window->ShowThrobber(tr("Checking for new update"), tr("Please wait..."));
	}
	
	std::string DownloadURL;
    int newrev = 0;
	int currentrev = GetIntRev();

	struct block file = downloadfile("http://savegame-manager-gx.googlecode.com/svn/branches/Updates/update.txt");
	//struct block file = downloadfile("http://savegame-manager-gx-beta.googlecode.com/svn/branches/Updates/update.txt");
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
		if(!update_window)
		{
			update_window = new UpdateWindow();
			newWindow = true;
		}
		
		char text[100];
		sprintf(text, tr("Update to Rev%i available"), newrev);
		
		int choice = 0;
		do
		{
			choice = update_window->Choose(text, tr("Do you want to update now ?"), (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_UPDATES)) ? tr("Yes") : "", tr("Show Changelog"), tr("Cancel"));
			if(choice == 1)
			{
				if(UpdateApp(DownloadURL.c_str(), update_window) >= 0)
				{
					if(update_window->Choose(tr("Update successfully finished"), tr("Do you want to reboot now ?"), tr("Yes"), tr("No")) == 1)
						RebootApp();
				}
				break;
			}
			else if (choice == 2)
			{
				ChangeLog Changelog(update_window);
				if(!Changelog.Show())
					update_window->ShowError(tr("Failed to get the Changelog."));
			}
		} while(choice != 3);
	}
	else
		newrev = 0;
	
	if(newWindow)
	{
		Application::Instance()->PushForDelete(update_window);
		Application::Instance()->updateEvents();
	}
	
	Settings.updateChecked = true;
	return newrev;
}

/****************************************************************************
 * UpdateApp from a given url. The dol is downloaded and overwrites the old one.
 ***************************************************************************/
int UpdateApp(const char *url, UpdateWindow * updateWindow)
{
	UpdateWindow * update_window = updateWindow;
	bool newWindow = false;
	int result = -1;
	
	if(!update_window)
	{
		update_window = new UpdateWindow();
		newWindow = true;
	}
	update_window->ShowThrobber(tr("Downloading..."), "boot.dol");
	
	if(!url)
	{
		update_window->ShowError(tr("URL is empty."));
		goto out;
	}

	if(!DownloadDol(url))
	{
		update_window->ShowMessage(tr("Update failed"), tr("Could not download file."));
		goto out;
	}
	
	if(!UpdateFiles(true, update_window))
		goto out;
	
	result = 1;
	
  out:
	if(newWindow)
	{
		Application::Instance()->PushForDelete(update_window);
		Application::Instance()->updateEvents();
	}
	
	return result;
}

/****************************************************************************
 * Download the boot.dol file
 ***************************************************************************/
bool DownloadDol(const char *url)
{
	struct block file = downloadfile(url);
	if(!file.data)
		return false;

	CreateSubfolder(Settings.UpdatePath);

	char dest[strlen(Settings.UpdatePath)+10];
	snprintf(dest, sizeof(dest), "%s/boot.tmp", Settings.UpdatePath);
	
	FILE * pFile = fopen(dest, "wb");
	if(!pFile)
		return false;

	if(fwrite(file.data, 1, file.size, pFile) != file.size)
	{
		fclose(pFile);
		RemoveFile(dest);
		return false;
	}
	fclose(pFile);
	
	char realdest[strlen(dest)+1];
	snprintf(realdest, sizeof(realdest), "%s/boot.dol", Settings.UpdatePath);
	RemoveFile(realdest);
	rename(dest, realdest);
	
	return true;
}

/****************************************************************************
 * Update files selected in settings
 ***************************************************************************/
 bool UpdateFiles(bool updateMeta, UpdateWindow * updateWindow)
{
	
	UpdateWindow * update_window = updateWindow;
	bool newWindow = false;
	
	if(!update_window)
	{
		update_window = new UpdateWindow();
		newWindow = true;
	}
	
	ErrorLog::DestroyInstance();
	
	if(updateMeta)
	{
		update_window->ShowThrobber(tr("Downloading..."), "meta.xml");
		if(!UpdateMetaXml())
			ErrorLog::Instance()->AddEntry("meta.xml", tr("Update failed"));
		usleep(1300000);
	}
	
	if(Settings.UpdateIcon > 0)
	{
		update_window->ShowThrobber(tr("Downloading..."), "icon.png");
		if(!UpdateIconPNG())
			ErrorLog::Instance()->AddEntry("icon.png", tr("Update failed"));
		usleep(1000000);
	}
	
	if(Settings.UpdateLanguage > 0)
	{
		if (Settings.UpdateLanguage == LANG_UPDATE_ALL)
		{
			if(!DownloadAllLanguages(update_window))
				ErrorLog::Instance()->AddEntry(tr("All Languages Files"), tr("Update failed"));
		
		}
		else if (Settings.UpdateLanguage == LANG_UPDATE_INSTALLED)
		{
			if(!UpdateLanguages(update_window))
				ErrorLog::Instance()->AddEntry(tr("Installed Languages Files"), tr("Update failed"));
		}
	}
	
	if(Settings.UpdateGameTDB)
	{
		std::string text = tr("Downloading from");
		text += " gametdb.com";
		update_window->ShowThrobber(text, "wiitdb.zip");
		if(!UpdateGameTDB(update_window))
			ErrorLog::Instance()->AddEntry("wiitdb.zip", tr("Update failed"));
	}
	
	if(ErrorLog::Instance()->GetCount() > 0)
	{
		int choice = update_window->Choose(tr("Error"), tr("Show error log?"), tr("OK"), tr("Cancel"));
		if(choice == 1)
			ErrorLog::Instance()->Show();
	}
	
	if(newWindow)
	{
		Application::Instance()->PushForDelete(update_window);
		Application::Instance()->updateEvents();
	}

	return ((ErrorLog::Instance()->GetCount() > 0) ? false : true);
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
bool UpdateGameTDB(UpdateWindow * updateWindow)
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
	updateWindow->ShowThrobber(tr("Extracting files..."), "wiitdb.zip");
	ZipFile *zipfile = new ZipFile(zippath);

	if(zipfile->ExtractAll(Settings.GameTDBPath) < 0)
		return false;
	
	delete zipfile;
	
	RemoveFile(zippath);

	return true;
}

/****************************************************************************
 * Download/Update all Languages Files found on SVN
 ***************************************************************************/
bool DownloadAllLanguages(UpdateWindow * updateWindow)
{
	if(updateWindow)
		updateWindow->ShowProgress(tr("Downloading..."), tr("All Languages File"));
	
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
		
		if(updateWindow)
			updateWindow->ShowProgress(tr("Downloading..."), filename, i, listsize);
		
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
bool UpdateLanguages(UpdateWindow * updateWindow)
{
	if(updateWindow)
		updateWindow->ShowProgress(tr("Downloading..."), tr("Installed Languages Files"));
	
	if(!CreateSubfolder(Settings.LanguagePath))
		return false;

	DirList Dir(Settings.LanguagePath, ".lang");

	if (Dir.GetFilecount() == 0) return false;

	char savepath[150];
	char codeurl[200];
	
	int fileCount = Dir.GetFilecount();
	for(int i = 0; i < fileCount; ++i)
	{
		std::string fileName = Dir.GetFilename(i);
		
		if(updateWindow)
			updateWindow->ShowProgress(tr("Downloading..."), fileName, i, fileCount);
		
		snprintf(codeurl, sizeof(codeurl), "%s%s", LanguageFilesURL, fileName.c_str());
		snprintf(savepath, sizeof(savepath), "%s/%s", Settings.LanguagePath, fileName.c_str());
		
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