#ifndef _GUI_SAVEBROWSER_H_
#define _GUI_SAVEBROWSER_H_

#include "libwiigui/gui_banner.h"

#define SAVE_PAGESIZE	4

//!Display a list of saves stored in wii
class GuiSaveBrowser : public GuiElement
{
	public:
		GuiSaveBrowser(int w, int h);
		~GuiSaveBrowser();
		void ResetState();
		void SetFocus(int f);
		void Draw();
		void TriggerUpdate();
		void Update(GuiTrigger * t);
		GuiButton * saveBtn[SAVE_PAGESIZE];
	protected:
		int selectedItem;
		int numEntries;
		bool listChanged;

		GuiImage * saveBg[SAVE_PAGESIZE];
		GuiImage * bgImg[SAVE_PAGESIZE];
		
		GuiButton * arrowUpBtn;
		GuiButton * arrowDownBtn;
		GuiButton * scrollbarBoxBtn;

		GuiImage * bgSaveSelectionImg;
		GuiImage * scrollbarImg;
		GuiImage * arrowDownImg;
		GuiImage * arrowDownOverImg;
		GuiImage * arrowUpImg;
		GuiImage * arrowUpOverImg;
		GuiImage * scrollbarBoxImg;
		GuiImage * scrollbarBoxOverImg;

		GuiImageData * bgSaveSelection;
		GuiImageData * bgSaveSelectionEntry;
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

		GuiSaveBanner ** banner;
};

#endif
