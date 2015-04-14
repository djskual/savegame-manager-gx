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
 * MiiWindow.hpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#ifndef _MIIWINDOW_HPP_
#define _MIIWINDOW_HPP_

#include "MiiInfos.hpp"
#include "ManageButtons.hpp"
#include "MiiManageTools.hpp"

class MiiWindow : public GuiWindow
{
	public:
		MiiWindow(int Selected);
		virtual ~MiiWindow();
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
		
		enum
		{
			EXTRACT_MII = 1,
			DELETE_NAND_MII,
			INSTALL_EMU_MII,
		};
		
		int returnVal;
		int miiSelected;
		int action;
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
		ManageButtons * pathButtons;
		ManageButtons * confirmButtons;
		ManageButtons * succesButtons;
		ManageButtons * errorButtons;
		ManageButtons * cancelButtons;
		ManageButtons * overWriteButtons;
		
		MiiManageTools * Tools;
};

#endif
