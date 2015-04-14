/****************************************************************************
 * 
 * manage_all_tools.cpp
 *
 * Menu flow routines - handles all menu logic
 ***************************************************************************/

#include <gccore.h>
#include <ogcsys.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "libwiigui/gui.h"
#include "prompts/browsers/browsers.h"
#include "manage_all/Buttons.h"
#include "manage_all/ManageAllProgress.h"
#include "manage_all/ManageMiiProgress.h"
#include "saves/savegame.h"
#include "saves/wii_list.h"
#include "mii/mii_tools.h"
#include "mii/mii_list.h"
#include "mii/mii_installer.h"
#include "file/fileops.h"
#include "tools/ErrorLog.h"
#include "menu/MainWindow.h"
#include "main.h"

#define round_up(x,n)   (-(-(x) & -(n)))

extern bool	progressmiicanceled;

/*** Static variables ***/
static s32	result;
static char	nandPath[200];
static char	devPath[200];
static int	entriesDone = 0;
static int	totalEntries = 0;

Buttons CancelledAll(0,
					 tr("Cancelled"),
					 0,
					 tr("OK"),
					 0,
					 0,
					 0);

Buttons ErrorAll(tr("Error"),
				 0,
				 0,
				 tr("OK"),
				 0,
				 0,
				 0);

Buttons ShowErrorReport(0,
						tr("Display the report?"),
						0,
						tr("Yes"),
						0,
						0,
						tr("No"));


int AllSaveInstall(char * sourcePath)
{
	u32 bannerToDisplay = 0;
	int i = 0, j = 0, choice = 0;
	bool exit = false;
	totalEntries = 0;
	entriesDone = 0;
	
	ErrorLog ErrorLog;
	
	CancelledAll.SetEffect(EFFECT_FADE, 20);
	ErrorAll.SetEffect(EFFECT_FADE, 20);
	ShowErrorReport.SetEffect(EFFECT_FADE, 20);
	
	CreateSubfolder(sourcePath);
	
	navinfo.firstdevice = true;
	BrowseDevice(sourcePath);	
	
	StartManageAllProgress();
	
	// get total valid entries //
	for(i = 0; i < browserD.numEntries; i++)
	{
		if(browserDevice[i].isSave == 1 && 
		   browserDevice[i].isNotInstalled == 0 && 
		   browserDevice[i].isNotSupported == 0) {
			totalEntries++;
		}
	}
	
	if (totalEntries == 0)
	{
		ErrorAll.SetMessage(tr("No SaveGame found in this folder."));
		
		StopManageAllProgress();
	
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&ErrorAll);
		MainWindow::Instance()->ResumeGui();
		
		while(ErrorAll.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(ErrorAll.GetState() == STATE_CLICKED) {
				ErrorAll.ResetState();
				exit = true;
			}
		}
		
		ErrorAll.SetEffect(EFFECT_FADE, -20);
		
		while(ErrorAll.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&ErrorAll);
		MainWindow::Instance()->ResumeGui();

		return 0;//failed
	}
	
	// install //
	for(i = 0; i < browserD.numEntries; i++)
	{
		if(browserDevice[i].isSave == 1 && 
		   browserDevice[i].isNotInstalled == 0 && 
		   browserDevice[i].isNotSupported == 0)
		{
			// get banner to display //
			for(j = 0; j < wiiSaveInfo.Cnt; j++)
			{
				if(wiiSaveList[j].tid == browserDevice[i].tid){
					bannerToDisplay = j;
					j=wiiSaveInfo.Cnt;
				}
			}
			
			char* buffer = (char*)memalign(32, ISFS_MAXPATH);
			// call progress //
			ShowManageAllProgress(entriesDone, totalEntries, browserDevice[i].displayname, browserDevice[i].subname, bannerToDisplay);
			usleep(800000);
			
			// Generate device path // 
			snprintf(devPath, sizeof(devPath), "%s%s", sourcePath, browserDevice[i].filename);
			
			// Set title UID //
			result = ES_SetUID(browserDevice[i].tid);
			if (result < 0)
			{
				ManageAllProgressShowError(tr("Unable to SetUID"));
				
				while(GetManageAllErrorState() == true)
					usleep(100);
				
				ErrorLog.AddToLog(browserDevice[i].displayname, tr("Unable to SetUID"));
				
				goto next;
			}

			// Get NAND path //
			result = ES_GetDataDir(browserDevice[i].tid, buffer);
			if (result < 0)
			{
				free(buffer);
				
				ManageAllProgressShowError(tr("Unable to get NAND path"));
				
				while(GetManageAllErrorState() == true)
					usleep(100);
				
				ErrorLog.AddToLog(browserDevice[i].displayname, tr("Unable to get NAND path"));
				
				goto next;
			}
			snprintf(nandPath, sizeof(nandPath), "%s", buffer);
			free(buffer);
			
			// Install savegame //
			result = Savegame_Install(devPath, nandPath);
			if (result == -10)
				goto out;
			
			else if (result < 0)
			{
				ManageAllProgressShowError(tr("Unable to copy data"));
				
				while(GetManageAllErrorState() == true)
					usleep(100);
				
				ErrorLog.AddToLog(browserDevice[i].displayname, tr("Unable to copy data"));
				
				goto next;
			}
			
		  next:
			
			entriesDone++;
		}
	}
	
	result = 1;//succes
	if(ErrorLog.GetCount() == totalEntries)
		result = 0;
	
  out:
	
	StopManageAllProgress();
	
	if(ErrorLog.GetCount()>0)
	{
		char text[300];
		snprintf(text, sizeof(text), "%i %s", ErrorLog.GetCount(), tr("Error encountered:"));
		if(ErrorLog.GetCount()!=1)
			snprintf(text, sizeof(text), "%i %s", ErrorLog.GetCount(), tr("Errors encountered:"));
		
		ShowErrorReport.SetTitle(text);
		ShowErrorReport.SetMessage(tr("Display the report?"));
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&ShowErrorReport);
		MainWindow::Instance()->ResumeGui();
		
		while(ShowErrorReport.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(ShowErrorReport.GetState() == STATE_CLICKED) {
				choice = ShowErrorReport.GetChoice();
				ShowErrorReport.ResetState();
				exit = true;
			}
		}
		
		ShowErrorReport.SetEffect(EFFECT_FADE, -20);
		
		while(ShowErrorReport.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&ShowErrorReport);
		MainWindow::Instance()->ResumeGui();
		
		if(choice == 1)
			ErrorLog.Show();
		
	}
	
	if(result == -10)
	{
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&CancelledAll);
		MainWindow::Instance()->ResumeGui();
		
		while(CancelledAll.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(CancelledAll.GetState() == STATE_CLICKED) {
				CancelledAll.ResetState();
				exit = true;
			}
		}
		
		CancelledAll.SetEffect(EFFECT_FADE, -20);
		
		while(CancelledAll.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&CancelledAll);
		MainWindow::Instance()->ResumeGui();
	}
	
	return result;
}

int AllSaveExtract(char * destPath)
{
	int i = 0, choice = 0;
	bool exit = false;
	entriesDone = 0;
	
	CancelledAll.SetEffect(EFFECT_FADE, 20);
	ErrorAll.SetEffect(EFFECT_FADE, 20);
	ShowErrorReport.SetEffect(EFFECT_FADE, 20);
	
	ErrorLog ErrorLog;
	
	StartManageAllProgress();
	
	if (wiiSaveInfo.Cnt == 0)
	{
		ErrorAll.SetMessage(tr("No SaveGame found in Wii."));
		
		StopManageAllProgress();
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&ErrorAll);
		MainWindow::Instance()->ResumeGui();
		
		while(ErrorAll.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(ErrorAll.GetState() == STATE_CLICKED) {
				ErrorAll.ResetState();
				exit = true;
			}
		}
		
		ErrorAll.SetEffect(EFFECT_FADE, -20);
		
		while(ErrorAll.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&ErrorAll);
		MainWindow::Instance()->ResumeGui();
		
		return 0;
	}
	
	for(i = 0; i < wiiSaveInfo.Cnt; i++)
	{
		char* buffer = (char*)memalign(32, ISFS_MAXPATH);
		ShowManageAllProgress(entriesDone, wiiSaveInfo.Cnt, wiiSaveList[i].name, wiiSaveList[i].subname, i);
		usleep(800000);
		
		// Generate device path //
		snprintf(devPath, sizeof(devPath), "%s%016llx", destPath, wiiSaveList[i].tid);
		CreateSubfolder(destPath);
		
		// Get NAND path //
		result = ES_SetUID(wiiSaveList[i].tid);
		if (result < 0)
		{
			ManageAllProgressShowError(tr("Unable to SetUID"));
			
			while(GetManageAllErrorState() == true)
				usleep(100);
			
			ErrorLog.AddToLog(wiiSaveList[i].name, tr("Unable to SetUID"));
			
			goto next;
		}
		
		result = ES_GetDataDir(wiiSaveList[i].tid, buffer);
		if (result < 0)
		{
			free(buffer);
			
			ManageAllProgressShowError(tr("Unable to get NAND path"));
			
			while(GetManageAllErrorState() == true)
				usleep(100);
			
			ErrorLog.AddToLog(wiiSaveList[i].name, tr("Unable to get NAND path"));
			
			goto next;
		}
		snprintf(nandPath, sizeof(nandPath), "%s", buffer);
		free(buffer);
		
		// Extract SaveGame //
		result = Savegame_Extract(nandPath, devPath, devPath);
		if(result < 0)
			RemoveDirectory(devPath);
		
		if (result == -10)
			goto out;
		
		else if (result < 0)
		{
			ManageAllProgressShowError(tr("Unable to copy data"));
			
			while(GetManageAllErrorState() == true)
				usleep(THREAD_SLEEP);
			
			ErrorLog.AddToLog(wiiSaveList[i].name, tr("Unable to copy data"));
			
			goto next;
		}
		
	  next:
		
		entriesDone++;
	}
	
	result = 1;
	if(ErrorLog.GetCount() == wiiSaveInfo.Cnt)
		result = 0;
	
  out:
	
	StopManageAllProgress();
	
	if(ErrorLog.GetCount()>0)
	{
		char text[300];
		snprintf(text, sizeof(text), "%i %s", ErrorLog.GetCount(), tr("Error encountered:"));
		if(ErrorLog.GetCount()!=1)
			snprintf(text, sizeof(text), "%i %s", ErrorLog.GetCount(), tr("Errors encountered:"));
		
		ShowErrorReport.SetTitle(text);
		ShowErrorReport.SetMessage(tr("Display the report?"));
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&ShowErrorReport);
		MainWindow::Instance()->ResumeGui();
		
		while(ShowErrorReport.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(ShowErrorReport.GetState() == STATE_CLICKED) {
				choice = ShowErrorReport.GetChoice();
				ShowErrorReport.ResetState();
				exit = true;
			}
		}
		
		ShowErrorReport.SetEffect(EFFECT_FADE, -20);
		
		while(ShowErrorReport.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&ShowErrorReport);
		MainWindow::Instance()->ResumeGui();
		
		if(choice == 1)
			ErrorLog.Show();
		
	}
	
	if(result == -10)
	{
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&CancelledAll);
		MainWindow::Instance()->ResumeGui();
		
		while(CancelledAll.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(CancelledAll.GetState() == STATE_CLICKED) {
				CancelledAll.ResetState();
				exit = true;
			}
		}
		
		CancelledAll.SetEffect(EFFECT_FADE, -20);
		
		while(CancelledAll.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&CancelledAll);
		MainWindow::Instance()->ResumeGui();
	}
	
	return result;
}

int AllMiiInstall(char * sourcePath)
{
	int i = 0, choice = 0;
	bool exit = false;
	totalEntries = 0;
	entriesDone = 0;
	
	ErrorLog ErrorLog;
	
	CancelledAll.SetEffect(EFFECT_FADE, 20);
	ErrorAll.SetEffect(EFFECT_FADE, 20);
	ShowErrorReport.SetEffect(EFFECT_FADE, 20);
	
	navinfo.firstdevice = true;
	BrowseDevice(sourcePath);	
	
	StartManageMiiProgress();
	
	// get total valid entries //
	for(i = 0; i < browserD.numEntries; i++)
	{
		if(browserDevice[i].isMii == 1)
		{
			totalEntries++;
		}
	}
	
	if (totalEntries == 0)
	{
		ErrorAll.SetMessage(tr("No Mii found in this folder."));
		
		StopManageMiiProgress();
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&ErrorAll);
		MainWindow::Instance()->ResumeGui();
		
		while(ErrorAll.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(ErrorAll.GetState() == STATE_CLICKED) {
				ErrorAll.ResetState();
				exit = true;
			}
		}
		
		ErrorAll.SetEffect(EFFECT_FADE, -20);
		
		while(ErrorAll.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&ErrorAll);
		MainWindow::Instance()->ResumeGui();
		
		return 0;//failed
	}
	
	// install //
	for(i = 0; i < browserD.numEntries; i++)
	{
		if(browserDevice[i].isMii == 1)
		{
			// call progress //
			ShowManageMiiProgress(entriesDone,
								  totalEntries,
								  browserDevice[i].displayname,
								  browserDevice[i].miiCreator,
								  browserDevice[i].miiDay,
								  browserDevice[i].miiMonth,
								  browserDevice[i].miiFemale,
								  browserDevice[i].miiFavColor);
			usleep(800000);
			
			char miiPath[200];
			snprintf(miiPath, sizeof(miiPath), "%s%s", sourcePath, browserDevice[i].filename);
			
			//Check if same mii already installed in wii
			int n = 0;
			int reference = 0; // reference to the place of mii in wii file
			bool overwriteOk = false;
			for(n = 0; n < miiListInfo.Cnt; n++)
			{
				if(!strcmp(miiList[n].displayname, browserDevice[i].displayname)){
					reference = miiList[n].reference;
					overwriteOk = true ;
					n = miiListInfo.Cnt;
				}
			}
			
			int result = 0;
			if(overwriteOk)
				result = install_mii(miiPath, reference);
			else
				result = WriteMii(miiPath);
			
			if (result == -10)
				goto out;
			
			else if (result < 0)
			{
				ManageMiiProgressShowError(tr("Unable to copy data"));
				
				while(GetManageMiiErrorState() == true)
					usleep(THREAD_SLEEP);
				
				ErrorLog.AddToLog(browserDevice[i].displayname, tr("Unable to copy data"));
				
				goto next;
			}
			
		  next:
			
			entriesDone++;
			CreateMiiList();
		}
	}
	
	result = 1;
	if(ErrorLog.GetCount() == totalEntries)
		result = 0;
	
  out:
	
	StopManageMiiProgress();
	
	if(ErrorLog.GetCount()>0)
	{
		char text[300];
		snprintf(text, sizeof(text), "%i %s", ErrorLog.GetCount(), tr("Error encountered:"));
		if(ErrorLog.GetCount()!=1)
			snprintf(text, sizeof(text), "%i %s", ErrorLog.GetCount(), tr("Errors encountered:"));
		
		ShowErrorReport.SetTitle(text);
		ShowErrorReport.SetMessage(tr("Display the report?"));
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&ShowErrorReport);
		MainWindow::Instance()->ResumeGui();
		
		while(ShowErrorReport.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(ShowErrorReport.GetState() == STATE_CLICKED) {
				choice = ShowErrorReport.GetChoice();
				ShowErrorReport.ResetState();
				exit = true;
			}
		}
		
		ShowErrorReport.SetEffect(EFFECT_FADE, -20);
		
		while(ShowErrorReport.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&ShowErrorReport);
		MainWindow::Instance()->ResumeGui();
		
		if(choice == 1)
			ErrorLog.Show();
		
	}
	
	if(result == -10)
	{
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&CancelledAll);
		MainWindow::Instance()->ResumeGui();
		
		while(CancelledAll.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(CancelledAll.GetState() == STATE_CLICKED) {
				CancelledAll.ResetState();
				exit = true;
			}
		}
		
		CancelledAll.SetEffect(EFFECT_FADE, -20);
		
		while(CancelledAll.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&CancelledAll);
		MainWindow::Instance()->ResumeGui();
	}
	
	return result;
}

int AllMiiExtract(char * destPath)
{
	int i = 0, ret = 0, choice = 0;
	bool exit = false;
	entriesDone = 0;
	
	FILE * miidest;
	u32 * memory = NULL;
	
	ErrorLog ErrorLog;
	
	CancelledAll.SetEffect(EFFECT_FADE, 20);
	ErrorAll.SetEffect(EFFECT_FADE, 20);
	ShowErrorReport.SetEffect(EFFECT_FADE, 20);
	
	CreateSubfolder(destPath);
	
	StartManageMiiProgress();
	
	if (miiListInfo.Cnt == 0)
	{
		ErrorAll.SetMessage(tr("No Mii found in Wii memory."));
		
		StopManageMiiProgress();
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&ErrorAll);
		MainWindow::Instance()->ResumeGui();
		
		while(ErrorAll.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(ErrorAll.GetState() == STATE_CLICKED) {
				ErrorAll.ResetState();
				exit = true;
			}
		}
		
		ErrorAll.SetEffect(EFFECT_FADE, -20);
		
		while(ErrorAll.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&ErrorAll);
		MainWindow::Instance()->ResumeGui();
		
		return 0;//failed
	}
	
	for(i = 0; i < miiListInfo.Cnt; i++)
	{
		ShowManageMiiProgress(entriesDone,
							  miiListInfo.Cnt,
							  miiList[i].displayname,
							  miiList[i].creator,
							  miiList[i].day,
							  miiList[i].month,
							  miiList[i].female,
							  miiList[i].favColor);
		usleep(800000);
		
		s32 miisrc = ISFS_Open(MII_FILE, ISFS_OPEN_READ);
		if (miisrc < 0)
		{
			ManageMiiProgressShowError(tr("Can't open the Nand Mii file."));
			
			while(GetManageMiiErrorState() == true)
				usleep(THREAD_SLEEP);
			
			ErrorLog.AddToLog(miiList[i].displayname, tr("Can't open the Nand Mii file."));
			
			goto next;
		}
		
		// Get extract file name
		char dest[strlen(destPath)+MII_NAME_LENGTH*4+6];
		fix_miiName(miiList[i].displayname);
		snprintf(dest, sizeof(dest), "%s%s.miigx", destPath, miiList[i].displayname);
		
		// go to extracted mii offset
		ISFS_Seek(miisrc, MII_HEADER+(miiList[i].reference*MII_SIZE), 0);
		
		memory = (u32*)memalign(32, round_up(MII_SIZE, 32));
		
		if(!memory) {
			ISFS_Close(miisrc);
			
			ManageMiiProgressShowError(tr("Not enough memory."));
			
			while(GetManageMiiErrorState() == true)
				usleep(THREAD_SLEEP);
			
			ErrorLog.AddToLog(miiList[i].displayname, tr("Not enough memory."));
			
			goto next;
        }
		
        // Read Mii
		ret = ISFS_Read(miisrc, memory, MII_SIZE);
		if (ret != MII_SIZE) {
			ISFS_Close(miisrc);
			free(memory);
			memory = NULL;
			
			ManageMiiProgressShowError(tr("Can't read the Nand Mii file."));
			
			while(GetManageMiiErrorState() == true)
				usleep(THREAD_SLEEP);
			
			ErrorLog.AddToLog(miiList[i].displayname, tr("Can't read the Nand Mii file."));
			
			goto next;
		}
		
		ISFS_Close(miisrc);
		
		// Open destination file
		miidest = fopen(dest, "wb");
		if(!miidest) {
			free(memory);
			memory = NULL;
			
			ManageMiiProgressShowError(tr("Can't create the Mii file."));
			
			while(GetManageMiiErrorState() == true)
				usleep(THREAD_SLEEP);
			
			ErrorLog.AddToLog(miiList[i].displayname, tr("Can't create the Mii file."));
			
			goto next;
		}
		
		// write file
		fwrite(memory, 1, MII_SIZE, miidest);
		fclose(miidest);
		free(memory);
		memory = NULL;
		
		if (progressmiicanceled == true)
		{
			result =  -10; //canceled
			goto out;
		}
		
	  next:
		
		entriesDone++;
	}
	
	result = 1;
	if(ErrorLog.GetCount() == miiListInfo.Cnt)
		result = 0;
	
  out:
	
	StopManageMiiProgress();
	
	if(ErrorLog.GetCount()>0)
	{
		char text[300];
		snprintf(text, sizeof(text), "%i %s", ErrorLog.GetCount(), tr("Error encountered:"));
		if(ErrorLog.GetCount()!=1)
			snprintf(text, sizeof(text), "%i %s", ErrorLog.GetCount(), tr("Errors encountered:"));
		
		ShowErrorReport.SetTitle(text);
		ShowErrorReport.SetMessage(tr("Display the report?"));
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&ShowErrorReport);
		MainWindow::Instance()->ResumeGui();
		
		while(ShowErrorReport.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(ShowErrorReport.GetState() == STATE_CLICKED) {
				choice = ShowErrorReport.GetChoice();
				ShowErrorReport.ResetState();
				exit = true;
			}
		}
		
		ShowErrorReport.SetEffect(EFFECT_FADE, -20);
		
		while(ShowErrorReport.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&ShowErrorReport);
		MainWindow::Instance()->ResumeGui();
		
		if(choice == 1)
			ErrorLog.Show();
		
	}
	
	if(result == -10)
	{
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&CancelledAll);
		MainWindow::Instance()->ResumeGui();
		
		while(CancelledAll.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(CancelledAll.GetState() == STATE_CLICKED) {
				CancelledAll.ResetState();
				exit = true;
			}
		}
		
		CancelledAll.SetEffect(EFFECT_FADE, -20);
		
		while(CancelledAll.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&CancelledAll);
		MainWindow::Instance()->ResumeGui();
	}
	
	return result;
}

/****************************************************************************
 * DeleteAllNandMii
 *
 * Delete all miis from the Wii Nand memory
 ***************************************************************************/
int DeleteAllNandMii() {
	
	bool exit = false;
	
	if(!install_mii("DELETE_ALL", 0)) {
		ErrorAll.SetMessage(tr("Unable to delete the selected Mii"));
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&ErrorAll);
		MainWindow::Instance()->ResumeGui();
		
		while(ErrorAll.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(ErrorAll.GetState() == STATE_CLICKED) {
				ErrorAll.ResetState();
				exit = true;
			}
		}
		
		while(ErrorAll.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&ErrorAll);
		MainWindow::Instance()->ResumeGui();
		
		return 0;
	}
	
	return 1;
}

/****************************************************************************
 * DeleteAllDeviceMii
 *
 * Delete all miis from the path selected on your device
 ***************************************************************************/
int DeleteAllDeviceMii(char * deletePath) {
	
	int i = 0, choice = 0;
	bool exit = false;
	totalEntries = 0;
	entriesDone = 0;
	
	CancelledAll.SetEffect(EFFECT_FADE, 20);
	ErrorAll.SetEffect(EFFECT_FADE, 20);
	
	navinfo.firstdevice = true;
	BrowseDevice(deletePath);	
	
	ErrorLog ErrorLog;
	
	StartManageMiiProgress();
	
	// get total valid entries //
	for(i = 0; i < browserD.numEntries; i++)
	{
		if(browserDevice[i].isMii == 1)
		{
			totalEntries++;
		}
	}
	
	if (totalEntries == 0)
	{
		ErrorAll.SetMessage(tr("No Mii found in this folder."));
		
		StopManageMiiProgress();
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&ErrorAll);
		MainWindow::Instance()->ResumeGui();
		
		while(ErrorAll.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(ErrorAll.GetState() == STATE_CLICKED) {
				ErrorAll.ResetState();
				exit = true;
			}
		}
		
		ErrorAll.SetEffect(EFFECT_FADE, -20);
		
		while(ErrorAll.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&ErrorAll);
		MainWindow::Instance()->ResumeGui();
		
		return 0;//failed
	}
	
	// install //
	for(i = 0; i < browserD.numEntries; i++)
	{
		if(browserDevice[i].isMii == 1)
		{
			// call progress //
			ShowManageMiiProgress(entriesDone,
								  totalEntries,
								  browserDevice[i].displayname,
								  browserDevice[i].miiCreator,
								  browserDevice[i].miiDay,
								  browserDevice[i].miiMonth,
								  browserDevice[i].miiFemale,
								  browserDevice[i].miiFavColor);
			usleep(800000);
			
			char miiPath[200];
			snprintf(miiPath, sizeof(miiPath), "%s%s", deletePath, browserDevice[i].filename);
			
			result = RemoveFile(miiPath);
			
			if (result == -10)
				goto out;
			
			else if (result < 0)
			{
				ManageMiiProgressShowError(tr("Unable to delete data"));
				
				while(GetManageMiiErrorState() == true)
					usleep(THREAD_SLEEP);
				
				ErrorLog.AddToLog(browserDevice[i].displayname, tr("Unable to delete data"));
				
				goto next;
			}
			
		  next:
			
			entriesDone++;
		}
	}
	
	result = 1;
	if(ErrorLog.GetCount() == totalEntries)
		result = 0;
	
  out:
	
	StopManageMiiProgress();
	
	if(ErrorLog.GetCount()>0)
	{
		char text[300];
		snprintf(text, sizeof(text), "%i %s", ErrorLog.GetCount(), tr("Error encountered:"));
		if(ErrorLog.GetCount()!=1)
			snprintf(text, sizeof(text), "%i %s", ErrorLog.GetCount(), tr("Errors encountered:"));
		
		ShowErrorReport.SetTitle(text);
		ShowErrorReport.SetMessage(tr("Display the report?"));
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&ShowErrorReport);
		MainWindow::Instance()->ResumeGui();
		
		while(ShowErrorReport.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(ShowErrorReport.GetState() == STATE_CLICKED) {
				choice = ShowErrorReport.GetChoice();
				ShowErrorReport.ResetState();
				exit = true;
			}
		}
		
		ShowErrorReport.SetEffect(EFFECT_FADE, -20);
		
		while(ShowErrorReport.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&ShowErrorReport);
		MainWindow::Instance()->ResumeGui();
		
		if(choice == 1)
			ErrorLog.Show();
		
	}
	
	if(result == -10)
	{
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&CancelledAll);
		MainWindow::Instance()->ResumeGui();
		
		while(CancelledAll.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		exit = false;
		while(!exit)
		{
			if(CancelledAll.GetState() == STATE_CLICKED) {
				CancelledAll.ResetState();
				exit = true;
			}
		}
		
		CancelledAll.SetEffect(EFFECT_FADE, -20);
		
		while(CancelledAll.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&CancelledAll);
		MainWindow::Instance()->ResumeGui();
	}
	
	return result;
	
}

