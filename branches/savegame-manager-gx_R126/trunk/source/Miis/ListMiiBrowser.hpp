/***************************************************************************
 * Copyright (C) 2010
 * by Dimok
 * modified by Dj_Skual
 * for SaveGame Manager Gx 2011
 *
 * Original ListBrowser by Tantric (C) 2009
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
 * for WiiXplorer 2010
 ***************************************************************************/
#ifndef __LISTMIIBROWSER_HPP_
#define __LISTMIIBROWSER_HPP_

#include <vector>

#include "../Files/GuiFileBrowser.hpp"
#include "../Controls/Scrollbar.hpp"

//!Display a list of files
class ListMiiBrowser : public GuiFileBrowser
{
	public:
		ListMiiBrowser(Browser * filebrowser, GuiImageData * browserBg, int w, int h);
		~ListMiiBrowser();
		void ResetState();
		void SetSelected(int i);
		void SetTriggerUpdate(bool t) { triggerupdate = t; };
		void TriggerUpdate() { listChanged = true; };
		void Draw();
		void Update(GuiTrigger * t);
		
	protected:
		void AddButton();
		void SetButton(int i, std::string name, bool enable);
		void RemoveButton(int i);
		void SetSelectedInfos();
		std::string GetDate();
		GXColor GetFavColor();
		
		int selectedItem;
		int oldSelectedItem;
		int oldPageIndex;
		int numEntries;
		bool listChanged;
		bool triggerupdate;

		std::vector<GuiText *> miiBtnText;
		std::vector<GuiText *> miiBtnTextOver;
		std::vector<GuiImage *> miiSelectionImg;
		std::vector<GuiImage *> miiBtnIcon;
		std::vector<GuiButton *> miiBtn;

		Scrollbar * scrollbar;

		GuiImage * bgMiiSelectionImg;
		GuiImage * browserSeparatorImg;
		
		GuiWindow * infosWindow;
		GuiImage * infoIconImg;
		GuiText * infoNameTxt;
		GuiText * infoCreatorTxt;
		GuiText * infoDateTxt;
		GuiImage * infoGenderImg;
		GuiImage * infoColorImg;
		GuiText * pathTxt;
		GuiText * infoPathTxt;
		
		GuiImageData * bgMiiSelectionEntry;
		GuiImageData * browserSeparator;
		GuiImageData * fileMii;
		GuiImageData * maleImgData;
		GuiImageData * femaleImgData;
		
		SimpleGuiTrigger * trigA;
};

#endif
