/****************************************************************************
 * Copyright (C) 2010
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
#ifndef FLYINGBUTTONSMENU_HPP_
#define FLYINGBUTTONSMENU_HPP_

#include <vector>
#include <string>

#include "../../GUI/gui.h"

class FlyingButtonsMenu : public GuiFrame, public sigslot::has_slots<>
{
	public:
		FlyingButtonsMenu();
		virtual ~FlyingButtonsMenu();
		virtual void hide();
		virtual void show();
		virtual void FadeIn();
		virtual void FadeOut();
		
		sigslot::signal1<int> FlyingBtnMenuClicked;
		
	protected:
		virtual void SetupMainButtons() { };
		virtual void AddMainButtons();
		virtual void ShowButtonsEffects(int effect, int effectSpeed);
		virtual void SlideButtons(int slideDirection);
		virtual void SetPageIndicators();
		virtual void SetMainButton(int position, std::string buttonText, GuiImageData * imageData, GuiImageData * imageOver);
		
		void OnButtonClick(GuiButton *sender, int pointer, const POINT &p);
		
		int currentPage;
		int firstIndicator;
		enum
		{
			SLIDE_LEFT, SLIDE_RIGHT
		};
		
		GuiImageData * pageIndicatorImgData;
		GuiImageData * arrowLeft;
		GuiImageData * arrowRight;
		
		GuiImage * goLeftImg;
		GuiImage * goRightImg;
		
		GuiTrigger * trigA;
		GuiTrigger * trigB;
		GuiTrigger * trigL;
		GuiTrigger * trigR;
		GuiTrigger * trigMinus;
		GuiTrigger * trigPlus;
		
		GuiButton * goLeftBtn;
		GuiButton * goRightBtn;
		
		GuiSound * btnSoundClick;
		GuiSound * btnSoundOver;
		
		std::vector<GuiImage *> pageIndicatorImg;
		std::vector<GuiText *> pageIndicatorTxt;
		std::vector<GuiButton *> pageIndicatorBtn;
		
		std::vector<GuiImage *> mainButtonImg;
		std::vector<GuiImage *> mainButtonImgOver;
		std::vector<GuiText *> mainButtonTxt;
		std::vector<GuiButton *> mainButton;
};

#endif
