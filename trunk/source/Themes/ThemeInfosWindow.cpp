/****************************************************************************
 * Copyright (C) 2012 Dj_Skual
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
#include "ThemeInfosWindow.h"
#include "CTheme.h"
#include "../Controls/Application.h"

/****************************************************************************
* ThemeInfosWindow Class
*
* Displays a prompt window to user, with information information about the
* selected Theme.
***************************************************************************/
ThemeInfosWindow::ThemeInfosWindow(const char * title, const char * author, int type)
	: GuiFrame(0, 0)
{
	choice = -1;
	
	char thumbpath[300];
	snprintf(thumbpath, sizeof(thumbpath), "%s/tmp/%s.jpg", Settings.ThemePath, title);
	thumbImageData = new GuiImageData(thumbpath);
	
	btnSoundClick = Resources::GetSound("button_click.wav");
	btnSoundOver = Resources::GetSound("button_over.wav");

	trigA = new SimpleGuiTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	trigB = new GuiTrigger();
	trigB->SetButtonOnlyTrigger(-1, WiiControls.BackButton | ClassicControls.BackButton << 16, GCControls.BackButton);

	bgWindowData = Resources::GetImageData("theme_dialogue_box.png");
	bgWindowImg = new GuiImage(bgWindowData);
	width = bgWindowData->GetWidth();
	height = bgWindowData->GetHeight();

	titleTxt = new GuiText(tr( "Theme Title:" ), 18, thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
	titleTxt->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	titleTxt->SetPosition(230, 30);

	titleTxt2 = new GuiText(title, 18, thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
	titleTxt2->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	titleTxt2->SetPosition(230, 50);
	titleTxt2->SetMaxWidth(width - 220, WRAP);

	authorTxt = new GuiText(tr( "Author(s):" ), 18, thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
	authorTxt->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	authorTxt->SetPosition(230, 100);

	authorTxt2 = new GuiText(author, 18, thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
	authorTxt2->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	authorTxt2->SetPosition(230, 120);
	authorTxt2->SetMaxWidth(width - 220, DOTTED);

	actionBtnTxt = new GuiText((type == DOWNLOAD) ? tr( "Download" ) : tr( "Load" ), 22, thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
	actionBtn = new PictureButton("button.png", "button.png", btnSoundClick, btnSoundOver);
	actionBtn->SetAlignment(ALIGN_RIGHT | ALIGN_TOP);
	actionBtn->SetPosition(-5, 170);
	actionBtn->SetLabel(actionBtnTxt);
	actionBtn->SetTrigger(trigA);
	actionBtn->SetScale(0.8);
	actionBtn->Clicked.connect(this, &ThemeInfosWindow::OnButtonClick);

	backBtnTxt = new GuiText(tr( "Back" ), 22, thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
	backBtn = new PictureButton("button.png", "button.png", btnSoundClick, btnSoundOver);
	backBtn->SetAlignment(ALIGN_RIGHT | ALIGN_TOP);
	backBtn->SetPosition(-5, 220);
	backBtn->SetLabel(backBtnTxt);
	backBtn->SetTrigger(trigA);
	backBtn->SetTrigger(trigB);
	backBtn->SetScale(0.8);
	backBtn->Clicked.connect(this, &ThemeInfosWindow::OnButtonClick);

	themeImg = new GuiImage(thumbImageData);
	themeImg->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	themeImg->SetPosition(-5, 10);
	themeImg->SetScale(0.8);

	Append(bgWindowImg);
	Append(titleTxt);
	Append(titleTxt2);
	Append(authorTxt);
	Append(authorTxt2);
	Append(actionBtn);
	Append(backBtn);
	Append(themeImg);

	SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_IN, 50);
	SetPosition(0, 0);
	SetAlignment(ALIGN_CENTER | ALIGN_MIDDLE);
}

ThemeInfosWindow::~ThemeInfosWindow()
{
	RemoveAll();

	delete bgWindowImg;
	delete themeImg;
	
	delete thumbImageData;

	Resources::Remove(bgWindowData);

	Resources::Remove(btnSoundOver);
	Resources::Remove(btnSoundClick);

	delete titleTxt;
	delete titleTxt2;
	delete authorTxt;
	delete authorTxt2;
	delete actionBtnTxt;
	delete backBtnTxt;

	delete actionBtn;
	delete backBtn;

	delete trigA;
	delete trigB;
}

void ThemeInfosWindow::OnButtonClick(GuiButton *sender, int pointer UNUSED, const POINT &p UNUSED)
{
	if(sender == actionBtn)
		choice = 1;
	else
		choice = 0;
}
