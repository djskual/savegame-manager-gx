 /****************************************************************************
 * Copyright (C) 2009
 * by Dimok
 * Modified by Dj_Skual
 * for SaveGame Manager GX
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
#include "prompts/ProgressWindow.h"
#include "file/fileops.h"
#include "menu/MainWindow.h"
#include "main.h"

#define KBSIZE       1024.0
#define MBSIZE       1048576.0
#define GBSIZE       1073741824.0

bool actioncanceled = false;

ProgressWindow * ProgressWindow::instance = NULL;

ProgressWindow::ProgressWindow()
{
    ProgressThread = LWP_THREAD_NULL;
    showProgress = 0;
    TotalDone = 0;
    TotalSize = 0;
    progressDone = 0;
    progressTotal = 0;
    Changed = false;
    resetstate = false;
	ProgressMsg[0] = '\0';

    dialogBox = NULL;
    progressbarOutline = NULL;
    progressbarEmpty = NULL;
	progressbar = NULL;
	throbber = NULL;
    btnOutline = NULL;
    dialogBoxImg = NULL;
    progressbarOutlineImg = NULL;
    progressbarEmptyImg = NULL;
    progressbarImg = NULL;
    throbberImg = NULL;
    buttonImg = NULL;
    titleTxt = NULL;
    msgTxt = NULL;
    prTxt = NULL;
    prsTxt = NULL;
    speedTxt = NULL;
    sizeTxt = NULL;
    TimeTxt = NULL;
    AbortTxt = NULL;
    AbortBtn = NULL;
    trigA = NULL;
    soundClick = NULL;
    soundOver = NULL;

    LWP_CreateThread(&ProgressThread, ThreadCallback, this, NULL, 0, 70);
}

ProgressWindow::~ProgressWindow()
{
    actioncanceled = true;
    showProgress = 0;
    LWP_ResumeThread(ProgressThread);
	LWP_JoinThread(ProgressThread, NULL);
	ProgressThread = LWP_THREAD_NULL;
	
	ClearMemory();
}

ProgressWindow * ProgressWindow::Instance()
{
	if (instance == NULL)
	{
		instance = new ProgressWindow();
	}
	return instance;
}

void ProgressWindow::DestroyInstance()
{
    if(instance)
    {
        delete instance;
        instance = NULL;
    }
}

void ProgressWindow::LoadWindow()
{
	if(!showProgress)
        return;

	SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	SetPosition(0, -10);

    dialogBox = Resources::GetImageData(dialogue_box_png, dialogue_box_png_size);
	dialogBoxImg = new GuiImage(dialogBox);
	Append(dialogBoxImg);

    width = dialogBox->GetWidth();
    height = dialogBox->GetHeight();

	trigA = new SimpleGuiTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

	soundClick = Resources::GetSound(button_click_wav, button_click_wav_size, cfg.SFXVolume);
	soundOver = Resources::GetSound(button_over_wav, button_over_wav_size, cfg.SFXVolume);

	titleTxt = new GuiText(ProgressTitle.c_str(), 24, (GXColor){0, 0, 0, 255});
	titleTxt->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	titleTxt->SetPosition(0,50);
	titleTxt->SetMaxWidth(430, 1);
	Append(titleTxt);

    msgTxt = new GuiText((char*) NULL, 22, (GXColor){0, 0, 0, 255});
	msgTxt->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	msgTxt->SetPosition(0,110);
	msgTxt->SetMaxWidth(430, 1);
	Append(msgTxt);

    btnOutline = Resources::GetImageData(button_png, button_png_size);
	buttonImg = new GuiImage(btnOutline);
    AbortTxt = new GuiText(tr("Cancel"), 22, (GXColor){0, 0, 0, 255});
	AbortBtn = new GuiButton(buttonImg->GetWidth(), buttonImg->GetHeight());
	AbortBtn->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	AbortBtn->SetPosition(0, -48);
	AbortBtn->SetLabel(AbortTxt);
	AbortBtn->SetImage(buttonImg);
	AbortBtn->SetSoundClick(soundClick);
	AbortBtn->SetSoundOver(soundOver);
	AbortBtn->SetTrigger(trigA);
	AbortBtn->SetEffectGrow();
	Append(AbortBtn);

    if(showProgress == PROGRESSBAR || showProgress == LANGUAGE)
        SetupProgressbar();

    else if(showProgress == THROBBER || showProgress == CONNECT)
        SetupThrobber();

    SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_IN, 50);

    if(resetstate)
		MainWindow::Instance()->SetState(STATE_DISABLED);
	MainWindow::Instance()->Append(this);

	MainLoop();

    MainWindow::Instance()->HaltGui();
    MainWindow::Instance()->Remove(this);
    if(resetstate)
		MainWindow::Instance()->SetState(STATE_DEFAULT);
    ClearMemory();
    MainWindow::Instance()->ResumeGui();

	actioncanceled = false;
}

void ProgressWindow::MainLoop()
{
    int angle = 0;

	while(showProgress)
	{
	    usleep(10000);

	    if(showProgress == CONNECT || showProgress == THROBBER)
	    {
            angle = (angle + 5) % 360;
            if(throbberImg)
                throbberImg->SetAngle(angle);
	    }

        if(Changed && showProgress != CONNECT)
	    {
            msgTxt->SetText(ProgressMsg);

            UpdateValues();

            Changed = false;
	    }

        else if(AbortBtn->GetState() == STATE_CLICKED)
        {
            actioncanceled = true;
            AbortBtn->ResetState();
        }
	}

	SetEffect(EFFECT_SLIDE_BOTTOM | EFFECT_SLIDE_OUT, 50);
	while(GetEffect() > 0) usleep(100);
	
}

void ProgressWindow::SetupProgressbar()
{
    progressbarEmpty = Resources::GetImageData(progressbar_empty_png, progressbar_empty_png_size);
	progressbarEmptyImg = new GuiImage(progressbarEmpty);
	progressbarEmptyImg->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	progressbarEmptyImg->SetPosition(35, 15);
	progressbarEmptyImg->SetTile(100);
	Append(progressbarEmptyImg);

    progressbar = Resources::GetImageData(progressbar_png, progressbar_png_size);
	progressbarImg = new GuiImage(progressbar);
    progressbarImg->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	progressbarImg->SetPosition(35, 15);
	Append(progressbarImg);

	progressbarOutline = Resources::GetImageData(progressbar_outline_png, progressbar_outline_png_size);
	progressbarOutlineImg = new GuiImage(progressbarOutline);
	progressbarOutlineImg->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	progressbarOutlineImg->SetPosition(35, 15);
	Append(progressbarOutlineImg);

    prTxt = new GuiText((char*) NULL, 24, (GXColor){0, 0, 0, 255});
	prTxt->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	prTxt->SetPosition(210, 15);
	Append(prTxt);

	prsTxt = new GuiText("%", 24, (GXColor){0, 0, 0, 255});
	prsTxt->SetAlignment(ALIGN_RIGHT, ALIGN_MIDDLE);
	prsTxt->SetPosition(-178, 15);
	Append(prsTxt);

    sizeTxt = new GuiText((char*) NULL, 24, (GXColor){0, 0, 0, 255});
    sizeTxt->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	sizeTxt->SetPosition(50, 195);
	if (showProgress == PROGRESSBAR)
		Append(sizeTxt);

    speedTxt = new GuiText((char*) NULL, 24, (GXColor){0, 0, 0, 255});
    speedTxt->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	speedTxt->SetPosition(350, 195);
	if (showProgress == PROGRESSBAR)
		Append(speedTxt);
}

void ProgressWindow::SetupThrobber()
{
    throbber = Resources::GetImageData(throbber_png, throbber_png_size);
    throbberImg = new GuiImage(throbber);
	throbberImg->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	throbberImg->SetPosition(0, -20);
	if(showProgress != CONNECT)
		throbberImg->SetPosition(0, 15);
	Append(throbberImg);
}

void ProgressWindow::UpdateValues()
{
    if(!showProgress || showProgress == CONNECT || showProgress == THROBBER)
		return;

    //! File progress
    float Percent = progressTotal > 0 ? 100.0f*progressDone/progressTotal : 0.0f;

    progressbarImg->SetTile((int)Percent);

    if(Percent >= 100.0f)
        prTxt->SetTextf("%0.1f", Percent);
    else
        prTxt->SetTextf("%0.2f", Percent);

    int speed = 0;

    if(TimerSize > 0)
        speed = (int) (TimerSize/ProgressTimer.elapsed());

    speedTxt->SetTextf("%iKB/s", (int) (speed/KBSIZE));

    if(progressTotal > KBSIZE && progressTotal < MBSIZE)
        sizeTxt->SetTextf("%0.2fKB/%0.2fKB", progressDone/KBSIZE, progressTotal/KBSIZE);
    else if(progressTotal > MBSIZE && progressTotal < GBSIZE)
        sizeTxt->SetTextf("%0.2fMB/%0.2fMB", progressDone/MBSIZE, progressTotal/MBSIZE);
    else if(progressTotal > GBSIZE)
        sizeTxt->SetTextf("%0.2fGB/%0.2fGB", progressDone/GBSIZE, progressTotal/GBSIZE);
    else
        sizeTxt->SetTextf("%lldB/%lldB", progressDone, progressTotal);

    
}

void * ProgressWindow::ThreadCallback(void *arg)
{
    ((ProgressWindow *) arg)->InternalUpdate();
	return NULL;
}

void ProgressWindow::InternalUpdate()
{
	while(1)
	{
		if(!showProgress)
			LWP_SuspendThread(ProgressThread);
		
        if(showProgress)
			LoadWindow();
	}
}

void ProgressWindow::StartProgress(const char *title, int progressmode, bool reset)
{
    if(title)
        ProgressTitle.assign(title);
    if(titleTxt)
        titleTxt->SetText(title);

	showProgress = progressmode;
	
    TimerSize = 0;
    ProgressTimer.reset();

    resetstate = reset;
	
    LWP_ResumeThread(ProgressThread);
}

void ProgressWindow::ShowProgress(u64 done, u64 total, const char *msg)
{
	if(done > total)
		done = total;

    if(msg)
        strcpy(ProgressMsg, msg);
    else
        strcpy(ProgressMsg, "");

    if(!done)
    {
	    TotalDone += progressTotal;
        if(TotalSize == 0)
            TotalSize = total;
    }

    if(done > progressDone)
        TimerSize += done-progressDone;

    progressDone = done;
    progressTotal = total;

    Changed = true;
}

void ProgressWindow::StopProgress()
{
	showProgress = 0;

	while(!LWP_ThreadIsSuspended(ProgressThread))
        usleep(100);
	
}

void ProgressWindow::ClearMemory()
{
    RemoveAll();

    if(soundClick)
        Resources::Remove(soundClick);
    if(soundOver)
        Resources::Remove(soundOver);

    if(dialogBox)
        Resources::Remove(dialogBox);
    if(progressbarOutline)
        Resources::Remove(progressbarOutline);
    if(progressbarEmpty)
        Resources::Remove(progressbarEmpty);
    if(progressbar)
        Resources::Remove(progressbar);
    if(throbber)
        Resources::Remove(throbber);
    if(btnOutline)
        Resources::Remove(btnOutline);

    if(dialogBoxImg)
		delete dialogBoxImg;
    if(progressbarOutlineImg)
		delete progressbarOutlineImg;
    if(progressbarEmptyImg)
		delete progressbarEmptyImg;
    if(progressbarImg)
		delete progressbarImg;
    if(throbberImg)
		delete throbberImg;
    if(buttonImg)
		delete buttonImg;
    
    if(titleTxt)
        delete titleTxt;
    if(msgTxt)
        delete msgTxt;
    if(prTxt)
        delete prTxt;
    if(prsTxt)
        delete prsTxt;
    if(speedTxt)
        delete speedTxt;
    if(sizeTxt)
        delete sizeTxt;
    if(TimeTxt)
        delete TimeTxt;
    if(AbortTxt)
        delete AbortTxt;
    
    if(AbortBtn)
        delete AbortBtn;
    
    if(trigA)
        delete trigA;

    dialogBox = NULL;
    progressbarOutline = NULL;
    progressbarEmpty = NULL;
	progressbar = NULL;
	throbber = NULL;
    btnOutline = NULL;
    dialogBoxImg = NULL;
    progressbarOutlineImg = NULL;
    progressbarEmptyImg = NULL;
    progressbarImg = NULL;
    throbberImg = NULL;
    buttonImg = NULL;
    titleTxt = NULL;
    msgTxt = NULL;
    prTxt = NULL;
    prsTxt = NULL;
    speedTxt = NULL;
    sizeTxt = NULL;
    TimeTxt = NULL;
    AbortTxt = NULL;
    AbortBtn = NULL;
    trigA = NULL;
    soundClick = NULL;
    soundOver = NULL;
}

void StartProgress(const char *title, int mode, bool reset)
{
    ProgressWindow::Instance()->StartProgress(title, mode, reset);
}

void ShowProgress(u64 done, u64 total, const char *msg)
{
    ProgressWindow::Instance()->ShowProgress(done, total, msg);
}

void StopProgress()
{
    ProgressWindow::Instance()->StopProgress();
}
