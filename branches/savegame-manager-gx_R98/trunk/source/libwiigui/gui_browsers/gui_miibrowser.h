#ifndef _GUI_MIIBROWSER_H_
#define _GUI_MIIBROWSER_H_

#include "libwiigui/gui.h"

#define MII_PAGESIZE 			9

//!Display a list of miis stored in wii
class GuiMiiBrowser : public GuiElement
{
	public:
		GuiMiiBrowser(int w, int h);
		~GuiMiiBrowser();
		void ResetState();
		void SetFocus(int f);
		void Draw();
		void TriggerUpdate();
		void Update(GuiTrigger * t);
		GuiButton * miiBtn[MII_PAGESIZE];
	protected:
		int selectedItem;
		int numEntries;
		bool listChanged;

		GuiText * miiTxt[MII_PAGESIZE];
		GuiImage * bgMiiSelectionImg[MII_PAGESIZE];
		
		GuiButton * arrowUpBtn;
		GuiButton * arrowDownBtn;
		GuiButton * scrollbarBoxBtn;

		GuiImage * bgImg;
		GuiImage * scrollbarImg;
		GuiImage * arrowDownImg;
		GuiImage * arrowDownOverImg;
		GuiImage * arrowUpImg;
		GuiImage * arrowUpOverImg;
		GuiImage * scrollbarBoxImg;
		GuiImage * scrollbarBoxOverImg;

		GuiImageData * bgMiiSelection;
		GuiImageData * bg;
		GuiImageData * scrollbar;
		GuiImageData * arrowDown;
		GuiImageData * arrowDownOver;
		GuiImageData * arrowUp;
		GuiImageData * arrowUpOver;
		GuiImageData * scrollbarBox;
		GuiImageData * scrollbarBoxOver;

		GuiSound * btnSoundOver;
		GuiSound * btnSoundClick;
		GuiTrigger * trigA;
		GuiTrigger * trigHeldA;
};

#endif
