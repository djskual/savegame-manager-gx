/****************************************************************************
 * Copyright (C) 2009-2011 Dimok
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
#include "ProgressWindow.h"
#include "../Controls/Application.h"
#include "../Themes/CTheme.h"
#include "ProgressTask.h"

#undef StartProgress
#undef StartThrobber
#undef ShowProgress
#undef ShowThrobber
#undef FinishProgress
#undef StopProgress
#undef ProgressCanceled

ProgressWindow * ProgressWindow::instance = NULL;

ProgressWindow::ProgressWindow()
	: GuiFrame(0, 0)
{
	type = PROGRESSBAR;
	memset(ProgressTitle, 0, sizeof(ProgressTitle));
	memset(ProgressMsg, 0, sizeof(ProgressMsg));
	progressDone = 0;
	progressTotal = 0;
	Changed = false;
	OpenRequest = false;
	CloseRequest = true;
	WindowClosed = true;
	Canceled = false;
	
	progressBar = NULL;
	
	dialogBox = NULL;
	btnOutline = NULL;
	throbber = NULL;

	dialogBoxImg = NULL;
	buttonImg = NULL;
	throbberImg = NULL;
	
	titleTxt = NULL;
	msgTxt = NULL;
	AbortBtnTxt = NULL;
	
	AbortBtn = NULL;
	
	soundClick = NULL;
	soundOver = NULL;

	trigA = NULL;
}

ProgressWindow::~ProgressWindow()
{
	Canceled = true;
	TryCloseWindow();
}

void ProgressWindow::TryOpenWindow()
{
	if(!WindowClosed)
		return;
	
	//! To skip progressbar for fast processes
	if(delayTimer.elapsedMilliSecs() < 500)
		return;
	
	WindowClosed = false;

	dialogBox = Resources::GetImageData("dialogue_box.png");
	dialogBoxImg = new GuiImage(dialogBox);
	Append(dialogBoxImg);

	width = dialogBoxImg->GetWidth();
	height = dialogBoxImg->GetHeight();

	progressBar = new ProgressBar;
	progressBar->SetAlignment(ALIGN_CENTER | ALIGN_TOP);
	progressBar->SetPosition(0, 140);
	if(type == PROGRESSBAR)
		Append(progressBar);

	throbber = Resources::GetImageData("progress_throbber.png");
	throbberImg = new GuiImage(throbber);
	throbberImg->SetAlignment(ALIGN_CENTER | ALIGN_MIDDLE);
	throbberImg->SetPosition(0, 20);
	if(type == THROBBER)
		Append(throbberImg);
	
	trigA = new GuiTrigger;
	trigA->SetSimpleTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);

	soundClick = Resources::GetSound("button_click.wav");
	soundOver = Resources::GetSound("button_over.wav");

	titleTxt = new GuiText(ProgressTitle, thInt("22 - prompt windows message text size"), thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
	titleTxt->SetAlignment(ALIGN_CENTER | ALIGN_TOP);
	titleTxt->SetPosition(0,50);
	titleTxt->SetMaxWidth(430, DOTTED);
	Append(titleTxt);

	msgTxt = new GuiText((char*) NULL, thInt("22 - prompt windows message text size"), thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
	msgTxt->SetAlignment(ALIGN_CENTER | ALIGN_TOP);
	msgTxt->SetPosition(0,100);
	msgTxt->SetMaxWidth(430, DOTTED);
	Append(msgTxt);

	btnOutline = Resources::GetImageData("button.png");
	buttonImg = new GuiImage(btnOutline);
	AbortBtnTxt = new GuiText(tr("Cancel"), thInt("20 - prompt windows button text size"), thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
	AbortBtn = new GuiButton(buttonImg->GetWidth(), buttonImg->GetHeight());
	AbortBtn->SetAlignment(ALIGN_CENTER | ALIGN_BOTTOM);
	AbortBtn->SetPosition(0, -48);
	AbortBtn->SetLabel(AbortBtnTxt);
	AbortBtn->SetImage(buttonImg);
	AbortBtn->SetTrigger(trigA);
	AbortBtn->SetSoundClick(soundClick);
	AbortBtn->SetSoundOver(soundOver);
	AbortBtn->SetEffectGrow();
	AbortBtn->Clicked.connect(this, &ProgressWindow::OnCancelClick);
	Append(AbortBtn);

	SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_IN, 50);
	SetAlignment(ALIGN_CENTER | ALIGN_MIDDLE);
	DimBackground(true);

	Application::Instance()->Append(this);
	Application::Instance()->SetUpdateOnly(this);
}

void ProgressWindow::TryCloseWindow()
{
	if(WindowClosed)
		return;
	
	static bool effectRunning = false;

	if(!effectRunning)
	{
		SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_OUT, 50);
		
		effectRunning = true;
	}

	if(GetEffect() > 0 && !Application::Instance()->isClosing())
		return;
	
	ProgressTitle[0] = 0;
	ProgressMsg[0] = 0;

	RemoveAll();
	Application::Instance()->UnsetUpdateOnly(this);
	DimBackground(false);

	if(soundClick)
		Resources::Remove(soundClick);
	if(soundOver)
		Resources::Remove(soundOver);

	if(dialogBox)
		Resources::Remove(dialogBox);
	if(btnOutline)
		Resources::Remove(btnOutline);
	if(throbber)
		Resources::Remove(throbber);
	
	if(progressBar)
		delete progressBar;
	if(throbberImg)
		delete throbberImg;
	
	delete dialogBoxImg;
	delete buttonImg;
	
	delete titleTxt;
	delete msgTxt;
	delete AbortBtnTxt;
	
	delete AbortBtn;
	
	delete trigA;

	progressBar = NULL;
	
	dialogBox = NULL;
	btnOutline = NULL;
	throbber = NULL;

	dialogBoxImg = NULL;
	buttonImg = NULL;
	throbberImg = NULL;
	
	titleTxt = NULL;
	AbortBtnTxt = NULL;
	
	AbortBtn = NULL;
	
	soundClick = NULL;
	soundOver = NULL;

	trigA = NULL;

	effectRunning = false;
	WindowClosed = true;
}

void ProgressWindow::AdvanceRotation()
{
	int iThrobberAngle = throbberImg->GetAngle() + 3;
	if(iThrobberAngle >= 360)
		iThrobberAngle -= 360;
	
	throbberImg->SetAngle(iThrobberAngle);
}

void ProgressWindow::Draw()
{
	if(OpenRequest && WindowClosed)
		TryOpenWindow();

	if(CloseRequest && !WindowClosed)
		TryCloseWindow();

	if(!WindowClosed)
	{
		if(Changed)
		{
			titleTxt->SetText(ProgressTitle);
			msgTxt->SetText(ProgressMsg);
			
			if(type == PROGRESSBAR)
			{
				float fDone = (float) progressDone;
				float fTotal = (float) progressTotal;
				
				if(fDone > fTotal)
					fDone = fTotal;
				
				float Percent;
				if(fTotal > 0.0f)
					Percent = 100.0f*fDone/fTotal;
				else
					Percent = 0.0f;
				
				Percent = LIMIT(Percent, 0.0f, 100.0f);
				
				progressBar->SetPercent(Percent);
			}
			
			Changed = false;
		}
		
		if(type == THROBBER)
			AdvanceRotation();
	}
	
	GuiFrame::Draw();
}

void ProgressWindow::StartProgress(const char *title, const char *msg)
{
	if(IsRunning() && (type == THROBBER))
	{
		Append(progressBar);
		Remove(throbberImg);
	}
	
	type = PROGRESSBAR;
	
	ProgressTask::Instance();
	
	if(title)
		strncpy(ProgressTitle, title, sizeof(ProgressTitle)-1);

	if(msg)
		strncpy(ProgressMsg, msg, sizeof(ProgressMsg)-1);
	else
		ProgressMsg[0] = 0;
	
	delayTimer.reset();

	progressDone = 0;
	progressTotal = 0;
	
	Changed = true;
	Canceled = false;
	OpenRequest = true;
	CloseRequest = false;
}

void ProgressWindow::StartThrobber(const char *title, const char *msg)
{
	if(IsRunning() && (type == PROGRESSBAR))
	{
		Remove(progressBar);
		Append(throbberImg);
	}
	
	type = THROBBER;
	
	ProgressTask::Instance();
	
	if(title)
		strncpy(ProgressTitle, title, sizeof(ProgressTitle)-1);

	if(msg)
		strncpy(ProgressMsg, msg, sizeof(ProgressMsg)-1);
	else
		ProgressMsg[0] = 0;
	
	delayTimer.reset();

	progressDone = 0;
	progressTotal = 0;
	
	Changed = true;
	Canceled = false;
	OpenRequest = true;
	CloseRequest = false;
}

void ProgressWindow::FinishProgress()
{
	if(delayTimer.elapsedMilliSecs() < 500)
		return;
	
	completeTimer.reset();
	
	ShowProgress(progressTotal, progressTotal);
	
	ProgressTask::DestroyInstance();
	
	while(completeTimer.elapsed() <= 1)
		Application::Instance()->updateEvents();
}

void ProgressWindow::StopProgress()
{
	OpenRequest = false;
	CloseRequest = true;
	ProgressTitle[0] = 0;
	ProgressMsg[0] = 0;
	
	ProgressTask::DestroyInstance();
}

void ProgressWindow::ShowProgress(const u64 &done, const u64 &total)
{
	progressDone = (done > total) ? total : done;
	progressTotal = total;
	
	Changed = true;
}

void ProgressWindow::ShowProgress(const u64 &done, const u64 &total, const char *msg)
{
	if(msg)
		strncpy(ProgressMsg, msg, sizeof(ProgressMsg)-1);
	else
		ProgressMsg[0] = 0;

	ShowProgress(done, total);
}

void ProgressWindow::ShowThrobber(const char *msg)
{
	if(msg)
		strncpy(ProgressMsg, msg, sizeof(ProgressMsg)-1);
	else
		ProgressMsg[0] = 0;
	
	Changed = true;
}

void ProgressWindow::SetTitle(const char *title)
{
	if(title)
		strncpy(ProgressTitle, title, sizeof(ProgressTitle)-1);
	else
		ProgressTitle[0] = 0;
}

void ProgressWindow::SetMessage(const char *msg)
{
	if(msg)
		strncpy(ProgressMsg, msg, sizeof(ProgressMsg)-1);
	else
		ProgressMsg[0] = 0;
}
