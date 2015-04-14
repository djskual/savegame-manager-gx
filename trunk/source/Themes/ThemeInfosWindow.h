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
#ifndef _THEMEINFOSWINDOW_H_
#define _THEMEINFOSWINDOW_H_

#include "../GUI/PictureButton.h"
#include "../GUI/gui.h"

class ThemeInfosWindow : public GuiFrame, public sigslot::has_slots<>
{
	public:
		ThemeInfosWindow(const char * title, const char * author, int type);
		virtual ~ThemeInfosWindow();
		
		int GetChoice() { return choice; }
		
	protected:
		void OnButtonClick(GuiButton *sender, int pointer, const POINT &p);
		
		enum
		{
			DOWNLOAD,
			LOAD,
		};
		
		int choice;
		
		GuiImage * bgWindowImg;
		GuiImage * themeImg;
		
		GuiImageData * bgWindowData;
		GuiImageData * thumbImageData;
		
		GuiSound * btnSoundClick;
		GuiSound * btnSoundOver;
		
		GuiText * titleTxt;
		GuiText * titleTxt2;
		GuiText * authorTxt;
		GuiText * authorTxt2;
		GuiText * actionBtnTxt;
		GuiText * backBtnTxt;
		
		PictureButton * actionBtn;
		PictureButton * backBtn;
		
		GuiTrigger * trigA;
		GuiTrigger * trigB;
};

#endif
