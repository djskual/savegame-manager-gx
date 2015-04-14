/****************************************************************************
 * MiiAllProgress
 * SaveGame Manager GX 2011
 * by dj_skual
 *
 * MiiAllProgress.cpp
 ***************************************************************************/
#include <gccore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "MiiAllProgress.h"
#include "AllButtons.hpp"
#include "MiiAllInfos.hpp"
#include "../Manage/ProgressThrobber.hpp"
#include "../Tools/StringTools.h"
#include "../Language/gettext.h"
#include "../LibWiiGui/gui.h"
#include "../Themes/CTheme.h"
#include "../menu.h"

/*** Variables used only in this file ***/
static lwp_t	miiallthread = LWP_THREAD_NULL;
static mutex_t	ProgressMutex = LWP_MUTEX_NULL;
static MiiAllProgressAbortCallback AbortCallback = NULL;
static const char * progressName = NULL;
static int progressDone = -1;
static int progressTotal = -1;
static int progressSelected = -1;
static int progressEmuSelected = -1;
static std::string progressCreator;
static int progressFemale = -1;
static int progressDay = -1;
static int progressMonth = -1;
static int progressColor = -1;
static int showProgress = 0;
static bool changed = true;

/****************************************************************************
 * ShowMiiAllProgress
 ***************************************************************************/
void ShowMiiAllProgress(const char *name,
						int done,
						int total,
						int selected,
						int emu_selected,
						std::string creator,
						int female,
						int day,
						int month,
						int favColor)
{
	if (total <= 0)
		return;

	else if (done > total)
		done = total;

	LWP_MutexLock(ProgressMutex);

	progressName = name;
	
	progressDone = done;
	progressTotal = total;

	progressSelected = selected;
	progressEmuSelected = emu_selected;
	progressCreator = creator;
	progressFemale = female;
	progressDay = day;
	progressMonth = month;
	progressColor = favColor;

	changed = true;
	
	if(!done)
	{
		LWP_ResumeThread(miiallthread);
		showProgress = 1;
	}

	
	LWP_MutexUnlock(ProgressMutex);
}

/****************************************************************************
 * MiiAllProgressStop
 ***************************************************************************/
extern "C" void MiiAllProgressStop()
{
	showProgress = 0;
	progressName = NULL;
	
	//! wait for thread to finish
	while (!LWP_ThreadIsSuspended(miiallthread))
		usleep(100);
}

/****************************************************************************
 * MiiAllProgressSetAbortCallback
 *
 * Set a callback for the cancel button
 ***************************************************************************/
extern "C" void MiiAllProgressSetAbortCallback(MiiAllProgressAbortCallback callback)
{
	AbortCallback = callback;
}

/****************************************************************************
 * MiiAllProgressWindow
 *
 * Opens a window, which displays progress to the user.
 ***************************************************************************/
static void MiiAllProgressWindow()
{
	GuiText nameTxt(progressName, 26, thColor("r=255 g=255 b=255 a=255 - manage windows text color"));
	nameTxt.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	nameTxt.SetPosition(-6, 70);
	nameTxt.SetMaxWidth(500, DOTTED);

	MiiAllInfos miiInfos(progressSelected, progressEmuSelected, progressCreator, progressFemale, progressDay, progressMonth, progressColor);
	miiInfos.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	miiInfos.SetPosition(-6, -30);
	
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
	promptWindow.Append(&miiInfos);
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
			countTxt.SetText(fmt("%d / %d", progressDone+1, progressTotal));
			
			HaltGui();
			miiInfos.SetInfos(progressSelected, progressEmuSelected, progressCreator, progressFemale, progressDay, progressMonth, progressColor);
			ResumeGui();
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
}

/****************************************************************************
 * MiiAllThread
 ***************************************************************************/
static void * MiiAllThread(void *arg)
{
	while (1)
	{
		if (!showProgress) LWP_SuspendThread(miiallthread);

		MiiAllProgressWindow();
		usleep(100);
	}
	return NULL;
}

/****************************************************************************
 * InitMiiAllProgressThread
 *
 * Startup MiiAllThread in idle prio
 ***************************************************************************/
void InitMiiAllProgressThread()
{
	LWP_MutexInit(&ProgressMutex, true);
	LWP_CreateThread(&miiallthread, MiiAllThread, NULL, NULL, 16384, 70);
}

/****************************************************************************
 * ExitMiiAllProgressThread
 *
 * Shutdown MiiAllThread
 ***************************************************************************/
void ExitMiiAllProgressThread()
{
	miiallthread = LWP_THREAD_NULL;
	LWP_MutexUnlock(ProgressMutex);
	LWP_MutexDestroy(ProgressMutex);
}
