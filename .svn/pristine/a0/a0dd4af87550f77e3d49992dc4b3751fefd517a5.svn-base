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

#include "GUISettingsMenu.hpp"
#include "../../DeviceControls/DeviceHandler.hpp"
#include "../../Prompts/SelectBrowser.h"
#include "../../Prompts/PromptWindows.h"
#include "../../Language/gettext.h"
#include "../settingsprompts.h"

static const char * OnOffText[MAX_ON_OFF] =
{
	trNOOP( "OFF" ),
	trNOOP( "ON" )
};

static const char * PromptButtonsText[MAX_ON_OFF] =
{
	trNOOP( "4/3" ),
	trNOOP( "16/9" )
};

static const char * BootBrowserText[BROWSE_MAX] =
{
	trNOOP( "SD/USB" ),
	trNOOP( "Wii" ),
	trNOOP( "Mii" )
};

static const char * KeyboardText[KEYBOARD_MAX] =
{
	"QWERTY",
	"DVORAK",
	"QWERTZ",
	"AZERTY",
	"QWERTY 2"
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

GuiSettingsMenu::GuiSettingsMenu()
	: SettingsMenu(tr("GUI Settings"), &GuiOptions, MENU_NONE)
{
	int Idx = 0;
	Options->SetName(Idx++, tr( "Language" ));
	Options->SetName(Idx++, tr( "Boot Browser" ));
	Options->SetName(Idx++, tr( "Clock" ));
	Options->SetName(Idx++, tr( "Tooltips" ));
	Options->SetName(Idx++, tr( "Screen Type" ));
	Options->SetName(Idx++, tr( "Keyboard" ));
	Options->SetName(Idx++, tr( "Rumble" ));
	Options->SetName(Idx++, tr( "Screensaver" ));
	Options->SetName(Idx++, tr( "Pointer Speed" ));
	
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
		Options->SetValue(Idx++, tr( "Default" ));
	else
		Options->SetValue(Idx++, language);

	//! Settings: BootMethod
	Options->SetValue(Idx++, tr(BootBrowserText[Settings.BootBrowser]));
	
	//! Settings: Clock
	if (Settings.Clock == CLOCK_HR12)
		Options->SetValue(Idx++, "12 %s", tr( "Hour" ));
	else if (Settings.Clock == CLOCK_HR24)
		Options->SetValue(Idx++, "24 %s", tr( "Hour" ));
	else if (Settings.Clock == OFF)
		Options->SetValue(Idx++, tr( "OFF" ));

	//! Settings: Tooltips
	Options->SetValue(Idx++, tr(OnOffText[Settings.Tooltips]));

	//! Settings: Screen Size
	Options->SetValue(Idx++, tr( PromptButtonsText[Settings.wsprompt] ));

	//! Settings: Keyboard
	Options->SetValue(Idx++, KeyboardText[Settings.keyset]);

	//! Settings: Rumble
	Options->SetValue(Idx++, tr( OnOffText[Settings.DoRumble] ));

	//! Settings: Screensaver
	Options->SetValue(Idx++, tr( ScreensaverText[Settings.Screensaver] ));
	
	//! Settings: Pointer Speed
	Options->SetValue(Idx++, "%g", Settings.PointerSpeed);
}

int GuiSettingsMenu::GetMenuInternal()
{
	int ret = optionBrowser->GetClickedOption();

	if (ret < 0)
		return MENU_NONE;

	int Idx = -1;

	//! Settings: App Language
	if (ret == ++Idx)
	{
		if (!Settings.GodMode)
		{
			WindowPrompt(tr( "Language change:" ), tr( "Console must be unlocked for this option." ), tr( "OK" ));
			return MENU_NONE;
		}
		SetEffect(EFFECT_FADE, -20);
		while (GetEffect() > 0) usleep(100);
		HaltGui();
		if(parentElement)
		{
			((GuiWindow *) parentElement)->Remove(this);
			((GuiWindow *) parentElement)->SetState(STATE_DISABLED);
		}
		ResumeGui();

		int returnhere = 1;
		while (returnhere == 1)
			returnhere = selectBrowser(NULL, LANGBROWSER);

		if (returnhere == 2)
			return MENU_SETTINGS;

		HaltGui();
		if(parentElement)
		{
			((GuiWindow *) parentElement)->Append(this);
			((GuiWindow *) parentElement)->SetState(STATE_DEFAULT);
		}
		SetEffect(EFFECT_FADE, 20);
		ResumeGui();
		while (GetEffect() > 0) usleep(100);
	}

	//! Settings: BootMethod
	else if (ret == ++Idx)
	{
		if (++Settings.BootBrowser >= BROWSE_EMU) Settings.BootBrowser = 0;
	}

	//! Settings: Clock
	else if (ret == ++Idx)
	{
		if (++Settings.Clock >= CLOCK_MAX) Settings.Clock = 0;
	}

	//! Settings: Tooltips
	else if (ret == ++Idx)
	{
		if (++Settings.Tooltips >= MAX_ON_OFF) Settings.Tooltips = 0;
	}

	//! Settings: Screen Size
	else if (ret == ++Idx)
	{
		if (++Settings.wsprompt >= MAX_ON_OFF) Settings.wsprompt = 0;
	}

	//! Settings: Keyboard
	else if (ret == ++Idx)
	{
		if (++Settings.keyset >= KEYBOARD_MAX) Settings.keyset = 0;
	}

	//! Settings: Rumble
	else if (ret == ++Idx)
	{
		if (++Settings.DoRumble >= MAX_ON_OFF) Settings.DoRumble = 0;
	}

	//! Settings: Screensaver
	else if (ret == ++Idx)
	{
		if (++Settings.Screensaver >= SCREENSAVER_MAX) Settings.Screensaver = 0;
		
		SetWPADTimeout();
	}
	
	//! Settings: Pointer Speed
	else if (ret == ++Idx)
	{
		char entrie[20];
		snprintf(entrie, sizeof(entrie), "%g", Settings.PointerSpeed);
		if( OnScreenKeyboard(entrie, sizeof(entrie), 0) )
		{
			for(u32 i = 0; i < sizeof(entrie); ++i)
			{
				if(entrie[i] == ',')
					entrie[i] = '.';
			}
			
			Settings.PointerSpeed = atof(entrie);
		}
	}
	
	SetOptionValues();

	return MENU_NONE;
}
