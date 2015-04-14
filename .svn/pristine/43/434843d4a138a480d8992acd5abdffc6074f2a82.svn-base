/****************************************************************************
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
 * SaveWindow.hpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#ifndef _SAVEWINDOW_HPP_
#define _SAVEWINDOW_HPP_

#include <string>

#include "../LibWiiGui/gui_banner.h"
#include "ManageButtons.hpp"
#include "SaveManageTools.hpp"

enum
{
	EXTRACT_SAVE = 1,
	COMPRESS_SAVE,
	INSTALL_EMU_SAVE,
};

class SaveWindow : public GuiWindow
{
	public:
		SaveWindow(int Selected);
		virtual ~SaveWindow();
		int Show();
		int GetSelectedSave() { return saveSelected; };
		
	protected:
		int MainLoop();
		void LoadBannerImage(u64 tid);
		void SetBoxes();
		void SetWindowEffect(int direction, int in_out);
		void ResetButtons();
		void ChangeSave(int EffectDirection);
		void SetButtonsClickable(bool clickable);
		
		int action;
		int returnVal;
		int saveSelected;
		
		GuiTrigger * trigA;
		GuiTrigger * trigB;
		GuiTrigger * trigL;
		GuiTrigger * trigR;
		GuiTrigger * trigPlus;
		GuiTrigger * trigMinus;
		GuiTrigger * trigHome;
		
		GuiImageData * imgDialogBox;
		GuiImageData * imgButtonBox;
		GuiImageData * imgClose;
		GuiImageData * imgCloseOver;
		GuiImageData * imgWifi;
		GuiImageData * imgWifiOver;
		GuiImageData * imgLeft;
		GuiImageData * imgRight;
		
		GuiImage * dialogBoxImg;
		GuiImage * buttonBoxImg;
		GuiImage * btnCloseBoxImg;
		GuiImage * btnCloseImg;
		GuiImage * btnCloseImgOver;
		GuiImage * btnWifiImg;
		GuiImage * btnWifiImgOver;
		GuiImage * btnLeftImg;
		GuiImage * btnRightImg;
		
		GuiText * nameTxt;
		GuiText * subnameTxt;
		
		GuiButton * btnClose;
		GuiButton * btnWifi;
		GuiButton * btnLeft;
		GuiButton * btnRight;
		
		GuiSaveBanner * saveBanner;
		
		GuiBlockBox * idBox;
		GuiBlockBox * blockBox;
		
		ManageButtons * actionButtons;
		ManageButtons * pathButtons;
		ManageButtons * confirmButtons;
		ManageButtons * succesButtons;
		ManageButtons * errorButtons;
		ManageButtons * cancelButtons;
		
		SaveManageTools * Tools;
};

#endif
