/****************************************************************************
 * Copyright (C) 2010
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
 ***************************************************************************/
#include <unistd.h>
#include "settings/menus/SoundSettingsMenu.hpp"
#include "settings/settingsprompts.h"
#include "prompts/browsers/browsers.h"
#include "prompts/PromptWindows.h"
#include "language/gettext.h"
#include "menu/MainWindow.h"
#include "file/fileops.h"
#include "main.h"

SoundSettingsMenu::SoundSettingsMenu()
    : SettingsMenu(tr("Audio Settings"), &GuiOptions, MENU_NONE)
{
    int Idx = 0;
    Options->SetName(Idx++, "%s", tr( "Music Volume" ));
    Options->SetName(Idx++, "%s", tr( "Credits Volume" ));
    Options->SetName(Idx++, "%s", tr( "SFX Volume" ));
    Options->SetName(Idx++, "%s", tr( "BgMusic Path" ));
    Options->SetName(Idx++, "%s", tr( "BgMusic Loop" ));
    Options->SetName(Idx++, "%s", tr( "Reset BgMusic" ));
    
	SetOptionValues();
}

void SoundSettingsMenu::SetOptionValues()
{
    int Idx = 0;

	//! Settings: Music Volume
    if (cfg.MusicVolume > 0)
        Options->SetValue(Idx++, "%i", cfg.MusicVolume);
    else
        Options->SetValue(Idx++, tr( "OFF" ));

    //! Settings: Credits Volume
    if (cfg.CreditsVolume > 0)
        Options->SetValue(Idx++, "%i", cfg.CreditsVolume);
    else
        Options->SetValue(Idx++, tr( "OFF" ));
	
	//! Settings: SFX Volume
    if (cfg.SFXVolume > 0)
        Options->SetValue(Idx++, "%i", cfg.SFXVolume);
    else
        Options->SetValue(Idx++, tr( "OFF" ));
		
	//! Settings: BgMusic Path
    const char * filename = strrchr(cfg.BgMusicPath, '/');
    if (filename)
        Options->SetValue(Idx++, filename+1);
    else
        Options->SetValue(Idx++, tr( "Default" ));
	
	//! Settings: BgMusic Loop
    if (cfg.BgMusicLoop == 0)
        Options->SetValue(Idx++, tr( "Play Once" ));
    else if (cfg.BgMusicLoop == 1)
        Options->SetValue(Idx++, tr( "Title Loop" ));
    else if (cfg.BgMusicLoop == 2)
        Options->SetValue(Idx++, tr( "Random Play" ));
    else
        Options->SetValue(Idx++, tr( "Directory Loop" ));
	
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
        cfg.MusicVolume += 10;
        if (cfg.MusicVolume > 100) cfg.MusicVolume = 0;
        bgMusic->SetVolume(cfg.MusicVolume);
    }

    //! Settings: Credits Volume
    else if (ret == ++Idx)
    {
        cfg.CreditsVolume += 10;
        if (cfg.CreditsVolume > 100) cfg.CreditsVolume = 0;
    }

    //! Settings: SFX Volume
    else if (ret == ++Idx)
    {
        cfg.SFXVolume += 10;
        if (cfg.SFXVolume > 100) cfg.SFXVolume = 0;
    }

    //! Settings: BgMusic Path
	else if (ret == ++Idx)
    {
		MainWindow::Instance()->SetState(STATE_DISABLED);

		char entered[MAXPATHLEN];
		int ret = MenuBackgroundMusic(entered);
		if(ret == 1)
		{
			if (!bgMusic->Load(entered))
			{
				WindowPrompt(tr("Not supported format!"), tr("Loading standard music."), tr("OK"));
			}
			else
			{
				bgMusic->SetLoop(cfg.BgMusicLoop);
				bgMusic->SetVolume(cfg.MusicVolume);
				bgMusic->Play();
			}
		}
		else if(ret == 2)
		{
			bgMusic->PlayPrevious();
		}
		else if(ret == 3)
		{
			bgMusic->PlayNext();
		}
		
		MainWindow::Instance()->SetState(STATE_DEFAULT);
	}
	
	//! Settings: BgMusic Loop
    else if (ret == ++Idx)
    {
        if (++cfg.BgMusicLoop > 3) cfg.BgMusicLoop = 0;
		bgMusic->SetLoop(cfg.BgMusicLoop);
		bgMusic->Play();
    }

    //! Button: Reset BgMusic
    else if (ret == ++Idx)
    {
        strcpy(cfg.BgMusicPath, "");
		cfg.BgMusicLoop = LOOP;
		bgMusic->Load(cfg.BgMusicPath);
		bgMusic->SetLoop(cfg.BgMusicLoop);
		bgMusic->SetVolume(cfg.MusicVolume);
		bgMusic->Play();
    }

    SetOptionValues();

    return MENU_NONE;
}
