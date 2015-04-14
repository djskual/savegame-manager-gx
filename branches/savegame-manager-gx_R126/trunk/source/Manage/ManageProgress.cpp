/****************************************************************************
 * ProgressWindow
 * USB Loader GX 2009
 *
 * ProgressWindow.cpp
 ***************************************************************************/
#include <gccore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../menu.h"
#include "ManageProgress.h"
#include "ProgressThrobber.hpp"
#include "ManageButtons.hpp"
#include "../Language/gettext.h"
#include "../LibWiiGui/gui.h"
#include "../Themes/CTheme.h"

/*** Variables used only in this file ***/
static lwp_t	managethread = LWP_THREAD_NULL;
static mutex_t	ProgressMutex = LWP_MUTEX_NULL;
static ManageProgressAbortCallback AbortCallback = NULL;
static const char * progressTitle = NULL;
static const char * progressMsg = NULL;
static int showProgress = 0;

/****************************************************************************
 * StartManageProgress
 ***************************************************************************/
extern "C" void StartManageProgress(const char * title, const char * msg)
{
	progressTitle = title;
	if(msg)
		progressMsg = msg;
	
	showProgress = 1;
	
	LWP_ResumeThread(managethread);
}

/****************************************************************************
 * ManageProgressStop
 ***************************************************************************/
extern "C" void ManageProgressStop()
{
	showProgress = 0;
	progressTitle = NULL;
	progressMsg = NULL;
	
	//! wait for thread to finish
	while (!LWP_ThreadIsSuspended(managethread))
		usleep(100);
}

/****************************************************************************
 * ManageProgressSetAbortCallback
 *
 * Set a callback for the cancel button
 ***************************************************************************/
extern "C" void ManageProgressSetAbortCallback(ManageProgressAbortCallback callback)
{
	AbortCallback = callback;
}

/****************************************************************************
 * ManageProgressWindow
 *
 * Opens a window, which displays progress to the user. Can either display a
 * progress bar showing % completion, or a throbber that only shows that an
 * action is in progress.
 ***************************************************************************/
static void ManageProgressWindow()
{
	ManageButtons Cancel(progressTitle , progressMsg, tr( "Cancel" ));
	Cancel.SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	Cancel.SetPosition(-6, -30);
	
	ProgressThrobber Throbber(0);
	Throbber.SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
	Throbber.SetPosition(-30, -30);
	
	GuiWindow promptWindow(572, 372);
	promptWindow.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	promptWindow.SetPosition(6, 0);
	
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

		if(Cancel.GetState() == STATE_CLICKED)
		{
			if(AbortCallback) AbortCallback();
			Cancel.ResetState();
		}
	}

	promptWindow.SetEffect(EFFECT_FADE, -20);
	while (promptWindow.GetEffect() > 0) usleep(100);

	HaltGui();
	mainWindow->Remove(&promptWindow);
	ResumeGui();
}

/****************************************************************************
 * ManageThread
 ***************************************************************************/
static void * ManageThread(void *arg)
{
	while (1)
	{
		if (!showProgress) LWP_SuspendThread(managethread);

		ManageProgressWindow();
		usleep(100);
	}
	return NULL;
}

/****************************************************************************
 * InitManageProgressThread
 *
 * Startup ManageThread in idle prio
 ***************************************************************************/
void InitManageProgressThread()
{
	LWP_MutexInit(&ProgressMutex, true);
	LWP_CreateThread(&managethread, ManageThread, NULL, NULL, 16384, 70);
}

/****************************************************************************
 * ExitManageProgressThread
 *
 * Shutdown ManageThread
 ***************************************************************************/
void ExitManageProgressThread()
{
	managethread = LWP_THREAD_NULL;
	LWP_MutexUnlock(ProgressMutex);
	LWP_MutexDestroy(ProgressMutex);
}
