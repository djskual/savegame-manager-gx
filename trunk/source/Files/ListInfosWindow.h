/****************************************************************************
 * Copyright (C) 2012 Dj_Skual
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
#ifndef _LISTINFOSWINDOW_H
#define _LISTINFOSWINDOW_H

#include "../GUI/gui.h"

class InfosWindow : public GuiFrame
{
	public:
		InfosWindow(int width, int height);
		virtual ~InfosWindow();
		
		void SetInfos(std::wstring ItemDisplayName, std::string CurrentSelectedFilepath, bool Dir, bool Save, bool Databin, bool Mii);
		
	private:
		void FoundSaveType();
		GuiImageData * GetTypeIcon();
		//void GetSaveIcon();
		//void GetDataBinIcon();
		GuiImage * GetIconFromExt();
		std::string GetTypeFromExt();
		
		bool dir;
		bool save;
		bool databin;
		bool mii;
		int fileType;
		std::string itemName;
		std::string fileext;
		
		GuiImage * infoIconImg;
		GuiImage * infoTypeImg;
		GuiText * infoNameTxt;
		GuiText * infoTypeTxt;
		GuiText * infoSizeTxt;
		GuiText * pathTxt;
		GuiText * infoPathTxt;
		
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
};

#endif
