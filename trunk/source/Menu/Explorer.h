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
 #ifndef __EXPLORER_H_
 #define __EXPLORER_H_

#include "../Gui/gui.h"
#include "../Files/filebrowser.h"
#include "../Files/GuiFileBrowser.hpp"
#include "../Files/Browser.hpp"

class Explorer : public GuiFrame, public sigslot::has_slots<>
{
	public:
		Explorer(GuiFrame *parent = 0, std::string path = NULL);
		virtual ~Explorer();
		
		int LoadPath(std::string path);
		std::string GetCurrectPath() { return fileBrowser->GetCurrentPath(); }
		std::string GetRootDir() { return fileBrowser->GetRootDir(); }
		
		int FilterList(std::wstring fileFilter = L"", bool resetBrowser = false) { return fileBrowser->FilterList(fileFilter, resetBrowser); }
		std::wstring GetCurrentFilter() { return fileBrowser->GetCurrentFilter(); }
		std::set<wchar_t> GetSearchChars() { return fileBrowser->GetSearchChars(); }
		
		void show();
		void hide();
		void Refresh();
		
	protected:
		void Init();
		void OnBrowserChanges(int index);
		void BackInDirectory(GuiButton *sender, int pointer, const POINT &p);
		
		FileBrowser *fileBrowser;
		Browser * curBrowser;
		GuiFileBrowser * guiBrowser;
		int guiBrowserType;
		
		GuiImage * BackgroundImg;
		
		GuiImageData * Background;
		
		GuiButton * BackInDirBtn;
		
		SimpleGuiTrigger * trigA;
		GuiTrigger * trigBackInDir;
};

#endif
