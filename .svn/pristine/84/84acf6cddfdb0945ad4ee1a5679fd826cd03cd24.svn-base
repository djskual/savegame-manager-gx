/***************************************************************************
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
 * for SaveGame Manager GX 2010
 ***************************************************************************/

#ifndef _MANAGEBUTTONS_H_
#define _MANAGEBUTTONS_H_

#include <gctypes.h>
#include "libwiigui/gui.h"

class ManageButtons : public GuiElement
{
    public:
        ManageButtons(const char *title = NULL,
					  const char *msg = NULL,
					  const char *btn1Label = NULL,
					  const char *btn2Label = NULL,
					  const char *btn3Label = NULL,
					  const char *btn4Label = NULL);
        ~ManageButtons();
		
        void SetTitle(const char *title);
        void SetMessage(const char *msg);
		void SetAlignment(int h, int v);
		void SetButtonsEffect();
		int GetChoice();
        void ResetState();
		void Draw();
		void Update(GuiTrigger * t);
		
    private:
		bool firstrun;
        bool change;
        int choice;
		short move;
		
		/** Window **/
		GuiWindow * buttons;
		
		/** Images **/
		GuiImage * btn1Img;
        GuiImage * btn2Img;
        GuiImage * btn3Img;
        GuiImage * btn4Img;
        GuiImage * btn1OverImg;
        GuiImage * btn2OverImg;
        GuiImage * btn3OverImg;
        GuiImage * btn4OverImg;
        
        /** ImageDatas **/
		GuiImageData * btnOutline;
        GuiImageData * btnOutlineOver;
        
        /** Sounds **/
		GuiSound * btnClick;
        GuiSound * btnSoundOver;
		
        /** Texts **/
		GuiText * titleTxt;
        GuiText * msgTxt;
        GuiText * btn1Txt;
        GuiText * btn2Txt;
        GuiText * btn3Txt;
        GuiText * btn4Txt;
		
        /** Buttons **/
		GuiButton * btn1;
        GuiButton * btn2;
        GuiButton * btn3;
        GuiButton * btn4;
		
        /** Triggers **/
		GuiTrigger * trigA;
};

#endif
