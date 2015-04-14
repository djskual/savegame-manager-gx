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
#ifndef SCROLLBAR_HPP_
#define SCROLLBAR_HPP_

#include "../GUI/gui_element.h"
#include "../GUI/gui_button.h"
#include "../GUI/PictureButton.h"

class Scrollbar : public GuiElement, public sigslot::has_slots<>
{
	public:
		Scrollbar(int height, u8 mode = LISTMODE);
		virtual ~Scrollbar();
		
		void SetDPadControl(bool a) { AllowDPad = a; }
		void SetButtonScroll(u32 wiibtns, u16 gcbtns) { ButtonScroll = wiibtns; ButtonScrollGC = gcbtns; }
		void ScrollOneUp();
		void ScrollOneDown();
		int GetSelectedItem() { return SelItem; }
		int GetSelectedIndex() { return SelInd; }
		void SetScrollSpeed(u16 speed) { ScrollSpeed = speed; }
		void SetButtonScrollSpeed(u16 speed) { ButtonScrollSpeed = speed; }
		void SetBounds(iRect w);
		void Draw();
		void Update(GuiTrigger * t);
		
		enum
		{
			ICONMODE = 0,
			LISTMODE,
			CUSTOMMODE
		};
		
		void SetPageSize(int size);
		void SetRowSize(int size);
		void SetSelectedItem(int pos);
		void SetSelectedIndex(int pos);
		void SetSelected(int selItem, int selIndex);
		void SetEntrieCount(int cnt);
		
		sigslot::signal2<int, int> listChanged;
		
	protected:
		void setScrollboxPosition(int SelItem, int SelInd);
		void CheckDPadControls(GuiTrigger *t);
		void ScrollByButton(GuiTrigger *t);
		
		void OnUpButtonHold(GuiButton *sender, int pointer, const POINT &p);
		void OnDownButtonHold(GuiButton *sender, int pointer, const POINT &p);
		void OnBoxButtonHold(GuiButton *sender, int pointer, const POINT &p);
		
		u8 Mode;
		u32 ScrollState;
		u16 ScrollSpeed;
		u16 ButtonScrollSpeed;
		u32 ButtonScroll;
		u16 ButtonScrollGC;
		bool AllowDPad;
		
		int MinHeight;
		int MaxHeight;
		int SelItem;
		int SelInd;
		int RowSize;
		int PageSize;
		int EntrieCount;
		int ButtonPositionX;
		int pressedChan;
		bool listchanged;
		
		PictureButton * arrowUpBtn;
		PictureButton * arrowDownBtn;
		PictureButton * scrollbarBoxBtn;
		
		GuiImage * scrollbarTopImg;
		GuiImage * scrollbarBottomImg;
		GuiImage * scrollbarTileImg;
		GuiImage * oneButtonScrollImg;
		
		GuiImageData * scrollbarTop;
		GuiImageData * scrollbarBottom;
		GuiImageData * scrollbarTile;
		GuiImageData * oneButtonScrollImgData;
		
		GuiSound * btnSoundOver;
		GuiSound * btnSoundClick;
		
		GuiTrigger * trigHeldA;
};

#endif
