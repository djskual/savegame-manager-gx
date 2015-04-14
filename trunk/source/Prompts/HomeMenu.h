/***************************************************************************
 * Copyright (C) 2010
 * by dude
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
 * HomeMenu.h
 *
 * for WiiXplorer 2010
 ***************************************************************************/

#ifndef _HOMEMENU_H
#define _HOMEMENU_H

#include "../GUI/gui.h"
#include "../Gui/PictureButton.h"

class HomeMenu : public GuiFrame, public sigslot::has_slots<>
{
	public:
		HomeMenu();
		virtual ~HomeMenu();
		void quit();
		bool IsAnimated();
		void Draw();
		
	private:
		void OnStateChange(GuiElement *sender, int state, int stateChan);
		void OnButtonClick(GuiButton *sender, int pointer, const POINT &p);
		void SettingsShow();
		void SettingsHide();
		void SettingsAnim(bool expand);
		void SettingsBtnUpdate();
		
		bool settingsActive;
		
		GuiImageData * BatteryBarDataRed;
		GuiImageData * BatteryBarDataWh;
		GuiImageData * BatteryDataRed;
		GuiImageData * BatteryDataWh;
		GuiImageData * WiimoteData;
		GuiImageData * SettingsBgData;
		GuiImageData * SettingsBtnData;
		GuiImageData * SettingsBtnActiveData;
		
		GuiImage * BatteryImg[4];
		GuiImage * BatteryBarImg[4];
		GuiImage * WiimoteImg;
		GuiImage * SettingsBgImg;
		GuiImage * SettingsBtnImg;
		GuiImage * SettingsBtnActiveImg;
		
		GuiText * TitleText;
		GuiText * SettingsText;
		GuiText * CloseBtnText;
		GuiText * LeftBtnText;
		GuiText * RightBtnText;
		GuiText * PlayerText[4];
		GuiText * RumbleText;
		GuiText * RumbleOnText;
		GuiText * RumbleOffText;
		
		PictureButton * TopBtn;
		PictureButton * CloseBtn;
		PictureButton * BottomBtn;
		PictureButton * LeftBtn;
		PictureButton * RightBtn;
		GuiButton * RumbleOnBtn;
		GuiButton * RumbleOffBtn;
		GuiButton * BatteryBtn[4];
		
		GuiTrigger trigA;
		GuiTrigger trigB;
		GuiTrigger trigHome;
		
		GuiSound * HomeInSnd;
		GuiSound * HomeOutSnd;
		GuiSound * BtnSoundClick;
		GuiSound * BtnSoundOver;
};

#endif
