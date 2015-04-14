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
 * MiiInfos.hpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#ifndef _MIIINFOS_HPP_
#define _MIIINFOS_HPP_

#include <string>

#include "../LibWiiGui/gui.h"

class MiiInfos : public GuiWindow
{
	public:
		MiiInfos(int Selected, int Source);
		virtual ~MiiInfos();
		
	protected:
		void GetInfos(std::string miipath);
		void GetDeviceCreator(std::string miipath);
		std::string GetCreator();
		std::string GetDate();
		GXColor GetFavColor();
		
		int miiSelected;
		int source;
		
		std::string Creator;
		int Female;
		int Day;
		int Month;
		int FavColor;
		
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
