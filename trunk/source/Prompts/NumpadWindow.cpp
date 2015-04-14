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
#include "NumpadWindow.h"
#include "../Themes/CTheme.h"
#include "../Controls/Application.h"

NumpadWindow::NumpadWindow(char *text, int max, bool crypted)
	: GuiFrame(0, 0)
{
	choice = -1;
	inText = text;
	maxlen = (max ? max : strlen(inText)+1);

	numpad = new GuiNumpad(text, maxlen, crypted);
	numpad->SetAlignment(ALIGN_CENTER | ALIGN_TOP);
	
	btnSoundOver = Resources::GetSound("button_over.wav");
	btnSoundClick = Resources::GetSound("button_click.wav");
	btnSoundOver->SetVolume(Settings.SFXVolume);
	btnSoundClick->SetVolume(Settings.SFXVolume);
	
	trigA.SetSimpleTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);

	okBtnTxt = new GuiText(tr("OK"), 22, thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
	okBtn = new PictureButton("button.png", "button.png", btnSoundClick, btnSoundOver);
	okBtn->SetAlignment(ALIGN_CENTER | ALIGN_BOTTOM);
	okBtn->SetPosition(-90, 0);
	okBtn->SetLabel(okBtnTxt);
	okBtn->SetTrigger(&trigA);
	okBtn->Clicked.connect(this, &NumpadWindow::OnOkButtonClick);

	cancelBtnTxt = new GuiText(tr("Cancel"), 22, thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
	cancelBtn = new PictureButton("button.png", "button.png", btnSoundClick, btnSoundOver);
	cancelBtn->SetAlignment(ALIGN_CENTER | ALIGN_BOTTOM);
	cancelBtn->SetPosition(90, 0);
	cancelBtn->SetLabel(cancelBtnTxt);
	cancelBtn->SetTrigger(&trigA);
	cancelBtn->Clicked.connect(this, &NumpadWindow::OnCancelButtonClick);

	width = numpad->GetWidth();
	height = numpad->GetHeight() + okBtn->GetHeight() + 10;

	Append(numpad);
	Append(okBtn);
	Append(cancelBtn);

	SetAlignment(ALIGN_CENTER | ALIGN_MIDDLE);
}

NumpadWindow::~NumpadWindow()
{
	RemoveAll();

	Resources::Remove(btnSoundOver);
	Resources::Remove(btnSoundClick);

	delete okBtn;
	delete cancelBtn;

	delete okBtnTxt;
	delete cancelBtnTxt;

	delete numpad;
}

void NumpadWindow::OnOkButtonClick(GuiButton *sender UNUSED, int pointer UNUSED, const POINT &p UNUSED)
{
	choice = 1;
	
	strncpy(inText, numpad->GetText(), maxlen);
	inText[maxlen-1] = 0;

	OkButtonClicked(this, inText);
}

void NumpadWindow::OnCancelButtonClick(GuiButton *sender UNUSED, int pointer UNUSED, const POINT &p UNUSED)
{
	choice = 0;
}
