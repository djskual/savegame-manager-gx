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

#include "../DeviceControls/DeviceHandler.hpp"
#include "TaskBar.hpp"
#include "../Themes/CTheme.h"
#include "../menu.h"
#include "../Settings/CSettings.h"
#include "../Language/gettext.h"

/**
 * Constructor for the TaskBar class.
 */
TaskBar::TaskBar(const char * UNUSED)
{
	window = new GuiWindow(160, 32);
	window->SetParent(this);
	
	this->SetSize(160, 32);
	
	trigA = new SimpleGuiTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	
	sdIcon = Resources::GetImageData("taskbar_sd.png");
	sdIcon_gray = Resources::GetImageData("taskbar_sd_gray.png");
	usbIcon = Resources::GetImageData("taskbar_usb.png");
	usbIcon_gray = Resources::GetImageData("taskbar_usb_gray.png");
	wiiIcon = Resources::GetImageData("taskbar_wii.png");
	wiiIcon_gray = Resources::GetImageData("taskbar_wii_gray.png");
	emuIcon = Resources::GetImageData("taskbar_emu.png");
	emuIcon_gray = Resources::GetImageData("taskbar_emu_gray.png");
	miiIcon = Resources::GetImageData("taskbar_mii.png");
	miiIcon_gray = Resources::GetImageData("taskbar_mii_gray.png");
	emuMiiIcon = Resources::GetImageData("taskbar_emu_mii.png");
	emuMiiIcon_gray = Resources::GetImageData("taskbar_emu_mii_gray.png");
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
	emuIconImg = new GuiImage(emuIcon);
	emuIconImg_gray = new GuiImage(emuIcon_gray);
	miiIconImg = new GuiImage(miiIcon);
	miiIconImg_gray = new GuiImage(miiIcon_gray);
	emuMiiIconImg = new GuiImage(emuMiiIcon);
	emuMiiIconImg_gray = new GuiImage(emuMiiIcon_gray);
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
	
	deviceTT = new GuiTooltip(tr( "Select Device" ));
	if (Settings.wsprompt) deviceTT->SetWidescreen(Settings.Widescreen);
	deviceTT->SetAlpha(thInt("255 - tooltip alpha"));
	modeTT = new GuiTooltip(tr( "Select Mode" ));
	if (Settings.wsprompt) modeTT->SetWidescreen(Settings.Widescreen);
	modeTT->SetAlpha(thInt("255 - tooltip alpha"));
	lockTT = new GuiTooltip(Settings.GodMode ? tr( "Lock Console" ) : tr( "Unlock Console" ));
	if (Settings.wsprompt) lockTT->SetWidescreen(Settings.Widescreen);
	lockTT->SetAlpha(thInt("255 - tooltip alpha"));
	searchTT = new GuiTooltip(tr( "Search" ));
	if (Settings.wsprompt) searchTT->SetWidescreen(Settings.Widescreen);
	searchTT->SetAlpha(thInt("255 - tooltip alpha"));
	
	int buttonPos = 0;
	
	deviceBtn = new GuiButton(sdIconImg_gray, sdIconImg,
							  ALIGN_LEFT, ALIGN_TOP,
							  buttonPos, 0,
							  trigA,
							  btnSoundOver,btnSoundClick,
							  1,
							  deviceTT, sdIconImg_gray->GetWidth(), 0, ALIGN_LEFT, ALIGN_MIDDLE);
	buttonPos += sdIconImg_gray->GetWidth();
	
	modeBtn = new GuiButton(arrangeIconImg_gray, arrangeIconImg,
							ALIGN_LEFT, ALIGN_TOP,
							buttonPos, 0,
							trigA,
							btnSoundOver,btnSoundClick,
							1,
							modeTT, -arrangeIconImg_gray->GetWidth(), 0, ALIGN_RIGHT, ALIGN_MIDDLE);
	buttonPos += arrangeIconImg_gray->GetWidth();
	
	lockBtn = new GuiButton((Settings.GodMode) ? unlockedIconImg_gray : lockedIconImg_gray, (Settings.GodMode) ? unlockedIconImg : lockedIconImg,
							ALIGN_LEFT, ALIGN_TOP,
							buttonPos, 0,
							trigA,
							btnSoundOver,btnSoundClick,
							1,
							lockTT, -unlockedIconImg_gray->GetWidth(), 0, ALIGN_RIGHT, ALIGN_MIDDLE);
	buttonPos += lockedIconImg->GetWidth();
	
	searchBtn = new GuiButton(searchIconImg_gray, searchIconImg,
							  ALIGN_LEFT, ALIGN_TOP,
							  buttonPos, 0,
							  trigA,
							  btnSoundOver,btnSoundClick,
							  1,
							  searchTT, -searchIconImg_gray->GetWidth(), 0, ALIGN_RIGHT, ALIGN_MIDDLE);
	
	window->Append(deviceBtn);
	window->Append(modeBtn);
	window->Append(lockBtn);
	window->Append(searchBtn);
	
	this->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	this->SetPosition(0, 0);
	this->SetState(STATE_DEFAULT);
}

/**
 * Destructor for the TaskBar class.
 */
TaskBar::~TaskBar()
{
	window->RemoveAll();

	/** Buttons **/
	delete deviceBtn;
	delete modeBtn;
	delete lockBtn;
	delete searchBtn;
	
	/** Images **/
	delete sdIconImg;
	delete sdIconImg_gray;
	delete usbIconImg;
	delete usbIconImg_gray;
	delete wiiIconImg;
	delete wiiIconImg_gray;
	delete emuIconImg;
	delete emuIconImg_gray;
	delete miiIconImg;
	delete miiIconImg_gray;
	delete emuMiiIconImg;
	delete emuMiiIconImg_gray;
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
	
	/** ImageDatas **/
	delete sdIcon;
	delete sdIcon_gray;
	delete usbIcon;
	delete usbIcon_gray;
	delete wiiIcon;
	delete wiiIcon_gray;
	delete emuIcon;
	delete emuIcon_gray;
	delete miiIcon;
	delete miiIcon_gray;
	delete emuMiiIcon;
	delete emuMiiIcon_gray;
	delete arrangeIcon;
	delete arrangeIcon_gray;
	delete arrangeList;
	delete arrangeList_gray;
	delete lockedIcon;
	delete lockedIcon_gray;
	delete unlockedIcon;
	delete unlockedIcon_gray;
	delete searchIcon;
	delete searchIcon_gray;
	
	/** GuiTooltip **/
	delete deviceTT;
	delete modeTT;
	delete lockTT;
	delete searchTT;
	
	delete trigA;
	
	/** Window **/
	delete window;	
}

int TaskBar::GetClicked()
{
	if(deviceBtn->GetState() == STATE_CLICKED)
		return TASKBAR_DEVICE;
	else if(modeBtn->GetState() == STATE_CLICKED)
		return TASKBAR_MODE;
	else if(lockBtn->GetState() == STATE_CLICKED)
		return TASKBAR_LOCK;
	else if(searchBtn->GetState() == STATE_CLICKED)
		return TASKBAR_SEARCH;
	
	return TASKBAR_NONE;
}

int TaskBar::GetSelected()
{
	if(deviceBtn->GetState() == STATE_SELECTED)
		return TASKBAR_DEVICE;
	else if(modeBtn->GetState() == STATE_SELECTED)
		return TASKBAR_MODE;
	else if(lockBtn->GetState() == STATE_SELECTED)
		return TASKBAR_LOCK;
	else if(searchBtn->GetState() == STATE_SELECTED)
		return TASKBAR_SEARCH;
	
	return TASKBAR_NONE;
}

void TaskBar::SetDeviceImage(int type)
{
	if (type == TASKBAR_SD)
	{
		deviceBtn->SetImage(sdIconImg_gray);
		deviceBtn->SetImageOver(sdIconImg);
	}
	else if (type == TASKBAR_USB)
	{
		deviceBtn->SetImage(usbIconImg_gray);
		deviceBtn->SetImageOver(usbIconImg);
	}
	else if (type == TASKBAR_WII)
	{
		deviceBtn->SetImage(wiiIconImg_gray);
		deviceBtn->SetImageOver(wiiIconImg);
	}
	else if (type == TASKBAR_EMU)
	{
		deviceBtn->SetImage(emuIconImg_gray);
		deviceBtn->SetImageOver(emuIconImg);
	}
	else if (type == TASKBAR_MII)
	{
		deviceBtn->SetImage(miiIconImg_gray);
		deviceBtn->SetImageOver(miiIconImg);
	}
	else if (type == TASKBAR_EMU_MII)
	{
		deviceBtn->SetImage(emuMiiIconImg_gray);
		deviceBtn->SetImageOver(emuMiiIconImg);
	}
}

void TaskBar::SetModeButton()
{
	if (Settings.BrowserMode == 0)
	{
		modeBtn->SetImage(arrangeIconImg_gray);
		modeBtn->SetImageOver(arrangeIconImg);
	}
	else
	{
		modeBtn->SetImage(arrangeListImg_gray);
		modeBtn->SetImageOver(arrangeListImg);
	}
}

void TaskBar::SetLockButton()
{
	if (Settings.GodMode)
	{
		lockBtn->SetImage(unlockedIconImg_gray);
		lockBtn->SetImageOver(unlockedIconImg);
		lockTT->SetText( tr("Lock Console") );
	}
	else
	{
		lockBtn->SetImage(lockedIconImg_gray);
		lockBtn->SetImageOver(lockedIconImg);
		lockTT->SetText( tr("Unlock Console") );
	}
}

void TaskBar::SetSearchEffect(bool searchwindow)
{
	if (searchwindow)
	{
		searchBtn->SetEffect(EFFECT_PULSE, 10, 105);
		searchBtn->SetImage(searchIconImg);
		searchBtn->SetImageOver(searchIconImg);
	}
	else
	{
		searchBtn->SetImage(searchIconImg_gray);
		searchBtn->SetImageOver(searchIconImg_gray);
	}
}

void TaskBar::StopSearchEffect()
{
	searchBtn->StopEffect();
	searchBtn->SetImage(searchIconImg_gray);
	searchBtn->SetImageOver(searchIconImg);
	searchBtn->SetEffectGrow();
}

void TaskBar::ResetState()
{
	state = STATE_DEFAULT;
	stateChan = -1;

	deviceBtn->ResetState();
	modeBtn->ResetState();
	lockBtn->ResetState();
	searchBtn->ResetState();
}

/**
 * Draw the window
 */
void TaskBar::Draw()
{
	if(!this->IsVisible())
		return;

	window->Draw();
	if (Settings.Tooltips && Theme::ShowTooltips)
		window->DrawTooltip();
	
	this->UpdateEffects();
}

void TaskBar::Update(GuiTrigger * t)
{
	if (state == STATE_DISABLED || !t) return;
	
	deviceBtn->Update(t);
	modeBtn->Update(t);
	lockBtn->Update(t);
	searchBtn->Update(t);
	
	if (GetClicked() != TASKBAR_NONE)
		this->SetState(STATE_CLICKED);
	else if (GetSelected() != TASKBAR_NONE)
		this->SetState(STATE_SELECTED);
	
	if (GetClicked() == TASKBAR_NONE && GetSelected() == TASKBAR_NONE)
		this->SetState(STATE_DEFAULT);
	
	if(updateCB)
		updateCB(this);
}
