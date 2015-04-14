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
#include "CustomPathsSM.hpp"
#include "SettingsMenu.h"
#include "../../Controls/Application.h"
#include "../../Prompts/PromptWindows.h"

CustomPathsSM::CustomPathsSM()
	: SettingsBrowser()
	, setPath(0)
	, setPathSize(0)
{
	SettingsMenu::Instance()->SetTitle(tr( "Custom Paths" ));
	
	SetupOptionName();
	show();
}

void CustomPathsSM::SetupOptionName()
{
	int Idx = 0;
	
	options->SetName(Idx++, tr("Saves Path"));
	options->SetName(Idx++, tr("Update Path"));
	options->SetName(Idx++, tr("Custom Font"));
	options->SetName(Idx++, tr("Music Path"));
	options->SetName(Idx++, tr("Screenshot Path"));
	options->SetName(Idx++, tr("Theme Path"));
	//options->SetName(Idx++, tr("Homebrew Path"));
	//options->SetName(Idx++, tr("Miis Path"));
	//options->SetName(Idx++, tr("GameTDB Path"));
	//options->SetName(Idx++, tr("Data.bin Path"));
	//if(!Settings.Sneek) Options->SetName(Idx++, tr("Emu Nand Path"));
	
	SetOptionValues();
}

void CustomPathsSM::SetOptionValues()
{
	int Idx = 0;
	
	//! Settings: Saves Path
	options->SetValue(Idx++, Settings.SaveGamePath);
	
	//! Settings: Update Path
	options->SetValue(Idx++, Settings.UpdatePath);
	
	//! Settings: Custom Font
	options->SetValue(Idx++, Settings.UseSystemFont ? tr( "Disabled" ) : Settings.CustomFontPath);
	
	//! Settings: Music Path
	options->SetValue(Idx++, Settings.MusicPath);

	//! Settings: Screenshot Path
	options->SetValue(Idx++, Settings.ScreenshotPath);
	
	//! Settings: Theme Path
	options->SetValue(Idx++, Settings.ThemePath);
	
	//! Settings: Homebrew Path
	//options->SetValue(Idx++, Settings.HomebrewPath);
	
	//! Settings: Miis Path
	//options->SetValue(Idx++, Settings.SaveMiisPath);
	
	//! Settings: GameTDB Path
	//options->SetValue(Idx++, Settings.GameTDBPath);
	
	//! Settings: Data.bin Path
	//options->SetValue(Idx++, Settings.DataBinPath);
	
	//! Settings: Emu Nand Path
	//if(!Settings.Sneek)
	//	options->SetValue(Idx++, Settings.EmuNandPath);
}

void CustomPathsSM::OnBrowserClick(GuiOptionBrowser * browser UNUSED, int option)
{
	switch (option)
	{
		case 0:
			ChangePath(Settings.SaveGamePath, sizeof(Settings.SaveGamePath));
			break;
		case 1:
			ChangePath(Settings.UpdatePath, sizeof(Settings.UpdatePath));
			break;
		case 2:
			if(!Settings.UseSystemFont)
			{
				ChangePath(Settings.CustomFontPath, sizeof(Settings.CustomFontPath), FILTER_FONT);
				Theme::LoadFont(Settings.CustomFontPath);
			}
			break;
		case 3:
			ChangePath(Settings.MusicPath, sizeof(Settings.MusicPath));
			break;
		case 4:
			ChangePath(Settings.ScreenshotPath, sizeof(Settings.ScreenshotPath));
			break;
		case 5:
			ChangePath(Settings.ThemePath, sizeof(Settings.ThemePath));
			break;
		/*case 6:
			ChangePath(Settings.HomebrewPath, sizeof(Settings.HomebrewPath));
			break;
		case 7:
			ChangePath(Settings.SaveMiisPath, sizeof(Settings.SaveMiisPath));
			break;
		case 8:
			ChangePath(Settings.GameTDBPath, sizeof(Settings.GameTDBPath));
			break;
		case 9:
			ChangePath(Settings.DataBinPath, sizeof(Settings.DataBinPath));
			break;
		case 10:
			ChangePath(Settings.EmuNandPath, sizeof(Settings.EmuNandPath));
			break;*/
		default:
			break;
	}
	
	SetOptionValues();
}

void CustomPathsSM::ChangePath(char *path, int size, u8 filter)
{
	int choice = WindowPrompt(tr("How do you want to change the path?"), 0, tr("Browse"), tr("Enter"), tr("Cancel"));
	if(choice == 1)
	{
		SetEffect(EFFECT_FADE, -20);
		while(this->IsAnimated())
			Application::Instance()->updateEvents();
		
		setPath = path;
		setPathSize = size;
		
		PathBrowser *pathBrowser = new PathBrowser(path, filter);
		pathBrowser->ButtonClick.connect(this, &CustomPathsSM::OnPathBrowserButtonClick);
	}
	else if(choice == 2)
	{
		char entered[MAXPATHLEN];
		snprintf(entered, sizeof(entered), "%s", path);
		if(OnScreenKeyboard(entered, sizeof(entered)))
		{
			while((entered[strlen(entered)-1] == '/') && (entered[strlen(entered)-2] != ':'))
				entered[strlen(entered)-1] = 0;
			
			struct stat filestat;
			stat(entered, &filestat);
			if(filestat.st_mode & S_IFDIR) //! folder
			{
				if(filter & FILTER_PATH)
				{
					snprintf(path, size, "%s", entered);
					WindowPrompt(tr("Path changed."), 0, tr("OK"));
				}
			}
			else //! file
			{
				char * fileExt = strrchr(entered, '.');
				if (fileExt)
				{
					if((filter & FILTER_FONT) && !Settings.FileExtensions.CompareFont(fileExt))
					{
						snprintf(path, size, "%s", entered);
						WindowPrompt(tr("Fontpath changed."), 0, tr("OK"));
					}
					//if((filter & FILTER_LANG) && !Settings.FileExtensions.CompareLanguageFiles(fileExt))
					//{
						//
					//}
				}
			}
		}
	}
}

void CustomPathsSM::OnPathBrowserButtonClick(PathBrowser *menu, u8 filter, bool doneClicked, const std::string &Path)
{
	menu->SetEffect(EFFECT_FADE, -20);

	if(doneClicked)
	{
		if (Path.size() && (filter & FILTER_PATH))
		{
			std::string tmp = Path;
			while((tmp[tmp.size()-1] == '/') && (tmp[tmp.size()-2] != ':'))
				tmp.erase(tmp.size()-1);
			
			snprintf(setPath, setPathSize, "%s", tmp.c_str());
		}
		else if (Path.size() && (filter & FILTER_FONT))
		{
			snprintf(setPath, setPathSize, "%s", Path.c_str());
		}
	}

	SetOptionValues();
	
	while(menu->IsAnimated())
		Application::Instance()->updateEvents();
	
	SetEffect(EFFECT_FADE, 20);
	Application::Instance()->PushForDelete(menu);
}
