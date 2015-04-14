 /****************************************************************************
 * Copyright (C) 2011
 * by Dj_Skual
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
#include "ThemeSettingsMenu.hpp"
#include "SettingsMenu.h"
#include "../../Controls/Application.h"
#include "../../Prompts/PromptWindows.h"
#include "../../Themes/ThemeMenu.h"

static const char * OnOffText[MAX_ON_OFF] =
{
	trNOOP( "OFF" ),
	trNOOP( "ON" )
};

ThemeSettingsMenu::ThemeSettingsMenu()
	: SettingsBrowser()
{
	SettingsMenu::Instance()->SetTitle(tr( "Theme Settings" ));
	
	SetupOptionName();
	show();
}

void ThemeSettingsMenu::SetupOptionName()
{
	int Idx = 0;
	
	options->SetName(Idx++, tr( "Use System Font" ));
	options->SetName(Idx++, tr( "Font Scale Factor" ));
	options->SetName(Idx++, tr( "Clock Scale Factor" ));
	options->SetName(Idx++, tr( "Download Themes" ));
	options->SetName(Idx++, tr( "Select Theme" ));
	options->SetName(Idx++, tr( "Default Theme" ));
	
	SetOptionValues();
}

void ThemeSettingsMenu::SetOptionValues()
{
	int Idx = 0;
	
	//! Settings: Use System Font
	options->SetValue(Idx++, tr( OnOffText[Settings.UseSystemFont] ));
	
	//! Settings: Font Scale Factor
	options->SetValue(Idx++, "%0.3f", Settings.FontScaleFactor);
	
	//! Settings: Clock Font Scale Factor
	options->SetValue(Idx++, "%0.3f", Settings.ClockFontScaleFactor);
	
	//! Button: Download Themes
	options->SetValue(Idx++, " ");
	
	//! Button: Select Theme
	options->SetValue(Idx++, " ");
	
	//! Button: Default Theme
	options->SetValue(Idx++, " ");
}

void ThemeSettingsMenu::OnBrowserClick(GuiOptionBrowser * browser UNUSED, int option)
{
	char entrie[20];
	int ret = 0;
	
	switch (option)
	{
		case 0:
			if (++Settings.UseSystemFont >= MAX_ON_OFF) Settings.UseSystemFont = OFF;
			
			if(Settings.UseSystemFont == ON) Theme::LoadFont("");
			else Theme::LoadFont(Settings.CustomFontPath);
			
			if(Settings.FontScaleFactor == 1.0f && Settings.UseSystemFont == ON)
				Settings.FontScaleFactor = 0.95f;
			else if(Settings.FontScaleFactor == 0.95f && Settings.UseSystemFont == OFF)
				Settings.FontScaleFactor = 1.0f;
			
			break;
		case 1:
			snprintf(entrie, sizeof(entrie), "%0.3f", Settings.FontScaleFactor);
			ret = OnScreenNumpad(entrie);
			if(ret)
				Settings.FontScaleFactor = LIMIT(atof(entrie), 0.500f, 1.500f);
			break;
		case 2:
			snprintf(entrie, sizeof(entrie), "%0.3f", Settings.ClockFontScaleFactor);
			ret = OnScreenNumpad(entrie);
			if(ret)
				Settings.ClockFontScaleFactor = LIMIT(atof(entrie), 0.500f, 1.500f);
			break;
		case 3:
			if(WindowPrompt(tr("Theme Downloader"), tr("Do you want to download themes now ?"), tr("Yes"), tr("Cancel")))
			{
				ThemeMenu * Menu = new ThemeMenu(ThemeMenu::DOWNLOADER);
				Menu->Closed.connect(this, &ThemeSettingsMenu::OnDownloaderClose);
			}
			break;
		case 4:
			if(WindowPrompt(tr("Theme Menu"), tr("Do you want to select a theme now ?"), tr("Yes"), tr("Cancel")))
			{
				ThemeMenu * Menu = new ThemeMenu(ThemeMenu::LOADER);
				Menu->Closed.connect(this, &ThemeSettingsMenu::OnLoaderClose);
			}
			break;
		case 5:
			if(WindowPrompt(0, tr("Do you want to load the default theme?"), tr("Yes"), tr("Cancel")))
				Application::Instance()->ThemeReloadRequest(true);
			break;
		default:
			break;
	}

	SetOptionValues();
}

void ThemeSettingsMenu::OnDownloaderClose()
{
	
}

void ThemeSettingsMenu::OnLoaderClose()
{
	
}
