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
#include "GUISettingsMenu.hpp"
#include "SettingsMenu.h"
#include "../../Controls/Application.h"
#include "../../Prompts/PromptWindows.h"
#include "../../System/input.h"

static const char * OnOffText[MAX_ON_OFF] =
{
	trNOOP( "OFF" ),
	trNOOP( "ON" )
};

static const char *ScreensaverText[SCREENSAVER_MAX] =
{
	trNOOP( "OFF" ),
	trNOOP( "1 min" ),
	trNOOP( "3 mins" ),
	trNOOP( "5 mins" ),
	trNOOP( "10 mins" ),
	trNOOP( "20 mins" ),
	trNOOP( "30 mins" ),
	trNOOP( "1 hour" )
};

static const char * HomeMenuText[HOME_MENU_MAX] =
{
	trNOOP( "System Default" ),
	trNOOP( "Full Menu" ),
	trNOOP( "Default" )
};

GuiSettingsMenu::GuiSettingsMenu()
	: SettingsBrowser()
{
	SettingsMenu::Instance()->SetTitle(tr( "GUI Settings" ));
	
	SetupOptionName();
	show();
}

void GuiSettingsMenu::SetupOptionName()
{
	int Idx = 0;
	
	options->SetName(Idx++, tr( "Language" ));
	options->SetName(Idx++, tr( "Clock" ));
	options->SetName(Idx++, tr( "Tooltips" ));
	options->SetName(Idx++, tr( "Tooltip Delay" ));
	options->SetName(Idx++, tr( "HOME Menu" ));
	options->SetName(Idx++, tr( "Keyboard Del Delay" ));
	options->SetName(Idx++, tr( "Screensaver" ));
	options->SetName(Idx++, tr( "Pointer Speed" ));
	options->SetName(Idx++, tr( "Scroll Speed" ));
	
	SetOptionValues();
}

void GuiSettingsMenu::SetOptionValues()
{
	int Idx = 0;
	
	//! Settings: App Language
	const char * language = strrchr(Settings.LanguageFile, '/');
	if(language)
		language += 1;
	if (!language || strcmp(Settings.LanguageFile, "") == 0)
		options->SetValue(Idx++, tr( "Default" ));
	else
		options->SetValue(Idx++, language);

	//! Settings: Clock
	if (Settings.Clock == CLOCK_HR12)
		options->SetValue(Idx++, "12 %s", tr( "Hours" ));
	else if (Settings.Clock == CLOCK_HR24)
		options->SetValue(Idx++, "24 %s", tr( "Hours" ));
	else if (Settings.Clock == OFF)
		options->SetValue(Idx++, tr( "OFF" ));

	//! Settings: Tooltips
	options->SetValue(Idx++, tr(OnOffText[Settings.Tooltips]));

	//! Settings: Tooltip Delay
	options->SetValue(Idx++, "%d %s", Settings.TooltipDelay, tr("seconds"));

	//! Settings: Home Menu style
	options->SetValue(Idx++, tr( HomeMenuText[Settings.HomeMenu] ));

	//! Settings: Keyboard Delete Delay
	options->SetValue(Idx++, "%i", Settings.KeyboardDeleteDelay);

	//! Settings: Screensaver
	options->SetValue(Idx++, tr( ScreensaverText[Settings.Screensaver] ));
	
	//! Settings: Pointer Speed
	options->SetValue(Idx++, "%0.3f", Settings.PointerSpeed);
	
	//! Settings: Scroll Speed
	options->SetValue(Idx++, "%i", Settings.ScrollSpeed);
}

void GuiSettingsMenu::OnBrowserClick(GuiOptionBrowser * browser UNUSED, int option)
{
	char entrie[20];
	int ret = 0;
	
	switch (option)
	{
		case 0:
			if (!Settings.GodMode)
			{
				WindowPrompt(tr( "Language change:" ), tr( "Console must be unlocked for this option." ), tr( "OK" ));
				return;
			}
			else
			{
				SetEffect(EFFECT_FADE, -20);
				while(this->IsAnimated())
					Application::Instance()->updateEvents();
				
				PathBrowser *pathBrowser = new PathBrowser(Settings.LanguagePath, FILTER_LANG);
				pathBrowser->ButtonClick.connect(this, &GuiSettingsMenu::OnPathBrowserButtonClick);
			}
			break;
		case 1:
			if (++Settings.Clock >= CLOCK_MAX) Settings.Clock = OFF;
			break;
		case 2:
			if (++Settings.Tooltips >= MAX_ON_OFF) Settings.Tooltips = OFF;
			break;
		case 3:
			snprintf(entrie, sizeof(entrie), "%i", Settings.TooltipDelay);
			ret = OnScreenNumpad(entrie);
			if(ret)
				Settings.TooltipDelay = atoi(entrie);
			break;
		case 4:
			if (++Settings.HomeMenu >= HOME_MENU_MAX) Settings.HomeMenu = HOME_MENU_SYSTEM;
			break;
		case 5:
			snprintf(entrie, sizeof(entrie), "%i", Settings.KeyboardDeleteDelay);
			ret = OnScreenNumpad(entrie);
			if(ret)
				Settings.KeyboardDeleteDelay = atoi(entrie);
			break;
		case 6:
			if (++Settings.Screensaver >= SCREENSAVER_MAX) Settings.Screensaver = 0;
			SetWPADTimeout();
			break;
		case 7:
			snprintf(entrie, sizeof(entrie), "%0.3f", Settings.PointerSpeed);
			ret = OnScreenNumpad(entrie);
			if(ret)
				Settings.KeyboardDeleteDelay = atof(entrie);
			break;
		case 8:
			snprintf(entrie, sizeof(entrie), "%i", Settings.ScrollSpeed);
			ret = OnScreenNumpad(entrie);
			if(ret)
				Settings.ScrollSpeed = atoi(entrie);
			break;
		default:
			break;
	}
	
	SetOptionValues();
}

void GuiSettingsMenu::OnPathBrowserButtonClick(PathBrowser *menu, u8 filter, bool doneClicked, const std::string &Path)
{
	menu->SetEffect(EFFECT_FADE, -20);

	if(doneClicked)
	{
		if (filter & FILTER_LANG)
		{
			snprintf(Settings.LanguageFile, sizeof(Settings.LanguageFile), "%s", Path.c_str());
			
			std::string tmp = Path;
			tmp.erase(tmp.find_last_of('/'));
			snprintf(Settings.LanguagePath, sizeof(Settings.LanguagePath), "%s", tmp.c_str());
		}
	}

	while(menu->IsAnimated())
		Application::Instance()->updateEvents();
	Application::Instance()->PushForDelete(menu);
	
	if(doneClicked && (filter & FILTER_LANG))
		Application::Instance()->ReloadLanguageRequest();
	
	SetOptionValues();
	
	SetEffect(EFFECT_FADE, 20);
}
