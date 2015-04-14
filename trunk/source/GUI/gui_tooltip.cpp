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
#include "gui_tooltip.h"
#include "../Themes/CTheme.h"
#include "../TextOperations/FreeTypeGX.h"

GuiTooltip::GuiTooltip(const char *t)
{
	InternalInit();
	
	SetText(t);
	SetVisible(false);
}

GuiTooltip::GuiTooltip(const wchar_t *t)
{
	InternalInit();
	
	SetText(t);
	SetVisible(false);
}

GuiTooltip::GuiTooltip(std::string t)
{
	InternalInit();
	
	SetText(t);
	SetVisible(false);
}

GuiTooltip::GuiTooltip(std::wstring t)
{
	InternalInit();
	
	SetText(t);
	SetVisible(false);
}

void GuiTooltip::InternalInit()
{
	text = NULL;
	FontSize = 22;
	ElapseTime = Settings.TooltipDelay;
	color = (GXColor){0, 0, 0, 255};

	tooltipLeft = Resources::GetImageData("tooltip_left.png");
	tooltipTile = Resources::GetImageData("tooltip_tile.png");
	tooltipRight = Resources::GetImageData("tooltip_right.png");
	leftImage = new GuiImage(tooltipLeft);
	tileImage = new GuiImage(tooltipTile);
	rightImage = new GuiImage(tooltipRight);

	height = leftImage->GetHeight();

	leftImage->SetParent(this);
	tileImage->SetParent(this);
	rightImage->SetParent(this);
}

GuiTooltip::~GuiTooltip()
{
	if(text)
		delete text;

	delete leftImage;
	delete tileImage;
	delete rightImage;

	Resources::Remove(tooltipLeft);
	Resources::Remove(tooltipTile);
	Resources::Remove(tooltipRight);
}

void GuiTooltip::SetText(const char * t)
{
	if(!t)
		return;
	
	SetText(charToWideChar(t));
}

void GuiTooltip::SetText(const wchar_t * t)
{
	if(text)
		delete text;
	text = NULL;

	int tile_cnt = 0;
	if(t)
	{
		text = new GuiText(t, FontSize, color);
		text->SetParent(this);
		text->SetAlignment(ALIGN_CENTER | ALIGN_MIDDLE);
		tile_cnt = (int) ((float) (text->GetTextWidth()-12) / (float) tileImage->GetWidth());
		if(tile_cnt < 0)
			tile_cnt = 0;
	}
	tileImage->SetPosition(leftImage->GetWidth(), 0);
	tileImage->SetTileHorizontal(tile_cnt);
	rightImage->SetPosition(leftImage->GetWidth() + tile_cnt * tileImage->GetWidth(), 0);
	width = leftImage->GetWidth() + tile_cnt * tileImage->GetWidth() + rightImage->GetWidth();
}

void GuiTooltip::SetFontSize(int size)
{
	FontSize = size;

	if(text)
	{
		std::wstring wText(text->GetText());
		SetText(wText);
	}
}

void GuiTooltip::SetColor(GXColor c)
{
	color = c;

	if(text)
	{
		std::wstring wText(text->GetText());
		SetText(wText);
	}
}

void GuiTooltip::SetElapseTime(float t)
{
	ElapseTime = t;
}

void GuiTooltip::Draw()
{
	if(ElapseTime != Settings.TooltipDelay)
		SetElapseTime(Settings.TooltipDelay);
	
	if((!IsVisible() && !GetEffect()) || state == STATE_DISABLED)
		return;

	leftImage->Draw();
	tileImage->Draw();
	rightImage->Draw();
	if(text)
		text->Draw();

	this->UpdateEffects();
}
