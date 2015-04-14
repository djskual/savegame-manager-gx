/****************************************************************************
 * Copyright (C) 2009 r-win
 * Copyright (C) 2012 Dimok
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
 ***************************************************************************/
#ifndef GUI_NUMPAD_H_
#define GUI_NUMPAD_H_

#include "gui.h"
#include "PictureButton.h"

#define NUMPAD_BUTTONS	12

//!On-screen keyboard
class GuiNumpad: public GuiFrame, public sigslot::has_slots<>
{
	public:
		GuiNumpad(char * t, u32 max, bool crypted = false);
		virtual ~GuiNumpad();
		const char *GetText() const { return kbtextstr; }
		
	protected:
		void OnNormalKeyPress(GuiButton *sender, int pointer, const POINT &p);
		void OnSpecialKeyPress(GuiButton *sender, int pointer, const POINT &p);
		
		u32 kbtextmaxlen;
		char keys[NUMPAD_BUTTONS];
		char kbtextstr[256];
		GuiText * kbText;
		
		GuiText * keyBackText;
		PictureButton * keyBack;
		
		GuiText * keyClearText;
		PictureButton * keyClear;
		
		GuiText * keyTxt[NUMPAD_BUTTONS];
		PictureButton * keyBtn[NUMPAD_BUTTONS];
		
		GuiImage * keyTextboxImg;
		GuiImageData * keyTextbox;
		
		GuiSound * SoundOver;
		GuiSound * SoundClick;
		
		GuiTrigger * trigA;
		GuiTrigger * trigB;
};

#endif
