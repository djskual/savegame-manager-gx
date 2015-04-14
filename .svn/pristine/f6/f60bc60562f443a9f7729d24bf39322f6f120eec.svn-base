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
#include <unistd.h>

#include "ThemeSettingsMenu.hpp"
#include "../../Prompts/SelectBrowser.h"
#include "../../Prompts/PromptWindows.h"
#include "../../Language/gettext.h"
#include "../../Themes/CTheme.h"

static const char * OnOffText[MAX_ON_OFF] =
{
	trNOOP( "OFF" ),
	trNOOP( "ON" )
};

ThemeSettingsMenu::ThemeSettingsMenu()
	: SettingsMenu(tr( "Theme Settings" ), &GuiOptions, MENU_NONE)
{
	int Idx = 0;
	
	Options->SetName(Idx++, tr( "Use System Font" ));
	Options->SetName(Idx++, tr( "Custom Font" ));
	Options->SetName(Idx++, tr( "Font Scale Factor" ));
	Options->SetName(Idx++, tr( "Theme Path" ));
	Options->SetName(Idx++, tr( "Download Themes" ));
	Options->SetName(Idx++, tr( "Select Theme" ));
	Options->SetName(Idx++, tr( "Default Theme" ));
	
	SetOptionValues();
}

void ThemeSettingsMenu::SetOptionValues()
{
	int Idx = 0;
	
	//! Settings: Use System Font
	Options->SetValue(Idx++, tr( OnOffText[Settings.UseSystemFont] ));
	
	//! Settings: Custom Font
	Options->SetValue(Idx++, Settings.UseSystemFont ? tr( "Disabled" ) : Settings.CustomFontPath);
	
	//! Settings: Font Scale Factor
	Options->SetValue(Idx++, "%0.3f", Settings.FontScaleFactor);
	
	//! Settings: Theme Path
	Options->SetValue(Idx++, Settings.ThemePath);
	
	//! Button: Download Themes
	Options->SetValue(Idx++, " ");
	
	//! Button: Select Theme
	Options->SetValue(Idx++, " ");
	
	//! Button: Default Theme
	Options->SetValue(Idx++, " ");
	
}

int ThemeSettingsMenu::GetMenuInternal()
{
	int ret = optionBrowser->GetClickedOption();

	if (ret < 0)
		return MENU_NONE;

	int Idx = -1;

	//! Settings: Use System Font
	if (ret == ++Idx)
	{
		if (++Settings.UseSystemFont >= MAX_ON_OFF) Settings.UseSystemFont = 0;
		
		HaltGui();
		if(Settings.UseSystemFont == ON) Theme::LoadFont("");
		else Theme::LoadFont(Settings.CustomFontPath);
		ResumeGui();
		
		if(Settings.FontScaleFactor == 1.0f && Settings.UseSystemFont == ON)
			Settings.FontScaleFactor = 0.95f;
		else if(Settings.FontScaleFactor == 0.95f && Settings.UseSystemFont == OFF)
			Settings.FontScaleFactor = 1.0f;
	}

	//! Settings: Custom Font
	else if (ret == ++Idx)
	{
		if(!Settings.UseSystemFont)
		{
			titleTxt->SetText(tr( "Custom Font" ));
			showSelectBrowser(Settings.CustomFontPath, FONTBROWSER);
			
			Settings.UseSystemFont = OFF;
			HaltGui();
			Theme::LoadFont(Settings.CustomFontPath);
			ResumeGui();
		}
	}
	
	//! Settings: Font Scale Factor
	else if (ret == ++Idx)
	{
		char entrie[20];
		snprintf(entrie, sizeof(entrie), "%0.3f", Settings.FontScaleFactor);
		int ret = OnScreenKeyboard(entrie, sizeof(entrie), 0);
		if(ret)
			Settings.FontScaleFactor = LIMIT(atof(entrie), 0.500f, 1.500f);
	}

	//! Settings: Theme Path
	if (ret == ++Idx)
	{
		if(!Settings.GodMode)
		{
			WindowPrompt(tr( "Permission denied." ), tr( "Console must be unlocked for this option." ), tr( "OK" ));
			return MENU_NONE;
		}
		
		titleTxt->SetText(tr( "Theme Path" ));
		showSelectBrowser(Settings.ThemePath, THEMEPATHBROWSER);
		
		titleTxt->SetText(tr( "Theme Settings" ));
	}

	//! Button: Download Themes
	else if (ret == ++Idx)
	{
		if(WindowPrompt(tr("Theme Downloader"), tr("Do you want to download themes now ?"), tr("Yes"), tr("Cancel")) == 1)
			return MENU_THEMEDOWNLOADER;
	}

	//! Button: Select Theme
	else if (ret == ++Idx)
	{
		if(WindowPrompt(tr("Theme Menu"), tr("Do you want to select a theme now ?"), tr("Yes"), tr("Cancel")) == 1)
			return MENU_THEMEMENU;
	}

	//! Button: Default Theme
	else if (ret == ++Idx)
	{
		if(WindowPrompt(0, tr("Do you want to load the default theme?"), tr("Yes"), tr("Cancel")) == 1)
		{
			HaltGui();
			Theme::SetDefault();
			Theme::Reload();
			ResumeGui();
			return MENU_SETTINGS;
		}
	}

	SetOptionValues();

	return MENU_NONE;
}

void ThemeSettingsMenu::showSelectBrowser(const char * enterPath, short Type)
{
	GuiWindow * parent = (GuiWindow *) parentElement;
	if(parent) parent->SetState(STATE_DISABLED);
	this->Remove(optionBrowser);
	
	selectBrowser(enterPath, Type);
	
	if(parent) parent->SetState(STATE_DEFAULT);
	this->Append(optionBrowser);
}
