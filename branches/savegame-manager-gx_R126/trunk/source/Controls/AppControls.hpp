/****************************************************************************
 * Copyright (C) 2010
 * by Dimok
 * modified by dj_skual
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
 * AppControls.hpp
 *
 * for SaveGame Manager GX 2012
 ***************************************************************************/
#ifndef APP_CONTROLS_HPP_
#define APP_CONTROLS_HPP_

#include <gccore.h>
#include <string>

typedef struct _Controls
{
	u16	 ClickButton;
	u16	 BackButton;
	u16	 UpButton;
	u16	 DownButton;
	u16	 LeftButton;
	u16	 RightButton;
	u16	 NextButton;
	u16	 PrevButton;
	u16	 HomeButton;
	u16	 UpInDirectory;
} ControlItems;

class AppControls
{
	public:
		AppControls();
		~AppControls();
		void SetDefault();
		bool Load(std::string filepath);
		bool Save();
		bool SetControl(char *name, char *value);
		
		ControlItems WiiControls;
		ControlItems ClassicControls;
		ControlItems GCControls;
		u16 ScreenshotHoldButton;
		u16 ScreenshotClickButton;
		
	protected:
		std::string FilePath;
		void ParseLine(char *line);
		void TrimLine(char *dest, char *src, int size);
};

#endif
