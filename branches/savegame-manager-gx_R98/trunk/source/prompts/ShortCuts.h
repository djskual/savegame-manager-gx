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
 * ShortCuts.h
 * for SaveGame Manager GX 2010
 ***************************************************************************/

#ifndef SHORTCUTS_H
#define SHORTCUTS_H

#include "libwiigui/gui.h"

class Shortcuts : public GuiWindow
{
	public:
		Shortcuts();
		~Shortcuts();
		
		int GetChoice();
        
	protected:
		int choice;
		
        /** Buttons **/
		GuiButton * closeBtn;
		GuiButton * miiBtn;
		GuiButton * saveBtn;
		
        /** Images **/
		GuiImage * dialogBoxImg;
		GuiImage * miiImg;
		GuiImage * saveImg;
		GuiImage * closeImg;
		GuiImage * closeOverImg;
		GuiImage * buttonMiiImg;
		GuiImage * buttonMiiOverImg;
		GuiImage * buttonSaveImg;
		GuiImage * buttonSaveOverImg;
		
        /** ImageDatas **/
		GuiImageData * dialogBox;
		GuiImageData * mii;
		GuiImageData * save;
		GuiImageData * close;
		GuiImageData * closeOver;
		GuiImageData * button;
		GuiImageData * buttonOver;
		
		/** Sounds **/
		GuiSound * btnSoundOver;
		GuiSound * btnClick;
		
        /** Triggers **/
		GuiTrigger * trigA;
		GuiTrigger * trigB;
		GuiTrigger * trigHome;
		
        /** Texts **/
		GuiText * nameTxt;
};

#endif
