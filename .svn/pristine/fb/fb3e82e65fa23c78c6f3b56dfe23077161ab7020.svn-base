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
#ifndef __LISTSAVEBROWSER_HPP_
#define __LISTSAVEBROWSER_HPP_

#include <vector>

#include "../Files/GuiFileBrowser.hpp"
#include "../Controls/Scrollbar.hpp"

//!Display a list of files
class ListSaveBrowser : public GuiFileBrowser
{
	public:
		ListSaveBrowser(Browser * filebrowser, GuiImageData * browserBg, int w, int h);
		~ListSaveBrowser();
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
		void FoundSaveType();
		GuiImage * GetTypeIcon();
		GuiImage * GetSaveIcon();
		void SetSelectedInfos();
		std::string GetCurrentPath();
		
		int selectedItem;
		int oldSelectedItem;
		int oldPageIndex;
		int numEntries;
		int type;
		bool listChanged;
		bool triggerupdate;

		std::vector<GuiText *> saveBtnText;
		std::vector<GuiText *> saveBtnTextOver;
		std::vector<GuiImage *> saveSelectionImg;
		std::vector<GuiImage *> saveBtnIcon;
		std::vector<GuiButton *> saveBtn;

		Scrollbar * scrollbar;

		GuiImage * bgSaveSelectionImg;
		GuiImage * browserSeparatorImg;
		
		GuiWindow * infosWindow;
		GuiImage * infoIconImg;
		GuiImage * infoTypeImg;
		GuiText * infoNameTxt;
		GuiText * infoSubnameTxt;
		GuiText * infoIDTxt;
		GuiText * infoSizeTxt;
		GuiText * pathTxt;
		GuiText * infoPathTxt;
		
		GuiImageData * bgSaveSelectionEntry;
		GuiImageData * browserSeparator;
		GuiImageData * fileSave;
		
		GuiImageData * type_Wii;
		GuiImageData * type_Wiiware;
		GuiImageData * type_Gamecube;
		GuiImageData * type_VC_NES;
		GuiImageData * type_VC_SNES;
		GuiImageData * type_VC_N64;
		GuiImageData * type_VC_SMS;
		GuiImageData * type_VC_MD;
		GuiImageData * type_VC_PCE;
		GuiImageData * type_VC_Neogeo;
		GuiImageData * type_VC_Arcade;
		GuiImageData * type_VC_C64;
		GuiImageData * type_Channel;
		GuiImageData * type_Unknow;
		
		SimpleGuiTrigger * trigA;
};

#endif
