/***************************************************************************
 * Copyright (C) 2010
 * by Dimok
 * modified by dj_skual
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
 * for WiiXplorer 2010
 ***************************************************************************/
#include "../Themes/CTheme.h"
#include "../Settings/CSettings.h"
#include "Scrollbar.hpp"

Scrollbar::Scrollbar(int h, u8 m)
{
	SelItem = 0;
	SelInd = 0;
	RowSize = 0;
	PageSize = 0;
	EntrieCount = 0;
	ScrollSpeed = 5;
	ScrollState = 0;
	listchanged = false;
	Mode = m;

	trigHeldA = new GuiTrigger();
	trigHeldA->SetHeldTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);

	if (Mode == ICONMODE || Mode == OPTIONMODE)
	{
		scrollbarTop = Resources::GetImageData("scrollbar_device_top.png");
		scrollbarButtom = Resources::GetImageData("scrollbar_device_bottom.png");
		scrollbarTile = Resources::GetImageData("scrollbar_device_tile.png");
	}
	else if (Mode == LISTMODE)
	{
		scrollbarTop = Resources::GetImageData("scrollbar_custom_top.png");
		scrollbarButtom = Resources::GetImageData("scrollbar_custom_bottom.png");
		scrollbarTile = Resources::GetImageData("scrollbar_custom_tile.png");
	}
	arrowDown = Resources::GetImageData("scrollbar_arrowdown.png");
	arrowUp = Resources::GetImageData("scrollbar_arrowup.png");
	scrollbarBox = Resources::GetImageData("scrollbar_box.png");
	
	height = h;
	width = scrollbarBox->GetWidth();

	MinHeight = 20+3;
	MaxHeight = height-MinHeight-scrollbarBox->GetHeight();
	
	int PositionY = (Mode == ICONMODE || Mode == OPTIONMODE) ? 11 : 21;
	int Tiles = (height-(PositionY*2)-scrollbarTop->GetHeight()-scrollbarButtom->GetHeight())/4;
	
	scrollbarTopImg = new GuiImage(scrollbarTop);
	scrollbarTopImg->SetParent(this);
	scrollbarTopImg->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	scrollbarTopImg->SetPosition((Mode == ICONMODE || Mode == OPTIONMODE) ? 2 : 0, PositionY);
	PositionY += scrollbarTop->GetHeight();

	scrollbarTileImg = new GuiImage(scrollbarTile);
	scrollbarTileImg->SetParent(this);
	scrollbarTileImg->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	scrollbarTileImg->SetPosition((Mode == ICONMODE || Mode == OPTIONMODE) ? 2 : 0, PositionY);
	scrollbarTileImg->SetTileVertical(Tiles);
	PositionY += Tiles*scrollbarTile->GetHeight();

	scrollbarButtomImg = new GuiImage(scrollbarButtom);
	scrollbarButtomImg->SetParent(this);
	scrollbarButtomImg->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	scrollbarButtomImg->SetPosition((Mode == ICONMODE || Mode == OPTIONMODE) ? 2 : 0, PositionY);

	arrowDownImg = new GuiImage(arrowDown);
	arrowUpImg = new GuiImage(arrowUp);
	scrollbarBoxImg = new GuiImage(scrollbarBox);
	
	arrowUpBtn = new GuiButton(arrowUpImg->GetWidth(), arrowUpImg->GetHeight());
	arrowUpBtn->SetParent(this);
	arrowUpBtn->SetImage(arrowUpImg);
	arrowUpBtn->SetImageOver(arrowUpImg);
	arrowUpBtn->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	arrowUpBtn->SetPosition(0, 0);
	arrowUpBtn->SetHoldable(true);
	arrowUpBtn->SetTrigger(trigHeldA);
	arrowUpBtn->SetSoundOver(btnSoundOver);
	arrowUpBtn->SetSoundClick(btnSoundClick);
	arrowUpBtn->SetEffectGrow();
	
	arrowDownBtn = new GuiButton(arrowDownImg->GetWidth(), arrowDownImg->GetHeight());
	arrowDownBtn->SetParent(this);
	arrowDownBtn->SetImage(arrowDownImg);
	arrowDownBtn->SetImageOver(arrowDownImg);
	arrowDownBtn->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	arrowDownBtn->SetPosition(0, 0);
	arrowDownBtn->SetHoldable(true);
	arrowDownBtn->SetTrigger(trigHeldA);
	arrowDownBtn->SetSoundOver(btnSoundOver);
	arrowDownBtn->SetSoundClick(btnSoundClick);
	arrowDownBtn->SetEffectGrow();
	
	scrollbarBoxBtn = new GuiButton(scrollbarBoxImg->GetWidth(), scrollbarBoxImg->GetHeight());
	scrollbarBoxBtn->SetParent(this);
	scrollbarBoxBtn->SetImage(scrollbarBoxImg);
	scrollbarBoxBtn->SetImageOver(scrollbarBoxImg);
	scrollbarBoxBtn->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	scrollbarBoxBtn->SetPosition(0, MinHeight);
	scrollbarBoxBtn->SetMinY(MinHeight);
	scrollbarBoxBtn->SetMaxY(MaxHeight);
	scrollbarBoxBtn->SetHoldable(true);
	scrollbarBoxBtn->SetTrigger(trigHeldA);
	scrollbarBoxBtn->SetEffectGrow();
}

Scrollbar::~Scrollbar()
{
	delete scrollbarTop;
	delete scrollbarButtom;
	delete scrollbarTile;
	delete arrowDown;
	delete arrowUp;
	delete scrollbarBox;
	
	delete arrowUpBtn;
	delete arrowDownBtn;
	delete scrollbarBoxBtn;

	delete scrollbarTopImg;
	delete scrollbarButtomImg;
	delete scrollbarTileImg;
	delete arrowDownImg;
	delete arrowUpImg;
	delete scrollbarBoxImg;
	
	delete trigHeldA;
}

void Scrollbar::OnUpButtonHold()
{
	if(ScrollState < ScrollSpeed)
		return;

	if(Mode == ICONMODE)
	{
		if(SelInd+SelItem-RowSize >= 0)
		{
			SelItem = SelItem-RowSize;
			if(SelItem < 0)
			{
				//! move list up by 1
				SelInd = SelInd-RowSize;
				SelItem = SelItem+RowSize;
			}
		}
	}
	else if(Mode == LISTMODE || Mode == OPTIONMODE)
	{
		if(SelItem == 0 && SelInd > 0)
		{
			//! move list up by 1
			--SelInd;
		}
		else if(SelInd+SelItem > 0)
		{
			--SelItem;
		}
	}

	ScrollState = 0;
	listchanged = true;
}

void Scrollbar::OnDownButtonHold()
{
	if(ScrollState < ScrollSpeed)
		return;

	if(Mode == ICONMODE)
	{
		int i = RowSize;
		while(SelInd+SelItem+RowSize >= EntrieCount && i > 0 && SelItem > 0 && RowSize < EntrieCount)
		{
			--i;
			--SelItem;
		}
		if(SelInd+SelItem+RowSize < EntrieCount)
		{
			SelItem = SelItem+RowSize;
			if(SelItem >= PageSize)
			{
				//! move list down by 1
				SelInd += RowSize;
				SelItem = SelItem-RowSize;
			}
		}
	}
	else if(Mode == LISTMODE || Mode == OPTIONMODE)
	{
		if(SelInd+SelItem + 1 < EntrieCount)
		{
			if(SelItem == PageSize-1)
			{
				//! move list down by 1
				SelInd++;
			}
			else
			{
				SelItem++;
			}
		}
	}

	ScrollState = 0;
	listchanged = true;
}

void Scrollbar::OnBoxButtonHold(GuiTrigger * t)
{
	if(ScrollState < ScrollSpeed)
		return;

	if(!t->wpad.ir.valid)
		return;

	int p_y = (t->wpad.ir.y)-this->GetTop()-scrollbarBox->GetHeight()/2;

	int positionWiimote = LIMIT(p_y-MinHeight, 0, MaxHeight-MinHeight);

	int newSelected = (int) ((float) positionWiimote / (float) (MaxHeight-MinHeight) * (float) (EntrieCount-1));

	if(Mode == ICONMODE)
	{
		int rows = (int) floor(((float) (newSelected-SelInd-SelItem)) / ((float) RowSize));

		while(SelInd+rows*RowSize >= EntrieCount-PageSize+RowSize)
			rows--;

		int pageIndex = LIMIT(SelInd+rows*RowSize, 0, EntrieCount-1-RowSize);

		if(newSelected <= 0)
			SelItem = 0;
		else if(newSelected >= EntrieCount-1)
			SelItem = EntrieCount-1-pageIndex;

		SelInd = pageIndex;
	}
	else if(Mode == LISTMODE || Mode == OPTIONMODE)
	{
		int diff = newSelected-SelInd-SelItem;

		if(newSelected <= 0)
			SelItem = 0;
		else if(newSelected >= EntrieCount-1)
			SelItem = (PageSize-1 < EntrieCount-1) ? PageSize-1 : EntrieCount-1;

		SelInd = LIMIT(SelInd+diff, 0, ((EntrieCount-PageSize < 0) ? 0 : EntrieCount-PageSize));
	}

	ScrollState = 0;
	listchanged = true;
}

void Scrollbar::SetPageSize(int size)
{
	if(PageSize == size)
		return;

	PageSize = size;
	SetScrollboxPosition();
}

void Scrollbar::SetRowSize(int size)
{
	if(RowSize == size)
		return;

	RowSize = size;
	SetScrollboxPosition();
}

void Scrollbar::SetSelectedItem(int pos)
{
	if(SelItem == pos)
		return;

	SelItem = pos;
	SetScrollboxPosition();
}

void Scrollbar::SetSelectedIndex(int pos)
{
	if(SelInd == pos)
		return;

	SelInd = pos;
	SetScrollboxPosition();
}

void Scrollbar::SetEntrieCount(int cnt)
{
	if(EntrieCount == cnt)
		return;

	EntrieCount = cnt;
	if(EntrieCount < 0)
		EntrieCount = 0;
	
	SetScrollboxPosition();
}

bool Scrollbar::ListChanged()
{
	bool ret = listchanged;
	listchanged = false;

	return ret;
}

void Scrollbar::SetScrollboxPosition()
{
	if(!EntrieCount)
		return;
	
	if(Mode == ICONMODE)
	{
		u8 row = (u8) floor((float) SelItem / (float) RowSize);

		int position = MinHeight+(MaxHeight-MinHeight)*(SelInd+row*RowSize)/(EntrieCount-1);

		if(position < MinHeight)
			position = MinHeight;
		else if(position > MaxHeight || ((SelInd+PageSize >= (EntrieCount-1)) && row > 1))
			position = MaxHeight;

		scrollbarBoxBtn->SetPosition(0, position);
	}
	else if(Mode == LISTMODE || Mode == OPTIONMODE)
	{
		int position = MinHeight+(MaxHeight-MinHeight)*(SelInd+SelItem)/(EntrieCount-1);

		if(position < MinHeight)
			position = MinHeight;
		else if(position > MaxHeight || (SelInd+SelItem >= EntrieCount-1))
			position = MaxHeight;

		scrollbarBoxBtn->SetPosition(0, position);
	}
}

void Scrollbar::Draw()
{
	scrollbarTileImg->Draw();
	scrollbarTopImg->Draw();
	scrollbarButtomImg->Draw();
	arrowUpBtn->Draw();
	arrowDownBtn->Draw();
	scrollbarBoxBtn->Draw();

	++ScrollState;

	UpdateEffects();
}

void Scrollbar::Update(GuiTrigger * t)
{
	arrowUpBtn->Update(t);
	arrowDownBtn->Update(t);
	scrollbarBoxBtn->Update(t);
	
	if(arrowUpBtn->GetState() == STATE_HELD)
		OnUpButtonHold();
	if(arrowDownBtn->GetState() == STATE_HELD)
		OnDownButtonHold();
	if(scrollbarBoxBtn->GetState() == STATE_HELD)
		OnBoxButtonHold(t);

	if(listchanged)
		SetScrollboxPosition();
}
