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
 * ManageWindow.h
 * for SaveGame Manager GX 2010
 ***************************************************************************/

#ifndef MANAGEWINDOW_H
#define MANAGEWINDOW_H

#include "libwiigui/gui.h"
#include "libwiigui/gui_banner.h"

//!Display a prompt with information about the save selected
class ManageWindow : public GuiElement
{
	public:
		ManageWindow(const char *name,
					 const char *subname = NULL,
					 float block = NULL,
					 const char *ID = NULL,
					 u64 tid = NULL,
					 int supported = NULL,
					 int selected = NULL);
		~ManageWindow();
		
		enum {
			DEVICE,
			WII
		};
		
		enum {
			DEFAULT, //clip(reset)
			NO_MOVE_IN, //fade buttons in
			NO_MOVE_OUT, //fade buttons out
			MOVE_IN, //slide top in
			MOVE_OUT, //slide top out
			MOVE_PREV_IN,
			MOVE_PREV_OUT,
			MOVE_NEXT_IN,
			MOVE_NEXT_OUT
		};
		
		enum {
			NO_TYPE,
			DELETE,
			PREV,
			NEXT
		};
		
		void SetAlignment(int h, int v);
		void SetWindowEffect();
		int GetIfInstalled();
		int GetChoice();
        void ResetState();
		void Disabled();
		void Enabled();
		void Draw();
		void Update(GuiTrigger * t);
		
		
	protected:
		int notSupportedFormat;
		bool SaveInstalled;
		int choice;
		short move;
		
        /** Window **/
        GuiWindow * window;
		
		/** Buttons **/
		GuiButton * closeBtn;
		GuiButton * leftBtn;
		GuiButton * rightBtn;
		
        /** Images **/
		GuiImage * dialogBoxImg;
		GuiImage * iconBoxImg;
		GuiImage * blockBoxImg;
		GuiImage * idBoxImg;
		GuiImage * closeImg;
		GuiImage * closeOverImg;
		GuiImage * leftImg;
		GuiImage * rightImg;
		
        /** ImageDatas **/
		GuiImageData * dialogBox;
		GuiImageData * iconBox;
		GuiImageData * blockBox;
		GuiImageData * idBox;
		GuiImageData * close;
		GuiImageData * closeOver;
		GuiImageData * left;
		GuiImageData * right;
		
		/** GuiSaveIcon **/
		GuiSaveIcon * icon;		
		
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
		GuiText * subnameTxt;
		GuiText * blockTxt;
		GuiText * idTxt;
		GuiText * warningTxt;
		GuiText * supportTxt;
};

#endif
