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
 * AllWindow.hpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#ifndef _ALLWINDOW_HPP_
#define _ALLWINDOW_HPP_

#include <string>

#include "../LibWiiGui/gui_banner.h"
#include "AllButtons.hpp"
#include "SaveAllTools.hpp"
#include "MiiAllTools.hpp"

enum
{
	WII_NAND = 1,
	DEVICES,
	EMU_NAND,
};

enum
{
	SAVES = 1,
	MIIS,
};

enum
{
	EXTRACT = 1,
	INSTALL,
	DELETE,
};

enum
{
	EXTRACT_NAND_SAVES_TO_DEV = 1,
	INSTALL_NAND_SAVES_TO_EMU,
	INSTALL_DEV_SAVES_TO_NAND,
	INSTALL_DEV_SAVES_TO_EMU,
	EXTRACT_EMU_SAVES_TO_DEV,
	INSTALL_EMU_SAVES_TO_NAND,
	DELETE_DEV_SAVES,
	DELETE_EMU_SAVES,
	EXTRACT_NAND_MIIS_TO_DEV,
	INSTALL_NAND_MIIS_TO_EMU,
	INSTALL_DEV_MIIS_TO_NAND,
	INSTALL_DEV_MIIS_TO_EMU,
	INSTALL_EMU_MIIS_TO_NAND,
	EXTRACT_EMU_MIIS_TO_DEV,
	DELETE_NAND_MIIS,
	DELETE_DEV_MIIS,
	DELETE_EMU_MIIS,
};

class AllWindow : public GuiWindow
{
	public:
		AllWindow();
		virtual ~AllWindow();
		int Show();
		int RefreshNeeded() { return refreshNeeded; };
		
	protected:
		int MainLoop();
		void ResetButtons();
		void SetActionButton();
		void SetFinalAction();
		
		int returnVal;
		int source;
		int type;
		int action;
		int final_action;
		
		bool exit;
		int refreshNeeded;
		
		GuiTrigger * trigA;
		GuiTrigger * trigB;
		GuiTrigger * trigHome;
		
		GuiImageData * imgDialogBox;
		GuiImageData * imgButtonBox;
		GuiImageData * imgClose;
		GuiImageData * imgCloseOver;
		
		GuiImage * dialogBoxImg;
		GuiImage * btnCloseBoxImg;
		GuiImage * btnCloseImg;
		GuiImage * btnCloseImgOver;
		
		GuiText * titleTxt;
		
		GuiButton * btnClose;
		
		AllButtons * sourceButtons;
		AllButtons * typeButtons;
		AllButtons * actionButtons;
		AllButtons * pathButtons;
		AllButtons * confirmButtons;
		AllButtons * succesButtons;
		AllButtons * cancelButtons;
		AllButtons * errorButtons;
		
		SaveAllTools * saveTools;
		MiiAllTools * miiTools;
};

#endif
