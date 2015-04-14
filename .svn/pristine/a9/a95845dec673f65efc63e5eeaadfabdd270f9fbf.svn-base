 /****************************************************************************
 * Copyright (C) 2009
 * by Dimok
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
 * TextEditor.cpp
 * for SaveGame Manager GX 2010
 ***************************************************************************/

#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include "libwiigui/gui.h"

//!Display a list of files
class TextEditor : public GuiElement
{
	public:
		TextEditor(char *intext, int LinesToDraw, char *filename);
		~TextEditor();
		void DisableTriggerUpdate(bool set);
		void SetText(char *intext);
		void SetFocus(bool f);
		void SetAlignment(int h, int v);
		void ResetState();
		void Draw();
		void Update(GuiTrigger * t);
	protected:
		int currentLine;
		int linestodraw;
		int TotalLines;
		bool focus;
		bool triggerdisabled;

        /** Window **/
        GuiWindow * Window;

        /** Buttons **/
		GuiButton * arrowUpBtn;
		GuiButton * arrowDownBtn;
		GuiButton * scrollbarBoxBtn;
		GuiButton * closeBtn;

        /** Images **/
		GuiImage * bgTexteditorImg;
		GuiImage * arrowDownImg;
		GuiImage * arrowDownOverImg;
		GuiImage * arrowUpImg;
		GuiImage * arrowUpOverImg;
		GuiImage * scrollbarBoxImg;
		GuiImage * scrollbarBoxOverImg;
		GuiImage * closeImg;
		GuiImage * closeImgOver;
		
        /** ImageDatas **/
		GuiImageData * bgTexteditorData;
		GuiImageData * arrowDown;
		GuiImageData * arrowDownOver;
		GuiImageData * arrowUp;
		GuiImageData * arrowUpOver;
		GuiImageData * scrollbarBox;
		GuiImageData * scrollbarBoxOver;
		GuiImageData * closeImgData;
		GuiImageData * closeImgOverData;
		
        /** Sounds **/
		GuiSound * btnSoundOver;
		GuiSound * btnSoundClick;

        /** Triggers **/
		GuiTrigger * trigA;
		GuiTrigger * trigHeldA;
		GuiTrigger * trigB;
		GuiTrigger * trigHome;

        /** Texts **/
		GuiText * filenameTxt;
		GuiText * MainFileTxt;
};

#endif
