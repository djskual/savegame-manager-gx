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

#include "../GUI/gui.h"
#include "../GUI/PictureButton.h"

class TaskBar: public GuiFrame, public sigslot::has_slots<>
{
	public:
		static TaskBar * Instance() { if(!instance) instance = new TaskBar(); return instance; }
		static void DestroyInstance() { delete instance; instance = NULL; }
		
		void show();
		void hide();
		
		TaskBar *GetInstance() { return instance; }
		
		void Draw();
		void Update(GuiTrigger * t);
		void ResetSearchBtn();
		
		enum
		{
			DEVICE = 0,
			MODE,
			LOCK,
			SEARCH,
		};
		
		sigslot::signal2<TaskBar*, int> TaskbarClicked;
		sigslot::signal2<TaskBar*, int> TaskbarStateChanged;
		
	protected:
		TaskBar();
		virtual ~TaskBar();
		
		static TaskBar * instance;
		
		void SetModeButton();
		void SetLockButton();
		
		void OnDeviceButtonClick(GuiButton *sender, int pointer, const POINT &p);
		void OnModeButtonClick(GuiButton *sender, int pointer, const POINT &p);
		void OnLockButtonClick(GuiButton *sender, int pointer, const POINT &p);
		void OnSearchButtonClick(GuiButton *sender, int pointer, const POINT &p);
		void OnDeviceChange(int item);
		void OnSearchWindowStateChange(bool filterOn, bool displayOn);
		
		PictureButton * Buttons[4];
		
		GuiTooltip * Tooltip[4];
		
		GuiImage * sdIconImg;
		GuiImage * sdIconImg_gray;
		GuiImage * usbIconImg;
		GuiImage * usbIconImg_gray;
		GuiImage * wiiIconImg;
		GuiImage * wiiIconImg_gray;
		GuiImage * miiIconImg;
		GuiImage * miiIconImg_gray;
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
		
		GuiImageData * sdIcon;
		GuiImageData * sdIcon_gray;
		GuiImageData * usbIcon;
		GuiImageData * usbIcon_gray;
		GuiImageData * wiiIcon;
		GuiImageData * wiiIcon_gray;
		GuiImageData * miiIcon;
		GuiImageData * miiIcon_gray;
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
		
		GuiSound * BtnSoundClick;
		GuiSound * BtnSoundOver;
		
		GuiTrigger * trigA;
};

#endif
