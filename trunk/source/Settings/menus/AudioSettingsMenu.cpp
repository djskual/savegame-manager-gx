/****************************************************************************
 * Copyright (C) 2012
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
 ***************************************************************************/
#include "AudioSettingsMenu.hpp"
#include "SettingsMenu.h"
#include "../../Prompts/PromptWindows.h"
#include "../../SoundOperations/MusicPlayer.h"
#include "../../Controls/Application.h"

static const char * MusicLoopText[] =
{
	trNOOP( "Play Once" ),
	trNOOP( "Loop" ),
	trNOOP( "Random" ),
	trNOOP( "Loop Playlist" ),
};

AudioSettingsMenu::AudioSettingsMenu()
	: SettingsBrowser()
{
	SettingsMenu::Instance()->SetTitle(tr( "Audio Settings" ));
	
	SetupOptionName();
	show();
}

void AudioSettingsMenu::SetupOptionName()
{
	int Idx = 0;
	
	options->SetName(Idx++, tr( "Music Volume" ));
	options->SetName(Idx++, tr( "Credits Volume" ));
	options->SetName(Idx++, tr( "SFX Volume" ));
	options->SetName(Idx++, tr( "Music Loop Mode" ));
	options->SetName(Idx++, tr( "Soundblocks" ));
	options->SetName(Idx++, tr( "Soundblock Size" ));
	//options->SetName(Idx++, tr( "Reset BgMusic" ));
	
	SetOptionValues();
}

void AudioSettingsMenu::SetOptionValues()
{
	int Idx = 0;

	//! Settings: Music Volume
	if (Settings.MusicVolume > 0)
		options->SetValue(Idx++, "%i", Settings.MusicVolume);
	else
		options->SetValue(Idx++, tr( "OFF" ));

	//! Settings: Credits Music Volume
	if (Settings.CreditsVolume > 0)
		options->SetValue(Idx++, "%i", Settings.CreditsVolume);
	else
		options->SetValue(Idx++, tr( "OFF" ));

	//! Settings: SFX Volume
	if (Settings.SFXVolume > 0)
		options->SetValue(Idx++, "%i", Settings.SFXVolume);
	else
		options->SetValue(Idx++, tr( "OFF" ));

	//! Settings: Music Loop Mode
	options->SetValue(Idx++, tr(MusicLoopText[Settings.BGMLoopMode]));

	//! Settings: Soundblocks
	options->SetValue(Idx++, "%i (%0.1f KB)", Settings.SoundblockCount, Settings.SoundblockCount*Settings.SoundblockSize/1024.0f);

	//! Settings: Soundblock Size
	options->SetValue(Idx++, "%i Bytes", Settings.SoundblockSize);

	//! Settings: Reset BG Music
	//Options->SetValue(Idx++, " ");
}

void AudioSettingsMenu::OnBrowserClick(GuiOptionBrowser * browser UNUSED, int option)
{
	char entrie[20];
	int ret = 0;
	
	switch(option)
	{
		case 0:
			Settings.MusicVolume += 10;
			if (Settings.MusicVolume > 100)
				Settings.MusicVolume = 0;
			MusicPlayer::Instance()->SetVolume(Settings.MusicVolume);
			break;
		case 1:
			Settings.CreditsVolume += 10;
			if (Settings.CreditsVolume > 100)
				Settings.CreditsVolume = 0;
			break;
		case 2:
			Settings.SFXVolume += 10;
			if (Settings.SFXVolume > 100)
				Settings.SFXVolume = 0;
			break;
		case 3:
			Settings.BGMLoopMode++;
			if (Settings.BGMLoopMode >= MAX_LOOP_MODES)
				Settings.BGMLoopMode = ONCE;
			MusicPlayer::Instance()->SetLoop(Settings.BGMLoopMode);
			break;
		case 4:
			snprintf(entrie, sizeof(entrie), "%i", Settings.SoundblockCount);
			ret = OnScreenNumpad(entrie);
			if(ret)
			{
				Settings.SoundblockCount = atoi(entrie);
				if(Settings.SoundblockCount < 3)
					Settings.SoundblockCount = 3;
				
				WindowPrompt(tr("Warning:"), tr("The effect will take with next music load. It might break music playback."), tr("OK"));
				if(Settings.SoundblockSize*Settings.SoundblockCount > 512*1024)
					WindowPrompt(tr("WARNING:"), tr("The buffer size is really high. If the app doesn't start after this delete your config files."), tr("OK"));
			}
			break;
		case 5:
			snprintf(entrie, sizeof(entrie), "%i", Settings.SoundblockSize);
			ret = OnScreenNumpad(entrie);
			if(ret)
			{
				Settings.SoundblockSize = atoi(entrie);
				
				WindowPrompt(tr("Warning:"), tr("The effect will take with next music load. It might break music playback."), tr("OK"));
				if(Settings.SoundblockSize*Settings.SoundblockCount > 512*1024)
					WindowPrompt(tr("WARNING:"), tr("The buffer size is really high. If the app doesn't start after this delete your config files."), tr("OK"));
			}
			break;
		/*case 6:
			int result = WindowPrompt(tr( "Reset to default BGM?" ), 0, tr( "Yes" ), tr( "No" ));
			if (result)
			{
				bgMusic->LoadStandard();
				bgMusic->SetLoop(Settings.BgMusicLoop);
				bgMusic->Play();
			}
			break;*/
		default:
			break;
	}

	SetOptionValues();
}
