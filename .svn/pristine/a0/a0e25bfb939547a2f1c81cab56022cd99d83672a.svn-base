/****************************************************************************
 * Copyright (C) 2009
 * by Dimok
 * modified by Dj_Skual
 * for SaveGame Manager Gx 2011
 *
 * Original Filebrowser by Tantric for libwiigui
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
 * Browser Class
 *
 * Virtual class to inherit the browsers off
 * introducing the minimum of functions need to browse
 *
 * for WiiXplorer 2009
 ***************************************************************************/
#include "Browser.hpp"
#include "../main.h"

void Browser::MarkCurrentItem()
{
	ItemStruct * Item = GetCurrentItemStruct();
	
	if(Item)
	{
		IMarker.AddItem(Item);
		
		if(Item->itempath)
			free(Item->itempath);
		delete Item;
	}
}

void Browser::UnMarkCurrentItem()
{
	ItemStruct * Item = GetCurrentItemStruct();
	
	if(Item)
	{
		IMarker.RemoveItem(Item);
		
		if(Item->itempath)
			free(Item->itempath);
		delete Item;
	}
}

void Browser::MarkAllItems()
{
	for(int i = 0; i < GetEntrieCount(); i++)
	{
		ItemStruct * Item = GetItemStruct(i);
		
		if(Item)
		{
			IMarker.AddItem(Item);
			
			if(Item->itempath)
				free(Item->itempath);
			delete Item;
		}
	}
}

void Browser::UpdateMarker(GuiTrigger * t)
{
	/*if(t->wpad.btns_d & WPAD_BUTTON_MINUS ||
	   t->wpad.btns_d & WPAD_CLASSIC_BUTTON_MINUS ||
	   t->pad.btns_d & PAD_BUTTON_X)
	{
		this->ResetMarker();
	}
	else if(t->wpad.btns_d & WPAD_BUTTON_1 ||
			t->wpad.btns_d & WPAD_CLASSIC_BUTTON_X ||
			t->pad.btns_d & PAD_TRIGGER_R)
	{
		DelayCounter = 0;
		this->MarkCurrentItem();
	}
	else if(t->wpad.btns_d & WPAD_BUTTON_2 ||
			t->wpad.btns_d & WPAD_CLASSIC_BUTTON_Y ||
			t->pad.btns_d & PAD_TRIGGER_L)
	{
		this->UnMarkCurrentItem();
	}
	else if(t->wpad.btns_h & WPAD_BUTTON_1 ||
	   t->wpad.btns_h & WPAD_CLASSIC_BUTTON_X ||
	   t->pad.btns_h & PAD_TRIGGER_R)
	{
		++DelayCounter;
		
		if(DelayCounter == 40)
		{
			this->MarkAllItems();
		}
	}*/
}
