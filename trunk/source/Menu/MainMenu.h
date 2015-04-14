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

#ifndef _MAINMENU_H
#define _MAINMENU_H

#include "../Gui/gui.h"
#include "../Gui/PictureButton.h"
#include "../Themes/CTheme.h"
#include "../Prompts/DeviceMenu.h"
#include "../Prompts/SearchWindow.h"
#include "TaskBar.hpp"
#include "Explorer.h"

class Explorer;

class MainMenu : public GuiFrame, public sigslot::has_slots<>
{
	public:
		static MainMenu * Instance() { if(!instance) instance = new MainMenu(); return instance; }
		static void DestroyInstance() { delete instance; instance = NULL; }
		
		void show();
		void hide();
		void Refresh() { hide(); show(); }
		void Update(GuiTrigger * t);
		
		Explorer *GetExplorer() { return mainExplorer; }
		MainMenu *GetInstance() { return instance; }
		
		//! search tasks
		void ExplorerFilterList(wString fileFilter = L"", bool resetBrowser = false) { mainExplorer->FilterList(fileFilter, resetBrowser); }
		void SetSearchOn(bool b) { searchOn = b; }
		void SetFilterOn(bool b) { filterOn = b; }
		void RefreshSearchWindow() { refreshSearch = true; }
		bool SearchWindowDisplayed() { return searchOn; }
		sigslot::signal2<bool, bool> SearchStateChanged;
		
		//! taskBar tasks
		void OnTaskbarClick(TaskBar * taskbar, int sender);
		sigslot::signal1<int> DeviceChanged;
		
	private:
		MainMenu();
		virtual ~MainMenu();
		
		static MainMenu *instance;
		
		bool searchOn;
		bool filterOn;
		bool refreshSearch;
		
		short oldLockSetting;
		
		void OnManageButtonClick(GuiButton *sender, int pointer, const POINT &p);
		void OnSettingsButtonClick(GuiButton *sender, int pointer, const POINT &p);
		void OnHomebrewButtonClick(GuiButton *sender, int pointer, const POINT &p);
		void OnMusicButtonClick(GuiButton *sender, int pointer, const POINT &p);
		void OnSdCardButtonClick(GuiButton *sender, int pointer, const POINT &p);
		void OnDeviceMenuClose(DeviceMenu * menu, int item);
		void OnSearchWindowClose(GuiFrame * window);
		
		Explorer * mainExplorer;
		DeviceMenu * deviceMenu;
		SearchWindow * searchWindow;
		
		GuiTrigger trigA;
		
		GuiImageData * SdCardImgData;
		GuiImageData * SdCardImgGrayData;
		
		GuiImage * SdCardImg;
		GuiImage * SdCardImgGray;
		
		GuiTooltip * ManageTT;
		GuiTooltip * SettingsTT;
		GuiTooltip * HomebrewTT;
		GuiTooltip * MusicPlayerTT;
		GuiTooltip * SdCardTT;
		
		PictureButton * ManageBtn;
		PictureButton * SettingsBtn;
		PictureButton * HomebrewBtn;
		PictureButton * MusicPlayerBtn;
		PictureButton * SdCardBtn;
		
		GuiSound * BtnSoundClick;
		GuiSound * BtnSoundOver;
};

#endif
