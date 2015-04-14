/****************************************************************************
 * languagefile updater
 * for USB Loader GX    *giantpune*
 ***************************************************************************/
#include "UpdateLanguage.h"
#include "LanguagesSelectBrowser.hpp"
#include "CLangList.hpp"
#include "../Themes/CTheme.h"
#include "../FileOperations/fileops.h"
#include "../FileOperations/DirList.h"
#include "../Controls/Application.h"
#include "../Network/networkops.h"
#include "../Network/URL_List.h"
#include "../Prompts/ProgressWindow.h"
#include "../Prompts/PromptWindows.h"

static const char * LanguageFilesURL = "http://savegame-manager-gx.googlecode.com/svn/trunk/Languages/";

int DownloadAllLanguageFiles(std::string langPath)
{
	if(!CreateSubfolder(langPath))
	{
		ShowError(tr("Could not create path: %s"), langPath.c_str());
		return -1;
	}

	if(!NetworkInitPrompt())
	{
		ShowError(tr("Network is not initiated."));
		return -2;
	}

	char fullURL[300];

	URL_List LinkList(LanguageFilesURL);
	int listsize = LinkList.GetURLCount();
	int done = 0;

	StartProgress(tr("Downloading all Language Files:"));
	
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
		
		ShowProgress(i, listsize, filename);
		
		snprintf(fullURL, sizeof(fullURL), "%s%s", LanguageFilesURL, filename);
		
		struct block file = downloadfile(fullURL);
		if (file.data)
		{
			char filepath[300];
			snprintf(filepath, sizeof(filepath), "%s/%s", langPath.c_str(), filename);
			FILE * pfile = fopen(filepath, "wb");
			if(pfile)
			{
				fwrite(file.data, 1, file.size, pfile);
				fclose(pfile);
				done++;
			}
			free(file.data);
		}
	}

	FinishProgress();
	StopProgress();

	return done;
}

int UpdateLanguageFiles(std::string langPath)
{
	if(!CreateSubfolder(langPath))
	{
		ShowError(tr("Could not create path: %s"), langPath.c_str());
		return -1;
	}

	if(!NetworkInitPrompt())
	{
		ShowError(tr("Network is not initiated."));
		return -2;
	}

	DirList Dir(langPath, ".lang");

	if(!Dir.GetFilecount())
		return 0;

	char savepath[150];
	char codeurl[200];

	int done = 0;
	
	StartProgress(tr("Updating Language Files:"));

	for(int i = 0; i < Dir.GetFilecount(); ++i)
	{
		ShowProgress(i, Dir.GetFilecount(), Dir.GetFilename(i));
		
		snprintf(codeurl, sizeof(codeurl), "%s%s", LanguageFilesURL, Dir.GetFilename(i));
		snprintf(savepath, sizeof(savepath), "%s/%s", langPath.c_str(), Dir.GetFilename(i));
		
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
	
	FinishProgress();
	StopProgress();

	return done;
}

int SelectLanguageFiles(std::string langPath)
{
	if(!CreateSubfolder(langPath))
	{
		ShowError(tr("Could not create path: %s"), langPath.c_str());
		return -1;
	}

	if(!NetworkInitPrompt())
	{
		ShowError(tr("Network is not initiated."));
		return -2;
	}

	URL_List * LinkList = new URL_List(LanguageFilesURL);
	if (!LinkList->GetURLCount())
		return 0;

	LangList.Get(LinkList);
	
	delete LinkList;
	LinkList = NULL;
	
	if (!LangList.GetCount())
		return 0;
	
	int neededCount = languagesSelectBrowser();
	if (!neededCount)
		return 0;
	
	char savepath[150];
	char langurl[200];

	int downloaded_files = 0;
	
	StartProgress(tr("Downloading Language Files:"));
	
	for (int i = 0; i < LangList.GetCount(); i++)
	{
		ShowProgress(downloaded_files + 1, neededCount, LangList.GetFilename(i));
		
		if (LangList.Needed(i))
		{
			snprintf(langurl, sizeof(langurl), "%s%s", LanguageFilesURL, LangList.GetFilename(i));
			snprintf(savepath, sizeof(savepath), "%s/%s", langPath.c_str(), LangList.GetFilename(i));
			
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
	
	FinishProgress();
	StopProgress();
	
	return downloaded_files;
}
