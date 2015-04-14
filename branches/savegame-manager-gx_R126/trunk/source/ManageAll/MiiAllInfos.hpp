/****************************************************************************
 * Copyright (C) 2011
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
 *
 * MiiAllInfos.hpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#ifndef _MIIALLINFOS_HPP_
#define _MIIALLINFOS_HPP_

#include <string>

#include "../LibWiiGui/gui.h"

class MiiAllInfos : public GuiWindow
{
	public:
		MiiAllInfos(int Selected,
					int EmuSelected,
					std::string Creator = "",
					int Female = 0,
					int Day = 0,
					int Month = 0,
					int FavColor = 0);
		virtual ~MiiAllInfos();
		void SetInfos(int Selected,
					  int EmuSelected,
					  std::string Creator = "",
					  int Female = 0,
					  int Day = 0,
					  int Month = 0,
					  int FavColor = 0);
		
	protected:
		std::string GetCreator();
		std::string GetDate();
		GXColor GetFavColor();
		
		int miiSelected;
		int emuMiiSelected;
		
		std::string miiCreator;
		int miiFemale;
		int miiDay;
		int miiMonth;
		int miiFavColor;
		
		GuiImageData * creatorIcon;
		GuiImageData * dateIcon;
		GuiImageData * genderIcon;
		
		GuiImage * creatorImg;
		GuiImage * dateImg;
		GuiImage * genderImg;
		GuiImage * favColorImg;
		
		GuiText * creatorTxt;
		GuiText * dateTxt;
		
		GuiButton * btnCreator;
		GuiButton * btnDate;
};

#endif
