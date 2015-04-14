/****************************************************************************
 * Copyright (C) 2010
 * by Dimok for USB Loader GX
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
#include "gui_searchbar.h"
#include "../System/wpad.h"
#include "../main.h"
#include "../Settings/CSettings.h"
#include "../Themes/CTheme.h"
#include "../Saves/CSaveList.hpp"
#include "../Miis/CMiiList.hpp"
#include "../Files/CFileList.hpp"
#include "../EmuNand/CEmuList.hpp"
#include "../EmuNand/CEmuMiiList.hpp"

extern GuiWindow * mainWindow;

class cSearchButton
{
	public:
		cSearchButton(wchar_t *Char, GuiImageData *keyImageData, GuiImageData *keyOverImageData, int x, int y,
				GuiTrigger* trig, GuiSound* sndOver, GuiSound* sndClick) :
			wchar(*Char), image(keyImageData), imageOver(keyOverImageData), text((char *) NULL, 20, ( GXColor )
			{   0, 0, 0, 0xff}), button(&image, &imageOver, ALIGN_LEFT, ALIGN_TOP, x, y, trig, sndOver, sndClick, 1)
		{
			text.SetText(Char);
			button.SetLabel(&text);
		}
		wchar_t wchar;
		GuiImage image;
		GuiImage imageOver;
		GuiText text;
		GuiButton button;
	private:

};

GuiSearchBar::GuiSearchBar(const wchar_t *SearchChars, int browserMode) :
	inSide(0), text((char *) NULL, 22, ( GXColor )
	{   0, 0, 0, 255}), buttons(0),
	keyImageData(Resources::GetFile("keyboard_key.png"), Resources::GetFileSize("keyboard_key.png")),
	keyOverImageData(Resources::GetFile("keyboard_key_over.png"), Resources::GetFileSize("keyboard_key_over.png"))
{
	trigA.SetSimpleTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	trigB.SetButtonOnlyTrigger(-1, WiiControls.BackButton | ClassicControls.BackButton << 16, GCControls.BackButton);
	
	SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);

	cnt = wcslen(SearchChars);
	buttons = new cSearchButton*[cnt];

	wchar_t charstr[2] = { 0, 0 };
	int lines = (cnt + 9) / 10;
	int buttonsPerLine = (cnt + lines - 1) / lines;
	width = 10 + buttonsPerLine * 42 + 10;
	int x_start = 10, x = 0, y_start = 10 + 42, y = 0;
	if (width < 200)
	{
		x_start += (200 - width) >> 1;
		width = 200;
	}
	for (int i = 0; i < cnt; i++, x++)
	{
		if (x >= buttonsPerLine) x = 0;
		if (x == 0) y++;
		charstr[0] = SearchChars[i];
		buttons[i] = new cSearchButton(charstr, &keyImageData, &keyOverImageData, x_start + x * 42, y_start - 42 + y
				* 42, &trigA, btnSoundOver, btnSoundClick);
		this->Append(&(buttons[i]->button));
	}
	height = 10 + 42 + y * 42 + 10;

	if(browserMode == BROWSE_SDUSB)
		text.SetText(FileList.GetCurrentFilter());
	else if(browserMode == BROWSE_NAND)
		text.SetText(SaveList.GetCurrentFilter());
	else if(browserMode == BROWSE_MII)
		text.SetText(MiiList.GetCurrentFilter());
	else if (browserMode == BROWSE_EMU)
		text.SetText(EmuList.GetCurrentFilter());
	else if (browserMode == BROWSE_EMU_MII)
		text.SetText(EmuMiiList.GetCurrentFilter());
	text.SetPosition(10, 15);
	text.SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	text.SetWidescreen(Settings.Widescreen);
	text.SetMaxWidth(width - (10 + 2 * 42 + 10), SCROLL_HORIZONTAL);
	this->Append(&text);

	imgBacspaceBtn = Resources::GetImageData("keyboard_backspace_over.png");
	BacspaceBtnImg_Over = new GuiImage(imgBacspaceBtn);
	BacspaceBtnImg = new GuiImage(BacspaceBtnImg_Over);
	BacspaceBtnImg->SetGrayscale();
	BacspaceBtn = new GuiButton(BacspaceBtnImg, BacspaceBtnImg_Over, ALIGN_RIGHT, ALIGN_TOP, -52, 10, &trigA, btnSoundOver, btnSoundClick, 1);
	this->Append(BacspaceBtn);

	imgCloseBtn = Resources::GetImageData("keyboard_clear_over.png");
	CloseBtnImg_Over = new GuiImage(imgCloseBtn);
	CloseBtnImg = new GuiImage(CloseBtnImg_Over);
	CloseBtnImg->SetGrayscale();
	CloseBtn = new GuiButton(CloseBtnImg, CloseBtnImg_Over, ALIGN_RIGHT, ALIGN_TOP, -10, 10, &trigA, btnSoundOver, btnSoundClick, 1);
	this->Append(CloseBtn);
	
	ClearBtn = new GuiButton(0,0);
	ClearBtn->SetTrigger(&trigB);
	this->Append(ClearBtn);
}
GuiSearchBar::~GuiSearchBar()
{
	if (buttons)
	{
		for (int i = 0; i < cnt; i++)
			delete buttons[i];
		delete[] buttons;
	}
	delete CloseBtn;
	delete CloseBtnImg;
	delete CloseBtnImg_Over;
	delete imgCloseBtn;

	delete BacspaceBtn;
	delete BacspaceBtnImg;
	delete BacspaceBtnImg_Over;
	delete imgBacspaceBtn;
	if (inSide) mainWindow->SetState(STATE_DEFAULT);
}
void GuiSearchBar::Draw()
{
	Menu_DrawRectangle(this->GetLeft(), this->GetTop(), width, height, ( GXColor )
	{   0, 0, 0, 0xa0}, 1);
	Menu_DrawRectangle(this->GetLeft() + 10, this->GetTop() + 15, width - (10 + 2 * 42 + 10), 22, ( GXColor )
	{   255, 255, 255, 255}, 1);
	GuiWindow::Draw();
}
void GuiSearchBar::Update(GuiTrigger * t)
{
	LOCK( this );
	if (_elements.size() == 0 || (state == STATE_DISABLED && parentElement)) return;
	// cursor
	if (t->wpad.ir.valid && state != STATE_DISABLED)
	{
		if (this->IsInside(t->wpad.ir.x, t->wpad.ir.y))
		{
			if (inSide == 0)
			{
				mainWindow->SetState(STATE_DISABLED);
				this->SetState(STATE_DEFAULT);
			}
			inSide |= 1 << t->chan;
		}
		else if (inSide)
		{
			inSide &= ~(1 << t->chan);
			if (inSide == 0) mainWindow->SetState(STATE_DEFAULT);
		}
	}
	GuiWindow::Update(t);
}
wchar_t GuiSearchBar::GetClicked()
{
	if (buttons)
	{
		for (int i = 0; i < cnt; i++)
		{
			if (buttons[i]->button.GetState() == STATE_CLICKED)
			{
				buttons[i]->button.ResetState();
				return buttons[i]->wchar;
			}
		}
	}
	if (BacspaceBtn->GetState() == STATE_CLICKED) return 8;
	if (CloseBtn->GetState() == STATE_CLICKED) return 7;
	if (ClearBtn->GetState() == STATE_CLICKED && inSide) return 6;
	if (ClearBtn->GetState() == STATE_CLICKED && !inSide) ClearBtn->ResetState();
	
	return 0;
}
