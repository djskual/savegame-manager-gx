/***************************************************************************
 * Copyright (C) 2009
 * by Dimok for WiiXplorer
 * modified by dj_skual
 * for SaveGame Manager GX 2012
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
 * DeviceMenu.h
 *
 ***************************************************************************/
 #ifndef __DEVICE_MENU_H_
 #define __DEVICE_MENU_H_

#include <gctypes.h>
#include <unistd.h>

#include "../LibWiiGui/gui.h"
#include "../DeviceControls/DeviceHandler.hpp"

#define MII		20
#define WII		30
#define EMU		40
#define EMU_MII	50

class DeviceMenu : public GuiWindow, public sigslot::has_slots<>
{
	public:
		DeviceMenu(int x, int y);
		~DeviceMenu();
		int GetChoice();
		
	private:
		void OnButtonClick(GuiButton *sender, int pointer, const POINT &p);
		
		int deviceCount;
		int deviceSelection[MAXDEVICES];
		int tile;
		int choice;
		
		GuiImage * centerImg;
		GuiImage * leftImg;
		GuiImage * rightImg;
		GuiImage * deviceImgs[MAXDEVICES];
		GuiImage * deviceImgOver[MAXDEVICES];
		
		GuiImageData * device_choose_right_Data;
		GuiImageData * device_choose_left_Data;
		GuiImageData * device_choose_center_Data;
		GuiImageData * menu_select;
		GuiImageData * sd_ImgData;
		GuiImageData * usb_ImgData;
		GuiImageData * wii_ImgData;
		GuiImageData * mii_ImgData;
		GuiImageData * emu_ImgData;
		GuiImageData * emu_mii_ImgData;
		
		GuiText * deviceText[MAXDEVICES];
		
		GuiButton * NoBtn;
		GuiButton * deviceBtn[MAXDEVICES];
		
		SimpleGuiTrigger * trigA;
		GuiTrigger * trigB;
		GuiTrigger * trigHome;
};

#endif //__DEVICE_MENU_H_
