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

#include <vector>

#include "../LibWiiGui/gui.h"
#include "../LibWiiGui/gui_checkbox.hpp"
#include "../Controls/Scrollbar.hpp"

class LanguagesSelectBrowser : public GuiWindow
{
	public:
		LanguagesSelectBrowser();
		~LanguagesSelectBrowser();
		void Setup();
		void SetSelected(int i);
		int GetResult();
		
	protected:
		void AddButton();
		void RemoveButton(int i);
		void SetButton(int i, const char * filename, bool needed, bool enable);
		void Draw();
		void Update(GuiTrigger * t);
		
		int numEntries;
		int selectedItem;
		int pageIndex;
		bool listChanged;
		int result;
		
		std::vector<GuiText *> ListBtnText;
		std::vector<GuiText *> ListBtnTextOver;
		std::vector<GuiImage *> ListSelectionImg;
		std::vector<GuiButton *> ListBtn;
		std::vector<GuiCheckbox *> ListBtnCheckbox;
		
		GuiImage * WindowImg;
		GuiImage * BrowserImg;
		GuiImage * ValidImg;
		GuiImage * ValidOverImg;
		GuiImage * CloseImg;
		GuiImage * CloseOverImg;
		GuiImage * ButtonBoxImgClose;
		GuiImage * ButtonBoxImgVal;
		
		GuiImageData * WindowImgData;
		GuiImageData * BrowserImgData;
		GuiImageData * ValidImgData;
		GuiImageData * ValidOverImgData;
		GuiImageData * CloseImgData;
		GuiImageData * CloseOverImgData;
		GuiImageData * ButtonBoxImgData;
		GuiImageData * ListSelectionImgData;
		
		GuiText * TitleText;
		
		Scrollbar * ScrollBar;
		
		GuiButton * ValidBtn;
		GuiButton * CloseBtn;
		
		SimpleGuiTrigger * trigA;
		GuiTrigger * trigHome;
		GuiTrigger * trigB;
};

int languagesSelectBrowser();

#endif
