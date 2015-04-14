/***************************************************************************
 * Copyright (C) 2010
 * by Dj_skual
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
 * mii_tools.cpp
 * Miis operations for the SaveGame Manager GX
 *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gccore.h>
#include <dirent.h>

#include "prompts/ProgressWindow.h"
#include "prompts/promptwindows.h"
#include "prompts/browsers/browsers.h"
#include "file/fileops.h"
#include "mii/mii_tools.h"
#include "mii/mii_list.h"
#include "mii/mii_installer.h"
#include "mii/manage_mii/manage_mii.h"
#include "saves/manage/ManageButtons.h"
#include "saves/utils.h"
#include "tools/StringTools.h"
#include "menu/MainWindow.h"
#include "main.h"

#define DAY				86400

ManageButtons MiiSucces(0,
						0,
						tr("OK"),
						0,
						0,
						0);

ManageButtons OverWrite(tr("is already installed."),
						0,
						tr("OverWrite"),
						tr("Duplicate"),
						0,
						tr("Cancel"));

ManageButtons MiiError(tr("Error"),
					   0,
					   tr("OK"),
					   0,
					   0,
					   0);

void fix_miiName(char * filename)
{
	char * temp = NULL;
	
	temp = str_replace(filename, ":", "_");
	if(temp != NULL)
		strcpy(filename, temp);
	
	temp = str_replace(filename, "?", "_");
	if(temp != NULL)
		strcpy(filename, temp);
	
}

/****************************************************************************
 * ExtractMii
 *
 * Extract the selected mii to destination filepath
 ***************************************************************************/
int ExtractMii(const char * extractPath)
{
	bool exit = false;
	
	CreateSubfolder(extractPath);
	
	// Get extract file name
	char dest[strlen(extractPath)+MII_NAME_LENGTH*4+6];
	fix_miiName(miiList[selectedMii].displayname);
    snprintf(dest, sizeof(dest), "%s%s.miigx", extractPath, miiList[selectedMii].displayname);
	
    // Open Wii Mii File
	s32 miisrc = ISFS_Open(MII_FILE, ISFS_OPEN_READ);
	if (miisrc >= 0) {
		
		// got to extracted mii offset
		int m = miiList[selectedMii].reference;
		ISFS_Seek(miisrc, MII_HEADER+(m*MII_SIZE), 0);
		
		u32 * memory = NULL;
		memory = (u32*)memalign(32, round_up(MII_SIZE, 32));
		
		if(!memory) {
			ISFS_Close(miisrc);
			goto error;
        }
		
        // Read Mii
		int ret = ISFS_Read(miisrc, memory, MII_SIZE);
		if (ret != MII_SIZE) {
			ISFS_Close(miisrc);
			free(memory);
			memory = NULL;
			goto error;
		}
		
		ISFS_Close(miisrc);
		
		// Open destination file
		FILE * miidest = fopen(dest, "wb");
		if(!miidest) {
			free(memory);
			memory = NULL;
			goto error;
		}
		
		// write file
		fwrite(memory, 1, MII_SIZE, miidest);
		fclose(miidest);
		free(memory);
		memory = NULL;
		
		navinfo.refresh = true;
		
    } else {
		goto error;
    }
	
	MiiSucces.SetMessage(tr("Succesfully extracted")),
	
	MainWindow::Instance()->HaltGui();
	MainWindow::Instance()->Append(&MiiSucces);
	MainWindow::Instance()->ResumeGui();
	
	while(MiiSucces.GetEffect() > 0) usleep(THREAD_SLEEP);
	
	exit = false;
	while(!exit)
	{
		if(MiiSucces.GetState() == STATE_CLICKED) {
			MiiSucces.ResetState();
			exit = true;
		}
	}
	
	while(MiiSucces.GetEffect() > 0) usleep(THREAD_SLEEP);
	
	MainWindow::Instance()->HaltGui();
	MainWindow::Instance()->Remove(&MiiSucces);
	MainWindow::Instance()->ResumeGui();
	
	windowinfo.move = ManageWindow::NO_MOVE_IN;
	
	return 1;// succes
	
  error:
	
	MiiError.SetMessage(tr("Unable to extract the selected Mii"));
	
	MainWindow::Instance()->HaltGui();
	MainWindow::Instance()->Append(&MiiError);
	MainWindow::Instance()->ResumeGui();
	
	while(MiiError.GetEffect() > 0) usleep(THREAD_SLEEP);
	
	exit = false;
	while(!exit)
	{
		if(MiiError.GetState() == STATE_CLICKED) {
			MiiError.ResetState();
			exit = true;
		}
	}
	
	while(MiiError.GetEffect() > 0) usleep(THREAD_SLEEP);
	
	MainWindow::Instance()->HaltGui();
	MainWindow::Instance()->Remove(&MiiError);
	MainWindow::Instance()->ResumeGui();

	windowinfo.move = ManageWindow::NO_MOVE_IN;
	
	return 0;//failed
}

/****************************************************************************
 * readMii
 *
 * Read the Mii file on the device
 ***************************************************************************/
char * readMii(char * path){
	FILE * File;
	char * buffer;
	File = fopen(path, "rb");
	if (File==NULL) {
		return NULL;
	}
	buffer = (char*) malloc (sizeof(char)*MII_SIZE);
	fread(buffer,1,MII_SIZE,File);
	fclose (File);
	return buffer;
}

/****************************************************************************
 * WriteMii
 *
 * Search about first free place then install mii
 ***************************************************************************/
int WriteMii(char *Path)
{
	bool exit = false, freeSpaceOk = false;
	int reference = 0, n = 0;
	
	//get first free space
	for(reference = 0; reference < MII_MAX; reference++)
	{
		n = 0;
		freeSpaceOk = true;
		for(n=0; n<miiListInfo.Cnt; n++)
		{
			if(miiList[n].reference == reference)
			{
				freeSpaceOk = false;
				break;
			}
		}
		if(freeSpaceOk == true)
			break;
	}
	
	if(reference >= MII_MAX) {
		MiiError.SetMessage(tr("Mii memory is full."));
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&MiiError);
		MainWindow::Instance()->ResumeGui();
		
		while(MiiError.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(MiiError.GetState() == STATE_CLICKED) {
				MiiError.ResetState();
				exit = true;
			}
		}
		
		while(MiiError.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&MiiError);
		MainWindow::Instance()->ResumeGui();
		
		windowinfo.move = ManageWindow::NO_MOVE_IN;
		
		return 0;
	}
	
	return install_mii(Path, reference);	
}

/****************************************************************************
 * InstallMii
 *
 * Install the selected mii to Wii memory
 ***************************************************************************/
int InstallMii()
{
	bool exit = false;
	
	char miiPath[200];
	snprintf(miiPath, sizeof(miiPath), "%s%s", navinfo.LastBrowsedPath, browserDevice[selectedMii].filename);
	
	//Check if same mii already installed in wii
	int n = 0, choice = 0;
	int reference = 0; // reference to the place of mii in wii file
	bool overwriteOk = false;
	for(n = 0; n < miiListInfo.Cnt; n++)
	{
		if(!strcmp(miiList[n].displayname, browserDevice[selectedMii].displayname)){
			MainWindow::Instance()->HaltGui();
			MainWindow::Instance()->Append(&OverWrite);
			MainWindow::Instance()->ResumeGui();
			
			while(OverWrite.GetEffect() > 0) usleep(THREAD_SLEEP);
			
			exit = false;
			while(!exit)
			{
				if(OverWrite.GetState() == STATE_CLICKED) {
					choice = OverWrite.GetChoice();
					OverWrite.ResetState();
					
					if(choice != 4) {
						if(choice == 1) {
							overwriteOk = true ;
							reference = miiList[n].reference;
						}
					}
					
					n = miiListInfo.Cnt;
					exit = true;
					
					while(OverWrite.GetEffect() > 0) usleep(THREAD_SLEEP);
					
					MainWindow::Instance()->HaltGui();
					MainWindow::Instance()->Remove(&OverWrite);
					MainWindow::Instance()->ResumeGui();
					
					windowinfo.move = ManageWindow::NO_MOVE_IN;
				}
			}
		}
	}
	
	int result = 0;
	if(choice != 4) {
		if(overwriteOk)
			result = install_mii(miiPath, reference);
		else
			result = WriteMii(miiPath);
	}
	
	if(result) {
		MiiSucces.SetMessage(tr("Succesfully installed")),
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&MiiSucces);
		MainWindow::Instance()->ResumeGui();
		
		while(MiiSucces.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(MiiSucces.GetState() == STATE_CLICKED) {
				MiiSucces.ResetState();
				exit = true;
			}
		}
		
		while(MiiSucces.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&MiiSucces);
		MainWindow::Instance()->ResumeGui();
		
		windowinfo.move = ManageWindow::NO_MOVE_IN;
	}
	else if(!result && choice != 4) {
		MiiError.SetMessage(tr("Unable to install the selected Mii"));
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&MiiError);
		MainWindow::Instance()->ResumeGui();
		
		while(MiiError.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(MiiError.GetState() == STATE_CLICKED) {
				MiiError.ResetState();
				exit = true;
			}
		}
		
		while(MiiError.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&MiiError);
		MainWindow::Instance()->ResumeGui();
		
		windowinfo.move = ManageWindow::NO_MOVE_IN;
	}
	
	CreateMiiList();
	navinfo.refresh = true;
	
	return result;
}
/****************************************************************************
 * DeleteDeviceMii
 *
 * Delete the selected mii from device
 ***************************************************************************/
int DeleteDeviceMii()
{
	bool exit = false;
	
	char miiPath[200];
	snprintf(miiPath, sizeof(miiPath), "%s%s", navinfo.LastBrowsedPath, browserDevice[selectedMii].filename);
	
	if(!RemoveFile(miiPath)) {
		MiiError.SetMessage(tr("Unable to delete the selected Mii"));
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&MiiError);
		MainWindow::Instance()->ResumeGui();
		
		while(MiiError.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(MiiError.GetState() == STATE_CLICKED) {
				MiiError.ResetState();
				exit = true;
			}
		}
		
		while(MiiError.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&MiiError);
		MainWindow::Instance()->ResumeGui();

		windowinfo.move = ManageWindow::NO_MOVE_IN;
		
		return 0;
	}
	
	MiiSucces.SetMessage(tr("Succesfully deleted")),
	
	MainWindow::Instance()->HaltGui();
	MainWindow::Instance()->Append(&MiiSucces);
	MainWindow::Instance()->ResumeGui();
	
	while(MiiSucces.GetEffect() > 0) usleep(THREAD_SLEEP);
	
	exit = false;
	while(!exit)
	{
		if(MiiSucces.GetState() == STATE_CLICKED) {
			MiiSucces.ResetState();
			exit = true;
		}
	}
	
	while(MiiSucces.GetEffect() > 0) usleep(THREAD_SLEEP);
	
	MainWindow::Instance()->HaltGui();
	MainWindow::Instance()->Remove(&MiiSucces);
	MainWindow::Instance()->ResumeGui();
	
	windowinfo.move = ManageWindow::NO_MOVE_IN;
	windowinfo.refreshType = ManageWindow::DELETE;
	
	windowinfo.refresh = true;
	navinfo.refresh = true;
	
	return 1;
}
/****************************************************************************
 * DeleteNandMii
 *
 * Delete the selected mii from the Wii memeory
 ***************************************************************************/
int DeleteNandMii()
{
	bool exit = false;
	
	if(!install_mii("DELETE", miiList[selectedMii].reference)) {
		MiiError.SetMessage(tr("Unable to delete the selected Mii"));
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&MiiError);
		MainWindow::Instance()->ResumeGui();
		
		while(MiiError.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(MiiError.GetState() == STATE_CLICKED) {
				MiiError.ResetState();
				exit = true;
			}
		}
		
		while(MiiError.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&MiiError);
		MainWindow::Instance()->ResumeGui();
		
		windowinfo.move = ManageWindow::NO_MOVE_IN;
		
		return 0;
	}
	
	MiiSucces.SetMessage(tr("Succesfully deleted")),
	
	MainWindow::Instance()->HaltGui();
	MainWindow::Instance()->Append(&MiiSucces);
	MainWindow::Instance()->ResumeGui();
	
	while(MiiSucces.GetEffect() > 0) usleep(THREAD_SLEEP);
	
	exit = false;
	while(!exit)
	{
		if(MiiSucces.GetState() == STATE_CLICKED) {
			MiiSucces.ResetState();
			exit = true;
		}
	}
	
	while(MiiSucces.GetEffect() > 0) usleep(THREAD_SLEEP);
	
	MainWindow::Instance()->HaltGui();
	MainWindow::Instance()->Remove(&MiiSucces);
	MainWindow::Instance()->ResumeGui();
	
	windowinfo.move = ManageWindow::NO_MOVE_IN;
	windowinfo.refreshType = ManageWindow::DELETE;
	
	windowinfo.refresh = true;
	navinfo.refresh = true;
	
	return 1;
}

/****************************************************************************
 * CopyMii
 *
 * Copy the selected mii to the selected path on storage devices
 ***************************************************************************/
int CopyMii()
{
	bool exit = false;
	
	char miiSourcePath[200];
	char miiSource[200];
	char miiDestpath[200];
	char miiDest[200];
	snprintf(miiSourcePath, sizeof(miiSourcePath), "%s", navinfo.LastBrowsedPath);
	snprintf(miiSource, sizeof(miiSource), "%s%s", navinfo.LastBrowsedPath, browserDevice[selectedMii].filename);
	
	if(BrowseCustom(miiSourcePath, miiDestpath)) {
		if(!strcmp(miiSourcePath, miiDestpath)) {
			MiiError.SetMessage(tr("Sorry, you can't copy to the same path"));
			
			MainWindow::Instance()->HaltGui();
			MainWindow::Instance()->Append(&MiiError);
			MainWindow::Instance()->ResumeGui();
			
			while(MiiError.GetEffect() > 0) usleep(THREAD_SLEEP);
			
			exit = false;
			while(!exit)
			{
				if(MiiError.GetState() == STATE_CLICKED) {
					MiiError.ResetState();
					exit = true;
				}
			}
			
			while(MiiError.GetEffect() > 0) usleep(THREAD_SLEEP);
			
			MainWindow::Instance()->HaltGui();
			MainWindow::Instance()->Remove(&MiiError);
			MainWindow::Instance()->ResumeGui();
			
			windowinfo.move = ManageWindow::NO_MOVE_IN;
			
			return 0;
		}
	}
	else
		return 0;
	
	snprintf(miiDest, sizeof(miiDest), "%s%s", miiDestpath, browserDevice[selectedMii].filename);
	
	if(CopyFile(miiSource, miiDest) < 0) {
		MiiError.SetMessage(tr("Unable to copy the selected Mii"));
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&MiiError);
		MainWindow::Instance()->ResumeGui();
		
		while(MiiError.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(MiiError.GetState() == STATE_CLICKED) {
				MiiError.ResetState();
				exit = true;
			}
		}
		
		while(MiiError.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&MiiError);
		MainWindow::Instance()->ResumeGui();
		
		windowinfo.move = ManageWindow::NO_MOVE_IN;
		
		return 0;
	}
	
	MiiSucces.SetMessage(tr("Succesfully copied")),
	
	MainWindow::Instance()->HaltGui();
	MainWindow::Instance()->Append(&MiiSucces);
	MainWindow::Instance()->ResumeGui();
	
	while(MiiSucces.GetEffect() > 0) usleep(THREAD_SLEEP);
	
	exit = false;
	while(!exit)
	{
		if(MiiSucces.GetState() == STATE_CLICKED) {
			MiiSucces.ResetState();
			exit = true;
		}
	}
	
	while(MiiSucces.GetEffect() > 0) usleep(THREAD_SLEEP);
	
	MainWindow::Instance()->HaltGui();
	MainWindow::Instance()->Remove(&MiiSucces);
	MainWindow::Instance()->ResumeGui();
	
	windowinfo.move = ManageWindow::NO_MOVE_IN;
	navinfo.refresh = true;
	
	return 1;
}

/****************************************************************************
 * MoveMii
 *
 * Move the selected mii to the selected path on storage devices
 ***************************************************************************/
int MoveMii()
{
	bool exit = false;
	
	char miiSourcePath[200];
	char miiSource[200];
	char miiDestpath[200];
	char miiDest[200];
	snprintf(miiSourcePath, sizeof(miiSourcePath), "%s", navinfo.LastBrowsedPath);
	snprintf(miiSource, sizeof(miiSource), "%s%s", navinfo.LastBrowsedPath, browserDevice[selectedMii].filename);
	
	if(BrowseCustom(miiSourcePath, miiDestpath)) {
		if(!strcmp(miiSourcePath, miiDestpath)) {
			MiiError.SetMessage(tr("Sorry, you can't move to the same path"));
			
			MainWindow::Instance()->HaltGui();
			MainWindow::Instance()->Append(&MiiError);
			MainWindow::Instance()->ResumeGui();
			
			while(MiiError.GetEffect() > 0) usleep(THREAD_SLEEP);
			
			exit = false;
			while(!exit)
			{
				if(MiiError.GetState() == STATE_CLICKED) {
					MiiError.ResetState();
					exit = true;
				}
			}
			
			while(MiiError.GetEffect() > 0) usleep(THREAD_SLEEP);
			
			MainWindow::Instance()->HaltGui();
			MainWindow::Instance()->Remove(&MiiError);
			MainWindow::Instance()->ResumeGui();
			
			windowinfo.move = ManageWindow::NO_MOVE_IN;
			
			return 0;
		}
	}
	else
		return 0;
	
	snprintf(miiDest, sizeof(miiDest), "%s%s", miiDestpath, browserDevice[selectedMii].filename);
	
	if(MoveFile(miiSource, miiDest) < 0) {
		MiiError.SetMessage(tr("Unable to move the selected Mii"));
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&MiiError);
		MainWindow::Instance()->ResumeGui();
		
		while(MiiError.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(MiiError.GetState() == STATE_CLICKED) {
				MiiError.ResetState();
				exit = true;
			}
		}
		
		while(MiiError.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&MiiError);
		MainWindow::Instance()->ResumeGui();
		
		windowinfo.move = ManageWindow::NO_MOVE_IN;
		
		return 0;
	}
	
	MiiSucces.SetMessage(tr("Succesfully moved")),
	
	MainWindow::Instance()->HaltGui();
	MainWindow::Instance()->Append(&MiiSucces);
	MainWindow::Instance()->ResumeGui();
	
	while(MiiSucces.GetEffect() > 0) usleep(THREAD_SLEEP);
	
	exit = false;
	while(!exit)
	{
		if(MiiSucces.GetState() == STATE_CLICKED) {
			MiiSucces.ResetState();
			exit = true;
		}
	}
	
	while(MiiSucces.GetEffect() > 0) usleep(THREAD_SLEEP);
	
	MainWindow::Instance()->HaltGui();
	MainWindow::Instance()->Remove(&MiiSucces);
	MainWindow::Instance()->ResumeGui();
	
	windowinfo.move = ManageWindow::NO_MOVE_IN;
	windowinfo.refreshType = ManageWindow::DELETE;
	
	windowinfo.refresh = true;
	navinfo.refresh = true;
	
	return 1;
}
