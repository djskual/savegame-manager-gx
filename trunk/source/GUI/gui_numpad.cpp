/***************************************************************************
 * Copyright (C) 2009 r-win
 * Copyright (C) 2012 Dimok
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
#include "gui_numpad.h"
#include "../Themes/CTheme.h"

/**
 * Constructor for the GuiNumpad class.
 */

GuiNumpad::GuiNumpad(char * t, u32 max, bool crypted)
	: GuiFrame(400, 290)
{
	SetAlignment(ALIGN_CENTER | ALIGN_MIDDLE);
	
	kbtextmaxlen = max > sizeof(kbtextstr) ? sizeof(kbtextstr) : max; //! limit max up to sizeof(kbtextstr)
	strncpy(kbtextstr, t, kbtextmaxlen); //! strncpy is needed to fill the rest with \0
	kbtextstr[sizeof(kbtextstr) - 1] = 0; //! terminate with \0

	char thekeys[NUMPAD_BUTTONS] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '0', '.' };
	memcpy(keys, thekeys, sizeof(thekeys));

	SoundClick = Resources::GetSound("button_click.wav");
	SoundOver = Resources::GetSound("button_over.wav");
	SoundClick->SetVolume(Settings.SFXVolume);
	SoundOver->SetVolume(Settings.SFXVolume);
	
	keyTextbox = Resources::GetImageData("keyboard_textbox.png");
	keyTextboxImg = new GuiImage(keyTextbox);
	keyTextboxImg->SetAlignment(ALIGN_CENTER | ALIGN_TOP);
	keyTextboxImg->SetPosition(0,0);
	this->Append(keyTextboxImg);

	kbText = new GuiText("", 20, thColor("r=0 g=0 b=0 a=255 - keyboard text color"));
	kbText->SetAlignment(ALIGN_CENTER | ALIGN_TOP);
	kbText->SetPosition(0, 13);
	if(crypted) kbText->SetPassChar('*');
	kbText->SetText(kbtextstr);
	this->Append(kbText);

	trigA = new SimpleGuiTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	trigB = new GuiTrigger();
	trigB->SetButtonOnlyTrigger(-1, WiiControls.BackButton | ClassicControls.BackButton << 16, GCControls.BackButton);
	
	keyBackText = new GuiText("Back", 20, thColor("r=0 g=0 b=0 a=255 - keyboard spec buttons text color"));
	keyBack = new PictureButton("keyboard_mediumkey.png", "keyboard_mediumkey_over.png", SoundOver, SoundClick);
	keyBack->SetAlignment(ALIGN_CENTER | ALIGN_BOTTOM);
	keyBack->SetPosition(90, 0);
	keyBack->SetLabel(keyBackText);
	keyBack->SetTrigger(trigA);
	keyBack->SetTrigger(trigB);
	keyBack->Clicked.connect(this, &GuiNumpad::OnSpecialKeyPress);
	this->Append(keyBack);

	keyClearText = new GuiText("Clear", 20, thColor("r=0 g=0 b=0 a=255 - keyboard spec buttons text color"));
	keyClear = new PictureButton("keyboard_mediumkey.png", "keyboard_mediumkey_over.png", SoundOver, SoundClick);
	keyClear->SetAlignment(ALIGN_CENTER | ALIGN_BOTTOM);
	keyClear->SetPosition(-90, 0);
	keyClear->SetLabel(keyClearText);
	keyClear->SetTrigger(trigA);
	keyClear->Clicked.connect(this, &GuiNumpad::OnSpecialKeyPress);
	this->Append(keyClear);

	char txt[2] = { 0, 0 };
	for (int i = 0; i < NUMPAD_BUTTONS; i++)
	{
		int col = i % 3;
		int row = i / 3;
		
		txt[0] = keys[i];
		
		keyTxt[i] = new GuiText(txt, 20, thColor("r=0 g=0 b=0 a=255 - keyboard buttons text color"));
		keyTxt[i]->SetAlignment(ALIGN_CENTER | ALIGN_BOTTOM);
		keyTxt[i]->SetPosition(0, -10);
		
		keyBtn[i] = new PictureButton("keyboard_mediumkey.png", "keyboard_mediumkey_over.png", SoundOver, SoundClick);
		keyBtn[i]->SetAlignment(ALIGN_CENTER | ALIGN_MIDDLE);
		keyBtn[i]->SetPosition(-90 + 90 * col, -75 + 50 * row);
		keyBtn[i]->SetLabel(keyTxt[i]);
		keyBtn[i]->SetTrigger(trigA);
		keyBtn[i]->Clicked.connect(this, &GuiNumpad::OnNormalKeyPress);
		this->Append(keyBtn[i]);
	}
}

/**
 * Destructor for the GuiKeyboard class.
 */
GuiNumpad::~GuiNumpad()
{
	RemoveAll();
	
	delete kbText;
	delete keyTextboxImg;
	Resources::Remove(keyTextbox);
	
	delete keyBackText;
	delete keyBack;
	
	delete keyClear;
	delete keyClearText;
	
	delete trigA;
	delete trigB;

	for (int i = 0; i < NUMPAD_BUTTONS; i++)
	{
		if (keys[i] != '\0')
		{
			delete keyTxt[i];
			delete keyBtn[i];
		}
	}
	
	Resources::Remove(SoundOver);
	Resources::Remove(SoundClick);
}

void GuiNumpad::OnNormalKeyPress(GuiButton *sender, int pointer UNUSED, const POINT &p UNUSED)
{
	char txt[2] = { 0, 0 };
	for (int i = 0; i < NUMPAD_BUTTONS; i++)
	{
		if (sender == keyBtn[i])
		{
			txt[0] = keys[i];
			if (strlen(kbtextstr) < kbtextmaxlen - 1)
			{
				kbtextstr[strlen(kbtextstr)] = txt[0];
				kbText->SetText(kbtextstr);
			}
		}
	}
}

void GuiNumpad::OnSpecialKeyPress(GuiButton *sender, int pointer UNUSED, const POINT &p UNUSED)
{
	if(sender == keyBack)
	{
		if (strlen(kbtextstr) > 0)
		{
			kbtextstr[strlen(kbtextstr) - 1] = 0;
			kbText->SetText(kbtextstr);
		}
	}
	else if(sender == keyClear)
	{
		memset(kbtextstr, 0, sizeof(kbtextstr));
		kbText->SetText(kbtextstr);
	}
}
