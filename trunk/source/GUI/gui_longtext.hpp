/****************************************************************************
 * Copyright (C) 2009-2011 Dimok
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
#ifndef _LONGTEXT_HPP_
#define _LONGTEXT_HPP_

#include "gui_text.h"
#include "../TextOperations/wstring.hpp"

typedef struct
{
	int LineOffset;
	int CharCount;
	int width;
} TextLine;

class GuiLongText : public GuiText
{
	public:
		//!Constructor
		//!\param t Text
		//!\param s Font size
		//!\param c Font color
		GuiLongText(const char * t, int s, GXColor c);
		GuiLongText(const wchar_t * t, int s, GXColor c);
		virtual ~GuiLongText();
		//!Sets the text of the GuiText element
		//!\param t Text
		void SetText(const char * t);
		void SetText(const wchar_t * t);
		//!Set the max texwidth
		void SetMaxWidth(int width);
		//!Go to next line
		void NextLine();
		//!Go to previous line
		void PreviousLine();
		//!Refresh the rows to draw
		void Refresh();
		//!Set the text line
		void SetTextLine(int line);
		//!Set to the char pos in text
		void SetTextPos(int pos);
		//!Refresh the rows to draw
		int GetCurrPos() { return curLineStart; };
		//!Get  the count of loaded lines
		int GetLinesCount() { return textDyn.size(); };
		//!Get the total count of lines
		int GetTotalLinesCount() { return TextLines.size(); };
		//!Get the original full Text
		const wchar_t * GetText() const { return wText->c_str(); }
		//!Get the original full Text as wString
		wString * GetwString() { return wText; };
		//!Get the original Text as a UTF-8 text
		std::string toUTF8() const { return wText->toUTF8(); }
		//!Get a Textline
		const wchar_t * GetTextLine(int ind);
		//!Get the offset in the text of a drawn Line
		int GetLineOffset(int ind);
		//!Constantly called to draw the text
		void Draw();
	protected:
		void CalcLineOffsets();
		void FillRows();

		wString * wText;
		std::vector<TextLine> TextLines;
		int curLineStart;
		int FirstLineOffset;
		bool filling;
};

#endif
