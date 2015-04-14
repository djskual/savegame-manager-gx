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
#include "manage_all/ManageMiiProgress.h"
#include "manage_all/ManageThrobber.h"
#include "file/fileops.h"
#include "menu/MainWindow.h"
#include "main.h"

/*** Variables used only in this file ***/
static lwp_t		managemiithread = LWP_THREAD_NULL;
static char			progressName[200];
static char			progressCreator[200];
static char			progressTxt[20];
static char			progressError[200];
static int			progressDay = 0;
static int			progressMonth = 0;
static int			progressFemale = 0;
static int			progressColor = 0;
static short		showManageMiiProgress = 0;
static int			throbberCount = 1;
static int			progressDone = 0;
static int			progressTotal = 100;
static bool			changed = false;
static bool			showError = false;
static char			monthTxt[20];
static char			dateText[30];
static int			red = 0;
static int			green = 0;
static int			blue = 0;

/*** Variables used outside of this file ***/
bool            	progressmiicanceled = false;

int GetMonth(int m)
{
    switch (m)
	{
		case 0: { snprintf(monthTxt, sizeof(monthTxt), " "); break; }
		case 1: { snprintf(monthTxt, sizeof(monthTxt), tr("January")); break; }
		case 2: { snprintf(monthTxt, sizeof(monthTxt), tr("February")); break; }
		case 3: { snprintf(monthTxt, sizeof(monthTxt), tr("March")); break; }
		case 4: { snprintf(monthTxt, sizeof(monthTxt), tr("April")); break; }
		case 5: { snprintf(monthTxt, sizeof(monthTxt), tr("May")); break; }
		case 6: { snprintf(monthTxt, sizeof(monthTxt), tr("June")); break; }
		case 7: { snprintf(monthTxt, sizeof(monthTxt), tr("July")); break; }
		case 8: { snprintf(monthTxt, sizeof(monthTxt), tr("August")); break; }
		case 9: { snprintf(monthTxt, sizeof(monthTxt), tr("September")); break; }
		case 10: { snprintf(monthTxt, sizeof(monthTxt), tr("October")); break; }
		case 11: { snprintf(monthTxt, sizeof(monthTxt), tr("November")); break; }
		case 12: { snprintf(monthTxt, sizeof(monthTxt), tr("December")); break; }
		default: { snprintf(monthTxt, sizeof(monthTxt), tr(" ")); break; }
	}
	return 1;
}

int GetFavColor(int c)
{
	switch (c)
	{
		case 0: { red = 255; green = 0; blue = 0; break; }
		case 1: { red = 255; green = 150; blue = 0; break; }
		case 2: { red = 255; green = 255; blue = 0; break; }
		case 3: { red = 140; green = 255; blue = 0; break; }
		case 4: { red = 0; green = 130; blue = 0; break; }
		case 5: { red = 0; green = 100; blue = 255; break; }
		case 6: { red = 0; green = 200; blue = 255; break; }
		case 7: { red = 255; green = 50; blue = 140; break; }
		case 8: { red = 190; green = 0; blue = 255; break; }
		case 9: { red = 140; green = 80; blue = 45; break; }
		case 10: { red = 255; green = 255; blue = 255; break; }
		case 11: { red = 0; green = 0; blue = 0; break; }
		default: { red = 0; green = 0; blue = 0; break; }
	}
	return 1;
}
	
/****************************************************************************
 * ManageMiiProgress
 *
 * Opens a window, which displays progress to the user.
 ***************************************************************************/
void ManageMiiProgress()
{
	if(!showManageMiiProgress)
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
	
	GuiText nameTxt(NULL, 30, (GXColor){255, 255, 255, 255});
	nameTxt.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	nameTxt.SetPosition(0,-90);
	nameTxt.SetMaxWidth(430, 1);

    GuiText errorTxt(NULL, 24, (GXColor){255, 255, 255, 255});
	errorTxt.SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	errorTxt.SetPosition(0, -40);
	errorTxt.SetMaxWidth(430);
	
	snprintf(progressTxt, sizeof(progressTxt), "%i/%i", progressDone, progressTotal);
	GuiText stateTxt(progressTxt, 22, (GXColor){255, 255, 255, 255});
	stateTxt.SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
	stateTxt.SetPosition(-10, -8);
	
	GuiText creatorTxt(progressCreator, 22, (GXColor){255, 255, 255, 255});
    creatorTxt.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
    creatorTxt.SetPosition(25, 0);
    creatorTxt.SetMaxWidth(350, GuiText::SCROLL);
	
	GuiImageData creatorIcon(icon_creator_png, icon_creator_png_size);
	GuiImage creatorIconImg(&creatorIcon);
	creatorIconImg.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	
	if(progressDay && progressMonth) {
		GetMonth(progressMonth);
		snprintf(dateText, sizeof(dateText), "%i %s", progressDay, monthTxt); }
	else {
		snprintf(dateText, sizeof(dateText), "../..");}
	GuiText dateTxt(dateText, 22, (GXColor){255, 255, 255, 255});
    dateTxt.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
    dateTxt.SetPosition(25, 24+15);
    dateTxt.SetMaxWidth(350, GuiText::SCROLL);
	
	GuiImageData dateIcon(icon_date_png, icon_date_png_size);
	GuiImage dateIconImg(&dateIcon);
	dateIconImg.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	
	creatorIconImg.SetPosition(-((creatorTxt.GetTextWidth()/2)+15), 0);
    dateIconImg.SetPosition(-((dateTxt.GetTextWidth()/2)+15), 24+15);
    
	GuiImageData femaledata(icon_female_png, icon_female_png_size);
	GuiImage femaleImg(&femaledata);
	femaleImg.SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
    femaleImg.SetVisible(progressFemale ? true : false);
    
	GuiImageData maledata(icon_male_png, icon_male_png_size);
	GuiImage maleImg(&maledata);
	maleImg.SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
    maleImg.SetVisible(progressFemale ? false : true);
    
	if(progressColor>=0 && progressColor<12)
		GetFavColor(progressColor);
	
	GuiImage favColorImg(24, 24, (GXColor){red, green, blue, 255});
	favColorImg.SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
    
	GuiWindow infos(48+15, 72+30);
	infos.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
    infos.SetPosition(0, -40);
	
	infos.Append(&creatorTxt);
	infos.Append(&creatorIconImg);
	infos.Append(&dateTxt);
	infos.Append(&dateIconImg);
	infos.Append(&femaleImg);
	infos.Append(&maleImg);
	if(progressColor>=0 && progressColor<12)
		infos.Append(&favColorImg);
    
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

	promptWindow.Append(&nameTxt);
	promptWindow.Append(&errorTxt);
    promptWindow.Append(&stateTxt);
	promptWindow.Append(&infos);
    promptWindow.Append(&AbortBtn);
    promptWindow.Append(&Throbber);
	
	MainWindow::Instance()->HaltGui();
	promptWindow.SetEffect(EFFECT_FADE, 20);
	MainWindow::Instance()->Append(&promptWindow);
	MainWindow::Instance()->ChangeFocus(&promptWindow);
	MainWindow::Instance()->ResumeGui();

    while(promptWindow.GetEffect() > 0) usleep(100);

    while(showManageMiiProgress != 0 && progressmiicanceled == false)
	{
	    usleep(80000);
		
		throbberCount++;
		if(throbberCount > 8)
			throbberCount = 1;
		
		Throbber.SetThrobberCount(throbberCount);
		
		if(changed) {
			// name update
			nameTxt.SetText(progressName);
			
			// progress update
			snprintf(progressTxt, sizeof(progressTxt), "%i/%i", progressDone, progressTotal);
			stateTxt.SetText(progressTxt);
			
			// creator update
			if(progressCreator[0] == '\0')
				snprintf(progressCreator, sizeof(progressCreator), "...");
			creatorTxt.SetText(progressCreator);
			
			// date update
			if(progressDay && progressMonth) {
				GetMonth(progressMonth);
				snprintf(dateText, sizeof(dateText), "%i %s", progressDay, monthTxt); }
			else {
				snprintf(dateText, sizeof(dateText), "../..");}
			dateTxt.SetText(dateText);
			
			// creator and date position update
			creatorIconImg.SetPosition(-((creatorTxt.GetTextWidth()/2)+15), 0);
			dateIconImg.SetPosition(-((dateTxt.GetTextWidth()/2)+15), 24+15);
			
			// gender update
			femaleImg.SetVisible(progressFemale ? true : false);
			maleImg.SetVisible(progressFemale ? false : true);
			
			// color update
			if(progressColor>=0 && progressColor<12)
				GetFavColor(progressColor);
			favColorImg.SetImage(24, 24, (GXColor){red, green, blue, 255});
			
			changed = false;
		}
		
	    else if(AbortBtn.GetState() == STATE_CLICKED) {
            progressmiicanceled = true;
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

}
/****************************************************************************
 * ManageMiiThread
  ***************************************************************************/

static void * ManageMiiThread(void *arg UNUSED)
{
	while(1)
	{
		if(!showManageMiiProgress)
			LWP_SuspendThread(managemiithread);

        ManageMiiProgress();

        usleep(THREAD_SLEEP);
	}
	return NULL;
}

/****************************************************************************
 * StopManageMiiProgress
 ***************************************************************************/
void StopManageMiiProgress()
{
	showManageMiiProgress = 0;
	progressmiicanceled = false;

	// wait for thread to finish
	while(!LWP_ThreadIsSuspended(managemiithread))
		usleep(THREAD_SLEEP);
}

/****************************************************************************
 * StartManageMiiProgress
 ***************************************************************************/
void StartManageMiiProgress()
{
    LWP_ResumeThread(managemiithread);
}

/****************************************************************************
 * ShowManageMiiProgress
 *
 * Callbackfunction for updating the progress values
 ***************************************************************************/
void ShowManageMiiProgress(int done,
						   int total,
						   const char *name,
						   const char *creator,
						   int day,
						   int month,
						   int female,
						   int favColor)
{
	if(total <= 0)
		return;

	if(done > total)
		done = total;

    if(name)
		strncpy(progressName, name, sizeof(progressName));
	else
		sprintf(progressName, " ");
	if(creator)
		strncpy(progressCreator, creator, sizeof(progressCreator));
	else
		sprintf(progressCreator, "...");
	
	progressDay = day;
	progressMonth = month;
	progressFemale = female;
	progressColor = favColor;
	
	if(!done) //progress start
        showManageMiiProgress = 1;

    progressDone = done;
    progressTotal = total;

    changed = true;
}

/****************************************************************************
 * ManageMiiProgressShowError
 *
 * Callbackfunction for updating the error message
 ***************************************************************************/
void ManageMiiProgressShowError(const char *error) {
	
	if(error)
		strncpy(progressError, error, sizeof(progressError));
	else
		sprintf(progressError, " ");
	
	showError = true;
}

bool GetManageMiiErrorState() {
	
	return showError;
}

/****************************************************************************
 * InitManageMiiProgressThread
 *
 * Startup Progressthread in idle prio
 ***************************************************************************/
void InitManageMiiProgressThread() {
	LWP_CreateThread(&managemiithread, ManageMiiThread, NULL, NULL, 0, 70);
}

/****************************************************************************
 * ExitManageMiiProgressThread
 *
 * Shutdown Progressthread
 ***************************************************************************/
void ExitManageMiiProgressThread() {
	LWP_JoinThread(managemiithread, NULL);
	managemiithread = LWP_THREAD_NULL;
}
