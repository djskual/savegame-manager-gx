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
#include "gui_element.h"
#include "gui_image.h"
#include "gui_button.h"
#include "gui_text.h"
#include "OptionList.hpp"
#include "../Controls/Scrollbar.hpp"

#define OPTIONMAX		9

//!Display a list of menu options
class GuiOptionBrowser : public GuiElement, public sigslot::has_slots<>
{
	public:
		GuiOptionBrowser(OptionList * l);
		virtual ~GuiOptionBrowser();
		void Draw();
		void Refresh() { OnListChange(selectedItem, listOffset); }
		void Update(GuiTrigger * t);
		
		//! Signal
		sigslot::signal2<GuiOptionBrowser *, int> Clicked;
	protected:
		void OnButtonClick(GuiButton *sender, int pointer, const POINT &p);
		void OnListChange(int selItem, int selIndex);
		void SetCol2Position(int x);
		int selectedItem;
		int listOffset;
		int coL2;
		bool listChanged;
		
		OptionList *options;
		GuiButton * optionBtn[OPTIONMAX];
		GuiText * optionTxt[OPTIONMAX];
		GuiImage * optionBg[OPTIONMAX];
		GuiText * optionVal[OPTIONMAX];
		
		Scrollbar * scrollbar;
		
		GuiImage * bgOptionsImg;
		
		GuiImageData * bgOptions;
		GuiImageData * bgOptionsEntry;
		
		GuiSound * btnSoundClick;
		GuiTrigger * trigA;
};
