/***************************************************************************
 * Copyright (C) 2011
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
 ***************************************************************************/
#include "../Themes/CTheme.h"
#include "Scrollbar.hpp"
#include "../Tools/tools.h"
#include "../GUI/gui_trigger.h"
#include "../System/input.h"

Scrollbar::Scrollbar(int h, u8 m)
{
	SelItem = 0;
	SelInd = 0;
	RowSize = 0;
	PageSize = 0;
	EntrieCount = 0;
	ScrollSpeed = 15;
	ButtonScroll = 0;
	ButtonScrollGC = 0;
	ButtonScrollSpeed = 20;
	ScrollState = 0;
	pressedChan = -1;
	AllowDPad = true;
	Mode = m;
	height = h;
	listChanged.connect(this, &Scrollbar::setScrollboxPosition);

	btnSoundOver = Resources::GetSound("button_over.wav");
	btnSoundClick = Resources::GetSound("button_click.wav");
	
	trigHeldA = new GuiTrigger;
	trigHeldA->SetHeldTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);

	if(Mode != CUSTOMMODE)
	{
		scrollbarTop = Resources::GetImageData("scrollbar_top.png");
		scrollbarBottom = Resources::GetImageData("scrollbar_bottom.png");
		scrollbarTile = Resources::GetImageData("scrollbar_tile.png");
	}
	else
	{
		scrollbarTop = Resources::GetImageData("scrollbar_custom_top.png");
		scrollbarBottom = Resources::GetImageData("scrollbar_custom_bottom.png");
		scrollbarTile = Resources::GetImageData("scrollbar_custom_tile.png");
	}
	
	oneButtonScrollImgData = Resources::GetImageData("scroll_one_button.png");
	oneButtonScrollImg = new GuiImage(oneButtonScrollImgData);

	ButtonPositionX = (Mode == CUSTOMMODE ? 0 : 3);
	
	arrowUpBtn = new PictureButton("scrollbar_arrowup.png", "scrollbar_arrowup.png", btnSoundClick, btnSoundOver);
	arrowUpBtn->SetParent(this);
	arrowUpBtn->SetAlignment(ALIGN_CENTER | ALIGN_TOP);
	arrowUpBtn->SetPosition(ButtonPositionX, 0);
	arrowUpBtn->SetTrigger(trigHeldA);
	arrowUpBtn->SetHoldable(true);
	arrowUpBtn->Held.connect(this, &Scrollbar::OnUpButtonHold);
	
	arrowDownBtn = new PictureButton("scrollbar_arrowdown.png", "scrollbar_arrowdown.png", btnSoundClick, btnSoundOver);
	arrowDownBtn->SetParent(this);
	arrowDownBtn->SetAlignment(ALIGN_CENTER | ALIGN_BOTTOM);
	arrowDownBtn->SetPosition(ButtonPositionX, 0);
	arrowDownBtn->SetTrigger(trigHeldA);
	arrowDownBtn->SetHoldable(true);
	arrowDownBtn->Held.connect(this, &Scrollbar::OnDownButtonHold);

	MinHeight = (Mode == CUSTOMMODE ? arrowUpBtn->GetHeight()+3 : 23);
	
	scrollbarBoxBtn = new PictureButton("scrollbar_box.png", "scrollbar_box.png");
	scrollbarBoxBtn->SetParent(this);
	scrollbarBoxBtn->SetAlignment(ALIGN_CENTER | ALIGN_TOP);
	scrollbarBoxBtn->SetPosition(ButtonPositionX, MinHeight);
	scrollbarBoxBtn->SetTrigger(trigHeldA);
	scrollbarBoxBtn->SetHoldable(true);
	scrollbarBoxBtn->Held.connect(this, &Scrollbar::OnBoxButtonHold);
	
	int PositionX = (Mode == CUSTOMMODE ? 0 : 4);
	int PositionY = (Mode == CUSTOMMODE ? arrowUpBtn->GetHeight() : 10);
	width = MAX(scrollbarBoxBtn->GetWidth(), scrollbarTile->GetWidth());
	MaxHeight = height-MinHeight-scrollbarBoxBtn->GetHeight();
	
	int Tiles = (height-scrollbarTop->GetHeight()-scrollbarBottom->GetHeight()-(PositionY*2))/scrollbarTile->GetHeight();
	scrollbarTopImg = new GuiImage(scrollbarTop);
	scrollbarTopImg->SetParent(this);
	scrollbarTopImg->SetAlignment(ALIGN_CENTER | ALIGN_TOP);
	scrollbarTopImg->SetPosition(PositionX, PositionY);
	PositionY += scrollbarTop->GetHeight();

	scrollbarTileImg = new GuiImage(scrollbarTile);
	scrollbarTileImg->SetParent(this);
	scrollbarTileImg->SetAlignment(ALIGN_CENTER | ALIGN_TOP);
	scrollbarTileImg->SetPosition(PositionX, PositionY);
	scrollbarTileImg->SetTileVertical(Tiles);
	PositionY += Tiles*scrollbarTile->GetHeight();

	scrollbarBottomImg = new GuiImage(scrollbarBottom);
	scrollbarBottomImg->SetParent(this);
	scrollbarBottomImg->SetAlignment(ALIGN_CENTER | ALIGN_TOP);
	scrollbarBottomImg->SetPosition(PositionX, PositionY);
}

Scrollbar::~Scrollbar()
{
	Resources::Remove(btnSoundOver);
	Resources::Remove(btnSoundClick);
	Resources::Remove(scrollbarTop);
	Resources::Remove(scrollbarBottom);
	Resources::Remove(scrollbarTile);
	Resources::Remove(oneButtonScrollImgData);

	delete arrowUpBtn;
	delete arrowDownBtn;
	delete scrollbarBoxBtn;

	delete scrollbarTopImg;
	delete scrollbarBottomImg;
	delete scrollbarTileImg;
	delete oneButtonScrollImg;

	delete trigHeldA;
}

void Scrollbar::ScrollOneUp()
{
	if(Mode == ICONMODE)
	{
		if(SelInd+SelItem-RowSize >= 0)
		{
			SelItem -= RowSize;
			if(SelItem < 0)
			{
				//! move list up by 1
				SelInd -= RowSize;
				SelItem += RowSize;
			}
		}
	}
	else
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
}

void Scrollbar::ScrollOneDown()
{
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
			SelItem += RowSize;
			if(SelItem >= PageSize)
			{
				//! move list down by 1
				SelInd += RowSize;
				SelItem -= RowSize;
			}
		}
	}
	else
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
}

void Scrollbar::OnUpButtonHold(GuiButton *sender UNUSED, int pointer UNUSED, const POINT &p UNUSED)
{
	if(ScrollState < ScrollSpeed)
		return;

	ScrollOneUp();

	ScrollState = 0;
	listChanged(SelItem, SelInd);
}

void Scrollbar::OnDownButtonHold(GuiButton *sender UNUSED, int pointer UNUSED, const POINT &p UNUSED)
{
	if(ScrollState < ScrollSpeed)
		return;

	ScrollOneDown();

	ScrollState = 0;
	listChanged(SelItem, SelInd);
}

void Scrollbar::OnBoxButtonHold(GuiButton *sender UNUSED, int pointer, const POINT &p)
{
	if(ScrollState < ScrollSpeed)
		return;

	if(!userInput[pointer].wpad->ir.valid)
		return;

	int y = p.y-this->GetTop()-scrollbarBoxBtn->GetHeight()/2;

	int positionWiimote = LIMIT(y-MinHeight, 0, MaxHeight-MinHeight);

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
	else
	{
		int diff = newSelected-SelInd-SelItem;

		if(newSelected <= 0)
		{
			SelItem = 0;
			SelInd = 0;
		}
		else if(newSelected >= EntrieCount-1)
		{
			SelItem = (PageSize-1 < EntrieCount-1) ? PageSize-1 : EntrieCount-1;
			SelInd = EntrieCount-PageSize;
		}
		else if(newSelected < PageSize && SelInd == 0 && diff < 0)
		{
			SelItem = MAX(SelItem+diff, 0);
		}
		else if(EntrieCount-newSelected < PageSize && SelInd == EntrieCount-PageSize && diff > 0)
		{
			SelItem = MIN(SelItem+diff, PageSize-1);
		}
		else
		{
			SelInd = LIMIT(SelInd+diff, 0, ((EntrieCount-PageSize < 0) ? 0 : EntrieCount-PageSize));
		}
	}

	ScrollState = 0;
	listChanged(SelItem, SelInd);
}

void Scrollbar::SetPageSize(int size)
{
	if(PageSize == size)
		return;

	PageSize = size;
	listChanged(SelItem, SelInd);
}

void Scrollbar::SetRowSize(int size)
{
	if(RowSize == size)
		return;

	RowSize = size;
	listChanged(SelItem, SelInd);
}

void Scrollbar::SetSelectedItem(int pos)
{
	if(SelItem == pos)
		return;

	SelItem = pos;
	listChanged(SelItem, SelInd);
}

void Scrollbar::SetSelectedIndex(int pos)
{
	if(SelInd == pos)
		return;

	SelInd = pos;
	listChanged(SelItem, SelInd);
}

void Scrollbar::SetSelected(int selItem, int selIndex)
{
	if((SelItem == selItem) && (SelInd == selIndex))
		return;

	SelItem = selItem;
	SelInd = selIndex;
	listChanged(SelItem, SelInd);
}

void Scrollbar::SetEntrieCount(int cnt)
{
	if(EntrieCount == cnt)
		return;

	EntrieCount = cnt;
	listChanged(SelItem, SelInd);
}

void Scrollbar::setScrollboxPosition(int SelItem, int SelInd)
{
	if(Mode == ICONMODE)
	{
		u8 row = (u8) floor((float) SelItem / (float) RowSize);
		
		int position = MinHeight+(MaxHeight-MinHeight)*(SelInd+row*RowSize)/(EntrieCount > 1 ? EntrieCount-1 : 1);
		
		if(position < MinHeight || EntrieCount <= 1)
			position = MinHeight;
		else if(position > MaxHeight || ((SelInd+PageSize >= EntrieCount-1) && row > 1))
			position = MaxHeight;
		
		scrollbarBoxBtn->SetPosition(ButtonPositionX, position);
	}
	else
	{
		int position = MinHeight+(MaxHeight-MinHeight)*(SelInd+SelItem)/(EntrieCount > 1 ? EntrieCount-1 : 1);
		
		if(position < MinHeight || EntrieCount <= 1)
			position = MinHeight;
		else if(position > MaxHeight || (SelInd+SelItem >= EntrieCount-1))
			position = MaxHeight;
		
		scrollbarBoxBtn->SetPosition(ButtonPositionX, position);
	}
}

void Scrollbar::CheckDPadControls(GuiTrigger *t)
{
	if(t->Up())
	{
		ScrollOneUp();
		listChanged(SelItem, SelInd);
	}
	else if(t->Down())
	{
		ScrollOneDown();
		listChanged(SelItem, SelInd);
	}

	else if(t->Left())
	{
		SelInd -= PageSize;
		if(SelInd < 0)
		{
			SelInd = 0;
			SelItem = 0;
		}
		listChanged(SelItem, SelInd);
	}
	else if(t->Right())
	{
		SelInd += PageSize;
		if(SelInd+PageSize >= EntrieCount)
		{
			SelInd = MAX(EntrieCount-PageSize, 0);
			SelItem = LIMIT(PageSize-1, 0, EntrieCount-1);
		}
		listChanged(SelItem, SelInd);
	}
}

void Scrollbar::ScrollByButton(GuiTrigger *t)
{
	static int pressedPosition = -1;

	if(!t->wpad->ir.valid || ScrollState < ButtonScrollSpeed-ButtonScrollSpeed*fabs(pressedPosition-t->wpad->ir.y)/250.f)
		return;

	if(pressedChan == -1 && ((t->wpad->btns_d & ButtonScroll) || (t->pad.btns_d & ButtonScrollGC)) &&
	   parentElement && parentElement->IsInside(t->wpad->ir.x, t->wpad->ir.y))
	{
		pressedPosition = t->wpad->ir.y;
		pressedChan = t->chan;
		oneButtonScrollImg->SetPosition(t->wpad->ir.x-oneButtonScrollImg->GetWidth()/2, t->wpad->ir.y-oneButtonScrollImg->GetHeight()/2);
	}

	if(pressedChan == t->chan && ((t->wpad->btns_h & ButtonScroll) || (t->pad.btns_h & ButtonScrollGC)))
	{
		if(pressedPosition-oneButtonScrollImg->GetHeight()/2 > t->wpad->ir.y)
			ScrollOneUp();
		else if(pressedPosition+oneButtonScrollImg->GetHeight()/2 < t->wpad->ir.y)
			ScrollOneDown();

		ScrollState = 0;
		listChanged(SelItem, SelInd);
	}

	if(pressedChan == t->chan && !t->wpad->btns_d && !t->wpad->btns_h&& !t->pad.btns_d  && !t->pad.btns_h)
	{
		pressedChan = -1;
		pressedPosition = -1;
	}
}

void Scrollbar::Draw()
{
	scrollbarTileImg->Draw();
	scrollbarTopImg->Draw();
	scrollbarBottomImg->Draw();
	arrowUpBtn->Draw();
	arrowDownBtn->Draw();
	scrollbarBoxBtn->Draw();
	if(pressedChan >= 0 && userInput[pressedChan].wpad->ir.valid)
		oneButtonScrollImg->Draw();

	UpdateEffects();
}

void Scrollbar::Update(GuiTrigger * t)
{
	arrowUpBtn->Update(t);
	arrowDownBtn->Update(t);
	scrollbarBoxBtn->Update(t);
	if(AllowDPad)
		CheckDPadControls(t);
	if(ButtonScroll || ButtonScrollGC)
		ScrollByButton(t);

	++ScrollState;
}

void Scrollbar::SetBounds(iRect w)
{
	GuiElement::SetBounds(w);
	scrollbarTopImg->SetBounds(w);
	scrollbarBottomImg->SetBounds(w);
	scrollbarTileImg->SetBounds(w);
	oneButtonScrollImg->SetBounds(w);
	arrowUpBtn->SetBounds(w);
	arrowDownBtn->SetBounds(w);
	scrollbarBoxBtn->SetBounds(w);
}
