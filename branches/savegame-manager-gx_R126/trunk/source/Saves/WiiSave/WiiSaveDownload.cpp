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
 * WiiSaveDownload.cpp
 *
 * for SaveGame Manager GX 2012
 ***************************************************************************/
#include <unistd.h>

#include "WiiSaveDownload.hpp"
#include "WiiSave_List.h"
#include "WiiSaveBrowser.hpp"
#include "../../ArchiveOperations/Archive.h"
#include "../../Manage/ManageButtons.hpp"
#include "../../Manage/ManageProgress.h"
#include "../../Settings/CSettings.h"
#include "../../Prompts/SelectBrowser.h"
#include "../../FileOperations/fileops.h"
#include "../../FileOperations/DirList.h"
#include "../DataBin/SaveDataBin.h"
#include "../../Tools/StringTools.h"
#include "../../Language/gettext.h"
#include "../../Network/networkops.h"
#include "../../Network/http.h"
#include "../../Network/HTML_Stream.h"
#include "../../Menu/menus.h"

int WiiSave::Download(std::string ID)
{
	if(ID.empty())
		return -1;
	
	//! Check Network
	if (!IsNetworkInit())
	{
		ManageProgressStop();
		
		ManageButtons * connect = new ManageButtons(tr("No network connection"),
													tr("Do you want to connect?"),
													tr("Yes"),
													tr("Cancel"));
		
		connect->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
		connect->SetPosition(-6, 75);
		connect->SetEffect(EFFECT_FADE, 20);
		mainWindow->Append(connect);
		ResumeGui();
		
		while (connect->GetEffect()) usleep(50);
		
		while(!connect->GetChoice()) usleep(50);
		
		connect->SetEffect(EFFECT_FADE, -20);
		
		while (connect->GetEffect()) usleep(50);
		
		HaltGui();
		
		mainWindow->Remove(connect);
		
		ResumeGui();
		
		if(connect->GetChoice() == 1)
		{
			StartManageProgress(tr("Initializing Network"));
			
			Initialize_Network();
			
			ManageProgressStop();
		}
		
		delete connect;
		
		if (!IsNetworkInit())
			return -2;
		
		StartManageProgress(tr("Downloading saveslist:"), "www.wiisave.com");
	}

	//! Generate gameUrl
	std:: string WiiSaveListURL("http://wiisave.com/savegamecode_xml.php?gamecode=");
	WiiSaveListURL += ID.substr(0, 3);
	
	//! Check Connection
	if(!CheckConnection(WiiSaveListURL.c_str()))
	{
		ManageProgressStop();
		return -3;
	}
	
	//! Check Region
	std::string Region;
	switch (ID[3])
	{
		case 'E':
			Region = "E - USA / Canada";
			break;
		case 'J':
			Region = "J - Japanese";
			break;
		case 'W':
			Region = "J - Japanese";//Region = "NTSC T";
			break;
		case 'K':
			Region = "J - Japanese";//Region = "NTSC K";
			break;
		default:
		case 'P':
		case 'D':
		case 'F':
		case 'I':
		case 'S':
		case 'H':
		case 'U':
		case 'X':
		case 'Y':
		case 'Z':
			Region = "P - European / Other / PAL";
			break;
	}
	
	//! Get list
	WiiSave_List * SaveList = new WiiSave_List(WiiSaveListURL.c_str());

	ManageProgressStop();
	
	if (!SaveList->GetCount())
	{
		delete SaveList;
		return -4;
	}
	
	if (!SaveList->FilterList(Region))
	{
		delete SaveList;
		return -5;
	}
	
	//! open browser & select save
	WiiSaveBrowser * SaveBrowser = new WiiSaveBrowser(SaveList);
	mainWindow->Append(SaveBrowser);
	
	SaveBrowser->Show();
	
	int selected = SaveBrowser->GetSelectedSave();
	delete SaveBrowser;
	
	if (selected < 0)
	{
		delete SaveList;
		return -10;
	}
	
	//! select download destination
	ManageButtons * path = new ManageButtons(tr("Select a target path."),
											 tr("Be carrefull, present files can be overwritten"),
											 tr("OK"));
	
	path->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	path->SetPosition(-6, 75);
	path->SetEffect(EFFECT_FADE, 20);
	mainWindow->Append(path);
	ResumeGui();
	while (path->GetEffect()) usleep(50);
	
	while(!path->GetChoice()) usleep(50);
	
	path->SetEffect(EFFECT_FADE, -20);
	
	while (path->GetEffect()) usleep(50);
	
	HaltGui();
	
	mainWindow->Remove(path);
	delete path;
	
	ResumeGui();
	
	if( selectBrowser(Settings.BrowserPath.c_str(), PATH) != 2 )
	{
		delete SaveList;
		return -10;
	}
	
	//! download file
	std::string filepath = Settings.TmpPath;
	filepath += "/";
	filepath += SaveList->GetFilteredFilename(selected);
	
	if( WiiSave::InternalDownload(SaveList->GetFilteredDownloadLink(selected), filepath) < 0)
	{
		delete SaveList;
		return -6;
	}
	
	delete SaveList;
	
	//! check file type
	char destID[5];
	std::string srcfilepath;
	bool isSgmgxSave = false;
	bool isDataBin = false;
	
	std::string tmpDest = filepath.substr(0, filepath.rfind("/")+1);
	tmpDest += "tmpExtract";
	
	std::string fileext = filepath.substr(filepath.rfind("."));
	if( !Settings.FileExtensions.CompareArchive(fileext.c_str()) ) //! Archive
	{
		StartManageProgress(tr("Extracting files..."));
		
		//! extract files
		ArchiveHandle * Archive = new ArchiveHandle(filepath.c_str());
		
		Archive->ExtractAll(tmpDest.c_str());
		
		delete Archive;
		RemoveFile(filepath.c_str());
		
		//! check save type
		DirList * dirList = new DirList(tmpDest.c_str(), ".bin" , DirList::Files | DirList::Dirs | DirList::CheckSubfolders);
		
		for(int i = 0; i < dirList->GetFilecount(); i++)
		{
			if(!strcmp(dirList->GetFilename(i), "banner.bin"))
			{
				isSgmgxSave = true;
				srcfilepath = dirList->GetFilepath(i);
				break;
			}
			
			std::string name(dirList->GetFilename(i));
			fileext = name.substr(name.rfind("."));
			if( !Settings.FileExtensions.CompareBinaryFiles(fileext.c_str()) )
			{
				SaveInfos * infos = GetSaveInfos(dirList->GetFilepath(i));
				if(infos)
				{
					isDataBin = true;
					srcfilepath = dirList->GetFilepath(i);
					memcpy(destID, infos->ID, 5);
					delete infos;
					break;
				}
			}
		}
		
		delete dirList;
		
		ManageProgressStop();
	}
	else //! check for data.bin file
	{
		StartManageProgress(tr("Verifying file..."));
		
		SaveInfos * infos = GetSaveInfos(filepath);
		if(infos)
		{
			isDataBin = true;
			srcfilepath = filepath;
			memcpy(destID, infos->ID, 5);
			delete infos;
		}
		
		ManageProgressStop();
	}
	
	if(!isDataBin && !isSgmgxSave) //! supported files not found
	{
		RemoveFile(filepath.c_str());
		RemoveDirectory(tmpDest.c_str());
		return -7;
	}
	
	//! write files
	StartManageProgress(tr("Writing files..."));
	if(isSgmgxSave)
	{
		std::string srcfolder = srcfilepath.substr(0, srcfilepath.rfind("/"));
		
		DirList * dirList = new DirList(srcfolder.c_str(), NULL , DirList::Files | DirList::Dirs);
		
		srcfolder = srcfolder.substr(srcfolder.rfind("/"));
		std::string finaldest = Settings.TmpPath;
		finaldest += srcfolder;
		
		CreateSubfolder(finaldest.c_str());
		
		for(int i = 0; i < dirList->GetFilecount(); i++)
		{
			if(IsDir(i))
				CopyDirectory(dirList->GetFilepath(i), finaldest.c_str());
			else
				CopyFile(dirList->GetFilepath(i),  fmt("%s/%s", finaldest.c_str(), dirList->GetFilename(i)));
		}
		
		delete dirList;
	}
	else if(isDataBin)
	{
		std::string finaldest = Settings.TmpPath;
		finaldest += "/";
		finaldest += destID;
		
		CreateSubfolder(finaldest.c_str());
		
		finaldest += "/data.bin";
		CopyFile(srcfilepath.c_str(), finaldest.c_str());
	}
	
	RemoveFile(filepath.c_str());
	RemoveDirectory(tmpDest.c_str());
	ManageProgressStop();
	
	return 1;
}

int WiiSave::InternalDownload(std::string url, std::string filepath)
{
	if (url.empty()) return -1;
	
	StartManageProgress(tr("Downloading file..."), "www.wiisave.com");
	
	HTML_Stream HTML(url.c_str());

	HTML.FindStringEnd("location='");
	char * location = HTML.CopyString("'");
	if(location)
	{
		url = url.erase(23);
		url += location;
		
		free(location);
	}
	else
	{
		ManageProgressStop();
		return -2;
	}
	
	struct block file = downloadfile(url.c_str());
	if (!file.data || !file.size)
	{
		ManageProgressStop();
		return -3;
	}

	FILE * File = fopen(filepath.c_str(), "wb");
	if(!File)
	{
		ManageProgressStop();
		return -4;
	}
	
	fwrite(file.data, 1, file.size, File);
	
	fclose(File);
	ManageProgressStop();
	
	return 1;
}
