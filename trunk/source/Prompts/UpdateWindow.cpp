/****************************************************************************
 * Copyright (C) 2012 Dj_Skual
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
#include "UpdateWindow.h"
#include "ProgressTask.h"
#include "../Themes/CTheme.h"
#include "../Controls/Application.h"
#include "../Network/networkops.h"

bool UpdateWindow::cancelEnabled = false;

UpdateWindow::UpdateWindow()
	: GuiFrame(0, 0)
	, choice(0)
	, angle(0)
	, type(BUTTONS)
{
	btnSoundClick = Resources::GetSound("button_click.wav");
	btnSoundOver = Resources::GetSound("button_over.wav");

	trigA = new SimpleGuiTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	trigB = new GuiTrigger();
	trigB->SetButtonOnlyTrigger(-1, WiiControls.BackButton | ClassicControls.BackButton << 16, GCControls.BackButton);

	bgWindow = Resources::GetImageData("dialogue_box.png");
	bgWindowImg = new GuiImage(bgWindow);
	width = bgWindow->GetWidth();
	height = bgWindow->GetHeight();

	titleTxt = new GuiText((char*)NULL, thInt("26 - prompt windows title text size"), thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
	titleTxt->SetAlignment(ALIGN_CENTER | ALIGN_TOP);
	titleTxt->SetPosition(0,55);
	titleTxt->SetMaxWidth(this->GetWidth()-68, DOTTED);

	msgTxt = new GuiText((char*)NULL, thInt("22 - prompt windows message text size"), thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
	msgTxt->SetAlignment(ALIGN_CENTER | ALIGN_MIDDLE);
	msgTxt->SetPosition(0,-40);
	msgTxt->SetLinesToDraw(2);
	msgTxt->SetMaxWidth(this->GetWidth()-68, WRAP);

	for(int i = 0; i < 4; i++)
	{
		btnTxt[i] = NULL;
		btn[i] = NULL;
	}
	
	barOutline = Resources::GetImageData("progress_bar_outline.png");
	barEmpty = Resources::GetImageData("progress_bar_empty.png");
	bar = Resources::GetImageData("progress_bar.png");
	barOutlineImg = NULL;
	barEmptyImg = NULL;
	barImg = NULL;
	barPrTxt = NULL;
	barPrsTxt = NULL;
	
	throbber = Resources::GetImageData("progress_throbber.png");
	throbberImg = NULL;
	
	Append(bgWindowImg);
	Append(titleTxt);
	Append(msgTxt);
	
	SetEffect(EFFECT_FADE, 40);
	SetAlignment(ALIGN_CENTER | ALIGN_MIDDLE);
	DimBackground(true);
	Application::Instance()->SetUpdateOnly(this);
	Application::Instance()->Append(this);
}

UpdateWindow::~UpdateWindow()
{
	RemoveAll();

	delete bgWindowImg;
	Resources::Remove(bgWindow);

	ProgressTask::DestroyInstance();
	
	if(barImg)
	{
		delete barImg;
		delete barEmptyImg;
		delete barOutlineImg;
		delete barPrTxt;
		delete barPrsTxt;
	}
	Resources::Remove(bar);
	Resources::Remove(barEmpty);
	Resources::Remove(barOutline);

	if(throbberImg)
		delete throbberImg;
	Resources::Remove(throbber);
	
	Resources::Remove(btnSoundOver);
	Resources::Remove(btnSoundClick);

	delete titleTxt;
	delete msgTxt;

	for(int i = 0; i < 4; i++)
	{
		if(btnTxt[i])
			delete btnTxt[i];
		btnTxt[i] = NULL;
		
		if(btn[i])
			delete btn[i];
		btn[i] = NULL;
	}

	delete trigA;
	delete trigB;
}

void UpdateWindow::RemoveItems()
{
	//!remove buttons
	for(int i = 0; i < 4; i++)
	{
		if(btnTxt[i])
			delete btnTxt[i];
		btnTxt[i] = NULL;
		
		if(btn[i])
		{
			Remove(btn[i]);
			delete btn[i];
		}
		btn[i] = NULL;
	}
	
	ProgressTask::DestroyInstance();
	
	//!remove progress
	if(barImg)
	{
		Remove(barImg);
		delete barImg;
		barImg = NULL;
		
		Remove(barEmptyImg);
		delete barEmptyImg;
		barEmptyImg = NULL;
		
		Remove(barOutlineImg);
		delete barOutlineImg;
		barOutlineImg = NULL;
		
		Remove(barPrTxt);
		delete barPrTxt;
		barPrTxt = NULL;
		
		Remove(barPrsTxt);
		delete barPrsTxt;
		barPrsTxt = NULL;
	}

	//!remove throbber
	if(throbberImg)
	{
		Remove(throbberImg);
		delete throbberImg;
		throbberImg = NULL;
		
		msgTxt->SetLinesToDraw(2);
	}
}

void UpdateWindow::ShowButtons(std::string title, std::string msg, std::string btn1txt, std::string btn2txt, std::string btn3txt, std::string btn4txt)
{
	RemoveItems();
	
	titleTxt->SetText(title);
	msgTxt->SetText(msg);
	
	SetButtons(btn1txt, btn2txt, btn3txt, btn4txt);
	
	type = BUTTONS;
}

int UpdateWindow::Choose(std::string title, std::string msg, std::string btn1txt, std::string btn2txt, std::string btn3txt, std::string btn4txt)
{
	ShowButtons(title, msg, btn1txt, btn2txt, btn3txt, btn4txt);
	
	int result = 0;
	while(!(result = GetChoice()))
		Application::Instance()->updateEvents();
	
	return result;
}

void UpdateWindow::ShowMessage(std::string title, std::string msg)
{
	Choose(title, msg, tr("OK"));
}

void UpdateWindow::ShowError(std::string error)
{
	ShowMessage(tr("Error:"), error);
}

void UpdateWindow::ShowProgress(std::string title, std::string msg, int done, int total)
{
	RemoveItems();
	
	titleTxt->SetText(title);
	msgTxt->SetText(msg);
	
	if(cancelEnabled)
		SetButtons(tr("Cancel"));
	
	SetProgress(done, total);
	
	type = PROGRESS;
	
	if(!cancelEnabled)
		ProgressTask::Instance();
}

void UpdateWindow::ShowThrobber(std::string title, std::string msg)
{
	RemoveItems();
	
	titleTxt->SetText(title);
	msgTxt->SetLinesToDraw(1);
	msgTxt->SetText(msg);
	
	if(cancelEnabled)
		SetButtons(tr("Cancel"));
	
	throbberImg = new GuiImage(throbber);
	throbberImg->SetAlignment(ALIGN_CENTER | ALIGN_MIDDLE);
	throbberImg->SetPosition(0, 20);
	Append(throbberImg);
	
	type = THROBBER;
	
	if(!cancelEnabled)
		ProgressTask::Instance();
}

void UpdateWindow::SetButtons(std::string btn1txt, std::string btn2txt, std::string btn3txt, std::string btn4txt)
{
	if(!btn1txt.empty())
	{
		btnTxt[0] = new GuiText(btn1txt.c_str(), thInt("20 - prompt windows button text size"), thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
		btn[0] = new PictureButton("button.png", "button.png", btnSoundClick, btnSoundOver);
		btn[0]->SetLabel(btnTxt[0]);
		btn[0]->SetTrigger(trigA);
		btn[0]->SetState(STATE_SELECTED, -1);
		btn[0]->Clicked.connect(this, &UpdateWindow::OnButtonClick);
	}
	
	if(!btn2txt.empty())
	{
		btnTxt[1] = new GuiText(btn2txt.c_str(), thInt("20 - prompt windows button text size"), thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
		btn[1] = new PictureButton("button.png", "button.png", btnSoundClick, btnSoundOver);
		btn[1]->SetLabel(btnTxt[1]);
		if(btn3txt.empty() && btn4txt.empty())
			btn[1]->SetTrigger(trigB);
		btn[1]->SetTrigger(trigA);
		btn[1]->Clicked.connect(this, &UpdateWindow::OnButtonClick);
	}

	if(!btn3txt.empty())
	{
		btnTxt[2] = new GuiText(btn3txt.c_str(), thInt("20 - prompt windows button text size"), thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
		btn[2] = new PictureButton("button.png", "button.png", btnSoundClick, btnSoundOver);
		btn[2]->SetLabel(btnTxt[2]);
		if(btn4txt.empty())
			btn[2]->SetTrigger(trigB);
		btn[2]->SetTrigger(trigA);
		btn[2]->Clicked.connect(this, &UpdateWindow::OnButtonClick);
	}

	if(!btn4txt.empty())
	{
		btnTxt[3] = new GuiText(btn4txt.c_str(), thInt("20 - prompt windows button text size"), thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
		btn[3] = new PictureButton("button.png", "button.png", btnSoundClick, btnSoundOver);
		btn[3]->SetLabel(btnTxt[3]);
		btn[3]->SetTrigger(trigB);
		btn[3]->SetTrigger(trigA);
		btn[3]->Clicked.connect(this, &UpdateWindow::OnButtonClick);
	}

	if(btn[0] && btn[1] && !btn[2] && !btn[3])
	{
		btn[0]->SetAlignment(ALIGN_LEFT | ALIGN_BOTTOM);
		btn[0]->SetPosition(40, -50);
		btn[1]->SetAlignment(ALIGN_RIGHT | ALIGN_BOTTOM);
		btn[1]->SetPosition(-40, -50);
	}
	else if(btn[0] && btn[1] && btn[2] && !btn[3])
	{
		btn[0]->SetAlignment(ALIGN_LEFT | ALIGN_BOTTOM);
		btn[0]->SetPosition(50, -120);
		btn[1]->SetAlignment(ALIGN_RIGHT | ALIGN_BOTTOM);
		btn[1]->SetPosition(-50, -120);
		btn[2]->SetAlignment(ALIGN_CENTER | ALIGN_BOTTOM);
		btn[2]->SetPosition(0, -65);
	}
	else if(btn[0] && btn[1] && btn[2] && btn[3])
	{
		btn[0]->SetAlignment(ALIGN_LEFT | ALIGN_BOTTOM);
		btn[0]->SetPosition(50, -120);
		btn[1]->SetAlignment(ALIGN_RIGHT | ALIGN_BOTTOM);
		btn[1]->SetPosition(-50, -120);
		btn[2]->SetAlignment(ALIGN_LEFT | ALIGN_BOTTOM);
		btn[2]->SetPosition(50, -65);
		btn[3]->SetAlignment(ALIGN_RIGHT | ALIGN_BOTTOM);
		btn[3]->SetPosition(-50, -65);
	}
	else if(btn[0] && !btn[1] && btn[2] && btn[3])
	{
		btn[0]->SetAlignment(ALIGN_CENTER | ALIGN_BOTTOM);
		btn[0]->SetPosition(0, -120);
		btn[2]->SetAlignment(ALIGN_LEFT | ALIGN_BOTTOM);
		btn[2]->SetPosition(50, -65);
		btn[3]->SetAlignment(ALIGN_RIGHT | ALIGN_BOTTOM);
		btn[3]->SetPosition(-50, -65);
	}
	else if(!btn[0] && btn[1] && btn[2] && !btn[3])
	{
		btn[1]->SetAlignment(ALIGN_LEFT | ALIGN_BOTTOM);
		btn[1]->SetPosition(40, -50);
		btn[2]->SetAlignment(ALIGN_RIGHT | ALIGN_BOTTOM);
		btn[2]->SetPosition(-40, -50);
	}
	else
	{
		btn[0]->SetAlignment(ALIGN_CENTER | ALIGN_BOTTOM);
		btn[0]->SetPosition(0, -50);
	}

	for(int i = 0; i < 4; i++)
	{
		if(btn[i])
			Append(btn[i]);
	}
}

void UpdateWindow::SetProgress(int done, int total)
{
	int yPos = 20;
	
	barOutlineImg = new GuiImage(barOutline);
	barOutlineImg->SetAlignment(ALIGN_CENTER | ALIGN_MIDDLE);
	barOutlineImg->SetPosition(0, yPos);

	barEmptyImg = new GuiImage(barEmpty);
	barEmptyImg->SetAlignment(ALIGN_CENTER | ALIGN_MIDDLE);
	barEmptyImg->SetPosition(-barOutlineImg->GetWidth()/2+2, yPos);
	barEmptyImg->SetTileHorizontal(100);

	barImg = new GuiImage(bar);
	barImg->SetAlignment(ALIGN_CENTER | ALIGN_MIDDLE);
	barImg->SetPosition(-barOutlineImg->GetWidth()/2+2, yPos);
	barImg->SetTileHorizontal(100*done/total);
	
	float progressPercent;
	if(total > 0.0f)
	   progressPercent = LIMIT(100.0f * (float)done / (float)total, 0.0f, 100.0f);
	else
		progressPercent = 0.0f;
	
	barPrTxt = new GuiText(fmt("%.2f", progressPercent), thInt("22 - prompt windows message text size"), thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
	barPrTxt->SetAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
	barPrTxt->SetPosition(210, yPos);

	barPrsTxt = new GuiText("%", thInt("22 - prompt windows message text size"), thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
	barPrsTxt->SetAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
	barPrsTxt->SetPosition(barPrTxt->GetLeft()+barPrTxt->GetTextWidth()+5, yPos);
	
	Append(barEmptyImg);
	Append(barImg);
	Append(barOutlineImg);
	Append(barPrTxt);
	Append(barPrsTxt);
}

void UpdateWindow::OnButtonClick(GuiButton *sender, int pointer UNUSED, const POINT &p UNUSED)
{
	if(type != BUTTONS)
	{
		choice = PROGRESS_CANCELED;
	}
	else
	{
		for(int i = 0; i < 4; i++)
		{
			if (sender == btn[i])
			{
				choice = i+1;
				break;
			}
		}
	}
	
	ButtonClicked(this, choice);
}

int UpdateWindow::GetChoice()
{
	int ret = choice;
	choice = 0;
	return ret;
}

bool UpdateWindow::ProgressCanceled()
{
	int res = choice;
	choice = 0;
	return (res == PROGRESS_CANCELED);
}

bool UpdateWindow::NetInitPrompt()
{
	if(IsNetworkInit())
		return true;
	
	bool canceled = false;
	SetCancelEnable(true);
	ShowThrobber(tr("Network initialising..."), tr("Please wait..."));
	
	if(!Settings.AutoConnect)
	{
		InitNetworkThread();
		ResumeNetworkThread();
	}
	
	while(!(canceled = ProgressCanceled()) && !IsNetworkInit())
		Application::Instance()->updateEvents();
	
	SetCancelEnable(false);
	
	if(canceled || !IsNetworkInit())
	{
		ShowMessage(tr("No network connection."));
		return false;
	}
	
	return true;
}

void UpdateWindow::Update(GuiTrigger * t)
{
	if(!t || state == STATE_DISABLED)
		return;
	
	if(throbberImg)
	{
		if(angle++ > 360) angle = 0;
		throbberImg->SetAngle(angle);
	}
	
	GuiFrame::Update(t);
}
