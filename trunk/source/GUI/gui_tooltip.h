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
#ifndef GUI_TOOLTIP_H_
#define GUI_TOOLTIP_H_

#include "GUI/gui_element.h"
#include "GUI/gui_text.h"
#include "GUI/gui_image.h"
#include "GUI/gui_imagedata.h"

class GuiTooltip : public GuiElement
{
	public:
		//!Constructor
		GuiTooltip(const char *t);
		GuiTooltip(const wchar_t *t);
		GuiTooltip(std::string t);
		GuiTooltip(std::wstring t);
		void InternalInit();
		//!Destructor
		virtual ~GuiTooltip();
		//!Sets GuiTooltip text
		void SetText(const char * t);
		void SetText(const wchar_t * t);
		void SetText(std::string t) { SetText(t.c_str()); }
		void SetText(std::wstring t) { SetText(t.c_str()); }
		//!Set the fontsize of the text
		void SetFontSize(int size);
		//!Set the color of the text
		void SetColor(GXColor c);
		//!Set the min time before draw call
		void SetElapseTime(float t);
		//!Get the min time before draw call
		float GetElapseTime() { return ElapseTime; };
		//!Draw callback
		void Draw();
	protected:
		float ElapseTime;
		int FontSize;
		GXColor color;
		GuiImage * leftImage;
		GuiImage * tileImage;
		GuiImage * rightImage;
		GuiImageData * tooltipLeft;
		GuiImageData * tooltipTile;
		GuiImageData * tooltipRight;
		GuiText * text;
};

#endif
