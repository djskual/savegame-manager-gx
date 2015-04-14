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
 * WiiSaveBrowser.hpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#ifndef _WIISAVEBROWSER_HPP_
#define _WIISAVEBROWSER_HPP_

#include "../../LibWiiGui/gui.h"
#include "../../Text/Text.hpp"
#include "WiiSave_List.h"

class WiiSaveBrowser : public GuiWindow
{
	public:
		WiiSaveBrowser(WiiSave_List * savelist);
		virtual ~WiiSaveBrowser();
		int Show();
		int GetSelectedSave() { return saveSelected; };
		
	protected:
		int MainLoop();
		void SetWindowEffect(int change);
		void ChangeSave(int Change);
		
		int returnVal;
		int saveSelected;
		
		GuiTrigger * trigA;
		GuiTrigger * trigB;
		GuiTrigger * trigL;
		GuiTrigger * trigR;
		GuiTrigger * trigU;
		GuiTrigger * trigD;
		GuiTrigger * trigPlus;
		GuiTrigger * trigMinus;
		
		GuiImageData * imgDialogBox;
		GuiImageData * imgLine;
		GuiImageData * imgButtonBox;
		GuiImageData * imgClose;
		GuiImageData * imgCloseOver;
		GuiImageData * imgDownload;
		GuiImageData * imgDownloadOver;
		GuiImageData * imgNext;
		GuiImageData * imgNextOver;
		GuiImageData * imgPrevious;
		GuiImageData * imgPreviousOver;
		
		GuiImage * dialogBoxImg;
		GuiImage * lineImg;
		GuiImage * closeBoxImg;
		GuiImage * btnCloseImg;
		GuiImage * btnCloseImgOver;
		GuiImage * downloadBoxImg;
		GuiImage * btnDownloadImg;
		GuiImage * btnDownloadImgOver;
		GuiImage * nextBoxImg;
		GuiImage * btnNextImg;
		GuiImage * btnNextImgOver;
		GuiImage * previousBoxImg;
		GuiImage * btnPreviousImg;
		GuiImage * btnPreviousImgOver;
		
		GuiText * nameTxt;
		GuiText * numberTxt;
		Text * synopsisTxt;
		
		GuiButton * btnClose;
		GuiButton * btnDownload;
		GuiButton * btnNext;
		GuiButton * btnPrevious;
		GuiButton * btnUp;
		GuiButton * btnDown;
	
		WiiSave_List * SaveList;
};

#endif
