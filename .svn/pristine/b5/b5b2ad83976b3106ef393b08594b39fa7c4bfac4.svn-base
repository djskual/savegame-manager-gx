/****************************************************************************
 * 
 * manage_tools.cpp
 *
 * Menu flow routines - handles all menu logic
 ***************************************************************************/

#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ogcsys.h>
#include <unistd.h>
#include <sys/dir.h>
#include <sys/stat.h>

#include "libwiigui/gui.h"
#include "saves/manage/ManageButtons.h"
#include "saves/manage/ManageWindow.h"
#include "saves/manage/ManageProgress.h"
#include "saves/manage/manage.h"
#include "prompts/browsers/browsers.h"
#include "saves/savegame.h"
#include "saves/utils.h"
#include "file/fileops.h"
#include "menu/MainWindow.h"
#include "main.h"

/*** Extern functions ***/
extern void ResumeGui();
extern void HaltGui();

/*** Extern variables ***/
extern GuiWindow * mainWindow;
extern int selected;

void ResetWindowInfo()
{
	windowinfo.refresh = false;
	windowinfo.move = ManageWindow::MOVE_IN;
	windowinfo.refreshType = ManageWindow::NO_TYPE;
	
}


static s32  ret;
static char nandpath[200];
static char devpath[200];

ManageButtons Succes(0,
					 0,
					 tr("OK"),
					 0,
					 0,
					 0);

ManageButtons Cancelled(0,
					   0,
					   tr("OK"),
					   0,
					   0,
					   0);

ManageButtons Error(tr("Error"),
					0,
					tr("OK"),
					0,
					0,
					0);

int SaveInstall()
{
	bool exit = false;
	windowinfo.move = ManageWindow::NO_MOVE_IN;
	char* buffer = (char*)memalign(32, ISFS_MAXPATH);
	
	StartManageProgress(0);
	ShowManageProgress(0, 1, tr("Installing Save"));
	
	// Generate device path //
	snprintf(devpath, sizeof(devpath), "%s%s%s", navinfo.CurrentDevice, browserD.dir, browserDevice[selected].filename);
	
	// Set title UID //
	ret = ES_SetUID(browserDevice[selected].tid);
	if (ret < 0)
	{
		StopManageProgress();
		
		Error.SetMessage(tr("Unable to SetUID"));
		
		goto out;
	}

	// Get NAND path //
	ret = ES_GetDataDir(browserDevice[selected].tid, buffer);
	if (ret < 0)
	{
		StopManageProgress();
		
		Error.SetMessage(tr("Unable to get NAND path"));
		
		goto out;
	}
	snprintf(nandpath, sizeof(nandpath), "%s", buffer);
	free(buffer);
	
	// Install savegame //
	ret = Savegame_Install(devpath, nandpath);
	StopManageProgress();
	if (ret == -10)
	{
		Cancelled.SetMessage(tr("Install cancelled"));
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&Cancelled);
		MainWindow::Instance()->ResumeGui();
		
		while(Cancelled.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(Cancelled.GetState() == STATE_CLICKED) {
				Cancelled.ResetState();
				exit = true;
			}
		}
		
		while(Cancelled.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&Cancelled);
		MainWindow::Instance()->ResumeGui();
		
		windowinfo.move = ManageWindow::NO_MOVE_IN;
		
		return -10; //canceled
	}
	else if (ret < 0)
	{
		Error.SetMessage(tr("Unable to copy data"));
		
		goto out;
	}
	else
	{
		Succes.SetMessage(tr("Succesfully installed")),
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&Succes);
		MainWindow::Instance()->ResumeGui();
		
		while(Succes.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(Succes.GetState() == STATE_CLICKED) {
				Succes.ResetState();
				exit = true;
			}
		}
		
		while(Succes.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&Succes);
		MainWindow::Instance()->ResumeGui();
		
		windowinfo.move = ManageWindow::NO_MOVE_IN;
		
		return 1;//succes
	}
	
  out:
	
	MainWindow::Instance()->HaltGui();
	MainWindow::Instance()->Append(&Error);
	MainWindow::Instance()->ResumeGui();
	
	while(Error.GetEffect() > 0) usleep(THREAD_SLEEP);
	
	exit = false;
	while(!exit)
	{
		if(Error.GetState() == STATE_CLICKED) {
			Error.ResetState();
			exit = true;
		}
	}
	
	while(Error.GetEffect() > 0) usleep(THREAD_SLEEP);
	
	MainWindow::Instance()->HaltGui();
	MainWindow::Instance()->Remove(&Error);
	MainWindow::Instance()->ResumeGui();

	windowinfo.move = ManageWindow::NO_MOVE_IN;
	
	return 0;//failed
}

int SaveExtract(const char * name UNUSED, u64 tid, const char * extractPath)
{
	bool exit = false;
	windowinfo.move = ManageWindow::NO_MOVE_IN;
	char* buffer = (char*)memalign(32, ISFS_MAXPATH);
	
	StartManageProgress(0);
	ShowManageProgress(0, 1, tr("Extracting Save"));
	
	// Generate device path //
	snprintf(devpath, sizeof(devpath), "%s%016llx", extractPath, tid);
	CreateSubfolder(extractPath);
	
	// Set title UID //
	ret = ES_SetUID(tid);
	if (ret < 0)
	{
		StopManageProgress();
		
		Error.SetMessage(tr("Unable to SetUID"));
		
		goto out;
	}

	// Get NAND path //
	ret = ES_GetDataDir(tid, buffer);
	if (ret < 0)
	{
		StopManageProgress();
		
		Error.SetMessage(tr("Unable to get NAND path"));
		
		goto out;
	}
	snprintf(nandpath, sizeof(nandpath), "%s", buffer);
	free(buffer);
	
	// Extract SaveGame //
	ret = Savegame_Extract(nandpath, devpath, devpath);
	if(ret < 0)
		RemoveDirectory(devpath);
	
	StopManageProgress();
	
	if (ret == -10)
	{
		Cancelled.SetMessage(tr("Extract cancelled"));
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&Cancelled);
		MainWindow::Instance()->ResumeGui();
		
		while(Cancelled.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(Cancelled.GetState() == STATE_CLICKED) {
				Cancelled.ResetState();
				exit = true;
			}
		}
		
		while(Cancelled.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&Cancelled);
		MainWindow::Instance()->ResumeGui();
		
		windowinfo.move = ManageWindow::NO_MOVE_IN;
		
		return -10; //canceled
	}
	else if (ret < 0)
	{
		Error.SetMessage(tr("Unable to copy data"));
		
		goto out;
	}
	else
	{
		Succes.SetMessage(tr("Succesfully extracted")),
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&Succes);
		MainWindow::Instance()->ResumeGui();
		
		while(Succes.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(Succes.GetState() == STATE_CLICKED) {
				Succes.ResetState();
				exit = true;
			}
		}
		
		while(Succes.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&Succes);
		MainWindow::Instance()->ResumeGui();
		
		windowinfo.move = ManageWindow::NO_MOVE_IN;
		
		navinfo.refresh = true;
		return 1;//Succes
	}
	
  out:
	
	MainWindow::Instance()->HaltGui();
	MainWindow::Instance()->Append(&Error);
	MainWindow::Instance()->ResumeGui();
	
	while(Error.GetEffect() > 0) usleep(THREAD_SLEEP);
	
	exit = false;
	while(!exit)
	{
		if(Error.GetState() == STATE_CLICKED) {
			Error.ResetState();
			exit = true;
		}
	}
	
	while(Error.GetEffect() > 0) usleep(THREAD_SLEEP);
	
	MainWindow::Instance()->HaltGui();
	MainWindow::Instance()->Remove(&Error);
	MainWindow::Instance()->ResumeGui();
	
	windowinfo.move = ManageWindow::NO_MOVE_IN;
	
	return 0;//Failed
}

int SaveDelete()
{
	bool exit = false;
	windowinfo.move = ManageWindow::NO_MOVE_IN;
	
	snprintf(devpath, sizeof(devpath), "%s%s%s/", navinfo.CurrentDevice, browserD.dir, browserDevice[selected].filename);
	
	StartManageProgress(0);
	ShowManageProgress(0, 1, tr("Deleting Save"));
	
	usleep(800000);
	ret = RemoveDirectory(devpath);
	
	StopManageProgress();
	
	if (ret == -10)
	{
		Cancelled.SetMessage(tr("Delete cancelled"));
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&Cancelled);
		MainWindow::Instance()->ResumeGui();
		
		while(Cancelled.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(Cancelled.GetState() == STATE_CLICKED) {
				Cancelled.ResetState();
				exit = true;
			}
		}
		
		while(Cancelled.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&Cancelled);
		MainWindow::Instance()->ResumeGui();
		
		windowinfo.move = ManageWindow::NO_MOVE_IN;
		
		return -10; //canceled
	}
	else if(ret <= 0)
	{
		Error.SetMessage(tr("Unable to delete this SaveGame")),
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&Error);
		MainWindow::Instance()->ResumeGui();
		
		while(Error.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(Error.GetState() == STATE_CLICKED) {
				Error.ResetState();
				exit = true;
			}
		}
		
		while(Error.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&Error);
		MainWindow::Instance()->ResumeGui();
		
		windowinfo.move = ManageWindow::NO_MOVE_IN;
		
		return 0;
	}
	else
	{
		Succes.SetMessage(tr("Succesfully deleted")),
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&Succes);
		MainWindow::Instance()->ResumeGui();
		
		while(Succes.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(Succes.GetState() == STATE_CLICKED) {
				Succes.ResetState();
				exit = true;
			}
		}
		
		while(Succes.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&Succes);
		MainWindow::Instance()->ResumeGui();
	}
	
	windowinfo.move = ManageWindow::NO_MOVE_IN;
	windowinfo.refreshType = ManageWindow::DELETE;
	
	windowinfo.refresh = true;
	navinfo.refresh = true;
	
	return 1;
}

int SaveCopy()
{
	bool exit = false;
	windowinfo.move = ManageWindow::NO_MOVE_IN;
	
	char sourcepath[200];
	char destpath[200];
	char temppath[200];
	snprintf(sourcepath, sizeof(sourcepath), "%s%s", navinfo.CurrentDevice, browserD.dir);
	
	if(BrowseCustom(sourcepath, temppath)) {
		if(!strcmp(sourcepath, temppath)) {
			Error.SetMessage(tr("Sorry, you can't copy to the same path")),
			
			MainWindow::Instance()->HaltGui();
			MainWindow::Instance()->Append(&Error);
			MainWindow::Instance()->ResumeGui();
			
			while(Error.GetEffect() > 0) usleep(THREAD_SLEEP);
			
			exit = false;
			while(!exit)
			{
				if(Error.GetState() == STATE_CLICKED) {
					Error.ResetState();
					exit = true;
				}
			}
			
			while(Error.GetEffect() > 0) usleep(THREAD_SLEEP);
			
			MainWindow::Instance()->HaltGui();
			MainWindow::Instance()->Remove(&Error);
			MainWindow::Instance()->ResumeGui();
			
			windowinfo.move = ManageWindow::NO_MOVE_IN;
			
			return 0;
		}
	}
	else
		return 0;
	
	snprintf(sourcepath, sizeof(sourcepath), "%s%s%s/", navinfo.CurrentDevice, browserD.dir, browserDevice[selected].filename);
	snprintf(destpath, sizeof(destpath), "%s%s/", temppath, browserDevice[selected].filename);
	
	StartManageProgress(0);
	ShowManageProgress(0, 1, tr("Copying Save"));
	
	usleep(800000);
	
	ret = CopyDirectory(sourcepath, destpath);
	
	StopManageProgress();
	
	if (ret == -10)
	{
		Cancelled.SetMessage(tr("Copy cancelled"));
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&Cancelled);
		MainWindow::Instance()->ResumeGui();
		
		while(Cancelled.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(Cancelled.GetState() == STATE_CLICKED) {
				Cancelled.ResetState();
				exit = true;
			}
		}
		
		while(Cancelled.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&Cancelled);
		MainWindow::Instance()->ResumeGui();
		
		windowinfo.move = ManageWindow::NO_MOVE_IN;
		
		return -10; //canceled
	}
	else if(ret <= 0)
	{
		Error.SetMessage(tr("Unable to copy this SaveGame")),
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&Error);
		MainWindow::Instance()->ResumeGui();
		
		while(Error.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(Error.GetState() == STATE_CLICKED) {
				Error.ResetState();
				exit = true;
			}
		}
		
		while(Error.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&Error);
		MainWindow::Instance()->ResumeGui();
		
		windowinfo.move = ManageWindow::NO_MOVE_IN;
		
		return 0;
	}
	else
	{
		Succes.SetMessage(tr("Succesfully copied")),
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&Succes);
		MainWindow::Instance()->ResumeGui();
		
		while(Succes.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(Succes.GetState() == STATE_CLICKED) {
				Succes.ResetState();
				exit = true;
			}
		}
		
		while(Succes.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&Succes);
		MainWindow::Instance()->ResumeGui();
	}
	
	windowinfo.move = ManageWindow::NO_MOVE_IN;
	
	navinfo.refresh = true;
	
	return 1;
}
int SaveMove()
{
	bool exit = false;
	windowinfo.move = ManageWindow::NO_MOVE_IN;
	
	char sourcepath[200];
	char destpath[200];
	char temppath[200];
	snprintf(sourcepath, sizeof(sourcepath), "%s%s", navinfo.CurrentDevice, browserD.dir);
	
	if(BrowseCustom(sourcepath, temppath)) {
		if(!strcmp(sourcepath, temppath)) {
			Error.SetMessage(tr("Sorry, you can't move to the same path")),
			
			MainWindow::Instance()->HaltGui();
			MainWindow::Instance()->Append(&Error);
			MainWindow::Instance()->ResumeGui();
			
			while(Error.GetEffect() > 0) usleep(THREAD_SLEEP);
			
			exit = false;
			while(!exit)
			{
				if(Error.GetState() == STATE_CLICKED) {
					Error.ResetState();
					exit = true;
				}
			}
			
			while(Error.GetEffect() > 0) usleep(THREAD_SLEEP);
			
			MainWindow::Instance()->HaltGui();
			MainWindow::Instance()->Remove(&Error);
			MainWindow::Instance()->ResumeGui();
			
			windowinfo.move = ManageWindow::NO_MOVE_IN;
			
			return 0;
		}
	}
	else
		return 0;
	
	snprintf(sourcepath, sizeof(sourcepath), "%s%s%s/", navinfo.CurrentDevice, browserD.dir, browserDevice[selected].filename);
	snprintf(destpath, sizeof(destpath), "%s%s/", temppath, browserDevice[selected].filename);
	
	StartManageProgress(0);
	ShowManageProgress(0, 1, tr("Moving Save"));
	
	usleep(800000);
	
	ret = MoveDirectory(sourcepath, destpath);
	
	StopManageProgress();
	
	if (ret == -10)
	{
		Cancelled.SetMessage(tr("Move cancelled"));
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&Cancelled);
		MainWindow::Instance()->ResumeGui();
		
		while(Cancelled.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(Cancelled.GetState() == STATE_CLICKED) {
				Cancelled.ResetState();
				exit = true;
			}
		}
		
		while(Cancelled.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&Cancelled);
		MainWindow::Instance()->ResumeGui();
		
		windowinfo.move = ManageWindow::NO_MOVE_IN;
		
		return -10; //canceled
	}
	else if(ret <= 0)
	{
		Error.SetMessage(tr("Unable to move this SaveGame")),
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&Error);
		MainWindow::Instance()->ResumeGui();
		
		while(Error.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(Error.GetState() == STATE_CLICKED) {
				Error.ResetState();
				exit = true;
			}
		}
		
		while(Error.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&Error);
		MainWindow::Instance()->ResumeGui();
		
		windowinfo.move = ManageWindow::NO_MOVE_IN;
		
		return 0;
	}
	else
	{
		Succes.SetMessage(tr("Succesfully moved")),
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&Succes);
		MainWindow::Instance()->ResumeGui();
		
		while(Succes.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(Succes.GetState() == STATE_CLICKED) {
				Succes.ResetState();
				exit = true;
			}
		}
		
		while(Succes.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&Succes);
		MainWindow::Instance()->ResumeGui();
	}
	
	windowinfo.move = ManageWindow::NO_MOVE_IN;
	windowinfo.refreshType = ManageWindow::DELETE;
	
	windowinfo.refresh = true;
	navinfo.refresh = true;
	
	return 1;
}
