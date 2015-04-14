#ifndef SEARCHWINDOW_H_
#define SEARCHWINDOW_H_

#include <set>

#include "../GUI/gui.h"
#include "../GUI/PictureButton.h"
#include "../Themes/CTheme.h"

class cSearchButton: public PictureButton
{
	public:
		cSearchButton(std::wstring Char, const char *keyImageData, const char *keyOverImageData, int x, int y, GuiTrigger* trig, GuiSound* sndClick, GuiSound* sndOver);
		virtual ~cSearchButton();
		
		std::wstring wChar;
		
	private:
		GuiText * text;
};

class SearchWindow: public GuiFrame, public sigslot::has_slots<>
{
	public:
		SearchWindow(std::wstring currentFilter, std::set<wchar_t > searchChars, bool fadeIn = false);
		virtual ~SearchWindow();
		
		void Update(GuiTrigger * t);
		void AddNewChar(int i);
		void RemoveLastChar();
		
	private:
		void SetCharsButtons();
		void AddCharButton(std::wstring t, int x, int y);
		void OnButtonClick(GuiButton *sender, int pointer, const POINT &p);
		
		u16 inSide;
		std::set<wchar_t > SearchChars;
		std::wstring CurrentFilter;
		
		GuiText * Text;
		GuiText * modeText;
		
		GuiImage * BgImg;
		GuiImage * TextBarImg;
		
		GuiImageData * imgBacspaceBtn;
		GuiImage * BacspaceBtnImg;
		GuiImage * BacspaceBtnImg_Over;
		GuiButton * BacspaceBtn;
		
		GuiImageData * imgClearBtn;
		GuiImage * ClearBtnImg;
		GuiImage * ClearBtnImg_Over;
		GuiButton * ClearBtn;
		
		GuiButton * CloseBtn;
		PictureButton * searchModeBtn;
		
		std::vector<cSearchButton* > buttons;
		
		SimpleGuiTrigger * trigA;
		GuiTrigger * trigB;
		
		GuiSound * btnSoundClick;
		GuiSound * btnSoundOver;
};

#endif