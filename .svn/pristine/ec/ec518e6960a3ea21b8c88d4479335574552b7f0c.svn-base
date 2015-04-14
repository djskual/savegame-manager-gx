/***************************************************************************
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
 * ProgressThrobber.hpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#ifndef _PROGRESSTHROBBER_HPP_
#define _PROGRESSTHROBBER_HPP_

#include "../LibWiiGui/gui.h"
#include "../Tools/timer.h"

class ProgressThrobber : public GuiElement
{
	public:
		ProgressThrobber(int i);
		virtual ~ProgressThrobber();
		
		void Draw();
		void Update(GuiTrigger * t);
		
	private:
		int throbberCount;
		
		/** Window **/
		GuiWindow * window;
		
		/** ImageDatas **/
		GuiImageData * throbberBg;
		GuiImageData * throbber;
		
		/** Images **/
		GuiImage * throbberBgImg;
		GuiImage * throbberCentreImg;
		GuiImage * throbber1Img;
		GuiImage * throbber2Img;
		GuiImage * throbber3Img;
		GuiImage * throbber4Img;
		GuiImage * throbber5Img;
		GuiImage * throbber6Img;
		GuiImage * throbber7Img;
		GuiImage * throbber8Img;
		
		/** Timer **/
		Timer throbberTimer;
};

#endif