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
 * ManageMiiWindow.h
 * for SaveGame Manager GX 2010
 ***************************************************************************/

#ifndef MANAGEMIIWINDOW_H
#define MANAGEMIIWINDOW_H

#include "libwiigui/gui.h"

//!Display a prompt with information about the selected mii
class ManageMiiWindow : public GuiElement
{
	public:
		ManageMiiWindow(const char *name,
						const char *creator = NULL,
						int day = NULL,
						int month = NULL,
						int female = NULL,
						int favColor = NULL);
		~ManageMiiWindow();
		
		void SetAlignment(int h, int v);
		void SetWindowEffect();
		int GetMonth(int m);
		int GetFavColor(int c);
		int GetChoice();
        void ResetState();
		void Disabled();
		void Enabled();
		void Draw();
		void Update(GuiTrigger * t);
		
		
	protected:
		int choice;
		short move;
		
        /** Window **/
        GuiWindow * window;
		GuiWindow * infos;
		
		/** Buttons **/
		GuiButton * closeBtn;
		GuiButton * leftBtn;
		GuiButton * rightBtn;
		GuiButton * creatorBtn;
		GuiButton * dateBtn;
		
        /** Images **/
		GuiImage * dialogBoxImg;
		GuiImage * closeImg;
		GuiImage * closeOverImg;
		GuiImage * leftImg;
		GuiImage * rightImg;
		GuiImage * creatorIconImg;
		GuiImage * dateIconImg;
		GuiImage * favColorImg;
		GuiImage * genderImg;
		
        /** ImageDatas **/
		GuiImageData * dialogBox;
		GuiImageData * close;
		GuiImageData * closeOver;
		GuiImageData * left;
		GuiImageData * right;
		GuiImageData * creatorIcon;
		GuiImageData * dateIcon;
		GuiImageData * gender;
		
		/** Sounds **/
		GuiSound * btnSoundOver;
		GuiSound * btnClick;
		
        /** Triggers **/
		GuiTrigger * trigA;
		GuiTrigger * trigB;
		GuiTrigger * trigHome;
		GuiTrigger * trigMinus;
		GuiTrigger * trigPlus;
		GuiTrigger * trigLeft;
		GuiTrigger * trigRight;
		
        /** Texts **/
		GuiText * nameTxt;
		GuiText * creatorTxt;
		GuiText * dateTxt;
};

#endif
