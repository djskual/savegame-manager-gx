/***************************************************************************
 * Copyright (C) 2012 Dj_Skual
 * for SaveGame Manager GX 2012
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
 * MainMenu.h
 *
 ***************************************************************************/
#ifndef _THEMEMENU_H
#define _THEMEMENU_H

#include "../Gui/gui.h"
#include "../Gui/PictureButton.h"
#include "CTheme.h"
#include "ThemeDownloader.h"
//#include "ThemeLoader.h"

class ThemeMenu : public GuiFrame, public sigslot::has_slots<>
{
	public:
		ThemeMenu(int type);
		virtual ~ThemeMenu();
		
		void quit();
		void SetTitle(std::string t) { if(Title) Title->SetText(t); }
		
		enum
		{
			DOWNLOADER,
			LOADER,
		};
		
		sigslot::signal0<> Closed;
		
	private:
		void OnBackButtonClick(GuiButton *sender, int pointer, const POINT &p);
		
		ThemeDownloader * Downloader;
		//ThemeLoader * Loader;
		
		int type;
		
		GuiText * Title;
		
		GuiImageData * BgImgData;
		GuiImage * BgImg;
		
		GuiText * BackBtnTxt;
		PictureButton * BackBtn;
		
		GuiSound * BtnSoundClick;
		GuiSound * BtnSoundOver;
		
		GuiTrigger trigA;
		GuiTrigger trigB;
};

#endif
