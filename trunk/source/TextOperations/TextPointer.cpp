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
 * TextPointer.cpp
 * for WiiXplorer 2009
 ***************************************************************************/
#include "../GUI/gui_longtext.hpp"
#include "../Themes/CTheme.h"
//#include "../Prompts/PromptWindows.h"
#include "TextPointer.h"
#include "FreeTypeGX.h"

extern FreeTypeGX * fontSystem;

/**
 * Constructor for the TextPointer class.
 */
TextPointer::TextPointer(GuiText *parent, int linestodraw)
	: GuiButton(343, 240)
{
	TextPtr = parent;
	fontsize = TextPtr->GetFontSize();
	MarkImage = NULL;
	currentline = -1;
	Position_X = 0;
	Position_Y = 0;
	width = TextPtr->GetTextMaxWidth();
	if(width == 0)
		width = TextPtr->GetTextWidth();
	height = (linestodraw+1)*(fontsize+6);
	visibility = true;

	TextPointerImgData = Resources::GetImageData("textpointer_img.png");
	TextPointerImg = new GuiImage(TextPointerImgData);
	TextPointerImg->SetVisible(visibility);

	SetLabel(TextPtr);
	SetImage(TextPointerImg);
}

TextPointer::TextPointer(GuiText *parent, int w, int h)
	: GuiButton(w, h)
{
	TextPtr = parent;
	fontsize = TextPtr->GetFontSize();
	MarkImage = NULL;
	currentline = -1;
	Position_X = 0;
	Position_Y = 0;
	width = w;
	height = h;
	visibility = true;

	TextPointerImgData = Resources::GetImageData("textpointer_img.png");
	TextPointerImg = new GuiImage(TextPointerImgData);
	TextPointerImg->SetVisible(visibility);

	SetLabel(TextPtr);
	SetImage(TextPointerImg);
}

/**
 * Destructor for the TextPointer class.
 */
TextPointer::~TextPointer()
{
	Resources::Remove(TextPointerImgData);
	delete TextPointerImg;

	if(MarkImage)
		delete MarkImage;
}

void TextPointer::PositionChanged(int chan, int x, int y)
{
	if(MarkImage)
	{
		delete MarkImage;
		MarkImage = NULL;
	}

	currentChan = chan;
	currentline = -1;
	Position_X = 0;
	Position_Y = 0;
	LetterNumInLine = 0;
	Marking = false;

	int linenumber = 0;

	int differenz = 1000;

	int maxlines = TextPtr->GetLinesCount();
	if(maxlines < 1)
		maxlines = 1;

	for(int i = 0; i < maxlines; i++)
	{
		int linestart = i*(fontsize+6);
		if(differenz > abs(linestart-y))
		{
			differenz = abs(linestart-y);
			linenumber = i;
		}
	}
	differenz = 1000;
	const wchar_t * line = TextPtr->GetTextLine(linenumber);

	if(!line)
		return;

	lineLength = wcslen(line)+1;
	int i = 0;
	int w = 0;

	while(line[i] != 0)
	{
		w += fontSystem->getCharWidth(line[i], fontsize, i > 0 ? line[i-1] : 0);

		if(differenz > abs(x-w))
		{
			differenz = abs(x-w);
			Position_X = w;
			LetterNumInLine = i+1;
		}
		i++;
	}

	if(differenz > abs(x))
	{
		differenz = abs(x);
		Position_X = 0;
		LetterNumInLine = 0;
	}

	currentChan = chan;
	currentline = linenumber;
	Position_Y = linenumber*(fontsize+6);
	Marking = true;
}

void TextPointer::SetPointerPosition(int LetterPos)
{
	if(LetterPos <= 0)
	{
		Position_X = 0;
		LetterNumInLine = 0;
		Marking = false;
		return;
	}

	const wchar_t * line = TextPtr->GetTextLine(currentline);

	if(!line)
		return;

	lineLength = wcslen(line);

	if(LetterPos < 0)
		LetterPos = 0;
	else if(LetterPos > lineLength)
		LetterPos = lineLength;

	Position_X = 0;

	for(int i = 0; i < LetterPos; ++i)
	{
		Position_X += fontSystem->getCharWidth(line[i], fontsize, i > 0 ? line[i-1] : 0);
	}

	LetterNumInLine = LetterPos;
	Marking = false;
}

void TextPointer::TextWidthChanged()
{
	width = TextPtr->GetTextMaxWidth();
	if(width == 0)
		width = TextPtr->GetTextWidth();
}

void TextPointer::Draw()
{
	if(frameCount % 30 == 0)
	{
		visibility = !visibility;
		TextPointerImg->SetVisible(visibility);
	}
	else if(TextPointerImg->GetLeft() != Position_X || TextPointerImg->GetTop() != Position_Y)
		TextPointerImg->SetPosition(Position_X, Position_Y);

	GuiButton::Draw();
}
