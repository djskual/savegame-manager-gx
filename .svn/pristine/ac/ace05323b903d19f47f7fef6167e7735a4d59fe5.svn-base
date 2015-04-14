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
 * Explorer Class
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
 #ifndef __SELECTBROWSER_H_
 #define __SELECTBROWSER_H_

#include <gctypes.h>
#include <unistd.h>

#include "../Files/ListFileCustomBrowser.hpp"
#include "../Files/filecustombrowser.h"

enum
{
	BROWSER = 0,
	LANGBROWSER,
	SOUNDBROWSER,
	FONTBROWSER,
	SAVEPATHBROWSER,
	DATABINPATHBROWSER,
	MIIPATHBROWSER,
	UPDATEPATHBROWSER,
	GAMETDBPATHBROWSER,
	THEMEPATHBROWSER,
	HOMEBREWPATHBROWSER,
	SCREENSHOTPATHBROWSER,
	EMUNANDPATHBROWSER,
	PATH,
};

class SelectBrowser : public GuiWindow
{
	public:
		SelectBrowser(short Type);
		SelectBrowser(const char * path, short Type);
		~SelectBrowser();
		void Setup(short Type);
		int LoadPath(const char * path);
		int GetMenuChoice();
		void SetFilter(u8 filtermode) { DeviceBrowser->SetFilter(filtermode); };
		std::string GetCurrentPath() { if(!CurBrowser) return ""; return CurBrowser->GetCurrentPath(); };
		
	protected:
		void CheckBrowserChanges();
		void SetDeviceImage();
		void BackInDirectory();
		int CreateLangPath(char * path);
		int CreatePath(char * path);
		
		int result;
		short type;
		int currentDevice;
		
		Browser * CurBrowser;
		FileCustomBrowser * DeviceBrowser;
		
		GuiFileBrowser * fileBrowser;
		ListFileCustomBrowser * listBrowser;
		
		GuiImage * BrowserImg;
		GuiImage * ButtonBoxImgClose;
		GuiImage * ButtonBoxImgWifiVal;
		GuiImage * DeviceImg;
		GuiImage * FolderImg;
		GuiImage * CloseImg;
		GuiImage * CloseOverImg;
		GuiImage * WifiImg;
		GuiImage * WifiOverImg;
		GuiImage * ValidImg;
		GuiImage * ValidOverImg;
		
		GuiImageData * BrowserImgData;
		GuiImageData * ButtonBoxImgData;
		GuiImageData * CloseImgData;
		GuiImageData * CloseOverImgData;
		GuiImageData * WifiImgData;
		GuiImageData * WifiOverImgData;
		GuiImageData * ValidImgData;
		GuiImageData * ValidOverImgData;
		GuiImageData * SDImgData;
		GuiImageData * USBImgData;
		GuiImageData * FolderImgData;
		
		GuiText * AdressText;
		
		GuiButton * DeviceSwitchBtn;
		GuiButton * FolderBtn;
		GuiButton * CloseBtn;
		GuiButton * WifiBtn;
		GuiButton * ValidBtn;
		GuiButton * BackInDirBtn;
		
		SimpleGuiTrigger * trigA;
		GuiTrigger * trigHome;
		GuiTrigger * trigBackInDir;
};

int selectBrowser(const char * enterPath, short Type);

#endif
