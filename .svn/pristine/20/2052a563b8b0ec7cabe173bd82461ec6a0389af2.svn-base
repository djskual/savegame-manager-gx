/****************************************************************************
 * SaveAllProgress
 * SaveGame Manager GX 2011
 * by dj_skual
 *
 * SaveAllProgress.cpp
 ***************************************************************************/
#include <gccore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "SaveAllProgress.h"
#include "AllButtons.hpp"
#include "../Manage/ProgressThrobber.hpp"
#include "../FileOperations/Nand.hpp"
#include "../FileOperations/fileops.h"
#include "../Tools/StringTools.h"
#include "../Language/gettext.h"
#include "../LibWiiGui/gui.h"
#include "../LibWiiGui/gui_banner.h"
#include "../Themes/CTheme.h"
#include "../menu.h"

/*** Variables used only in this file ***/
static lwp_t	saveallthread = LWP_THREAD_NULL;
static mutex_t	ProgressMutex = LWP_MUTEX_NULL;
static SaveAllProgressAbortCallback AbortCallback = NULL;
static const char * progressName = NULL;
static const char * progressSubname = NULL;
static u64 progressTid = 0;
static std::string progressPath;
static int progressDone = -1;
static int progressTotal = -1;
static GuiSaveBanner * saveBanner = NULL;
static int showProgress = 0;
static bool changed = true;

/****************************************************************************
 * ShowSaveAllProgress
 ***************************************************************************/
void ShowSaveAllProgress(const char *name, const char *subname, int done, int total, u64 tid, std::string Path)
{
	if (total <= 0)
		return;

	else if (done > total)
		done = total;

	LWP_MutexLock(ProgressMutex);

	progressName = name;
	progressSubname = subname;
	
	progressTid = tid;
	progressPath = Path;
	
	progressDone = done;
	progressTotal = total;

	changed = true;
	
	if(!done)
	{
		LWP_ResumeThread(saveallthread);
		showProgress = 1;
	}

	
	LWP_MutexUnlock(ProgressMutex);
}

/****************************************************************************
 * SaveAllProgressStop
 ***************************************************************************/
extern "C" void SaveAllProgressStop()
{
	showProgress = 0;
	progressName = NULL;
	
	//! wait for thread to finish
	while (!LWP_ThreadIsSuspended(saveallthread))
		usleep(100);
}

/****************************************************************************
 * SaveAllProgressSetAbortCallback
 *
 * Set a callback for the cancel button
 ***************************************************************************/
extern "C" void SaveAllProgressSetAbortCallback(SaveAllProgressAbortCallback callback)
{
	AbortCallback = callback;
}

static GuiSaveBanner * GetBanner(u64 tid)
{
	char path[ISFS_MAXPATH];
	u32 high = (u32)(tid >> 32);
	u32 low  = (u32)(tid & 0xFFFFFFFF);

	snprintf(path, sizeof(path),"/title/%08x/%08x/data/banner.bin", high, low);
	
	u8* buffer = NULL;
	u32 size = 0;
	Nand::LoadFile(path, &buffer, &size);
	
	if (saveBanner)
		delete saveBanner;
	
	GuiSaveBanner * saveBanner = new GuiSaveBanner(buffer);
	
	free(buffer);
	buffer = NULL;
	
	return saveBanner;
}

static GuiSaveBanner * GetBanner(std::string Path)
{
	Path += "/banner.bin";
	
	u8* buffer = NULL;
	u64 size = 0;
	LoadFileToMem(Path.c_str(), &buffer, &size);
	
	if (saveBanner)
		delete saveBanner;
	
	GuiSaveBanner * saveBanner = new GuiSaveBanner(buffer);
	
	free(buffer);
	
	return saveBanner;
}

/****************************************************************************
 * SaveAllProgressWindow
 *
 * Opens a window, which displays progress to the user.
 ***************************************************************************/
static void SaveAllProgressWindow()
{
	GuiText nameTxt(progressName, 26, thColor("r=255 g=255 b=255 a=255 - manage windows text color"));
	nameTxt.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	nameTxt.SetPosition(-6, 70);
	nameTxt.SetMaxWidth(500, DOTTED);

	GuiText subnameTxt(progressSubname, 22, thColor("r=255 g=255 b=255 a=255 - manage windows text color"));
	subnameTxt.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	subnameTxt.SetPosition(-6, 110);
	subnameTxt.SetMaxWidth(430, DOTTED);

	GuiButton Banner(200, 62);
	Banner.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	Banner.SetPosition(-6, 0);
	if (progressTid) Banner.SetImage(GetBanner(progressTid));
	else if (progressPath.size()) Banner.SetImage(GetBanner(progressPath));
	
	AllButtons Cancel(0 , 0, tr( "Cancel" ));
	Cancel.SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	Cancel.SetPosition(-6, 0);
	
	GuiText countTxt(fmt("%d / %d", progressDone+1, progressTotal), 20, thColor("r=255 g=255 b=255 a=255 - manage windows text color"));
	countTxt.SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
	countTxt.SetPosition(-80, -40);
	
	ProgressThrobber Throbber(0);
	Throbber.SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
	Throbber.SetPosition(-30, -30);
	
	GuiWindow promptWindow(572, 372);
	promptWindow.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	promptWindow.SetPosition(6, 0);
	
	promptWindow.Append(&nameTxt);
	promptWindow.Append(&subnameTxt);
	promptWindow.Append(&Banner);
	promptWindow.Append(&countTxt);
	promptWindow.Append(&Throbber);
	promptWindow.Append(&Cancel);
	
	HaltGui();
	promptWindow.SetEffect(EFFECT_FADE, 20);
	mainWindow->Append(&promptWindow);
	ResumeGui();

	while (promptWindow.GetEffect() > 0) usleep(100);

	while (showProgress)
	{
		usleep(10000);

		if (changed)
		{
			if (progressName) nameTxt.SetText(progressName);
			if (progressSubname) subnameTxt.SetText(progressSubname);
			countTxt.SetText(fmt("%d / %d", progressDone+1, progressTotal));
			
			if (progressTid) Banner.SetImage(GetBanner(progressTid));
			else if (progressPath.size()) Banner.SetImage(GetBanner(progressPath));
		}
		
		if(Cancel.GetState() == STATE_CLICKED)
		{
			if(AbortCallback) AbortCallback();
			Cancel.ResetState();
		}
		
		changed = false;
	}

	promptWindow.SetEffect(EFFECT_FADE, -20);
	while (promptWindow.GetEffect() > 0) usleep(100);

	HaltGui();
	mainWindow->Remove(&promptWindow);
	ResumeGui();
	
	if (saveBanner)
		delete saveBanner;
}

/****************************************************************************
 * SaveAllThread
 ***************************************************************************/
static void * SaveAllThread(void *arg)
{
	while (1)
	{
		if (!showProgress) LWP_SuspendThread(saveallthread);
		
		SaveAllProgressWindow();
		usleep(100);
	}
	return NULL;
}

/****************************************************************************
 * InitSaveAllProgressThread
 *
 * Startup SaveAllThread in idle prio
 ***************************************************************************/
void InitSaveAllProgressThread()
{
	LWP_MutexInit(&ProgressMutex, true);
	LWP_CreateThread(&saveallthread, SaveAllThread, NULL, NULL, 16384, 70);
}

/****************************************************************************
 * ExitSaveAllProgressThread
 *
 * Shutdown SaveAllThread
 ***************************************************************************/
void ExitSaveAllProgressThread()
{
	saveallthread = LWP_THREAD_NULL;
	LWP_MutexUnlock(ProgressMutex);
	LWP_MutexDestroy(ProgressMutex);
}
