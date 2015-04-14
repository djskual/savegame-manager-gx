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
#include "gui_keyboard.h"
#include "../Themes/CTheme.h"

/**
 * Constructor for the GuiKeyboard class.
 */
GuiKeyboard::GuiKeyboard(const char * t, u32 max)
{
	SetupKeyboard(wfmt(t), max);
}

GuiKeyboard::GuiKeyboard(const wchar_t * t, u32 max)
{
	SetupKeyboard(t, max);
}

void GuiKeyboard::SetupKeyboard(const wchar_t * t, u32 max)
{
	width = 540;
	height = 400;
	shift = false;
	caps = false;
	UpdateKeys = false;
	DefaultKeys = false;
	selectable = true;
	ShiftChan = -1;
	kbtextstr.assign(t);
	kbtextmaxlen = max;
	DeleteDelay = 0;
	CurrentFirstLetter = 0;
	if(t)
	{
		CurrentFirstLetter = wcslen(t)-MAX_KEYBOARD_DISPLAY+1;
		if(CurrentFirstLetter < 0)
			CurrentFirstLetter = 0;
	}

	memset(keys, 0, sizeof(keys));

	wcsncpy(keys[0].ch,		wfmt("%s", tr("`1234567890-")), MAXKEYS);
	wcsncpy(keys[0].chShift,wfmt("%s", tr("~!@#$%^&*()_")), MAXKEYS);
	wcsncpy(keys[1].ch,		wfmt("%s", tr("qwertyuiop[]\\")), MAXKEYS);
	wcsncpy(keys[1].chShift,wfmt("%s", tr("QWERTYUIOP{}|")), MAXKEYS);
	wcsncpy(keys[2].ch,		wfmt("%s", tr("asdfghjkl;'=")), MAXKEYS);
	wcsncpy(keys[2].chShift,wfmt("%s", tr("ASDFGHJKL:\"+")), MAXKEYS);
	wcsncpy(keys[3].ch,		wfmt("%s", tr("zxcvbnm,./")), MAXKEYS);
	wcsncpy(keys[3].chShift,wfmt("%s", tr("ZXCVBNM<>?")), MAXKEYS);

	int KeyboardPosition = -25;

	keyTextbox = Resources::GetImageData("keyboard_textbox.png");
	keyTextboxImg = new GuiImage(keyTextbox);
	keyTextboxImg->SetAlignment(ALIGN_CENTER | ALIGN_TOP);
	keyTextboxImg->SetPosition(0, 0);
	this->Append(keyTextboxImg);

	trigHeldA = new GuiTrigger;
	trigHeldA->SetHeldTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);

	kbText = new GuiText(GetDisplayText(), 20, thColor("r=0 g=0 b=0 a=255 - keyboard text color"));

	TextPointerBtn = new TextPointer(kbText, 0);
	TextPointerBtn->SetAlignment(ALIGN_CENTER | ALIGN_TOP);
	kbText->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	TextPointerBtn->SetPosition(0, 11);
	TextPointerBtn->SetHoldable(true);
	TextPointerBtn->SetTrigger(trigHeldA);
	TextPointerBtn->Held.connect(this, &GuiKeyboard::OnPointerHeld);
	this->Append(TextPointerBtn);

	TextPointerBtn->PositionChanged(0, 0, 0);
	TextPointerBtn->SetPointerPosition(MAX_KEYBOARD_DISPLAY-1);

	key = Resources::GetImageData("keyboard_key.png");
	keyOver = Resources::GetImageData("keyboard_key_over.png");
	keyMedium = Resources::GetImageData("keyboard_mediumkey.png");
	keyMediumOver = Resources::GetImageData("keyboard_mediumkey_over.png");
	keyLarge = Resources::GetImageData("keyboard_largekey.png");
	keyLargeOver = Resources::GetImageData("keyboard_largekey_over.png");

	keySoundOver = Resources::GetSound("button_over.wav");
	keySoundClick = Resources::GetSound("button_click.wav");

	trigA = new GuiTrigger;
	trigA->SetSimpleTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);

	trigLeft = new GuiTrigger;
	trigLeft->SetButtonOnlyTrigger(-1, WiiControls.LeftButton | ClassicControls.LeftButton << 16, GCControls.LeftButton);
	trigRight = new GuiTrigger;
	trigRight->SetButtonOnlyTrigger(-1, WiiControls.RightButton | ClassicControls.RightButton << 16, GCControls.RightButton);

	GoRight = new GuiButton(1, 1);
	GoRight->SetSoundClick(keySoundClick);
	GoRight->SetTrigger(trigRight);
	GoRight->Clicked.connect(this, &GuiKeyboard::OnPositionMoved);
	this->Append(GoRight);

	GoLeft = new GuiButton(1, 1);
	GoLeft->SetSoundClick(keySoundClick);
	GoLeft->SetTrigger(trigLeft);
	GoLeft->Clicked.connect(this, &GuiKeyboard::OnPositionMoved);
	this->Append(GoLeft);

	keyBackImg = new GuiImage(keyMedium);
	keyBackOverImg = new GuiImage(keyMediumOver);
	keyBackText = new GuiText(tr("Back"), 20, thColor("r=0 g=0 b=0 a=255 - keyboard spec buttons text color"));
	keyBack = new GuiButton(keyMedium->GetWidth(), keyMedium->GetHeight());
	keyBack->SetImage(keyBackImg);
	keyBack->SetImageOver(keyBackOverImg);
	keyBack->SetLabel(keyBackText);
	keyBack->SetSoundOver(keySoundOver);
	keyBack->SetSoundClick(keySoundClick);
	keyBack->SetTrigger(trigA);
	keyBack->SetPosition(11*42+40+KeyboardPosition, 0*42+80);
	keyBack->SetEffectGrow();
	keyBack->Clicked.connect(this, &GuiKeyboard::OnSpecialKeyPress);
	this->Append(keyBack);

	keyCapsImg = new GuiImage(keyMedium);
	keyCapsOverImg = new GuiImage(keyMediumOver);
	keyCapsText = new GuiText(tr("Caps"), 20, thColor("r=0 g=0 b=0 a=255 - keyboard spec buttons text color"));
	keyCaps = new GuiButton(keyMedium->GetWidth(), keyMedium->GetHeight());
	keyCaps->SetImage(keyCapsImg);
	keyCaps->SetImageOver(keyCapsOverImg);
	keyCaps->SetLabel(keyCapsText);
	keyCaps->SetSoundOver(keySoundOver);
	keyCaps->SetSoundClick(keySoundClick);
	keyCaps->SetTrigger(trigA);
	keyCaps->SetPosition(0+KeyboardPosition, 2*42+80);
	keyCaps->SetEffectGrow();
	keyCaps->Clicked.connect(this, &GuiKeyboard::OnSpecialKeyPress);
	this->Append(keyCaps);

	keyShiftImg = new GuiImage(keyMedium);
	keyShiftOverImg = new GuiImage(keyMediumOver);
	keyShiftText = new GuiText(tr("Shift"), 20, thColor("r=0 g=0 b=0 a=255 - keyboard spec buttons text color"));
	keyShift = new GuiButton(keyMedium->GetWidth(), keyMedium->GetHeight());
	keyShift->SetImage(keyShiftImg);
	keyShift->SetImageOver(keyShiftOverImg);
	keyShift->SetLabel(keyShiftText);
	keyShift->SetSoundOver(keySoundOver);
	keyShift->SetSoundClick(keySoundClick);
	keyShift->SetTrigger(trigA);
	keyShift->SetPosition(0+KeyboardPosition, 3*42+80);
	keyShift->SetEffectGrow();
	keyShift->Clicked.connect(this, &GuiKeyboard::OnSpecialKeyPress);
	this->Append(keyShift);

	keyLangImg = new GuiImage(keyMedium);
	keyLangOverImg = new GuiImage(keyMediumOver);
	keyLangText = new GuiText(tr("Lang."), 20, thColor("r=0 g=0 b=0 a=255 - keyboard spec buttons text color"));
	keyLang = new GuiButton(keyMedium->GetWidth(), keyMedium->GetHeight());
	keyLang->SetImage(keyLangImg);
	keyLang->SetImageOver(keyLangOverImg);
	keyLang->SetLabel(keyLangText);
	keyLang->SetSoundOver(keySoundOver);
	keyLang->SetSoundClick(keySoundClick);
	keyLang->SetTrigger(trigA);
	keyLang->SetPosition(0+KeyboardPosition, 4*42+80);
	keyLang->SetEffectGrow();
	keyLang->Clicked.connect(this, &GuiKeyboard::OnSpecialKeyPress);
	this->Append(keyLang);

	keyLineBreakText = new GuiText(tr("Linebreak"), 18, thColor("r=0 g=0 b=0 a=255 - keyboard spec buttons text color"));
	keyLineBreakImg = new GuiImage(keyMedium);
	keyLineBreakOverImg = new GuiImage(keyMediumOver);
	keyLineBreak = new GuiButton(keyMedium->GetWidth(), keyMedium->GetHeight());
	keyLineBreak->SetImage(keyLineBreakImg);
	keyLineBreak->SetImageOver(keyLineBreakOverImg);
	keyLineBreak->SetSoundOver(keySoundOver);
	keyLineBreak->SetSoundClick(keySoundClick);
	keyLineBreak->SetLabel(keyLineBreakText);
	keyLineBreak->SetTrigger(trigA);
	keyLineBreak->SetPosition(9*42+40+KeyboardPosition, 4*42+80);
	keyLineBreak->SetEffectGrow();
	keyLineBreak->Clicked.connect(this, &GuiKeyboard::OnSpecialKeyPress);
	this->Append(keyLineBreak);

	keyClearText = new GuiText(tr("Clear"), 20, thColor("r=0 g=0 b=0 a=255 - keyboard spec buttons text color"));
	keyClearImg = new GuiImage(keyMedium);
	keyClearOverImg = new GuiImage(keyMediumOver);
	keyClear = new GuiButton(keyMedium->GetWidth(), keyMedium->GetHeight());
	keyClear->SetImage(keyClearImg);
	keyClear->SetImageOver(keyClearOverImg);
	keyClear->SetSoundOver(keySoundOver);
	keyClear->SetSoundClick(keySoundClick);
	keyClear->SetLabel(keyClearText);
	keyClear->SetTrigger(trigA);
	keyClear->SetPosition(11*42+40+KeyboardPosition, 4*42+80);
	keyClear->SetEffectGrow();
	keyClear->Clicked.connect(this, &GuiKeyboard::OnSpecialKeyPress);
	this->Append(keyClear);

	keySpaceImg = new GuiImage(keyLarge);
	keySpaceOverImg = new GuiImage(keyLargeOver);
	keySpace = new GuiButton(keyLarge->GetWidth(), keyLarge->GetHeight());
	keySpace->SetImage(keySpaceImg);
	keySpace->SetImageOver(keySpaceOverImg);
	keySpace->SetSoundOver(keySoundOver);
	keySpace->SetSoundClick(keySoundClick);
	keySpace->SetTrigger(trigA);
	keySpace->SetPosition(0, 4*42+80);
	keySpace->SetAlignment(ALIGN_CENTER | ALIGN_TOP);
	keySpace->SetEffectGrow();
	keySpace->Clicked.connect(this, &GuiKeyboard::OnSpecialKeyPress);
	this->Append(keySpace);

	int Pos = 0;
	wchar_t txt[2] = { 0, 0 };

	for(int i = 0; i < MAXROWS; i++)
	{
		for(int j = 0; j < MAXKEYS; j++)
		{
			if(keys[i].ch[j] != '\0')
			{
				txt[0] = keys[i].ch[j];
				keyImg[i][j] = new GuiImage(key);
				keyImgOver[i][j] = new GuiImage(keyOver);
				keyTxt[i][j] = new GuiText(txt, 20, thColor("r=0 g=0 b=0 a=255 - keyboard buttons text color"));
				keyTxt[i][j]->SetAlignment(ALIGN_CENTER | ALIGN_BOTTOM);
				keyTxt[i][j]->SetPosition(0, -10);
				keyBtn[i][j] = new GuiButton(key->GetWidth(), key->GetHeight());
				keyBtn[i][j]->SetImage(keyImg[i][j]);
				keyBtn[i][j]->SetImageOver(keyImgOver[i][j]);
				keyBtn[i][j]->SetSoundOver(keySoundOver);
				keyBtn[i][j]->SetSoundClick(keySoundClick);
				keyBtn[i][j]->SetTrigger(trigA);
				keyBtn[i][j]->SetLabel(keyTxt[i][j]);
				if(i == 1)
					Pos = i*20;
				else if(i > 0)
					Pos = (i+2)*20;
				keyBtn[i][j]->SetPosition(j*42+Pos+KeyboardPosition, i*42+80);
				keyBtn[i][j]->SetEffectGrow();
				keyBtn[i][j]->Clicked.connect(this, &GuiKeyboard::OnNormalKeyPress);
				this->Append(keyBtn[i][j]);
			}
		}
	}
}

/**
 * Destructor for the GuiKeyboard class.
 */
GuiKeyboard::~GuiKeyboard()
{
	delete TextPointerBtn;
	delete GoRight;
	delete GoLeft;
	delete kbText;
	delete keyTextboxImg;
	delete keyCapsText;
	delete keyCapsImg;
	delete keyCapsOverImg;
	delete keyCaps;
	delete keyShiftText;
	delete keyShiftImg;
	delete keyShiftOverImg;
	delete keyShift;
	delete keyBackText;
	delete keyBackImg;
	delete keyBackOverImg;
	delete keyBack;
	delete keySpaceImg;
	delete keySpaceOverImg;
	delete keySpace;
	delete keyLineBreakText;
	delete keyLineBreakImg;
	delete keyLineBreakOverImg;
	delete keyLineBreak;
	delete keyClearText;
	delete keyClearImg;
	delete keyClearOverImg;
	delete keyClear;
	delete keyLangText;
	delete keyLangImg;
	delete keyLangOverImg;
	delete keyLang;
	delete trigA;
	delete trigHeldA;
	delete trigLeft;
	delete trigRight;
	Resources::Remove(keyTextbox);
	Resources::Remove(key);
	Resources::Remove(keyOver);
	Resources::Remove(keyMedium);
	Resources::Remove(keyMediumOver);
	Resources::Remove(keyLarge);
	Resources::Remove(keyLargeOver);
	Resources::Remove(keySoundOver);
	Resources::Remove(keySoundClick);

	for(int i = 0; i < MAXROWS; i++)
	{
		for(int j = 0; j < MAXKEYS; j++)
		{
			if(keys[i].ch[j] != '\0')
			{
				delete keyImg[i][j];
				delete keyImgOver[i][j];
				delete keyTxt[i][j];
				delete keyBtn[i][j];
			}
		}
	}
}

std::string GuiKeyboard::GetUTF8String() const
{
	return kbtextstr.toUTF8();
}

const wchar_t * GuiKeyboard::GetString()
{
	return kbtextstr.c_str();
}

void GuiKeyboard::SwitchKeyLanguage()
{
	DefaultKeys = !DefaultKeys;

	memset(keys, 0, sizeof(keys));

	if(DefaultKeys)
	{
		wcsncpy(keys[0].ch,		L"`1234567890-", MAXKEYS);
		wcsncpy(keys[0].chShift,L"~!@#$%^&*()_", MAXKEYS);
		wcsncpy(keys[1].ch,		L"qwertyuiop[]\\", MAXKEYS);
		wcsncpy(keys[1].chShift,L"QWERTYUIOP{}|", MAXKEYS);
		wcsncpy(keys[2].ch,		L"asdfghjkl;'=", MAXKEYS);
		wcsncpy(keys[2].chShift,L"ASDFGHJKL:\"+", MAXKEYS);
		wcsncpy(keys[3].ch,		L"zxcvbnm,./", MAXKEYS);
		wcsncpy(keys[3].chShift,L"ZXCVBNM<>?", MAXKEYS);

	}
	else
	{
		wcsncpy(keys[0].ch,		wfmt("%s", tr("`1234567890-")), MAXKEYS);
		wcsncpy(keys[0].chShift,wfmt("%s", tr("~!@#$%^&*()_")), MAXKEYS);
		wcsncpy(keys[1].ch,		wfmt("%s", tr("qwertyuiop[]\\")), MAXKEYS);
		wcsncpy(keys[1].chShift,wfmt("%s", tr("QWERTYUIOP{}|")), MAXKEYS);
		wcsncpy(keys[2].ch,		wfmt("%s", tr("asdfghjkl;'=")), MAXKEYS);
		wcsncpy(keys[2].chShift,wfmt("%s", tr("ASDFGHJKL:\"+")), MAXKEYS);
		wcsncpy(keys[3].ch,		wfmt("%s", tr("zxcvbnm,./")), MAXKEYS);
		wcsncpy(keys[3].chShift,wfmt("%s", tr("ZXCVBNM<>?")), MAXKEYS);
	}
}

void GuiKeyboard::AddChar(int pos, wchar_t Char)
{
	if(pos < 0)
		return;

	kbtextstr.insert(pos, 1, Char);

	MoveText(1);
}

void GuiKeyboard::RemoveChar(int pos)
{
	if (pos < 0 || pos >= (int) kbtextstr.size())
		return;

	kbtextstr.erase(pos, 1);
	MoveText(-1);
}

void GuiKeyboard::MoveText(int n)
{
	int strlength = kbtextstr.size();

	if(strlength > MAX_KEYBOARD_DISPLAY)
	{
		CurrentFirstLetter += n;
		if(CurrentFirstLetter < 0)
			CurrentFirstLetter = 0;
	}
	else
		CurrentFirstLetter = 0;

	kbText->SetText(GetDisplayText());
	TextPointerBtn->TextWidthChanged();

	if(strlength > MAX_KEYBOARD_DISPLAY-2)
		TextPointerBtn->SetPointerPosition(TextPointerBtn->GetCurrentLetter());
	else
		TextPointerBtn->SetPointerPosition(TextPointerBtn->GetCurrentLetter()+n);
}

void GuiKeyboard::OnPointerHeld(GuiButton *sender UNUSED, int pointer, const POINT &p)
{
	TextPointerBtn->PositionChanged(pointer, p.x - TextPointerBtn->GetLeft(), p.y - TextPointerBtn->GetTop());
}

void GuiKeyboard::OnPositionMoved(GuiButton *sender, int pointer UNUSED, const POINT &p UNUSED)
{
	sender->ResetState();

	if(sender == GoLeft)
	{
		int currentPointLetter = TextPointerBtn->GetCurrentLetter();
		currentPointLetter--;
		if(currentPointLetter < 0)
		{
			currentPointLetter = 0;
			CurrentFirstLetter--;
			if(CurrentFirstLetter < 0)
				CurrentFirstLetter = 0;
		}
		kbText->SetText(GetDisplayText());
		TextPointerBtn->TextWidthChanged();
		TextPointerBtn->SetPointerPosition(currentPointLetter);
	}
	else if(sender == GoRight)
	{
		int currentPointLetter = TextPointerBtn->GetCurrentLetter();
		currentPointLetter++;
		int strlength = kbtextstr.length();
		if(currentPointLetter > (MAX_KEYBOARD_DISPLAY-1) || currentPointLetter > strlength)
		{
			currentPointLetter--;
			CurrentFirstLetter++;
			if(CurrentFirstLetter > (strlength-MAX_KEYBOARD_DISPLAY+1))
				CurrentFirstLetter = strlength-MAX_KEYBOARD_DISPLAY+1;
		}
		kbText->SetText(GetDisplayText());
		TextPointerBtn->TextWidthChanged();
		TextPointerBtn->SetPointerPosition(currentPointLetter);
	}
}

void GuiKeyboard::OnSpecialKeyPress(GuiButton *sender, int pointer UNUSED, const POINT &p UNUSED)
{
	if(sender == keySpace && kbtextstr.length() < kbtextmaxlen)
	{
		AddChar(CurrentFirstLetter+TextPointerBtn->GetCurrentLetter(), ' ');
	}
	else if(sender == keyBack)
	{
		RemoveChar(CurrentFirstLetter+TextPointerBtn->GetCurrentLetter()-1);
	}
	else if(sender == keyLineBreak)
	{
		AddChar(CurrentFirstLetter+TextPointerBtn->GetCurrentLetter(), '\n');
	}
	else if(sender == keyClear)
	{
		CurrentFirstLetter = 0;
		kbtextstr.clear();
		kbText->SetText(GetDisplayText());
		TextPointerBtn->TextWidthChanged();
		TextPointerBtn->SetPointerPosition(0);
	}
	else if(sender == keyShift)
	{
		shift = !shift;
		UpdateKeys = true;
	}
	else if(sender == keyCaps)
	{
		caps = !caps;
		UpdateKeys = true;
	}
	else if(sender == keyLang)
	{
		SwitchKeyLanguage();
		UpdateKeys = true;
	}
}

void GuiKeyboard::OnNormalKeyPress(GuiButton *sender, int pointer UNUSED, const POINT &p UNUSED)
{
	for(int i = 0; i < MAXROWS; i++)
	{
		for(int j = 0; j < MAXKEYS; j++)
		{
			if(sender == keyBtn[i][j])
			{
				if(kbtextstr.length() < kbtextmaxlen)
				{
					if(shift || caps)
					{
						AddChar(CurrentFirstLetter+TextPointerBtn->GetCurrentLetter(), keys[i].chShift[j]);
					}
					else
					{
						AddChar(CurrentFirstLetter+TextPointerBtn->GetCurrentLetter(), keys[i].ch[j]);
					}
				}

				if(shift)
				{
					shift = false;
					UpdateKeys = true;
				}
			}
		}
	}
}

void GuiKeyboard::Update(GuiTrigger * t)
{
	GuiFrame::Update(t);

	++DeleteDelay;

	if(((t->wpad->btns_h & WiiControls.KeyBackspaceButton) ||
		(t->wpad->btns_h & (ClassicControls.KeyBackspaceButton << 16)) ||
		(t->pad.btns_h & GCControls.KeyBackspaceButton)) &&
		DeleteDelay > (u32) Settings.KeyboardDeleteDelay)
	{
		RemoveChar(CurrentFirstLetter+TextPointerBtn->GetCurrentLetter()-1);
		DeleteDelay = 0;
	}

	if((t->wpad->btns_h & WiiControls.KeyShiftButton) ||
	   (t->wpad->btns_h & (ClassicControls.KeyShiftButton << 16)) ||
	   (t->pad.btns_h & GCControls.KeyShiftButton))
	{
		caps = true;

		if(ShiftChan < 0)
			UpdateKeys = true;

		ShiftChan = t->chan;
	}
	else
	{
		if(t->chan == ShiftChan)
		{
			caps = false;
			shift = false;
			UpdateKeys = true;
			ShiftChan = -1;
		}
	}

	if(UpdateKeys)
	{
		wchar_t txt[2] = { 0, 0 };

		for(int i = 0; i < MAXROWS; i++)
		{
			for(int j = 0; j < MAXKEYS; j++)
			{
				if(keys[i].ch[j] != 0)
				{
					if(shift || caps)
						txt[0] = keys[i].chShift[j];
					else
						txt[0] = keys[i].ch[j];

					keyTxt[i][j]->SetText(txt);
				}
			}
		}
		UpdateKeys = false;
	}
}

const wchar_t * GuiKeyboard::GetDisplayText()
{
	int len = kbtextstr.size();

	if(len < MAX_KEYBOARD_DISPLAY)
		return kbtextstr.c_str();

	int n = 0;
	int startPos = CurrentFirstLetter;
	int endPos = startPos+MAX_KEYBOARD_DISPLAY;

	for(int i = startPos; i < endPos && i < len; ++i)
	{
		displayTxt[n] = kbtextstr.at(i);
		++n;
	}

	displayTxt[n] = 0;

	return displayTxt;
}
