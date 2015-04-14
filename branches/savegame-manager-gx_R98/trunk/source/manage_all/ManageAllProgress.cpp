 /****************************************************************************
 * Copyright (C) 2009
 * by Dimok
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
 * ProgressWindow
 * Wii-FileXplorer 2009
 *
 * ProgressWindow.cpp
 ***************************************************************************/
#include <gccore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "libwiigui/gui.h"
#include "manage_all/ManageAllProgress.h"
#include "manage_all/ManageAllProgressBanner.h"
#include "manage_all/ManageThrobber.h"
#include "file/fileops.h"
#include "menu/MainWindow.h"
#include "main.h"

/*** Variables used only in this file ***/
static lwp_t		manageallthread = LWP_THREAD_NULL;
static char			progressName[200];
static char			progressSubname[200];
static char			progressTxt[20];
static char			progressError[200];
static short		showManageAllProgress = 0;
static int			throbberCount = 1;
static int			progressDone = 0;
static int			progressTotal = 100;
static u32			bannerToDisplay = 0;
static bool			changed = false;
static bool			showError = false;

/*** Variables used outside of this file ***/
extern bool			progresscanceled;

/****************************************************************************
 * ManageAllProgress
 *
 * Opens a window, which displays progress to the user.
 ***************************************************************************/
void ManageAllProgress()
{
	if(!showManageAllProgress)
		return;

	GuiSound btnSoundOver(button_over_wav, button_over_wav_size, cfg.SFXVolume);
    GuiSound btnClick(button_click_wav, button_click_wav_size, cfg.SFXVolume);
    
	GuiWindow promptWindow(400,230);
	promptWindow.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	promptWindow.SetPosition(0, -10);

	SimpleGuiTrigger trigA(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

	ManageThrobber Throbber(0);
	Throbber.SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
	Throbber.SetPosition(45, 0);
	
	GuiText nameTxt(NULL, 30, (GXColor){255, 255, 255, 255});
	nameTxt.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	nameTxt.SetMaxWidth(430, 1);

    GuiText subnameTxt(NULL, 24, (GXColor){255, 255, 255, 255});
	subnameTxt.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	subnameTxt.SetPosition(0, 35);
	subnameTxt.SetMaxWidth(430, 1);
	
	GuiText errorTxt(NULL, 24, (GXColor){255, 255, 255, 255});
	errorTxt.SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	errorTxt.SetPosition(0, -50);
	errorTxt.SetMaxWidth(430);
	
	snprintf(progressTxt, sizeof(progressTxt), "%i/%i", progressDone, progressTotal);
	GuiText stateTxt(progressTxt, 22, (GXColor){255, 255, 255, 255});
	stateTxt.SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
	stateTxt.SetPosition(-10, -8);
	
	// Get The banner //
	ManageAllProgressBanner bannerList(bannerToDisplay);
	bannerList.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	bannerList.SetPosition(-80, -40);
	
	GuiImageData btnOutline(save_manage_button_png, save_manage_button_png_size);
	GuiImageData btnOutlineOver(save_manage_button_over_png, save_manage_button_over_png_size);
	GuiImage buttonImg(&btnOutline);
    GuiImage buttonOverImg(&btnOutlineOver);
    GuiText AbortTxt(tr("Cancel"), 22, (GXColor){0, 0, 0, 255});
	GuiButton AbortBtn(buttonImg.GetWidth(), buttonImg.GetHeight());
	AbortBtn.SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	AbortBtn.SetLabel(&AbortTxt);
	AbortBtn.SetImage(&buttonImg);
	AbortBtn.SetImageOver(&buttonOverImg);
	AbortBtn.SetSoundOver(&btnSoundOver);
    AbortBtn.SetSoundClick(&btnClick);
    AbortBtn.SetTrigger(&trigA);
	AbortBtn.SetEffectGrow();

	promptWindow.Append(&stateTxt);
    promptWindow.Append(&nameTxt);
    promptWindow.Append(&subnameTxt);
    promptWindow.Append(&errorTxt);
    promptWindow.Append(&bannerList);
    promptWindow.Append(&AbortBtn);
    promptWindow.Append(&Throbber);
	
	
	MainWindow::Instance()->HaltGui();
	promptWindow.SetEffect(EFFECT_FADE, 20);
	MainWindow::Instance()->Append(&promptWindow);
	MainWindow::Instance()->ChangeFocus(&promptWindow);
	MainWindow::Instance()->ResumeGui();

    while(promptWindow.GetEffect() > 0) usleep(100);

    while(showManageAllProgress != 0)
	{
	    usleep(80000);
		
		throbberCount++;
		if(throbberCount > 8)
			throbberCount = 1;
		
		Throbber.SetThrobberCount(throbberCount);
		
		if(changed) {
			nameTxt.SetText(progressName);
			subnameTxt.SetText(progressSubname);
			
			snprintf(progressTxt, sizeof(progressTxt), "%i/%i", progressDone, progressTotal);
			stateTxt.SetText(progressTxt);
			
			bannerList.SetBannerToDisplay(bannerToDisplay);
			
			changed = false;
		}
		
		else if(AbortBtn.GetState() == STATE_CLICKED) {
			progresscanceled = true;
			AbortBtn.ResetState();
		}
		
		if(showError == true)
		{
			AbortBtn.SetVisible(false);
			AbortBtn.SetState(STATE_DISABLED);
			errorTxt.SetText(progressError);
			errorTxt.SetVisible(true);
			
			usleep(5000000);
			
			errorTxt.SetVisible(false);
			AbortBtn.SetVisible(true);
			AbortBtn.SetState(STATE_DEFAULT);
			
			showError = false;
			
		}
	}

	promptWindow.SetEffect(EFFECT_FADE, -20);
	while(promptWindow.GetEffect() > 0) usleep(100);

	MainWindow::Instance()->HaltGui();
	MainWindow::Instance()->Remove(&promptWindow);
	MainWindow::Instance()->ResumeGui();
	
	progresscanceled = false;
}
/****************************************************************************
 * ManageAllThread
  ***************************************************************************/

static void * ManageAllThread(void *arg UNUSED)
{
	while(1)
	{
		if(!showManageAllProgress)
			LWP_SuspendThread(manageallthread);

        ManageAllProgress();

        usleep(THREAD_SLEEP);
	}
	return NULL;
}

/****************************************************************************
 * StopManageAllProgress
 ***************************************************************************/
void StopManageAllProgress()
{
	showManageAllProgress = 0;

	// wait for thread to finish
	while(!LWP_ThreadIsSuspended(manageallthread))
		usleep(THREAD_SLEEP);
}

/****************************************************************************
 * StartManageAllProgress
 ***************************************************************************/
void StartManageAllProgress()
{
    LWP_ResumeThread(manageallthread);
}

/****************************************************************************
 * ShowManageAllProgress
 *
 * Callbackfunction for updating the progress values
 ***************************************************************************/
void ShowManageAllProgress(int done, int total, const char *name, const char *subname, u32 bannerCalled)
{
	if(total <= 0)
		return;

	if(done > total)
		done = total;

    bannerToDisplay = bannerCalled;
	
	if(name)
		strncpy(progressName, name, sizeof(progressName));
	else
		sprintf(progressName, " ");
	
	if(subname)
		strncpy(progressSubname, subname, sizeof(progressSubname));
	else
		sprintf(progressSubname, " ");
	
	
    //progress start for this file
    if(!done)
		showManageAllProgress = 1;
	
	progressTotal = total;
	progressDone = done+1;
    
    changed = true;
}

/****************************************************************************
 * ManageAllProgressShowError
 *
 * Callbackfunction for updating the error message
 ***************************************************************************/
void ManageAllProgressShowError(const char *error) {
	
	if(error)
		strncpy(progressError, error, sizeof(progressError));
	else
		sprintf(progressError, " ");
	
	showError = true;
}

bool GetManageAllErrorState() {
	
	return showError;
}

/****************************************************************************
 * InitManageAllProgressThread
 *
 * Startup AllProgressthread in idle prio
 ***************************************************************************/
void InitManageAllProgressThread() {
	LWP_CreateThread(&manageallthread, ManageAllThread, NULL, NULL, 0, 70);
}

/****************************************************************************
 * ExitManageAllProgressThread
 *
 * Shutdown AllProgressthread
 ***************************************************************************/
void ExitManageAllProgressThread() {
	LWP_JoinThread(manageallthread, NULL);
	manageallthread = LWP_THREAD_NULL;
}
