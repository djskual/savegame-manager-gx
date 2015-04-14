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
#ifndef __LISTFILEDEVICEBROWSER_HPP_
#define __LISTFILEDEVICEBROWSER_HPP_

#include <vector>

#include "GuiFileBrowser.hpp"
#include "../Controls/Scrollbar.hpp"

//!Display a list of files
class ListFileDeviceBrowser : public GuiFileBrowser
{
	public:
		ListFileDeviceBrowser(Browser * filebrowser, GuiImageData * browserBg, int w, int h);
		virtual ~ListFileDeviceBrowser();
		void ResetState();
		void SetSelected(int i);
		void SetTriggerUpdate(bool t) { triggerupdate = t; };
		void TriggerUpdate() { listChanged = true; };
		void Draw();
		void Update(GuiTrigger * t);
		
	protected:
		void AddButton();
		void SetButton(int i, std::string name, bool dir, bool save, bool databin, bool mii, bool enable);
		void RemoveButton(int i);
		void FoundSaveType();
		GuiImage * GetTypeIcon();
		GuiImage * GetSaveIcon();
		GuiImage * GetDataBinIcon();
		GuiImage * GetIconFromExt(std::string fileext, bool dir, bool save, bool databin, bool mii);
		std::string GetTypeFromExt(std::string fileext, bool dir, bool save, bool databin, bool mii);
		void SetSelectedInfos();
		
		int selectedItem;
		int oldSelectedItem;
		int oldPageIndex;
		int numEntries;
		u64 databintid;
		int type;
		bool listChanged;
		bool triggerupdate;

		std::vector<GuiText *> fileBtnText;
		std::vector<GuiText *> fileBtnTextOver;
		std::vector<GuiImage *> fileSelectionImg;
		std::vector<GuiImage *> fileBtnIcon;
		std::vector<GuiButton *> fileBtn;

		Scrollbar * scrollbar;

		GuiImage * bgFileSelectionImg;
		GuiImage * browserSeparatorImg;
		
		GuiWindow * infosWindow;
		GuiImage * infoIconImg;
		GuiImage * infoTypeImg;
		GuiText * infoNameTxt;
		GuiText * infoTypeTxt;
		GuiText * infoSizeTxt;
		GuiText * pathTxt;
		GuiText * infoPathTxt;
		
		GuiImageData * bgFileSelectionEntry;
		GuiImageData * browserSeparator;
		GuiImageData * fileCfg;
		GuiImageData * fileDataBin;
		GuiImageData * fileDefault;
		GuiImageData * fileExe;
		GuiImageData * fileFolder;
		GuiImageData * fileFont;
		GuiImageData * fileImg;
		GuiImageData * fileLang;
		GuiImageData * fileMii;
		GuiImageData * fileSave;
		GuiImageData * fileSound;
		GuiImageData * fileTheme;
		GuiImageData * fileTxt;
		GuiImageData * fileXml;
		
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
