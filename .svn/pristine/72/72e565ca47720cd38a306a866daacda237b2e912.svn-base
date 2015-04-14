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
 * FileMiiWindow.hpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#ifndef _FILEMIIWINDOW_HPP_
#define _FILEMIIWINDOW_HPP_

#include <string>

#include "MiiInfos.hpp"
#include "ManageButtons.hpp"
#include "MiiManageTools.hpp"

enum
{
	INSTALL_MII = 1,
	DELETE_MII,
	MOVE_MII,
	COPY_MII,
};

enum
{
	NAND_MII_INSTALL = 1,
	EMU_MII_INSTALL,
};


class FileMiiWindow : public GuiWindow
{
	public:
		FileMiiWindow(int Selected);
		virtual ~FileMiiWindow();
		int Show();
		int GetSelectedMii() { return miiSelected; };
		bool RefreshNeeded() { return refreshNeeded; };
		
	protected:
		int MainLoop();
		void SetWindowEffect(int direction, int in_out);
		void ResetButtons();
		void RemoveEntry();
		void ChangeMii(int EffectDirection);
		void SetButtonsClickable(bool clickable);
		
		int returnVal;
		int miiSelected;
		int action;
		int install_type;
		bool refreshNeeded;
		
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
		GuiImageData * imgLeft;
		GuiImageData * imgRight;
		
		GuiImage * dialogBoxImg;
		GuiImage * btnCloseBoxImg;
		GuiImage * btnCloseImg;
		GuiImage * btnCloseImgOver;
		GuiImage * btnLeftImg;
		GuiImage * btnRightImg;
		
		GuiText * nameTxt;
		
		GuiButton * btnClose;
		GuiButton * btnLeft;
		GuiButton * btnRight;
		
		MiiInfos * miiInfos;
		
		ManageButtons * actionsButtons;
		ManageButtons * installButtons;
		ManageButtons * confirmButtons;
		ManageButtons * succesButtons;
		ManageButtons * errorButtons;
		ManageButtons * cancelButtons;
		ManageButtons * overWriteButtons;
		
		MiiManageTools * Tools;
};

#endif
