/***************************************************************************
 * Copyright (C) 2011
 * by Dj_Skual
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
 * TaskBar.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include <gccore.h>
#include <unistd.h>

#include "TaskBar.hpp"
#include "MainMenu.h"
#include "../Tools/PasswordCheck.h"
#include "../Controls/Application.h"
#include "../DeviceControls/DeviceHandler.hpp"
#include "../Prompts/PromptWindows.h"
#include "../Themes/CTheme.h"

TaskBar *TaskBar::instance = NULL;

/**
 * Constructor for the TaskBar class.
 */
TaskBar::TaskBar()
	: GuiFrame(0, 0)
{
	BtnSoundClick = Resources::GetSound("button_click.wav");
	BtnSoundOver = Resources::GetSound("button_over.wav");
	BtnSoundClick->SetVolume(Settings.SFXVolume);
	BtnSoundOver->SetVolume(Settings.SFXVolume);
	
	trigA = new SimpleGuiTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	
	sdIcon = Resources::GetImageData("taskbar_sd.png");
	sdIcon_gray = Resources::GetImageData("taskbar_sd_gray.png");
	usbIcon = Resources::GetImageData("taskbar_usb.png");
	usbIcon_gray = Resources::GetImageData("taskbar_usb_gray.png");
	wiiIcon = Resources::GetImageData("taskbar_wii.png");
	wiiIcon_gray = Resources::GetImageData("taskbar_wii_gray.png");
	miiIcon = Resources::GetImageData("taskbar_mii.png");
	miiIcon_gray = Resources::GetImageData("taskbar_mii_gray.png");
	arrangeIcon = Resources::GetImageData("taskbar_arrangeIcon.png");
	arrangeIcon_gray = Resources::GetImageData("taskbar_arrangeIcon_gray.png");
	arrangeList = Resources::GetImageData("taskbar_arrangeList.png");
	arrangeList_gray = Resources::GetImageData("taskbar_arrangeList_gray.png");
	lockedIcon = Resources::GetImageData("taskbar_locked.png");
	lockedIcon_gray = Resources::GetImageData("taskbar_locked_gray.png");
	unlockedIcon = Resources::GetImageData("taskbar_unlocked.png");
	unlockedIcon_gray = Resources::GetImageData("taskbar_unlocked_gray.png");
	searchIcon = Resources::GetImageData("taskbar_search.png");
	searchIcon_gray = Resources::GetImageData("taskbar_search_gray.png");
	
	sdIconImg = new GuiImage(sdIcon);
	sdIconImg_gray = new GuiImage(sdIcon_gray);
	usbIconImg = new GuiImage(usbIcon);
	usbIconImg_gray = new GuiImage(usbIcon_gray);
	wiiIconImg = new GuiImage(wiiIcon);
	wiiIconImg_gray = new GuiImage(wiiIcon_gray);
	miiIconImg = new GuiImage(miiIcon);
	miiIconImg_gray = new GuiImage(miiIcon_gray);
	arrangeIconImg = new GuiImage(arrangeIcon);
	arrangeIconImg_gray = new GuiImage(arrangeIcon_gray);
	arrangeListImg = new GuiImage(arrangeList);
	arrangeListImg_gray = new GuiImage(arrangeList_gray);
	lockedIconImg = new GuiImage(lockedIcon);
	lockedIconImg_gray = new GuiImage(lockedIcon_gray);
	unlockedIconImg = new GuiImage(unlockedIcon);
	unlockedIconImg_gray = new GuiImage(unlockedIcon_gray);
	searchIconImg = new GuiImage(searchIcon);
	searchIconImg_gray = new GuiImage(searchIcon_gray);
	
	width = sdIcon->GetWidth()*4;
	height = sdIcon->GetHeight();
	
	int buttonPos = 0;
	
	Tooltip[0] = new GuiTooltip(tr( "Select Device" ));
	Tooltip[0]->SetAlpha(thInt("255 - tooltip alpha"));
	Tooltip[0]->SetAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
	Tooltip[0]->SetPosition(sdIcon_gray->GetWidth()+5, 0);
	
	Buttons[0] = new PictureButton("taskbar_sd_gray.png", "taskbar_sd.png", BtnSoundOver,BtnSoundClick);
	Buttons[0]->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	Buttons[0]->SetPosition(buttonPos, 0);
	Buttons[0]->SetParent(this);
	Buttons[0]->SetTrigger(trigA);
	Buttons[0]->SetTooltip(Tooltip[0]);
	Buttons[0]->Clicked.connect(this, &TaskBar::OnDeviceButtonClick);
	buttonPos += sdIcon_gray->GetWidth();
	
	Tooltip[1] = new GuiTooltip(tr( "Select Mode" ));
	Tooltip[1]->SetAlpha(thInt("255 - tooltip alpha"));
	Tooltip[1]->SetAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
	Tooltip[1]->SetPosition(arrangeIcon_gray->GetWidth()+5, 0);
	
	Buttons[1] = new PictureButton("taskbar_arrangeIcon_gray.png", "taskbar_arrangeIcon.png", BtnSoundOver,BtnSoundClick);
	Buttons[1]->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	Buttons[1]->SetPosition(buttonPos, 0);
	Buttons[1]->SetParent(this);
	Buttons[1]->SetTrigger(trigA);
	Buttons[1]->SetTooltip(Tooltip[1]);
	Buttons[1]->Clicked.connect(this, &TaskBar::OnModeButtonClick);
	buttonPos += arrangeIcon_gray->GetWidth();
	
	Tooltip[2] = new GuiTooltip(Settings.GodMode ? tr( "Lock Console" ) : tr( "Unlock Console" ));
	Tooltip[2]->SetAlpha(thInt("255 - tooltip alpha"));
	Tooltip[2]->SetAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
	Tooltip[2]->SetPosition(lockedIcon->GetWidth()+5, 0);
	
	Buttons[2] = new PictureButton((Settings.GodMode) ? "taskbar_unlocked_gray.png" : "taskbar_locked_gray.png", (Settings.GodMode) ? "taskbar_unlocked.png" : "taskbar_locked.png", BtnSoundOver,BtnSoundClick);
	Buttons[2]->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	Buttons[2]->SetPosition(buttonPos, 0);
	Buttons[2]->SetParent(this);
	Buttons[2]->SetTrigger(trigA);
	Buttons[2]->SetTooltip(Tooltip[2]);
	Buttons[2]->Clicked.connect(this, &TaskBar::OnLockButtonClick);
	buttonPos += lockedIcon->GetWidth();
	
	Tooltip[3] = new GuiTooltip(tr( "Search" ));
	Tooltip[3]->SetAlpha(thInt("255 - tooltip alpha"));
	Tooltip[3]->SetAlignment(ALIGN_RIGHT | ALIGN_MIDDLE);
	Tooltip[3]->SetPosition(-searchIcon_gray->GetWidth()-5, 0);
	
	Buttons[3] = new PictureButton("taskbar_search_gray.png", "taskbar_search.png", BtnSoundOver,BtnSoundClick);
	Buttons[3]->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	Buttons[3]->SetPosition(buttonPos, 0);
	Buttons[3]->SetParent(this);
	Buttons[3]->SetTrigger(trigA);
	Buttons[3]->SetTooltip(Tooltip[3]);
	Buttons[3]->Clicked.connect(this, &TaskBar::OnSearchButtonClick);
	
	SetAlignment(thAlign("center - taskbar align hor") | thAlign("top - taskbar align ver"));
	SetPosition(thInt("0 - taskbar pos x"), thInt("10 - taskbar pos y"));
	
	SetModeButton();
	SetLockButton();
	
	this->TaskbarClicked.connect(MainMenu::Instance()->GetInstance(), &MainMenu::OnTaskbarClick);
	MainMenu::Instance()->DeviceChanged.connect(this, &TaskBar::OnDeviceChange);
	MainMenu::Instance()->SearchStateChanged.connect(this, &TaskBar::OnSearchWindowStateChange);
}

/**
 * Destructor for the TaskBar class.
 */
TaskBar::~TaskBar()
{
	RemoveAll();

	for(int i = 0; i < 4; i++)
		delete Buttons[i];
	
	for(int i = 0; i < 4; i++)
		delete Tooltip[i];
	
	Resources::Remove(sdIcon);
	Resources::Remove(sdIcon_gray);
	Resources::Remove(usbIcon);
	Resources::Remove(usbIcon_gray);
	Resources::Remove(wiiIcon);
	Resources::Remove(wiiIcon_gray);
	Resources::Remove(miiIcon);
	Resources::Remove(miiIcon_gray);
	Resources::Remove(arrangeIcon);
	Resources::Remove(arrangeIcon_gray);
	Resources::Remove(arrangeList);
	Resources::Remove(arrangeList_gray);
	Resources::Remove(lockedIcon);
	Resources::Remove(lockedIcon_gray);
	Resources::Remove(unlockedIcon);
	Resources::Remove(unlockedIcon_gray);
	Resources::Remove(searchIcon);
	Resources::Remove(searchIcon_gray);
	
	delete sdIconImg;
	delete sdIconImg_gray;
	delete usbIconImg;
	delete usbIconImg_gray;
	delete wiiIconImg;
	delete wiiIconImg_gray;
	delete miiIconImg;
	delete miiIconImg_gray;
	delete arrangeIconImg;
	delete arrangeIconImg_gray;
	delete arrangeListImg;
	delete arrangeListImg_gray;
	delete lockedIconImg;
	delete lockedIconImg_gray;
	delete unlockedIconImg;
	delete unlockedIconImg_gray;
	delete searchIconImg;
	delete searchIconImg_gray;
	
	Resources::Remove(BtnSoundClick);
	Resources::Remove(BtnSoundOver);
	
	delete trigA;
}

void TaskBar::show()
{
	SetEffect(EFFECT_FADE, 50);
	
	MainMenu::Instance()->Append(this);
}

void TaskBar::hide()
{
	if(!Application::isClosing())
	{
		SetEffect(EFFECT_FADE, -50);
		while(this->GetEffect() > 0)
			Application::Instance()->updateEvents();
	}

	MainMenu::Instance()->Remove(this);
}

void TaskBar::OnDeviceButtonClick(GuiButton *sender UNUSED, int pointer UNUSED, const POINT &p UNUSED)
{
	TaskbarClicked(this, TaskBar::DEVICE);
}

void TaskBar::OnModeButtonClick(GuiButton *sender UNUSED, int pointer UNUSED, const POINT &p UNUSED)
{
	Settings.BrowserMode = !Settings.BrowserMode;
	
	SetModeButton();
	
	TaskbarClicked(this, TaskBar::MODE);
}

void TaskBar::OnLockButtonClick(GuiButton *sender UNUSED, int pointer UNUSED, const POINT &p UNUSED)
{
	if (!Settings.GodMode)
	{
		//!password check to unlock
		int result = PasswordCheck(Settings.unlockCode);
		if (result > 0)
		{
			if(result == 1)
				WindowPrompt( tr( "Correct Password" ), tr( "All the features of SaveGame Manager GX are unlocked." ), tr( "OK" ));
			Settings.GodMode = ON;
		}
		else if(result < 0)
			WindowPrompt(tr( "Wrong Password" ), tr( "SaveGame Manager GX is protected" ), tr( "OK" ));
	}
	else
	{
		int choice = WindowPrompt(tr( "Lock Console" ), tr( "Are you sure?" ), tr( "Yes" ), tr( "No" ));
		if (choice == 1)
		{
			WindowPrompt(tr( "Console Locked" ), tr( "SaveGame Manager GX is protected" ), tr( "OK" ));
			Settings.GodMode = OFF;
		}
	}
	
	SetLockButton();
	
	TaskbarClicked(this, TaskBar::LOCK);
}

void TaskBar::OnSearchButtonClick(GuiButton *sender UNUSED, int pointer UNUSED, const POINT &p UNUSED)
{
	TaskbarClicked(this, TaskBar::SEARCH);
}

void TaskBar::OnDeviceChange(int item)
{
	if(item < 0)
		return;
	
	if(item == SD)
	{
		Buttons[0]->SetImage(sdIconImg_gray);
		Buttons[0]->SetImageOver(sdIconImg);
		return;
	}
	
	for(int i = USB1; i <= USB10; i++)
	{
		if(item == i)
		{
			Buttons[0]->SetImage(usbIconImg_gray);
			Buttons[0]->SetImageOver(usbIconImg);
			return;
		}
	}
}

void TaskBar::OnSearchWindowStateChange(bool filterOn, bool displayOn)
{
	if (filterOn && !displayOn)
	{
		Buttons[3]->SetEffect(EFFECT_PULSE, 10, 105);
		Buttons[3]->SetImage(searchIconImg);
		Buttons[3]->SetImageOver(searchIconImg);
	}
	else if(displayOn)
	{
		Buttons[3]->ResetEffects();
		Buttons[3]->SetImage(searchIconImg_gray);
		Buttons[3]->SetImageOver(searchIconImg_gray);
		Buttons[3]->SetEffectGrow();
	}
	else
		ResetSearchBtn();
}

void TaskBar::ResetSearchBtn()
{
	Buttons[3]->ResetEffects();
	Buttons[3]->SetImage(searchIconImg_gray);
	Buttons[3]->SetImageOver(searchIconImg);
	Buttons[3]->SetEffectGrow();
}

void TaskBar::SetModeButton()
{
	if (Settings.BrowserMode == ICONBROWSER)
	{
		Buttons[1]->SetImage(arrangeIconImg_gray);
		Buttons[1]->SetImageOver(arrangeIconImg);
	}
	else
	{
		Buttons[1]->SetImage(arrangeListImg_gray);
		Buttons[1]->SetImageOver(arrangeListImg);
	}
}

void TaskBar::SetLockButton()
{
	if (Settings.GodMode)
	{
		Buttons[2]->SetImage(unlockedIconImg_gray);
		Buttons[2]->SetImageOver(unlockedIconImg);
		Tooltip[2]->SetText( tr("Lock Console") );
	}
	else
	{
		Buttons[2]->SetImage(lockedIconImg_gray);
		Buttons[2]->SetImageOver(lockedIconImg);
		Tooltip[2]->SetText( tr("Unlock Console") );
	}
}

void TaskBar::Draw()
{
	if(!IsVisible())
		return;

	for(int i = 0; i < 4; i++)
		Buttons[i]->Draw();

	for(int i = 0; i < 4; i++)
		Tooltip[i]->Draw();

	UpdateEffects();
}

void TaskBar::Update(GuiTrigger * t)
{
	int Selected = false;
	for(int i = 0; i < 4; i++)
	{
		Buttons[i]->Update(t);
		
		if(Buttons[i]->GetState() == STATE_SELECTED)
		{
			Selected = true;
			if(state == STATE_DEFAULT)
			{
				state = STATE_SELECTED;
				TaskbarStateChanged(this, STATE_SELECTED);
			}
		}
		else
			Buttons[i]->ResetTooltipTimer();
	}
	
	if(!Selected && state == STATE_SELECTED)
	{
		state = STATE_DEFAULT;
		TaskbarStateChanged(this, STATE_DEFAULT);
	}
}
