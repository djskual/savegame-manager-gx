/****************************************************************************
 * Copyright (C) 2011 Dimok
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
#include "WiiPointer.h"
#include "../Settings/CSettings.h"
#include "../Themes/CTheme.h"
#include "../System/input.h"
#include "../System/video.h"

WiiPointer::WiiPointer(const char *pntrImg)
	: posX(screenwidth/2), posY(screenheight/2),
	  angle(0.0f), Visible(false), lastActivity(180)
{
	pointer = Resources::GetImageData(pntrImg);
}

WiiPointer::~WiiPointer()
{
	Resources::Remove(pointer);
}

/*void WiiPointer::SetImage(const char *pntrImg)
{
	Visible = false;
	
	GuiImageData * newPointer = Resources::GetImageData(pntrImg);
	if(!newPointer)
		return;
	
	//! let's save us the work with mutex here
	GuiImageData * temp2 = pointer;
	pointer = newPointer;
	delete temp2;
}*/

void WiiPointer::Update(GuiTrigger *t)
{
	if(t && pointer)
	{
		if(t->wpad->ir.valid)
		{
			lastActivity = 0;
			posX = LIMIT(t->wpad->ir.x, 5, screenwidth-5);
			posY = LIMIT(t->wpad->ir.y, 5, screenheight-5);
			angle = t->wpad->ir.angle;
			Visible = true;
		}
		else
		{
			angle = 0.0f;
			//! GC PAD
			//! x-axis
			if(t->pad.stickX < -PADCAL)
			{
				posX += (t->pad.stickX + PADCAL) * Settings.PointerSpeed;
				lastActivity = 0;
			}
			else if(t->pad.stickX > PADCAL)
			{
				posX += (t->pad.stickX - PADCAL) * Settings.PointerSpeed;
				lastActivity = 0;
			}
			//! y-axis
			if(t->pad.stickY < -PADCAL)
			{
				posY -= (t->pad.stickY + PADCAL) * Settings.PointerSpeed;
				lastActivity = 0;
			}
			else if(t->pad.stickY > PADCAL)
			{
				posY -= (t->pad.stickY - PADCAL) * Settings.PointerSpeed;
				lastActivity = 0;
			}
			
			int wpadX = t->WPAD_Stick(0, 0);
			int wpadY = t->WPAD_Stick(0, 1);
			
			//! Wii Extensions
			//! x-axis
			if(wpadX < -PADCAL)
			{
				posX += (wpadX + PADCAL) * Settings.PointerSpeed;
				lastActivity = 0;
			}
			else if(wpadX > PADCAL)
			{
				posX += (wpadX - PADCAL) * Settings.PointerSpeed;
				lastActivity = 0;
			}
			//! y-axis
			if(wpadY < -PADCAL)
			{
				posY -= (wpadY + PADCAL) * Settings.PointerSpeed;
				lastActivity = 0;
			}
			else if(wpadY > PADCAL)
			{
				posY -= (wpadY - PADCAL) * Settings.PointerSpeed;
				lastActivity = 0;
			}
			
			if(t->pad.btns_h || t->wpad->btns_h)
				lastActivity = 0;
			
			posX = LIMIT(posX, 5, screenwidth-5);
			posY = LIMIT(posY, 5, screenheight-5);
			
			if(lastActivity < 180) //! (3s on 60Hz and 3.6s on 50Hz)
			{
				t->wpaddata.ir.valid = 1;
				t->wpaddata.ir.x = posX;
				t->wpaddata.ir.y = posY;
				t->wpaddata.ir.angle = angle;
				Visible = true;
			}
			else
				Visible = false;
		}
	}
	
	++lastActivity;
}

void WiiPointer::Draw()
{
	if(Visible)
	{
		Menu_DrawImg(pointer->GetImage(), pointer->GetWidth(),
					 pointer->GetHeight(), GX_TF_RGBA8,
					 posX-pointer->GetWidth()/2,
					 posY-pointer->GetHeight()/2, 100.0f,
					 angle, 1, 1, 255);
	}
}