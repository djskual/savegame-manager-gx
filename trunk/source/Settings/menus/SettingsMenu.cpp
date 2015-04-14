/***************************************************************************
 * Copyright (C) 2012 Dj_Skual
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
 * SettingsMenu.cpp
 *
 ***************************************************************************/
#include "SettingsMenu.h"
#include "GuiSettingsMenu.hpp"
#include "AudioSettingsMenu.hpp"
#include "CustomPathsSM.hpp"
#include "ThemeSettingsMenu.hpp"
#include "ControlsSettingsMenu.hpp"
#include "ParentalControlSM.hpp"
#include "FeaturesSettingsMenu.hpp"
#include "UpdateSettingsMenu.hpp"
#include "../../Controls/Application.h"
#include "../../Prompts/PromptWindows.h"

SettingsMenu *SettingsMenu::instance = NULL;

SettingsMenu::SettingsMenu()
	: GuiFrame(screenwidth, screenheight)
{
	globalSettings = NULL;
	settingsBrowser = NULL;
	
	BtnSoundClick = Resources::GetSound("button_click.wav");
	BtnSoundOver = Resources::GetSound("button_over.wav");
	BtnSoundClick->SetVolume(Settings.SFXVolume);
	BtnSoundOver->SetVolume(Settings.SFXVolume);
	
	trigA.SetSimpleTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	trigB.SetButtonOnlyTrigger(-1, WiiControls.BackButton | ClassicControls.BackButton << 16, GCControls.BackButton);
	
	BgImgData = Resources::GetImageData("settings_background.png");
	BgImg = new GuiImage(BgImgData);
	
	Title = new GuiText(tr("Global Settings"), 28, thColor("r=0 g=0 b=0 a=255 - settings menu title text color"));
	Title->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	Title->SetPosition(170, 38);
	Title->SetMaxWidth(310, SCROLL_HORIZONTAL);
	
	BackBtnTxt = new GuiText(tr( "Back" ), 22, thColor("r=0 g=0 b=0 a=255 - settings menu back button text color"));
	BackBtn = new PictureButton("settings_menu_button.png", "settings_menu_button.png", BtnSoundClick, BtnSoundOver);
	BackBtn->SetAlignment(thAlign("center - settings menu back button align hor") | thAlign("top - settings menu back button align ver"));
	BackBtn->SetPosition(thInt("-195 - settings menu back button pos x"), thInt("400 - settings menu back button pos y"));
	BackBtn->SetLabel(BackBtnTxt);
	BackBtn->SetTrigger(&trigA);
	BackBtn->SetTrigger(&trigB);
	BackBtn->Clicked.connect(this, &SettingsMenu::OnBackButtonClick);
	
	globalSettings = new GlobalSettings();
	globalSettings->FlyingBtnMenuClicked.connect(this, &SettingsMenu::OnFlyingButtonClick);
	
	Append(BgImg);
	Append(Title);
	Append(BackBtn);
	Append(globalSettings);
	
	SetEffect(EFFECT_FADE, 20);
}

SettingsMenu::~SettingsMenu()
{
	RemoveAll();
	
	if(globalSettings)
		delete globalSettings;
	
	delete Title;
	
	delete BgImg;
	Resources::Remove(BgImgData);
	
	delete BackBtnTxt;
	delete BackBtn;
	
	Resources::Remove(BtnSoundClick);
	Resources::Remove(BtnSoundOver);
}

void SettingsMenu::trash()
{
	if(settingsBrowser)
	{
		Application::Instance()->PushForDelete(settingsBrowser);
		Application::Instance()->updateEvents();
	}
	
	if(globalSettings && (globalSettings->GetState() == STATE_DEFAULT))
		globalSettings->hide();
	
	Settings.Save();

	Application::Instance()->UnsetUpdateOnly(this);
	Application::Instance()->Remove(this);
	
	instance->DestroyInstance();
}

void SettingsMenu::quit()
{
	if(settingsBrowser)
	{
		Application::Instance()->PushForDelete(settingsBrowser);
		Application::Instance()->updateEvents();
	}
	
	if(globalSettings && (globalSettings->GetState() == STATE_DEFAULT))
		globalSettings->FadeOut();
	
	Settings.Save();

	SetEffect(EFFECT_FADE, -20);
	while(this->IsAnimated())
		Application::Instance()->updateEvents();
	
	Application::Instance()->UnsetUpdateOnly(this);
	Application::Instance()->Remove(this);
	Application::Instance()->updateEvents();
	
	instance->DestroyInstance();
}

void SettingsMenu::show(short loadSM)
{
	Application::Instance()->SetUpdateOnly(this);
	Application::Instance()->Append(this);
	while(BackBtn->IsAnimated())
		Application::Instance()->updateEvents();
	
	if(loadSM == SettingsMenu::THEME)
	{
		settingsBrowser = new ThemeSettingsMenu();
		Append(settingsBrowser);
	}
	else if(loadSM == SettingsMenu::GUI)
	{
		settingsBrowser = new GuiSettingsMenu();
		Append(settingsBrowser);
	}
	else
		globalSettings->show();
}

void SettingsMenu::OnBackButtonClick(GuiButton *sender UNUSED, int pointer UNUSED, const POINT &p UNUSED)
{
	if(!settingsBrowser)
		quit();
	else
	{
		Application::Instance()->PushForDelete(settingsBrowser);
		Application::Instance()->updateEvents();
		settingsBrowser = NULL;
		
		SetTitle(tr("Global Settings"));
		globalSettings->FadeIn();
	}
}

void SettingsMenu::OnFlyingButtonClick(int menu)
{
	if(menu == SettingsMenu::DEFAULT)
	{
		if (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_RESET_SETTINGS))
		{
			if(WindowPrompt(tr( "Are you sure you want to reset?" ), 0, tr( "Yes" ), tr( "Cancel" )))
			{
				Settings.SetDefault();
				Application::Instance()->ThemeReloadRequest(true, false);
			}
		}
		else
			WindowPrompt(tr( "Permission denied." ), tr( "Console must be unlocked for this option." ), tr( "OK" ));
	}
	else if(menu == SettingsMenu::CREDITS)
	{
		Application::Instance()->updateEvents();
		CreditsWindow * creditsWindow = new CreditsWindow();
		creditsWindow->Clicked.connect(this, &SettingsMenu::OnCreditsClicked);
		Append(creditsWindow);
	}
	else
	{
		globalSettings->FadeOut();
		
		if(menu == SettingsMenu::GUI)
		{
			if(Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_GUI_SETTINGS))
				settingsBrowser = new GuiSettingsMenu();
			else
			{
				WindowPrompt(tr( "Permission denied." ), tr( "Console must be unlocked for this option." ), tr( "OK" ));
				goto abort;
			}
		}
		else if(menu == SettingsMenu::AUDIO)
		{
			if(Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_AUDIO_SETTINGS))
				settingsBrowser = new AudioSettingsMenu();
			else
			{
				WindowPrompt(tr( "Permission denied." ), tr( "Console must be unlocked for this option." ), tr( "OK" ));
				goto abort;
			}
		}
		else if(menu == SettingsMenu::PATH)
		{
			if(Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_CUSTOMPATH_SETTINGS))
				settingsBrowser = new CustomPathsSM();
			else
			{
				WindowPrompt(tr( "Permission denied." ), tr( "Console must be unlocked for this option." ), tr( "OK" ));
				goto abort;
			}
		}
		else if(menu == SettingsMenu::THEME)
		{
			if(Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_THEME_SETTINGS))
				settingsBrowser = new ThemeSettingsMenu();
			else
			{
				WindowPrompt(tr( "Permission denied." ), tr( "Console must be unlocked for this option." ), tr( "OK" ));
				goto abort;
			}
		}
		else if(menu == SettingsMenu::CONTROL)
		{
			if(Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_CONTROLS_SETTINGS))
				settingsBrowser = new ControlsSettingsMenu();
			else
			{
				WindowPrompt(tr( "Permission denied." ), tr( "Console must be unlocked for this option." ), tr( "OK" ));
				goto abort;
			}
		}
		else if(menu == SettingsMenu::PARENTAL)
		{
			if(Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_PARENTAL_SETTINGS))
				settingsBrowser = new ParentalControlSM();
			else
			{
				WindowPrompt(tr( "Permission denied." ), tr( "Console must be unlocked for this option." ), tr( "OK" ));
				goto abort;
			}
		}
		else if(menu == SettingsMenu::FEATURES)
		{
			if(Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_FEATURES_MENU))
				settingsBrowser = new FeaturesSettingsMenu();
			else
			{
				WindowPrompt(tr( "Permission denied." ), tr( "Console must be unlocked for this option." ), tr( "OK" ));
				goto abort;
			}
		}
		else if(menu == SettingsMenu::UPDATE)
		{
			if(Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_UPDATES))
				settingsBrowser = new UpdateSettingsMenu();
			else
			{
				WindowPrompt(tr( "Permission denied." ), tr( "Console must be unlocked for this option." ), tr( "OK" ));
				goto abort;
			}
		}
		
		Append(settingsBrowser);
		return;
		
	  abort:
		globalSettings->FadeIn();
	}
}

void SettingsMenu::OnCreditsClicked(CreditsWindow * creditsWindow)
{
	creditsWindow->SetEffect(EFFECT_FADE, -40);
	while(creditsWindow->IsAnimated())
		Application::Instance()->updateEvents();
	
	Application::Instance()->PushForDelete(creditsWindow);
	Application::Instance()->updateEvents();
}
