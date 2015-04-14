/***************************************************************************
 * Copyright (C) 2010
 * by Dimok
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
#ifndef __CUSTOMFILEBROWSER_HPP_
#define __CUSTOMFILEBROWSER_HPP_

#include "GuiFileBrowser.hpp"
#include "Controls/Scrollbar.hpp"

class CustomFileBrowser : public GuiFileBrowser, public sigslot::has_slots<>
{
	public:
		CustomFileBrowser(Browser * filebrowser, int w, int h);
		virtual ~CustomFileBrowser();
		void ResetState();
		void SetSelected(int i);
		void Refresh() { if(browser) browser->Refresh(); OnListChange(0, 0); };
		void Draw();
		void Update(GuiTrigger * t);
		
	protected:
		void OnListChange(int selItem, int selIndex);
		void AddButton();
		void SetButton(int i, std::wstring name, bool dir, bool enable);
		void RemoveButton(int i);
		GuiImage * GetIconFromExt(std::string fileext, bool dir);
		void OnClicked(GuiButton *sender, int pointer, const POINT &p);
		int selectedItem;
		int numEntries;
		int PageSize;
		
		std::vector<GuiText *> fileBtnText;
		std::vector<GuiText *> fileBtnTextOver;
		std::vector<GuiImage *> fileSelectionImg;
		std::vector<GuiImage *> fileBtnIcon;
		std::vector<GuiButton *> fileBtn;
		
		Scrollbar * scrollbar;
		
		GuiImageData * bgFileSelectionEntry;
		GuiImageData * fileFolder;
		GuiImageData * fileFont;
		GuiImageData * fileLang;
		
		GuiSound * btnSoundOver;
		GuiSound * btnSoundClick;
		
		GuiTrigger * trigA;
};

#endif
