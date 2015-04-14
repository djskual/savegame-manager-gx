/****************************************************************************
 * Copyright (C) 2009-2011 Dimok
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
#ifndef _KEYBOARDWINDOW_H_
#define _KEYBOARDWINDOW_H_

#include "../GUI/gui.h"
#include "../GUI/gui_keyboard.h"
#include "../GUI/PictureButton.h"

class KeyboardWindow : public GuiFrame, public sigslot::has_slots<>
{
	public:
		KeyboardWindow(wchar_t *text, int maxlen = 0);
		~KeyboardWindow();
		
		int GetChoice() { return choice; }
		
		sigslot::signal2<KeyboardWindow *, wchar_t *> OkButtonClicked;
		
	private:
		void OnOkButtonClick(GuiButton *sender, int pointer, const POINT &p);
		void OnCancelButtonClick(GuiButton *sender, int pointer, const POINT &p);
		
		int maxlen;
		int choice;
		wchar_t *inText;
		
		PictureButton * okBtn;
		PictureButton * cancelBtn;
		
		GuiText * okBtnTxt;
		GuiText * cancelBtnTxt;
		
		GuiSound * btnSoundOver;
		
		GuiKeyboard * keyboard;
		
		GuiTrigger trigA;
};

#endif
