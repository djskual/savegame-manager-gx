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

#ifndef _MANAGEALLBUTTONS_H_
#define _MANAGEALLBUTTONS_H_

#include "libwiigui/gui.h"

class ManageAllButtons : public GuiElement
{
    public:
        ManageAllButtons(const char *btn1Label = NULL,
						 const char *btn2Label = NULL,
						 int what = 0,					//0-no, 1-choice saves/miis
						 int mii = 0,					//0-no, 1-manage all miis
						 int miidelete = 0);			//0-no, 1-manage all mii delete
        ~ManageAllButtons();
		
        void SetAlignment(int h, int v);
		int GetChoice();
        void ResetState();
		void Draw();
		void Update(GuiTrigger * t);
		
    private:
		int choice;
		int what;
		int mii;
		int miidelete;
		
		// Window //
		GuiWindow * buttons;
		
		// Images //
		GuiImage * btn1Img;
        GuiImage * btn2Img;
        GuiImage * btn3Img;
        GuiImage * btn1OverImg;
        GuiImage * btn2OverImg;
        GuiImage * btn3OverImg;
        GuiImage * installImg;
        GuiImage * extractImg;
        GuiImage * deleteAllImg;
        GuiImage * nandImg;
        GuiImage * deviceImg;
        
        // ImageDatas //
		GuiImageData * btnOutline;
        GuiImageData * btnOutlineOver;
        GuiImageData * install;
        GuiImageData * extract;
        GuiImageData * deleteAll;
        GuiImageData * nand;
        GuiImageData * device;
        
        // Sounds //
		GuiSound * btnClick;
        GuiSound * btnSoundOver;
		
        // Texts //
		GuiText * btn1Txt;
        GuiText * btn2Txt;
        GuiText * btn3Txt;
        
        // Buttons //
		GuiButton * btn1;
        GuiButton * btn2;
        GuiButton * btn3;
        
        // Triggers //
		GuiTrigger * trigA;
};

#endif
