/****************************************************************************
 * Copyright (C) 2010
 * by Dimok
 * modified by Dj_Skual
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
 * HomebreBrowser.hpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#ifndef HOMEBREWBROWSER_HPP_
#define HOMEBREWBROWSER_HPP_

#include "../Settings/menus/FlyingButtonsMenu.hpp"
#include "../FileOperations/DirList.h"

#define DISPLAY_BUTTONS 4

class HomebrewBrowser : public FlyingButtonsMenu
{
	public:
		HomebrewBrowser();
		virtual ~HomebrewBrowser();
		static int Run();
		virtual int MainLoop();
		
	protected:
		void HomebrewFilter();
		void MainButtonClicked(int index);
		virtual void CreateSettingsMenu(int index) { MainButtonClicked(index); };
		virtual void DeleteSettingsMenu() { };
		virtual void SetupMainButtons();
		virtual void AddMainButtons();
		
		DirList * HomebrewList;
		GuiImageData * IconImgData[DISPLAY_BUTTONS];
		GuiImage * IconImg[DISPLAY_BUTTONS];
		std::vector<GuiText *> MainButtonDesc;
		std::vector<GuiText *> MainButtonDescOver;
		
		GuiImageData * channelImgData;
		GuiImage * channelBtnImg;
		GuiButton * channelBtn;
};

#endif
