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
#include <time.h>

#include "libwiigui/gui.h"
#include "saves/manage/ManageProgress.h"
#include "manage_all/ManageThrobber.h"
#include "file/fileops.h"
#include "menu/MainWindow.h"
#include "main.h"

/*** Variables used only in this file ***/
static lwp_t		managethread = LWP_THREAD_NULL;
static char			progressTitle[100];
static char			progressMsg[200];
static short		showManageProgress = 0;
static int			throbberCount = 1;
static u64			progressDone = 0;
static u64			progressTotal = 100;
static bool			changed = false;

/*** Variables used outside of this file ***/
bool            	progresscanceled = false;

/****************************************************************************
 * ManageProgress
 *
 * Opens a window, which displays progress to the user.
 ***************************************************************************/
void ManageProgress()
{
	if(!showManageProgress)
		return;

	GuiSound btnSoundOver(button_over_wav, button_over_wav_size, cfg.SFXVolume);
    GuiSound btnClick(button_click_wav, button_click_wav_size, cfg.SFXVolume);
    
	GuiWindow promptWindow(400,150);
	promptWindow.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	promptWindow.SetPosition(0, 65);

	SimpleGuiTrigger trigA(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

	ManageThrobber Throbber(0);
	Throbber.SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
	Throbber.SetPosition(45, 0);
	
	GuiText titleTxt(progressTitle, 26, (GXColor){255, 255, 255, 255});
	titleTxt.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	titleTxt.SetPosition(0,-36);
	titleTxt.SetMaxWidth(430, 1);

    GuiText msgTxt(NULL, 24, (GXColor){255, 255, 255, 255});
	msgTxt.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	msgTxt.SetMaxWidth(430, 1);

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

	promptWindow.Append(&titleTxt);
	promptWindow.Append(&msgTxt);
    promptWindow.Append(&AbortBtn);
    promptWindow.Append(&Throbber);
	
	MainWindow::Instance()->HaltGui();
	promptWindow.SetEffect(EFFECT_FADE, 20);
	MainWindow::Instance()->Append(&promptWindow);
	MainWindow::Instance()->ChangeFocus(&promptWindow);
	MainWindow::Instance()->ResumeGui();

    while(promptWindow.GetEffect() > 0) usleep(100);

    while(showManageProgress != 0 && progresscanceled == false)
	{
	    usleep(80000);
		
		throbberCount++;
		if(throbberCount > 8)
			throbberCount = 1;
		
		Throbber.SetThrobberCount(throbberCount);
		
		if(changed) {
			msgTxt.SetText(progressMsg);
			changed = false;
		}
		
	    else if(AbortBtn.GetState() == STATE_CLICKED) {
            progresscanceled = true;
            AbortBtn.ResetState();
        }
	}

	promptWindow.SetEffect(EFFECT_FADE, -20);
	while(promptWindow.GetEffect() > 0) usleep(100);

	MainWindow::Instance()->HaltGui();
	MainWindow::Instance()->Remove(&promptWindow);
	MainWindow::Instance()->ResumeGui();
}
/****************************************************************************
 * ManageThread
  ***************************************************************************/

static void * ManageThread(void *arg UNUSED)
{
	while(1)
	{
		if(!showManageProgress)
			LWP_SuspendThread(managethread);

        ManageProgress();

        usleep(THREAD_SLEEP);
	}
	return NULL;
}

/****************************************************************************
 * StopManageProgress
 ***************************************************************************/
void StopManageProgress()
{
	showManageProgress = 0;
	progresscanceled = false;

	// wait for thread to finish
	while(!LWP_ThreadIsSuspended(managethread))
		usleep(THREAD_SLEEP);
}

/****************************************************************************
 * StartManageProgress
 ***************************************************************************/
void StartManageProgress(const char *title)
{
    if(title)
		strncpy(progressTitle, title, sizeof(progressTitle));
	else
		sprintf(progressTitle, " ");
	
	LWP_ResumeThread(managethread);
}

/****************************************************************************
 * ShowManageProgress
 *
 * Callbackfunction for updating the progress values
 ***************************************************************************/
void ShowManageProgress(u64 done, u64 total, const char *msg)
{
	if(total <= 0)
		return;

	if(done > total)
		done = total;

    if(msg)
		strncpy(progressMsg, msg, sizeof(progressMsg));
	else
		sprintf(progressMsg, " ");
		
	if(!done) //progress start
        showManageProgress = 1;

    progressDone = done;
    progressTotal = total;

    changed = true;
}

/****************************************************************************
 * InitManageProgressThread
 *
 * Startup Progressthread in idle prio
 ***************************************************************************/
void InitManageProgressThread() {
	LWP_CreateThread(&managethread, ManageThread, NULL, NULL, 0, 70);
}

/****************************************************************************
 * ExitManageProgressThread
 *
 * Shutdown Progressthread
 ***************************************************************************/
void ExitManageProgressThread() {
	LWP_JoinThread(managethread, NULL);
	managethread = LWP_THREAD_NULL;
}
