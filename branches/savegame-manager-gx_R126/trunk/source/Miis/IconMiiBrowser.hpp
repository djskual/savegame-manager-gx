/***************************************************************************
 * Copyright (C) 2010
 * by Dimok
 * modified by Dj_Skual
 * for SaveGame Manager Gx 2011
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
#ifndef __ICONMIIBROWSER_HPP_
#define __ICONMIIBROWSER_HPP_

#include "../Controls/Scrollbar.hpp"
#include "../Files/GuiFileBrowser.hpp"
#include "../LibWiiGui/gui.h"
#include "../Tools/timer.h"

using namespace std;

class IconMiiBrowser : public GuiFileBrowser
{
	public:
		IconMiiBrowser(Browser * filebrowser, GuiImageData * browserBg, int w, int h);
		~IconMiiBrowser();
		void ResetState();
		void SetSelected(int i);
		void SetTriggerUpdate(bool t) { triggerupdate = t; };
		void TriggerUpdate() { listChanged = true; };
		void Draw();
		void Update(GuiTrigger * t);
		
	protected:
		void AddButton();
		void SetButton(int i, std::string name, bool enable, int x, int y);
		void RemoveButton(int i);
		
		u16 RowSize;
		u16 PageSize;
		int selectedItem;
		int numEntries;
		int oldSelectedItem;
		bool listChanged;
		bool triggerupdate;

		vector<GuiText *> ButtonText;
		vector<GuiImage *> FileSelectionImg;
		vector<GuiImage *> ButtonImg;
		vector<GuiButton *> Buttons;
		vector<GuiTooltip *> Tooltip;
		
		Scrollbar * scrollbar;

		GuiImage * bgSelectionImg;

		GuiImageData * bgSelectionEntry;
		GuiImageData * fileMii;
		

		GuiTrigger * trigA;

		Timer SelectTimer;
};

#endif
