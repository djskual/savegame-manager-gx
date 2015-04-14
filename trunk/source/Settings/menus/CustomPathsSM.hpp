/****************************************************************************
 * Copyright (C) 2012
 * by Dj_Skual
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
#ifndef CUSTOMPATHSMENU_HPP_
#define CUSTOMPATHSMENU_HPP_

#include "SettingsBrowser.hpp"
#include "../../Prompts/PathBrowser.h"

class CustomPathsSM : public SettingsBrowser
{
	public:
		CustomPathsSM();
		
	protected:
		virtual void SetupOptionName();
		virtual void SetOptionValues();
		virtual void OnBrowserClick(GuiOptionBrowser * browser, int option);
		
		void ChangePath(char *path, int size, u8 filter = FILTER_PATH);
		void OnPathBrowserButtonClick(PathBrowser *menu, u8 filter, bool doneClicked, const std::string &Path);
		
		char * setPath;
		int setPathSize;
};

#endif
