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
#ifndef _UPDATEWINDOW_H_
#define _UPDATEWINDOW_H_

#include "../GUI/gui.h"
#include "../GUI/PictureButton.h"

class UpdateWindow : public GuiFrame, public sigslot::has_slots<>
{
	public:
		UpdateWindow();
		virtual ~UpdateWindow();
		
		int GetChoice();
		bool NetInitPrompt();
		void Update(GuiTrigger * t);
		
		bool ProgressCanceled();
		static void SetCancelEnable(bool allowCancel) { cancelEnabled = allowCancel; }
		
		void ShowButtons(std::string title, std::string msg, std::string btn1txt, std::string btn2txt = "", std::string btn3txt = "", std::string btn4txt = "");
		int Choose(std::string title, std::string msg, std::string btn1txt, std::string btn2txt = "", std::string btn3txt = "", std::string btn4txt = "");
		void ShowMessage(std::string title, std::string msg = "");
		void ShowError(std::string error);
		void ShowProgress(std::string title, std::string msg, int done = 0, int total = 1);
		void ShowThrobber(std::string title, std::string msg = "");
		
		sigslot::signal2<UpdateWindow *, int> ButtonClicked;
		
	protected:
		void RemoveItems();
		void SetButtons(std::string btn1txt, std::string btn2txt = "", std::string btn3txt = "", std::string btn4txt = "");
		void SetProgress(int done = 0, int total = 1);
		
		void OnButtonClick(GuiButton *sender, int pointer, const POINT &p);
		
		static bool cancelEnabled;
		int choice;
		int angle;
		int type;
		
		enum
		{
			BUTTONS,
			PROGRESS,
			THROBBER,
		};
		
		GuiImageData * bgWindow;
		GuiImage * bgWindowImg;
		
		GuiImageData * throbber;
		GuiImage * throbberImg;
		
		GuiImageData * barOutline;
		GuiImageData * barEmpty;
		GuiImageData * bar;
		GuiImage * barOutlineImg;
		GuiImage * barEmptyImg;
		GuiImage * barImg;
		GuiText * barPrTxt;
		GuiText * barPrsTxt;
		
		GuiText * titleTxt;
		GuiText * msgTxt;
		
		GuiText * btnTxt[4];
		PictureButton * btn[4];
		
		GuiSound * btnSoundClick;
		GuiSound * btnSoundOver;
		
		GuiTrigger * trigA;
		GuiTrigger * trigB;
};

#endif
