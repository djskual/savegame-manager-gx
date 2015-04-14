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
#include "SearchWindow.h"
#include "../Menu/MainMenu.h"
#include "../Controls/Application.h"

/**
 *Class to display chars buttons
 */
cSearchButton::cSearchButton(std::wstring Char, const char *keyImageData, const char *keyOverImageData, int x, int y, GuiTrigger* trig, GuiSound* sndClick, GuiSound* sndOver)
	: PictureButton(keyImageData, keyOverImageData, sndClick, sndOver)
{
	wChar = Char;
	text = new GuiText(Char.c_str(), 20, thColor("r=0 g=0 b=0 a=255 - search buttons text color")),
	
	SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	SetPosition(x, y);
	SetLabel(text);
	SetTrigger(trig);
}

cSearchButton::~cSearchButton()
{
	delete text;
}

/**
 *Class to display the window
 */
SearchWindow::SearchWindow(std::wstring currentFilter, std::set<wchar_t > searchChars, bool fadeIn)
	: GuiFrame(0, 0)
{
	inSide = 0;
	SearchChars.clear();
	CurrentFilter.clear();
	if(!searchChars.empty())
		SearchChars = searchChars;
	if(!currentFilter.empty())
		CurrentFilter = currentFilter;
	
	this->SetAlignment(ALIGN_CENTER | ALIGN_MIDDLE);
	
	btnSoundClick = Resources::GetSound("button_click.wav");
	btnSoundOver = Resources::GetSound("button_over.wav");
	btnSoundClick->SetVolume(Settings.SFXVolume);
	btnSoundOver->SetVolume(Settings.SFXVolume);
	
	trigA = new SimpleGuiTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	trigB = new GuiTrigger();
	trigB->SetButtonOnlyTrigger(-1, WiiControls.BackButton | ClassicControls.BackButton << 16, GCControls.BackButton);
	
	this->SetCharsButtons();
	
	BgImg = new GuiImage(width, height, thColor("r=0 g=0 b=0 a=160 - search window color"));
	
	TextBarImg = new GuiImage(width - (10 + 3 * 45 + 10), 24, thColor("r=255 g=255 b=255 a=255 - search bar color"));
	TextBarImg->SetPosition(55, 15);
	
	Text = new GuiText(CurrentFilter.c_str(), 22, thColor("r=0 g=0 b=0 a=255 - search bar text color"));
	Text->SetPosition(TextBarImg->GetLeft()+5, TextBarImg->GetTop()+1);
	Text->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	Text->SetMaxWidth(TextBarImg->GetWidth()-10, SCROLL_HORIZONTAL);
	
	modeText = new GuiText(Settings.SearchMode == SEARCH_BEGINNING ? "=" : "*", 20, thColor("r=0 g=0 b=0 a=255 - search buttons text color"));
	modeText->SetAlignment(ALIGN_CENTER | ALIGN_MIDDLE);
	searchModeBtn = new PictureButton("keyboard_key.png", "keyboard_key_over.png", btnSoundClick, btnSoundOver);
	searchModeBtn->SetLabel(modeText);
	searchModeBtn->SetPosition(10, 10);
	searchModeBtn->SetTrigger(trigA);
	searchModeBtn->Clicked.connect(this, &SearchWindow::OnButtonClick);
	
	imgBacspaceBtn = Resources::GetImageData("keyboard_backspace_over.png");
	BacspaceBtnImg_Over = new GuiImage(imgBacspaceBtn);
	BacspaceBtnImg = new GuiImage(imgBacspaceBtn);
	BacspaceBtnImg->SetGrayscale();
	
	BacspaceBtn = new GuiButton(BacspaceBtnImg->GetWidth(), BacspaceBtnImg_Over->GetHeight());
	BacspaceBtn->SetAlignment(ALIGN_RIGHT | ALIGN_TOP);
	BacspaceBtn->SetPosition(-55, 10);
	BacspaceBtn->SetImage(BacspaceBtnImg);
	BacspaceBtn->SetImageOver(BacspaceBtnImg_Over);
	BacspaceBtn->SetSoundOver(btnSoundOver);
	BacspaceBtn->SetSoundClick(btnSoundClick);
	BacspaceBtn->SetTrigger(trigA);
	BacspaceBtn->SetEffectGrow();
	BacspaceBtn->Clicked.connect(this, &SearchWindow::OnButtonClick);
	
	imgClearBtn = Resources::GetImageData("keyboard_clear_over.png");
	ClearBtnImg_Over = new GuiImage(imgClearBtn);
	ClearBtnImg = new GuiImage(imgClearBtn);
	ClearBtnImg->SetGrayscale();
	
	ClearBtn = new GuiButton(ClearBtnImg->GetWidth(), ClearBtnImg_Over->GetHeight());
	ClearBtn->SetAlignment(ALIGN_RIGHT | ALIGN_TOP);
	ClearBtn->SetPosition(-10, 10);
	ClearBtn->SetImage(ClearBtnImg);
	ClearBtn->SetImageOver(ClearBtnImg_Over);
	ClearBtn->SetSoundOver(btnSoundOver);
	ClearBtn->SetSoundClick(btnSoundClick);
	ClearBtn->SetTrigger(trigA);
	ClearBtn->SetEffectGrow();
	ClearBtn->Clicked.connect(this, &SearchWindow::OnButtonClick);
	
	CloseBtn = new GuiButton(0,0);
	CloseBtn->SetTrigger(trigB);
	CloseBtn->Clicked.connect(this, &SearchWindow::OnButtonClick);
	
	this->Append(CloseBtn);
	this->Append(BgImg);
	this->Append(searchModeBtn);
	this->Append(TextBarImg);
	this->Append(Text);
	this->Append(BacspaceBtn);
	this->Append(ClearBtn);
	for(u32 i = 0; i < buttons.size(); i++)
		this->Append(buttons[i]);
	
	if(fadeIn)
		SetEffect(EFFECT_FADE, 30);
}

SearchWindow::~SearchWindow()
{
	RemoveAll();
	
	CurrentFilter.clear();
	SearchChars.clear();
	
	delete BgImg;
	delete TextBarImg;
	
	delete Text;
	delete modeText;
	
	if(!buttons.empty())
	{
		for(u32 i = 0; i < buttons.size(); ++i)
			delete buttons.at(i);
		buttons.clear();
	}
	
	delete ClearBtn;
	delete ClearBtnImg;
	delete ClearBtnImg_Over;
	Resources::Remove(imgClearBtn);

	delete BacspaceBtn;
	delete BacspaceBtnImg;
	delete BacspaceBtnImg_Over;
	Resources::Remove(imgBacspaceBtn);
	
	delete searchModeBtn;
	delete CloseBtn;
	
	Resources::Remove(btnSoundClick);
	Resources::Remove(btnSoundOver);
	
	delete trigA;
	delete trigB;
}

void SearchWindow::SetCharsButtons()
{
	buttons.clear();
	if(SearchChars.empty())
	{
		width = 320;
		height = 62;
		return;
	}
	int cnt = SearchChars.size();
	
	wchar_t charstr[2] = { 0, 0 };
	int lines = (cnt + 9) / 10;
	int buttonsPerLine = (cnt + lines - 1) / lines;
	width = 10 + buttonsPerLine * 42 + 10;
	int i = 0, x_start = 10, x = 0, y_start = 10 + 42, y = 0;
	if (width < 320)
	{
		x_start += (320 - width) >> 1;
		width = 320;
	}
	for (std::set<wchar_t>::iterator it=SearchChars.begin() ; it != SearchChars.end(); it++, i++, x++)
	{
		if (x >= buttonsPerLine) x = 0;
		if (x == 0) y++;
		charstr[0] = *it;
		if(i >= (int)buttons.size())
			this->AddCharButton(charstr, x_start+x*42, y_start-42+y*42);
	}
	height = 10 + 42 + y * 42 + 10;
}

void SearchWindow::AddCharButton(std::wstring t, int x, int y)
{
	cSearchButton * btn = new cSearchButton(t, "keyboard_key.png", "keyboard_key_over.png", x, y, trigA, btnSoundClick, btnSoundOver);
	btn->Clicked.connect(this, &SearchWindow::OnButtonClick);
	
	buttons.push_back(btn);
}

void SearchWindow::Update(GuiTrigger * t)
{
	if (_elements.size() == 0 || (state == STATE_DISABLED && parentElement)) return;
	
	//! cursor
	if (t->wpad->ir.valid && state != STATE_DISABLED)
	{
		if (this->IsInside(t->wpad->ir.x, t->wpad->ir.y))
		{
			if (inSide == 0)
			{
				Application::Instance()->SetUpdateOnly(this);
				this->DimBackground(true);
			}
			inSide |= 1 << t->chan;
		}
		else if (inSide)
		{
			inSide &= ~(1 << t->chan);
			if (inSide == 0)
			{
				Application::Instance()->UnsetUpdateOnly(this);
				this->DimBackground(false);
			}
		}
	}
	GuiFrame::Update(t);
}

void SearchWindow::OnButtonClick(GuiButton *sender, int pointer UNUSED, const POINT &p UNUSED)
{
	sender->ResetState();
	
	if (sender == BacspaceBtn)
	{
		RemoveLastChar();
	}
	else if (sender == ClearBtn)
	{
		MainMenu::Instance()->ExplorerFilterList(L"", true);
		MainMenu::Instance()->SetFilterOn(false);
		MainMenu::Instance()->RefreshSearchWindow();
	}
	else if ((sender == CloseBtn) && inSide)
	{
		SetEffect(EFFECT_FADE, -30);
		MainMenu::Instance()->SetSearchOn(false);
	}
	else if (sender == searchModeBtn)
	{
		Settings.SearchMode = Settings.SearchMode == SEARCH_BEGINNING ? SEARCH_CONTENT : SEARCH_BEGINNING;
		MainMenu::Instance()->ExplorerFilterList(L"", true);
		MainMenu::Instance()->SetFilterOn(false);
		MainMenu::Instance()->RefreshSearchWindow();
	}
	else if (!buttons.empty())
	{
		for (u32 i = 0; i < buttons.size(); i++)
		{
			if (sender == buttons[i])
			{
				AddNewChar(i);
				break;
			}
		}
	}
	else
		return;
	
	Application::Instance()->PushForDelete(this);
}

void SearchWindow::AddNewChar(int i)
{
	CurrentFilter += buttons[i]->wChar;
	
	MainMenu::Instance()->ExplorerFilterList(CurrentFilter);
	MainMenu::Instance()->SetFilterOn(true);
}

void SearchWindow::RemoveLastChar()
{
	if (CurrentFilter.size())
		CurrentFilter.erase(CurrentFilter.end()-1);
	
	MainMenu::Instance()->ExplorerFilterList(CurrentFilter, true);
	
	if(!CurrentFilter.size())
		MainMenu::Instance()->SetFilterOn(false);
}
