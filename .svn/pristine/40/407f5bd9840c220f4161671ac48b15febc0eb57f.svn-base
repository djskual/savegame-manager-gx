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
#ifndef SETTINGS_MENU_HPP_
#define SETTINGS_MENU_HPP_

#include "libwiigui/gui_browsers/gui_browsers.h"
#include "libwiigui/gui_browsers/CustomOptionList.hpp"
#include "libwiigui/gui_browsers/gui_customoptionbrowser.h"
#include "menu/menu.h"

enum
{
    CSettingsMenu = 0,
    CGUISettingsMenu,
    CSoundSettingsMenu,
    CCustomPathsSM,
	CWiiTDBSettingsMenu,
	CUpdateSettingsMenu,
};

class SettingsMenu : public GuiWindow
{
    public:
        SettingsMenu(const char * title, CustomOptionList * options, int returnTo);
        ~SettingsMenu();
        int GetClickedOption();
        int GetMenu();
        virtual int GetType() { return CSettingsMenu; }
    protected:
        virtual int GetMenuInternal() { return MENU_NONE; };
        int returnToMenu;
		
        GuiImageData * btnOutline;
		
        GuiTrigger * trigA;
        GuiTrigger * trigB;
		
        CustomOptionList * Options;
		
        GuiText * titleTxt;
        GuiText * backBtnTxt;
        GuiImage * backBtnImg;
        GuiButton * backBtn;
		
        GuiCustomOptionBrowser * optionBrowser;

};


#endif
