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
 * for Wii-FileXplorer 2009
 ***************************************************************************/

#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include "../LibWiiGui/gui.h"
#include "../Controls/Scrollbar.hpp"
#include "Text.hpp"

//!Display a list of files
class TextEditor : public GuiWindow
{
	public:
		TextEditor(const wchar_t *intext, int LinesToDraw, const char *path);
		~TextEditor();
		void SetTriggerUpdate(bool set);
		void SetText(const wchar_t *intext);
		int GetState();
		void ResetState();
		void Update(GuiTrigger * t);
	protected:
		
		bool triggerupdate;
		int linestodraw;
		u32 filesize;
		char * filepath;
		
		Scrollbar * scrollbar;
		
		/** Buttons **/
		GuiButton * closeBtn;
		
		/** Images **/
		GuiImage * bgTexteditorImg;
		GuiImage * closeImg;
		GuiImage * closeImgOver;
		
		/** ImageDatas **/
		GuiImageData * bgTexteditorData;
		GuiImageData * closeImgData;
		GuiImageData * closeImgOverData;
		
		/** Triggers **/
		GuiTrigger * trigA;
		GuiTrigger * trigB;
		GuiTrigger * trigHome;
		
		/** Texts **/
		GuiText * filenameTxt;
		Text * MainFileTxt;
};

#endif
