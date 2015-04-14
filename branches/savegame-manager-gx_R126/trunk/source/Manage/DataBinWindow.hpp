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
 * DataBinWindow.hpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#ifndef _DATABINWINDOW_HPP_
#define _DATABINWINDOW_HPP_

#include <string>

#include "SaveManageTools.hpp"
#include "ManageButtons.hpp"
#include "../LibWiiGui/gui_banner.h"
#include "../Saves/DataBin/SaveDataBin.h"

enum
{
	DECOMPRESS_DATABIN = 1,
	INSTALL_DATABIN,
	DELETE_DATABIN,
	COPY_DATABIN,
};

enum
{
	NAND_DATABIN = 1,
	EMU_DATABIN,
};

class DataBinWindow : public GuiWindow
{
	public:
		DataBinWindow(int Selected);
		virtual ~DataBinWindow();
		int Show();
		int GetSelectedDataBin() { return dataBinSelected; };
		bool RefreshNeeded() { return refreshNeeded; };
		
	protected:
		int MainLoop();
		void LoadBannerImage(u64 tid);
		void SetBoxes();
		void LoadDataBin();
		void SetButtonsClickable(bool clickable);
		
		int returnVal;
		int dataBinSelected;
		int action;
		int install_type;
		bool refreshNeeded;
		bool not_installed;
		bool display_install;
		std::string filePath;
		std::string DataBinPath;
		
		GuiTrigger * trigA;
		GuiTrigger * trigB;
		GuiTrigger * trigHome;
		
		GuiImageData * imgDialogBox;
		GuiImageData * imgButtonBox;
		GuiImageData * imgClose;
		GuiImageData * imgCloseOver;
		GuiImageData * imgWifi;
		GuiImageData * imgWifiOver;
		
		GuiImage * dialogBoxImg;
		GuiImage * buttonBoxImg;
		GuiImage * btnCloseBoxImg;
		GuiImage * btnCloseImg;
		GuiImage * btnCloseImgOver;
		GuiImage * btnWifiImg;
		GuiImage * btnWifiImgOver;
		
		GuiText * nameTxt;
		GuiText * subnameTxt;
		GuiText * warningTxt;
		
		GuiButton * btnClose;
		GuiButton * btnWifi;
		
		SaveDataBin * save;
		GuiSaveBanner * dataBinBanner;
		
		GuiBlockBox * idBox;
		GuiBlockBox * blockBox;
		
		ManageButtons * actionsButtons;
		ManageButtons * installButtons;
		ManageButtons * pathButtons;
		ManageButtons * confirmButtons;
		ManageButtons * succesButtons;
		ManageButtons * errorButtons;
		ManageButtons * cancelButtons;
};

#endif
