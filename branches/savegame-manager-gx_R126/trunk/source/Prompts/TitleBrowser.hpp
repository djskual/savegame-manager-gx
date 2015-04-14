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
 * TitleBrowser.hpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#ifndef _TITLEBROWSER_HPP_
#define _TITLEBROWSER_HPP_

#include <vector>

#include "../LibWiiGui/gui.h"
#include "../Controls/Scrollbar.hpp"
#include "../HomebrewBoot/CTitleList.hpp"

class TitleBrowser : public GuiWindow
{
	public:
		TitleBrowser();
		virtual ~TitleBrowser();
		void Setup();
		void SetSelected(int i);
		int GetResult();
		
	protected:
		void SetCol2();
		void AddButton();
		void RemoveButton(int i);
		void SetButton(int i, std::string id, std::string name, bool enable);
		void Draw();
		void Update(GuiTrigger * t);
		
		int col2;
		int numEntries;
		int selectedItem;
		int pageIndex;
		bool listChanged;
		bool col2Setted;
		int result;
		
		std::vector<GuiText *> ListBtnID;
		std::vector<GuiText *> ListBtnName;
		std::vector<GuiText *> ListBtnNameOver;
		std::vector<GuiImage *> ListSelectionImg;
		std::vector<GuiButton *> ListBtn;
		
		GuiImage * WindowImg;
		GuiImage * BrowserImg;
		GuiImage * CloseImg;
		GuiImage * CloseOverImg;
		GuiImage * ButtonBoxImgClose;
		
		GuiImageData * WindowImgData;
		GuiImageData * BrowserImgData;
		GuiImageData * CloseImgData;
		GuiImageData * CloseOverImgData;
		GuiImageData * ButtonBoxImgData;
		GuiImageData * ListSelectionImgData;
		
		GuiText * TitleText;
		
		Scrollbar * ScrollBar;
		
		GuiButton * CloseBtn;
		
		SimpleGuiTrigger * trigA;
		GuiTrigger * trigHome;
		GuiTrigger * trigB;
		
		CTitleList * TitleList;
};

int titleBrowser();

#endif
