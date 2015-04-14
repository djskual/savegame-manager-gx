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
#ifndef GUI_KEYBOARD_H_
#define GUI_KEYBOARD_H_

#include "gui_frame.h"
#include "gui_button.h"
#include "gui_text.h"
#include "gui_image.h"
#include "../TextOperations/TextPointer.h"
#include "../TextOperations/wstring.hpp"

#define MAX_KEYBOARD_DISPLAY	40
#define MAXKEYS					13
#define MAXROWS					4

typedef struct _keyrowtype
{
	wchar_t ch[MAXKEYS];
	wchar_t chShift[MAXKEYS];
} KeyboardRow;

//!On-screen keyboard
class GuiKeyboard : public GuiFrame, public sigslot::has_slots<>
{
	public:
		GuiKeyboard(const char * t, u32 max);
		GuiKeyboard(const wchar_t * t, u32 max);
		virtual ~GuiKeyboard();
		const wchar_t * GetString();
		std::string GetUTF8String() const;
		void AddChar(int pos, wchar_t Char);
		void RemoveChar(int pos);
		void Update(GuiTrigger * t);
	protected:
		void SwitchKeyLanguage();
		void SetupKeyboard(const wchar_t * t, u32 max);
		void MoveText(int n);
		const wchar_t * GetDisplayText();
		void OnPointerHeld(GuiButton *sender, int pointer, const POINT &p);
		void OnPositionMoved(GuiButton *sender, int pointer, const POINT &p);
		void OnSpecialKeyPress(GuiButton *sender, int pointer, const POINT &p);
		void OnNormalKeyPress(GuiButton *sender, int pointer, const POINT &p);

		u32 DeleteDelay;
		int CurrentFirstLetter;
		wString kbtextstr;
		wchar_t displayTxt[MAX_KEYBOARD_DISPLAY];
		u32 kbtextmaxlen;
		KeyboardRow keys[MAXROWS];
		int ShiftChan;
		bool shift;
		bool caps;
		bool UpdateKeys;
		bool DefaultKeys;
		TextPointer * TextPointerBtn;
		GuiButton * GoLeft;
		GuiButton * GoRight;
		GuiText * kbText;
		GuiImage * keyTextboxImg;
		GuiText * keyCapsText;
		GuiImage * keyCapsImg;
		GuiImage * keyCapsOverImg;
		GuiButton * keyCaps;
		GuiText * keyShiftText;
		GuiImage * keyShiftImg;
		GuiImage * keyShiftOverImg;
		GuiButton * keyShift;
		GuiText * keyBackText;
		GuiImage * keyBackImg;
		GuiImage * keyBackOverImg;
		GuiButton * keyBack;
		GuiImage * keySpaceImg;
		GuiImage * keySpaceOverImg;
		GuiButton * keySpace;
		GuiText * keyLineBreakText;
		GuiImage * keyLineBreakImg;
		GuiImage * keyLineBreakOverImg;
		GuiButton * keyLineBreak;
		GuiText * keyClearText;
		GuiImage * keyClearImg;
		GuiImage * keyClearOverImg;
		GuiButton * keyClear;
		GuiText * keyLangText;
		GuiImage * keyLangImg;
		GuiImage * keyLangOverImg;
		GuiButton * keyLang;
		GuiButton * keyBtn[MAXROWS][MAXKEYS];
		GuiImage * keyImg[MAXROWS][MAXKEYS];
		GuiImage * keyImgOver[MAXROWS][MAXKEYS];
		GuiText * keyTxt[MAXROWS][MAXKEYS];
		GuiImageData * keyTextbox;
		GuiImageData * key;
		GuiImageData * keyOver;
		GuiImageData * keyMedium;
		GuiImageData * keyMediumOver;
		GuiImageData * keyLarge;
		GuiImageData * keyLargeOver;
		GuiSound * keySoundOver;
		GuiSound * keySoundClick;
		GuiTrigger * trigA;
		GuiTrigger * trigHeldA;
		GuiTrigger * trigLeft;
		GuiTrigger * trigRight;
};

#endif
