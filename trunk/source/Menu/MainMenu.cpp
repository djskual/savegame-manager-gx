/***************************************************************************
 * Copyright (C) 2009
 * by r-win & Dimok
 * Modified by Dj_Skual
 * for SaveGame Manager GX 2011
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
 * MainWindow.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include <unistd.h>

#include "../DeviceControls/DeviceHandler.hpp"
#include "../SoundOperations/MusicPlayer.h"
#include "../Settings/menus/SettingsMenu.h"
#include "../Prompts/PromptWindows.h"
#include "../Controls/Application.h"
#include "../Network/networkops.h"
#include "../Network/update.h"
#include "../System/input.h"
#include "MainMenu.h"

MainMenu *MainMenu::instance = NULL;

MainMenu::MainMenu()
	: GuiFrame(screenwidth, screenheight)
{
	searchOn = false;
	filterOn = false;
	refreshSearch = false;
	
	oldLockSetting = Settings.GodMode;
	
	mainExplorer = NULL;
	deviceMenu = NULL;
	searchWindow = NULL;
	
	BtnSoundClick = Resources::GetSound("button_click.wav");
	BtnSoundOver = Resources::GetSound("button_over.wav");
	BtnSoundClick->SetVolume(Settings.SFXVolume);
	BtnSoundOver->SetVolume(Settings.SFXVolume);
	
	trigA.SetSimpleTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	
	ManageTT = new GuiTooltip(tr( "Manage All" ));
	ManageTT->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	ManageTT->SetPosition(thInt("24 - manageAll btn tooltip pos x"), thInt("-30 - manageAll btn tooltip pos y"));
	ManageTT->SetAlpha(thInt("255 - tooltip alpha"));
	
	ManageBtn = new PictureButton("menu_button_switch.png", "menu_button_switch_over.png", BtnSoundClick, BtnSoundOver);
	ManageBtn->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	ManageBtn->SetPosition(thInt("16 - manageAll btn pos x"), thInt("355 - managAll btn pos y"));
	ManageBtn->SetTooltip(ManageTT);
	ManageBtn->SetTrigger(&trigA);
	ManageBtn->SetEffect(EFFECT_FADE, 20);
	ManageBtn->Clicked.connect(this, &MainMenu::OnManageButtonClick);
	
	SettingsTT = new GuiTooltip(tr("Settings"));
	SettingsTT->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	SettingsTT->SetPosition(thInt("65 - settings btn tooltip pos x"), thInt("-30 - settings btn tooltip pos y"));
	SettingsTT->SetAlpha(thInt("255 - tooltip alpha"));
	
	SettingsBtn = new PictureButton("menu_button_settings.png", "menu_button_settings_over.png", BtnSoundClick, BtnSoundOver);
	SettingsBtn->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	SettingsBtn->SetPosition(thInt("64 - settings btn pos x"), thInt("371 - settings btn pos y"));
	SettingsBtn->SetTooltip(SettingsTT);
	SettingsBtn->SetTrigger(&trigA);
	SettingsBtn->SetEffect(EFFECT_FADE, 20);
	SettingsBtn->Clicked.connect(this, &MainMenu::OnSettingsButtonClick);
	
	HomebrewTT = new GuiTooltip(tr( "Homebrew Launcher" ));
	HomebrewTT->SetAlignment(ALIGN_RIGHT | ALIGN_TOP);
	HomebrewTT->SetPosition(thInt("-65 - homebrew btn tooltip pos x"), thInt("-30 - homebrew btn tooltip pos y"));
	HomebrewTT->SetAlpha(thInt("255 - tooltip alpha"));
	
	HomebrewBtn = new PictureButton("menu_button_wii.png", "menu_button_wii_over.png", BtnSoundClick, BtnSoundOver);
	HomebrewBtn->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	HomebrewBtn->SetPosition(thInt("489 - homebrew btn pos x"), thInt("371 - homebrew btn pos y"));
	HomebrewBtn->SetTooltip(HomebrewTT);
	HomebrewBtn->SetTrigger(&trigA);
	HomebrewBtn->SetEffect(EFFECT_FADE, 20);
	HomebrewBtn->Clicked.connect(this, &MainMenu::OnHomebrewButtonClick);
	
	MusicPlayerTT = new GuiTooltip(tr( "Music Player" ));
	MusicPlayerTT->SetAlignment(ALIGN_RIGHT | ALIGN_TOP);
	MusicPlayerTT->SetPosition(thInt("-24 - music btn tooltip pos x"), thInt("-30 - music btn tooltip pos y"));
	MusicPlayerTT->SetAlpha(thInt("255 - tooltip alpha"));
	
	MusicPlayerBtn = new PictureButton("menu_button_music.png", "menu_button_music_over.png", BtnSoundClick, BtnSoundOver);
	MusicPlayerBtn->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	MusicPlayerBtn->SetPosition(thInt("576 - music btn pos x"), thInt("355 - music btn pos y"));
	MusicPlayerBtn->SetTooltip(MusicPlayerTT);
	MusicPlayerBtn->SetTrigger(&trigA);
	MusicPlayerBtn->SetEffect(EFFECT_FADE, 20);
	MusicPlayerBtn->Clicked.connect(this, &MainMenu::OnMusicButtonClick);
	
	SdCardImgData = Resources::GetImageData("menu_button_sdcard.png");
	SdCardImgGrayData = Resources::GetImageData("menu_button_sdcard_gray.png");
	SdCardImg = new GuiImage(SdCardImgData);
	SdCardImgGray = new GuiImage(SdCardImgGrayData);
	
	SdCardTT = new GuiTooltip(tr( "Reload SD" ));
	SdCardTT->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	SdCardTT->SetPosition(thInt("15 - sd card btn tooltip pos x"), thInt("-30 - sd card btn tooltip pos y"));
	SdCardTT->SetAlpha(thInt("255 - tooltip alpha"));
	
	bool sd_inserted = DeviceHandler::Instance()->IsInserted(SD);
	SdCardBtn = new PictureButton("menu_button_sdcard.png", "menu_button_sdcard_over.png", BtnSoundClick, BtnSoundOver);
	SdCardBtn->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	SdCardBtn->SetPosition(thInt("160 - sd card btn pos x"), thInt("395 - sd card btn pos y"));
	SdCardBtn->SetImage(sd_inserted ? SdCardImg : SdCardImgGray);
	SdCardBtn->SetState(sd_inserted ? STATE_DEFAULT : STATE_DISABLED);
	SdCardBtn->SetTooltip(SdCardTT);
	SdCardBtn->SetTrigger(&trigA);
	SdCardBtn->SetEffect(EFFECT_FADE, 20);
	SdCardBtn->Clicked.connect(this, &MainMenu::OnSdCardButtonClick);
}

MainMenu::~MainMenu()
{
	RemoveAll();
	
	delete SdCardImg;
	delete SdCardImgGray;
	
	Resources::Remove(SdCardImgData);
	Resources::Remove(SdCardImgGrayData);
	
	delete ManageTT;
	delete SettingsTT;
	delete HomebrewTT;
	delete MusicPlayerTT;
	delete SdCardTT;
	
	delete ManageBtn;
	delete SettingsBtn;
	delete HomebrewBtn;
	delete MusicPlayerBtn;
	delete SdCardBtn;
	
	Resources::Remove(BtnSoundClick);
	Resources::Remove(BtnSoundOver);
	
	mainExplorer = NULL;
	deviceMenu = NULL;
	searchWindow = NULL;
	
	TaskBar::DestroyInstance();
}

void MainMenu::hide()
{
	TaskBar::DestroyInstance();
	
	RemoveAll();
	
	if(searchWindow)
		delete searchWindow;
	if(mainExplorer)
		delete mainExplorer;
}

void MainMenu::show()
{
	Append(ManageBtn);
	Append(SettingsBtn);
	Append(HomebrewBtn);
	Append(MusicPlayerBtn);
	if (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_SD_RELOAD_BUTTON))
		Append(SdCardBtn);
	
	TaskBar::Instance()->show();
	
	mainExplorer = new Explorer(MainMenu::Instance(), Settings.LastUsedPath);
	
	std::string root = mainExplorer->GetRootDir();
	root.erase(root.find_first_of(":"));
	for(int i = 0; i < MAXDEVICES; i++)
	{
		if(!root.compare(DeviceName[i]))
			DeviceChanged(i);
	}
}

void MainMenu::OnManageButtonClick(GuiButton *sender UNUSED, int pointer UNUSED, const POINT &p3 UNUSED)
{
	if (!Settings.GodMode && (Settings.ParentalBlocks & BLOCK_MANAGE_ALL))
	{
		WindowPrompt(tr( "Permission denied." ), tr( "Console must be unlocked for this option." ), tr( "OK" ));
		return;
	}
	
	gprintf("\n\t Manage Button Clicked");
}

void MainMenu::OnSettingsButtonClick(GuiButton *sender UNUSED, int pointer UNUSED, const POINT &p3 UNUSED)
{
	if (!Settings.GodMode && (Settings.ParentalBlocks & BLOCK_GLOBAL_SETTINGS))
	{
		WindowPrompt(tr( "Permission denied." ), tr( "Console must be unlocked for this option." ), tr( "OK" ));
		return;
	}
	
	SettingsMenu::Instance()->show();
}

void MainMenu::OnHomebrewButtonClick(GuiButton *sender UNUSED, int pointer UNUSED, const POINT &p3 UNUSED)
{
	if (!Settings.GodMode && (Settings.ParentalBlocks & BLOCK_HBC_MENU))
	{
		WindowPrompt(tr( "Permission denied." ), tr( "Console must be unlocked for this option." ), tr( "OK" ));
		return;
	}
	
	gprintf("\n\t Homebrew Button Clicked");
}

void MainMenu::OnMusicButtonClick(GuiButton *sender UNUSED, int pointer UNUSED, const POINT &p3 UNUSED)
{
	MusicPlayer::Instance()->Show();
}

void MainMenu::OnSdCardButtonClick(GuiButton *sender UNUSED, int pointer UNUSED, const POINT &p3 UNUSED)
{
	if(!MusicPlayer::Instance()->IsStopped())
		MusicPlayer::Instance()->Pause();

	DeviceHandler::Instance()->UnMountSD();
	DeviceHandler::Instance()->MountSD();
	
	gprintf("\tLoading config...%s\n", Settings.Load() ? "done" : "failed");
	gprintf("\tLoading language...%s\n", Settings.LoadLanguage(Settings.LanguageFile, CONSOLE_DEFAULT) ? "done" : "failed");
	gprintf("\tLoading font...%s\n", Theme::LoadFont(Settings.CustomFontPath) ? "done" : "failed (using default)");
	gprintf("\tLoading theme...%s\n", Theme::Load(Settings.Theme) ? "done" : "failed (using default)");
	
	if(!MusicPlayer::Instance()->IsStopped())
		MusicPlayer::Instance()->Resume();

	mainExplorer->Refresh();
}

void MainMenu::OnTaskbarClick(TaskBar *taskbar UNUSED, int sender)
{
	if(sender == TaskBar::DEVICE)
	{
		if(searchOn)
			return;
			
		filterOn = false;
		SearchStateChanged(filterOn, searchOn);
		
		deviceMenu = new DeviceMenu();
		deviceMenu->DeviceSelected.connect(this, &MainMenu::OnDeviceMenuClose);
		Application::Instance()->SetUpdateOnly(deviceMenu);
		this->Append(deviceMenu);
	}
	else if(sender == TaskBar::MODE)
	{
		if(searchWindow)
			this->Remove(searchWindow);
		
		mainExplorer->hide();
		mainExplorer->show();
		
		if(searchWindow)
			this->Append(searchWindow);
	}
	else if(sender == TaskBar::LOCK)
	{
		Remove(SdCardBtn);
		if (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_SD_RELOAD_BUTTON))
			Append(SdCardBtn);
	}
	else if(sender == TaskBar::SEARCH)
	{
		if(!searchOn)
		{
			if(!filterOn)
				mainExplorer->FilterList(L"", true);
			
			searchWindow = new SearchWindow(mainExplorer->GetCurrentFilter(), mainExplorer->GetSearchChars(), true);
			searchWindow->Closing.connect(this, &MainMenu::OnSearchWindowClose);
			this->Append(searchWindow);
			
			searchOn = true;
		}
		else
		{
			searchOn = false;
			Application::Instance()->PushForDelete(searchWindow);
			searchWindow = NULL;
		}
		
		SearchStateChanged(filterOn, searchOn);
	}
}

void MainMenu::OnDeviceMenuClose(DeviceMenu * deviceMenu UNUSED, int item)
{
	if(SD <= item && item < MAXDEVICES)
	{
		std::string device = DeviceName[item];
		device += ":/";
		
		mainExplorer->LoadPath(device);
		DeviceChanged(item);
	}
	
	Application::Instance()->PushForDelete(deviceMenu);
}

void MainMenu::OnSearchWindowClose(GuiFrame * window UNUSED)
{
	searchWindow = NULL;
	
	if(!filterOn && !refreshSearch)
		searchOn = false;
	refreshSearch = false;
	
	if(searchOn)
	{
		searchWindow = new SearchWindow(mainExplorer->GetCurrentFilter(), mainExplorer->GetSearchChars(), true);
		searchWindow->Closing.connect(this, &MainMenu::OnSearchWindowClose);
		this->Append(searchWindow);
	}
	
	SearchStateChanged(filterOn, searchOn);
}

void MainMenu::Update(GuiTrigger * t)
{
	if(oldLockSetting != Settings.GodMode)
	{
		oldLockSetting = Settings.GodMode;
		Refresh();
	}
	
	if (!Settings.updateChecked && Settings.AutoConnect && IsNetworkInit())
	{
		CheckForUpdate();
	}
	
	GuiFrame::Update(t);
}
