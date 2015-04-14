/***************************************************************************
 * Copyright (C) 2009
 * by Dimok
 * Modified by Dj_Skual
 * for SaveGame Manager GX 2011
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
 * for WiiXplorer 2009
 ***************************************************************************/

#ifndef _PROMPTWINDOW_H_
#define _PROMPTWINDOW_H_

#include <gctypes.h>
#include "libwiigui/gui.h"

class PromptWindow : public GuiWindow
{
    public:
        PromptWindow(const char *title,
					 const char *msg = NULL,
					 const char *btn1Label = NULL,
					 const char *btn2Label = NULL,
					 const char *btn3Label = NULL,
					 const char *btn4Label = NULL);
        ~PromptWindow();
        int GetChoice();
        void SetTitle(const char *title);
        void SetMessage(const char *msg);
		
    protected:
        int choice;
		
		GuiImage * btn1Img;
        GuiImage * btn2Img;
        GuiImage * btn3Img;
        GuiImage * btn4Img;
        GuiImage * bgWindowImg;

        GuiImageData * btnOutline;
        GuiImageData * bgWindow;

        GuiSound * btnClick;
        GuiSound * btnSoundOver;

        GuiText * titleTxt;
        GuiText * msgTxt;
        GuiText * btn1Txt;
        GuiText * btn2Txt;
        GuiText * btn3Txt;
        GuiText * btn4Txt;

        GuiButton * btn1;
        GuiButton * btn2;
        GuiButton * btn3;
        GuiButton * btn4;

        GuiTrigger * trigA;
        GuiTrigger * trigB;
};

#endif
