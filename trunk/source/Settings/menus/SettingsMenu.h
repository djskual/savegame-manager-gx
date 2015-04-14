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
 * MainMenu.h
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/

#ifndef _SETTINGSMENU_H
#define _SETTINGSMENU_H

#include "../../Gui/gui.h"
#include "../../Gui/PictureButton.h"
#include "../../Themes/CTheme.h"
#include "../../Prompts/CreditsWindow.h"
#include "GlobalSettings.hpp"
#include "SettingsBrowser.hpp"

class SettingsMenu : public GuiFrame, public sigslot::has_slots<>
{
	public:
		static SettingsMenu * Instance() { if(!instance) instance = new SettingsMenu(); return instance; }
		static void DestroyInstance() { delete instance; instance = NULL; }
		
		void show(short loadSM = -1);
		void quit();
		void trash();
		void SetTitle(std::string t) { if(Title) Title->SetText(t); }
		
		enum
		{
			GUI,
			AUDIO,
			PATH,
			THEME,
			CONTROL,
			PARENTAL,
			FEATURES,
			UPDATE,
			DEFAULT,
			CREDITS,
		};
		
	private:
		SettingsMenu();
		virtual ~SettingsMenu();
		
		static SettingsMenu *instance;
		
		void OnBackButtonClick(GuiButton *sender, int pointer, const POINT &p);
		void OnFlyingButtonClick(int menu);
		void OnCreditsClicked(CreditsWindow * creditsMenu);
		
		GlobalSettings * globalSettings;
		SettingsBrowser * settingsBrowser;
		
		GuiText * Title;
		
		GuiImageData * BgImgData;
		GuiImage * BgImg;
		
		GuiText * BackBtnTxt;
		PictureButton * BackBtn;
		
		GuiSound * BtnSoundClick;
		GuiSound * BtnSoundOver;
		
		GuiTrigger trigA;
		GuiTrigger trigB;
};

#endif
