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
 * LanguagesSelectBrowser
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#ifndef _LANGUAGESSELECTBROWSER_HPP_
#define _LANGUAGESSELECTBROWSER_HPP_

#include "../Gui/gui.h"
#include "../Gui/gui_checkbox.hpp"
#include "../GUI/PictureButton.h"
#include "../Controls/Scrollbar.hpp"

#include <vector>

class LanguagesSelectBrowser : public GuiFrame, public sigslot::has_slots<>
{
	public:
		LanguagesSelectBrowser();
		virtual ~LanguagesSelectBrowser();
		void Setup();
		int GetResult() { return result; }
		
	protected:
		void AddButton();
		void RemoveButton(int i);
		void SetButton(int i, const char * filename, bool needed, bool enable);
		void Draw();
		void Update(GuiTrigger * t);
		
		void OnButtonClick(GuiButton *sender, int pointer, const POINT &p);
		void OnListClick(GuiButton *sender, int pointer, const POINT &p);
		void OnListChange(int selItem, int selIndex);
		
		int numEntries;
		int selectedItem;
		int pageIndex;
		int result;
		
		std::vector<GuiText *> ListBtnText;
		std::vector<GuiText *> ListBtnTextOver;
		std::vector<GuiImage *> ListSelectionImg;
		std::vector<GuiButton *> ListBtn;
		std::vector<GuiCheckbox *> ListBtnCheckbox;
		
		GuiImage * WindowImg;
		GuiImage * BrowserImg;
		GuiImage * ButtonBoxImgClose;
		GuiImage * ButtonBoxImgVal;
		
		GuiImageData * WindowImgData;
		GuiImageData * BrowserImgData;
		GuiImageData * ButtonBoxImgData;
		GuiImageData * ListSelectionImgData;
		
		GuiText * TitleText;
		
		Scrollbar * ScrollBar;
		
		PictureButton * ValidBtn;
		PictureButton * CloseBtn;
		
		GuiSound * btnSoundClick;
		GuiSound * btnSoundOver;
		
		GuiTrigger trigA;
};

int languagesSelectBrowser();

#endif
