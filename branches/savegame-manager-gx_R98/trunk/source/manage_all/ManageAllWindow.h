 /****************************************************************************
 * Copyright (C) 2010
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
 * ManageAllWindow.h
 * for SaveGame Manager GX 2010
 ***************************************************************************/

#ifndef MANAGEALLWINDOW_H
#define MANAGEALLWINDOW_H

#include "libwiigui/gui.h"

//!Display a prompt with proposal for all saves manage
class ManageAllWindow : public GuiElement
{
	public:
		ManageAllWindow(const char *title);
		~ManageAllWindow();
		
		void SetAlignment(int h, int v);
		void SetTitle(const char *title);
        int GetChoice();
        void ResetState();
		void Disabled();
		void Enabled();
		void Draw();
		void Update(GuiTrigger * t);
		
		
	protected:
		/** Window **/
        GuiWindow * window;
		
		/** Buttons **/
		GuiButton * closeBtn;
		
        /** Images **/
		GuiImage * dialogBoxImg;
		GuiImage * closeImg;
		GuiImage * closeOverImg;
		
        /** ImageDatas **/
		GuiImageData * dialogBox;
		GuiImageData * close;
		GuiImageData * closeOver;
		
		/** Sounds **/
		GuiSound * btnSoundOver;
		GuiSound * btnClick;
		
        /** Triggers **/
		GuiTrigger * trigA;
		GuiTrigger * trigB;
		GuiTrigger * trigHome;
		
        /** Texts **/
		GuiText * titleTxt;
};

#endif
