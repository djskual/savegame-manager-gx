/****************************************************************************
 * languagefile updater
 * for USB Loader GX    *giantpune*
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "UpdateLanguage.h"
#include "LanguagesSelectBrowser.hpp"
#include "CLangList.hpp"
#include "gettext.h"
#include "../FileOperations/fileops.h"
#include "../FileOperations/DirList.h"
#include "../menu.h"
#include "../Network/networkops.h"
#include "../Network/http.h"
#include "../Network/URL_List.h"
#include "../Prompts/ProgressWindow.h"
#include "../Prompts/PromptWindows.h"
#include "../Tools/ShowError.h"

static const char * LanguageFilesURL = "http://savegame-manager-gx.googlecode.com/svn/trunk/Languages/";

int DownloadAllLanguageFiles(const char * langPath)
{
	if(!CreateSubfolder(langPath))
	{
		ShowError(tr( "Could not create path: %s" ), langPath);
		return -1;
	}

	if(!IsNetworkInit())
	{	
		if(!NetworkInitPrompt())
		{
			ShowError(tr( "Network is not initiated." ));
			return -2;
		}
	}
	
	char fullURL[300];

	URL_List LinkList(LanguageFilesURL);
	int listsize = LinkList.GetURLCount();
	int files_downloaded = 0;

	ShowProgress(tr( "Downloading all Languages Files:" ), 0, 0, 0, listsize, false, true);

	for (int i = 0; i < listsize; i++)
	{
		const char * filename = strrchr(LinkList.GetURL(i), '/');
		if(filename) filename++;
		else filename = LinkList.GetURL(i);
		
		if(!filename)
			continue;
		
		const char * FileExt = strrchr(filename, '.');
		if (!FileExt || strcasecmp(FileExt, ".lang") != 0)
			continue;
		
		ShowProgress(tr( "Downloading all Languages Files:" ), 0, filename, i, listsize, false, true);
		
		snprintf(fullURL, sizeof(fullURL), "%s%s", LanguageFilesURL, filename);
		
		struct block file = downloadfile(fullURL);
		if (file.data)
		{
			char filepath[300];
			snprintf(filepath, sizeof(filepath), "%s/%s", langPath, filename);
			FILE * pfile = fopen(filepath, "wb");
			if(pfile)
			{
				fwrite(file.data, 1, file.size, pfile);
				fclose(pfile);
				files_downloaded++;
			}
			free(file.data);
		}
	}

	ProgressStop();

	return files_downloaded;
}

int UpdateLanguageFiles(const char * langPath)
{
	if(!CreateSubfolder(langPath))
	{
		ShowError(tr("Could not create path: %s"), langPath);
		return -1;
	}

	if(!IsNetworkInit())
	{
		if(!NetworkInitPrompt())
		{
			ShowError(tr("Network is not initiated."));
			return -2;
		}
	}

	DirList Dir(langPath, ".lang");

	if (Dir.GetFilecount() == 0) return 0;

	char savepath[150];
	char codeurl[200];

	int done = 0;

	ShowProgress(tr( "Updating Language Files:" ), 0, 0, 0, Dir.GetFilecount(), false, true);
	
	for(int i = 0; i < Dir.GetFilecount(); ++i)
	{
		ShowProgress(tr("Updating Language Files:"), 0, Dir.GetFilename(i), i, Dir.GetFilecount(), false, true);
		
		snprintf(codeurl, sizeof(codeurl), "%s%s", LanguageFilesURL, Dir.GetFilename(i));
		snprintf(savepath, sizeof(savepath), "%s/%s", langPath, Dir.GetFilename(i));
		
		struct block file = downloadfile(codeurl);
		
		if (file.data != NULL)
		{
			FILE * pfile;
			pfile = fopen(savepath, "wb");
			if (pfile != NULL)
			{
				fwrite(file.data, 1, file.size, pfile);
				fclose(pfile);
				done++;
			}
			free(file.data);
		}
	}

	ProgressStop();

	return done;
}

int SelectLanguageFiles(const char * langPath)
{
	if(!CreateSubfolder(langPath))
	{
		ShowError(tr( "Could not create path: %s" ), langPath);
		return -1;
	}

	if(!IsNetworkInit())
	{	
		if(!NetworkInitPrompt())
		{
			ShowError(tr( "Network is not initiated." ));
			return -2;
		}
	}
	
	URL_List * LinkList = new URL_List(LanguageFilesURL);
	if (LinkList->GetURLCount() == 0) return 0;

	LangList.Get(LinkList);
	
	delete LinkList;
	LinkList = NULL;
	
	if (LangList.GetCount() == 0) return 0;
	
	int neededCount = languagesSelectBrowser();
	if (neededCount == 0) return 0;
	
	char savepath[150];
	char langurl[200];

	int downloaded_files = 0;
	
	ShowProgress(tr( "Downloading Language Files:" ), 0, 0, 0, neededCount, false, true);

	for (int i = 0; i < LangList.GetCount(); i++)
	{
		ShowProgress(tr( "Downloading Language Files:" ), 0, LangList.GetFilename(i), downloaded_files + 1, neededCount, false, true);
		
		if (LangList.Needed(i))
		{
			snprintf(langurl, sizeof(langurl), "%s%s", LanguageFilesURL, LangList.GetFilename(i));
			snprintf(savepath, sizeof(savepath), "%s/%s", langPath, LangList.GetFilename(i));
			
			struct block file = downloadfile(langurl);
			
			if (file.data != NULL)
			{
				FILE * pfile;
				pfile = fopen(savepath, "wb");
				if (pfile != NULL)
				{
					fwrite(file.data, 1, file.size, pfile);
					fclose(pfile);
					downloaded_files++;
				}
				free(file.data);
			}
		}
	}
	
	ProgressStop();
	
	return downloaded_files;
}
