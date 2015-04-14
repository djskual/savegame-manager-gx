 /***************************************************************************
 * Copyright (C) 2011
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
 *
 * TaskBar.hpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#ifndef _TASKBAR_HPP_
#define _TASKBAR_HPP_

#include "../LibWiiGui/gui.h"

enum
{
	TASKBAR_NONE = 0,
	TASKBAR_DEVICE,
	TASKBAR_SD,
	TASKBAR_USB,
	TASKBAR_WII,
	TASKBAR_EMU,
	TASKBAR_MII,
	TASKBAR_EMU_MII,
	TASKBAR_MODE,
	TASKBAR_ICON,
	TASKBAR_LIST,
	TASKBAR_LOCK,
	TASKBAR_SEARCH,
};

//!Display a prompt with proposal for all saves manage
class TaskBar: public GuiElement
{
	public:
		TaskBar(const char * UNUSED);
		virtual ~TaskBar();
		
		int GetClicked();
		int GetSelected();
		void SetDeviceImage(int type);
		void SetModeButton();
		void SetLockButton();
		void SetSearchEffect(bool searchwindow);
		void StopSearchEffect();
		void ResetState();
		void Draw();
		void Update(GuiTrigger * t);
		
	protected:
		int DeviceMenu();
		
		/** Window **/
		GuiWindow * window;
		
		//* Buttons **/
		GuiButton * deviceBtn;
		GuiButton * modeBtn;
		GuiButton * lockBtn;
		GuiButton * searchBtn;
		
		/** Images **/
		GuiImage * sdIconImg;
		GuiImage * sdIconImg_gray;
		GuiImage * usbIconImg;
		GuiImage * usbIconImg_gray;
		GuiImage * wiiIconImg;
		GuiImage * wiiIconImg_gray;
		GuiImage * emuIconImg;
		GuiImage * emuIconImg_gray;
		GuiImage * miiIconImg;
		GuiImage * miiIconImg_gray;
		GuiImage * emuMiiIconImg;
		GuiImage * emuMiiIconImg_gray;
		GuiImage * arrangeIconImg;
		GuiImage * arrangeIconImg_gray;
		GuiImage * arrangeListImg;
		GuiImage * arrangeListImg_gray;
		GuiImage * lockedIconImg;
		GuiImage * lockedIconImg_gray;
		GuiImage * unlockedIconImg;
		GuiImage * unlockedIconImg_gray;
		GuiImage * searchIconImg;
		GuiImage * searchIconImg_gray;
		
		/** ImageDatas **/
		GuiImageData * sdIcon;
		GuiImageData * sdIcon_gray;
		GuiImageData * usbIcon;
		GuiImageData * usbIcon_gray;
		GuiImageData * wiiIcon;
		GuiImageData * wiiIcon_gray;
		GuiImageData * emuIcon;
		GuiImageData * emuIcon_gray;
		GuiImageData * miiIcon;
		GuiImageData * miiIcon_gray;
		GuiImageData * emuMiiIcon;
		GuiImageData * emuMiiIcon_gray;
		GuiImageData * arrangeIcon;
		GuiImageData * arrangeIcon_gray;
		GuiImageData * arrangeList;
		GuiImageData * arrangeList_gray;
		GuiImageData * lockedIcon;
		GuiImageData * lockedIcon_gray;
		GuiImageData * unlockedIcon;
		GuiImageData * unlockedIcon_gray;
		GuiImageData * searchIcon;
		GuiImageData * searchIcon_gray;
		
		/** GuiTooltip **/
		GuiTooltip * deviceTT;
		GuiTooltip * modeTT;
		GuiTooltip * lockTT;
		GuiTooltip * searchTT;
		
		GuiTrigger * trigA;
};

#endif
