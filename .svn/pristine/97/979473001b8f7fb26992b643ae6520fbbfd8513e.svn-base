/****************************************************************************
 * Copyright (C) 2010
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
 ***************************************************************************/
#include <unistd.h>
#include "SoundSettingsMenu.hpp"
#include "../settingsprompts.h"
#include "../../Prompts/PromptWindows.h"
#include "../../Language/gettext.h"

static const char * BgMusicLoopText[] =
{
	trNOOP( "Play Once" ),
	trNOOP( "Loop Music" ),
	trNOOP( "Random Directory Music" ),
	trNOOP( "Loop Directory" ),
};

SoundSettingsMenu::SoundSettingsMenu()
	: SettingsMenu(tr("Sound Settings"), &GuiOptions, MENU_NONE)
{
	int Idx = 0;
	Options->SetName(Idx++, tr( "Music Volume" ));
	Options->SetName(Idx++, tr( "Credits Volume" ));
	Options->SetName(Idx++, tr( "SFX Volume" ));
	Options->SetName(Idx++, tr( "BgMusic Path" ));
	Options->SetName(Idx++, tr( "BgMusic Loop" ));
	Options->SetName(Idx++, tr( "Reset BgMusic" ));

	SetOptionValues();
}

void SoundSettingsMenu::SetOptionValues()
{
	int Idx = 0;

	//! Settings: Music Volume
	if (Settings.MusicVolume > 0)
		Options->SetValue(Idx++, "%i", Settings.MusicVolume);
	else
		Options->SetValue(Idx++, tr( "OFF" ));

	//! Settings: Credits Music Volume
	if (Settings.CreditsVolume > 0)
		Options->SetValue(Idx++, "%i", Settings.CreditsVolume);
	else
		Options->SetValue(Idx++, tr( "OFF" ));

	//! Settings: SFX Volume
	if (Settings.SFXVolume > 0)
		Options->SetValue(Idx++, "%i", Settings.SFXVolume);
	else
		Options->SetValue(Idx++, tr( "OFF" ));

	//! Settings: BgMusic Path
	const char * filename = strrchr(Settings.BgMusicPath, '/');
	if (filename)
		Options->SetValue(Idx++, filename+1);
	else
		Options->SetValue(Idx++, tr( "Default" ));

	//! Settings: BgMusic Loop Mode
	Options->SetValue(Idx++, tr( BgMusicLoopText[Settings.BgMusicLoop] ));

	//! Settings: Reset BG Music
	Options->SetValue(Idx++, " ");
}

int SoundSettingsMenu::GetMenuInternal()
{
	int ret = optionBrowser->GetClickedOption();

	if (ret < 0)
		return MENU_NONE;

	int Idx = -1;

	//! Settings: Music Volume
	if (ret == ++Idx)
	{
		Settings.MusicVolume += 10;
		if (Settings.MusicVolume > 100) Settings.MusicVolume = 0;
		bgMusic->SetVolume(Settings.MusicVolume);
	}

	//! Settings: Credits Music Volume
	else if (ret == ++Idx)
	{
		Settings.CreditsVolume += 10;
		if (Settings.CreditsVolume > 100) Settings.CreditsVolume = 0;
	}

	//! Settings: SFX Volume
	else if (ret == ++Idx)
	{
		Settings.SFXVolume += 10;
		if (Settings.SFXVolume > 100) Settings.SFXVolume = 0;
		btnSoundOver->SetVolume(Settings.SFXVolume);
		btnSoundClick->SetVolume(Settings.SFXVolume);
	}

	//! Settings: BgMusic Path
	if (ret == ++Idx)
	{
		titleTxt->SetText(tr( "Sound Path" ));
		
		GuiWindow * parent = (GuiWindow *) parentElement;
		if(parent) parent->SetState(STATE_DISABLED);
		this->Remove(optionBrowser);
		
		MenuBackgroundMusic();
		
		if(parent) parent->SetState(STATE_DEFAULT);
		this->Append(optionBrowser);
		
		titleTxt->SetText(tr( "Sound Settings" ));
	}

	//! Settings: BgMusic Loop Mode
	else if (ret == ++Idx)
	{
		if (++Settings.BgMusicLoop > 3) Settings.BgMusicLoop = 0;
		bgMusic->SetLoop(Settings.BgMusicLoop);
		bgMusic->Play();
	}

	//! Settings: Reset BgMusic
	else if (ret == ++Idx)
	{
		int result = WindowPrompt(tr( "Reset to default BGM?" ), 0, tr( "Yes" ), tr( "No" ));
		if (result)
		{
			bgMusic->LoadStandard();
			bgMusic->SetLoop(Settings.BgMusicLoop);
			bgMusic->Play();
		}
	}

	SetOptionValues();

	return MENU_NONE;
}
