
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

#include "libwiigui/gui.h"

class HomeMenu : public GuiWindow
{
	public:
		HomeMenu();
		~HomeMenu();
		int GetChoice();

	private:
		void FadeOut();

		int choice;

		GuiImageData * TopBtnImgData;
		GuiImageData * TopBtnOverImgData;
		GuiImageData * BottomBtnImgData;
		GuiImageData * BottomBtnOverImgData;
		GuiImageData * CloseBtnImgData;
		GuiImageData * StandardBtnImgData;
		GuiImageData * WiimoteBtnImgData;
		GuiImageData * BatteryImgData;
		GuiImageData * BatteryBarImgData;
		GuiImageData * BatteryRedImgData;
		GuiImageData * BatteryBarRedImgData;

		GuiImage * TopBtnImg;
		GuiImage * TopBtnOverImg;
		GuiImage * BottomBtnImg;
		GuiImage * BottomBtnOverImg;
		GuiImage * CloseBtnImg;
		GuiImage * ExitBtnImg;
		GuiImage * ShutdownBtnImg;
		GuiImage * WiimoteBtnImg;
		GuiImage * BatteryImg[4];
		GuiImage * BatteryBarImg[4];
		GuiImage * BatteryRedImg[4];
		GuiImage * BatteryBarRedImg[4];

		GuiText * TitleText;
		GuiText * CloseBtnText;
		GuiText * ExitBtnText;
		GuiText * ShutdownBtnText;
		GuiText * PlayerText[4];

		GuiButton * TopBtn;
		GuiButton * BottomBtn;
		GuiButton * CloseBtn;
		GuiButton * ExitBtn;
		GuiButton * ShutdownBtn;
		GuiButton * WiimoteBtn;
		GuiButton * BatteryBtn[4];

		GuiTrigger * trigA;
		GuiTrigger * trigB;
		GuiTrigger * trigHome;

		GuiSound * ButtonOverSnd;
		GuiSound * ButtonClickSnd;
		GuiSound * homein;
		GuiSound * homeout;
};

#endif // _HOMEMENU_H
