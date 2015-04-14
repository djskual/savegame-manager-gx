#ifndef GUI_BROWSERS_H
#define GUI_BROWSERS_H

#include "libwiigui/gui.h"
#include "libwiigui/gui_browsers/gui_savebrowser.h"
#include "libwiigui/gui_browsers/gui_miibrowser.h"

#define DEVICE_PAGESIZE		9
#define CUSTOM_PAGESIZE		9
#define PAGESIZE 			8
#define MAX_OPTIONS 		30
#define MAX_SAVES 			200

typedef struct _optionlist {
	int length;
	char name[MAX_OPTIONS][50];
	char value[MAX_OPTIONS][MAXPATHLEN];
} OptionList;

//!Display a list of menu options
class GuiOptionBrowser : public GuiElement
{
	public:
		GuiOptionBrowser(int w, int h, OptionList * l, int s);
		~GuiOptionBrowser();
		void SetCol2Position(int x);
		int FindMenuItem(int c, int d);
		int GetClickedOption();
		int GetSelectedOption();
		void ResetState();
		void SetFocus(int f);
		void Draw();
		void TriggerUpdate();
		void Update(GuiTrigger * t);
		GuiText * optionVal[PAGESIZE];
		int scrollable;
	protected:
		int selectedItem;
		int listOffset;
		bool listChanged;

		OptionList * options;
		int optionIndex[PAGESIZE];
		GuiButton * optionBtn[PAGESIZE];
		GuiText * optionTxt[PAGESIZE];
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

		GuiSound * btnSoundOver;
		GuiSound * btnSoundClick;
		GuiTrigger * trigA;
		GuiTrigger * trigHeldA;
};

//!Display a list of Lang
class GuiLangBrowser : public GuiElement
{
	public:
		GuiLangBrowser(int w, int h);
		~GuiLangBrowser();
		void ResetState();
		void SetFocus(int f);
		void Draw();
		void TriggerUpdate();
		void Update(GuiTrigger * t);
		GuiButton * customList[DEVICE_PAGESIZE];
	protected:
		int selectedItem;
		int numEntries;
		bool listChanged;
		
		GuiText * customListText[DEVICE_PAGESIZE];
		GuiImage * customListBg[DEVICE_PAGESIZE];
		GuiImage * customListFolder[DEVICE_PAGESIZE];
		
		GuiButton * arrowUpBtn;
		GuiButton * arrowDownBtn;
		GuiButton * scrollbarBoxBtn;
		
		GuiImage * arrowDownImg;
		GuiImage * arrowDownOverImg;
		GuiImage * arrowUpImg;
		GuiImage * arrowUpOverImg;
		GuiImage * scrollbarBoxImg;
		GuiImage * scrollbarBoxOverImg;
		
		GuiImageData * bgCustomSelectionEntry;
		GuiImageData * customFolder;
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

//!Display a list of files on devices
class GuiDeviceBrowser : public GuiElement
{
	public:
		GuiDeviceBrowser(int w, int h);
		~GuiDeviceBrowser();
		void ResetState();
		void SetFocus(int f);
		void Draw();
		void TriggerUpdate();
		void Update(GuiTrigger * t);
		GuiButton * deviceList[DEVICE_PAGESIZE];
	protected:
		int selectedItem;
		int numEntries;
		bool listChanged;

		GuiText * deviceListText[DEVICE_PAGESIZE];
		GuiImage * deviceListBg[DEVICE_PAGESIZE];
		GuiImage * deviceListFolder[DEVICE_PAGESIZE];
		GuiImage * deviceListLog[DEVICE_PAGESIZE];
		GuiImage * deviceListSave[DEVICE_PAGESIZE];
		GuiImage * deviceListNoSave[DEVICE_PAGESIZE];
		GuiImage * deviceListMii[DEVICE_PAGESIZE];

		GuiButton * arrowUpBtn;
		GuiButton * arrowDownBtn;
		GuiButton * scrollbarBoxBtn;

		GuiImage * bgDeviceSelectionImg;
		GuiImage * scrollbarImg;
		GuiImage * arrowDownImg;
		GuiImage * arrowDownOverImg;
		GuiImage * arrowUpImg;
		GuiImage * arrowUpOverImg;
		GuiImage * scrollbarBoxImg;
		GuiImage * scrollbarBoxOverImg;

		GuiImageData * bgDeviceSelection;
		GuiImageData * bgDeviceSelectionEntry;
		GuiImageData * deviceFolder;
		GuiImageData * deviceLog;
		GuiImageData * deviceSave;
		GuiImageData * deviceNoSave;
		GuiImageData * deviceMii;
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

//!Display a list of files on devices for custumPathBrowser
class GuiCustomPathBrowser : public GuiElement
{
	public:
		GuiCustomPathBrowser(int w, int h);
		~GuiCustomPathBrowser();
		void ResetState();
		void SetFocus(int f);
		void Draw();
		void TriggerUpdate();
		void Update(GuiTrigger * t);
		GuiButton * customList[DEVICE_PAGESIZE];
	protected:
		int selectedItem;
		int numEntries;
		bool listChanged;
		
		GuiText * customListText[DEVICE_PAGESIZE];
		GuiImage * customListBg[DEVICE_PAGESIZE];
		GuiImage * customListFolder[DEVICE_PAGESIZE];
		
		GuiButton * arrowUpBtn;
		GuiButton * arrowDownBtn;
		GuiButton * scrollbarBoxBtn;
		
		GuiImage * arrowDownImg;
		GuiImage * arrowDownOverImg;
		GuiImage * arrowUpImg;
		GuiImage * arrowUpOverImg;
		GuiImage * scrollbarBoxImg;
		GuiImage * scrollbarBoxOverImg;
		
		GuiImageData * bgCustomSelectionEntry;
		GuiImageData * customFolder;
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

//!Display a list of files on devices for custumFilePathBrowser
class GuiCustomFilePathBrowser : public GuiElement
{
	public:
		GuiCustomFilePathBrowser(int w, int h);
		~GuiCustomFilePathBrowser();
		void ResetState();
		void SetFocus(int f);
		void Draw();
		void TriggerUpdate();
		void Update(GuiTrigger * t);
		GuiButton * customList[DEVICE_PAGESIZE];
	protected:
		int selectedItem;
		int numEntries;
		bool listChanged;
		
		GuiText * customListText[DEVICE_PAGESIZE];
		GuiImage * customListBg[DEVICE_PAGESIZE];
		GuiImage * customListFolder[DEVICE_PAGESIZE];
		
		GuiButton * arrowUpBtn;
		GuiButton * arrowDownBtn;
		GuiButton * scrollbarBoxBtn;
		
		GuiImage * arrowDownImg;
		GuiImage * arrowDownOverImg;
		GuiImage * arrowUpImg;
		GuiImage * arrowUpOverImg;
		GuiImage * scrollbarBoxImg;
		GuiImage * scrollbarBoxOverImg;
		
		GuiImageData * bgCustomSelectionEntry;
		GuiImageData * customFolder;
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

