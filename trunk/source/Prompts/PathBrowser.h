/****************************************************************************
 * Copyright (C) 2009-2012 Dimok
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
#ifndef PATHBROWSER_H_
#define PATHBROWSER_H_

#include "../GUI/gui.h"
#include "../GUI/PictureButton.h"
#include "../Files/custombrowser.h"
#include "../Files/GuiFileBrowser.hpp"

class PathBrowser : public GuiFrame, public sigslot::has_slots<>
{
	public:
		PathBrowser(const std::string &in_outPath, u8 Filter = FILTER_PATH);
		virtual ~PathBrowser();
		
		sigslot::signal4<PathBrowser*, u8, bool, const std::string &> ButtonClick;
		
	protected:
		void SetDeviceImage();
		
		void OnButtonClick(GuiButton *, int, const POINT &);
		void OnFolderButtonClick(GuiButton *, int, const POINT &);
		void OnDevButtonClick(GuiButton *, int, const POINT &);
		void OnWifiButtonClick(GuiButton *, int, const POINT &);
		void OnBackInDirClick(GuiButton *, int, const POINT &);
		void OnBrowserChanges(int index);
		
		u8 filter;
		int currentDevice;
		
		CustomBrowser *fileBrowser;
		Browser * curBrowser;
		GuiFileBrowser * guiBrowser;
		
		GuiSound * btnSoundClick;
		GuiSound * btnSoundOver;
		
		GuiImageData * browserImgData;
		GuiImageData * sdImgData;
		GuiImageData * usbImgData;
		GuiImageData * folderImgData;
		GuiImageData * btnBoxImgData;
		
		GuiImage * browserImg;
		GuiImage * deviceImg;
		GuiImage * btnCloseBoxImg;
		GuiImage * btnWifiValBoxImg;
		GuiImage * folderImg;
		
		GuiText * adressText;
		
		PictureButton * devSwitchBtn;
		PictureButton * closeBtn;
		PictureButton * wifiBtn;
		PictureButton * validBtn;
		GuiButton * folderBtn;
		GuiButton * backInDirBtn;
		
		GuiTrigger trigA;
		GuiTrigger trigB;
};

#endif
