#ifndef GUI_CUSTOMBROWSER_H_
#define GUI_CUSTOMBROWSER_H_

#include "libwiigui/gui_browsers/gui_browsers.h"
#include "libwiigui/gui_browsers/CustomOptionList.hpp"
#include <vector>

//!Display a list of menu options
class GuiCustomOptionBrowser: public GuiElement
{
    public:
        GuiCustomOptionBrowser(int w, int h, CustomOptionList * l);
        ~GuiCustomOptionBrowser();
        int FindMenuItem(int c, int d);
        int GetClickedOption();
        int GetSelectedOption();
        void SetClickable(bool enable);
        void SetOffset(int optionnumber);
        void ResetState();
        void SetFocus(int f);
        void Draw();
        void Update(GuiTrigger * t);
		
    protected:
        void UpdateListEntries();
        int selectedItem;
        int listOffset;
        int coL2;
        bool scrollbaron;

        CustomOptionList * options;
        int optionIndex[PAGESIZE];
        GuiButton * optionBtn[PAGESIZE];
        GuiText * optionTxt[PAGESIZE];
        GuiText * optionVal[PAGESIZE];
        GuiText * optionValOver[PAGESIZE];
        GuiImage * optionBg[PAGESIZE];

        GuiButton * arrowUpBtn;
        GuiButton * arrowDownBtn;
        GuiButton * scrollbarBoxBtn;

        GuiImage * bgOptionsImg;
        GuiImage * scrollbarImg;
        GuiImage * arrowDownImg;
        GuiImage * arrowDownOverImg;
        GuiImage * arrowUpImg;
        GuiImage * arrowUpOverImg;
        GuiImage * scrollbarBoxImg;
        GuiImage * scrollbarBoxOverImg;

        GuiImageData * bgOptions;
        GuiImageData * bgOptionsEntry;
        GuiImageData * scrollbar;
        GuiImageData * arrowDown;
        GuiImageData * arrowDownOver;
        GuiImageData * arrowUp;
        GuiImageData * arrowUpOver;
        GuiImageData * scrollbarBox;
        GuiImageData * scrollbarBoxOver;

        GuiSound * btnSoundClick;
		
		GuiTrigger * trigA;
        GuiTrigger * trigHeldA;
};

#endif
