#ifndef SAVEGAMEMENU_HPP_
#define SAVEGAMEMENU_HPP_

#include <string>

#include "../LibWiiGui/gui.h"
#include "../LibWiiGui/gui_searchbar.h"
#include "../Controls/TaskBar.hpp"
#include "../Prompts/DeviceMenu.h"
#include "../Saves/IconSaveBrowser.hpp"
#include "../Saves/ListSaveBrowser.hpp"
#include "../Saves/SaveBrowser.h"
#include "../Miis/IconMiiBrowser.hpp"
#include "../Miis/ListMiiBrowser.hpp"
#include "../Miis/MiiBrowser.h"
#include "../EmuNand/IconEmuBrowser.hpp"
#include "../EmuNand/ListEmuBrowser.hpp"
#include "../EmuNand/EmuBrowser.h"
#include "../EmuNand/IconEmuMiiBrowser.hpp"
#include "../EmuNand/ListEmuMiiBrowser.hpp"
#include "../EmuNand/EmuMiiBrowser.h"
#include "../Files/IconFileDeviceBrowser.hpp"
#include "../Files/ListFileDeviceBrowser.hpp"
#include "../Files/filebrowser.h"
#include "../Tools/ThreadedTask.hpp"
#include "../Text/wstring.hpp"

class SaveGameMenu : public GuiWindow
{
	public:
		SaveGameMenu();
		virtual ~SaveGameMenu();
		int Show();
		int LoadDevicePath(std::string path);
		
		const wchar_t * GetCurrentFilter();
		const wchar_t * GetAvailableSearchChars();
		
	private:
		void ReloadBrowser();
		void FilterList(const wchar_t * filter = NULL);
		int MainLoop();
		void CheckDeviceBrowserChanges();
		void CheckSaveBrowserChanges();
		void CheckEmuBrowserChanges();
		void CheckMiiBrowserChanges();
		void CheckEmuMiiBrowserChanges();
		bool CheckDeviceMenu();
		bool CheckTaskBarChanges();
		void BackInDirectory();
		void UpdateClock();
		static void UpdateCallback(void * e);
		
		int returnMenu;
		int browserMode;
		time_t lastrawtime;
		
		bool show_searchwindow;
		wchar_t searchChar;
		
		Browser * CurBrowser;
		FileBrowser * DeviceBrowser;
		SaveBrowser * NandSaveBrowser;
		EmuBrowser * EmuSaveBrowser;
		MiiBrowser * NandMiiBrowser;
		EmuMiiBrowser * EmulMiiBrowser;
		
		GuiFileBrowser * fileBrowser;
		IconFileDeviceBrowser * storageIconBrowser;
		ListFileDeviceBrowser * storageListBrowser;
		IconSaveBrowser * saveIconBrowser;
		ListSaveBrowser * saveListBrowser;
		IconEmuBrowser * emuIconBrowser;
		ListEmuBrowser * emuListBrowser;
		IconMiiBrowser * miiIconBrowser;
		ListMiiBrowser * miiListBrowser;
		IconEmuMiiBrowser * emuMiiIconBrowser;
		ListEmuMiiBrowser * emuMiiListBrowser;
		
		GuiSearchBar * searchBar;
		
		GuiTrigger * trigA;
		GuiTrigger * trigHome;
		GuiTrigger * trigBackInDir;
		
		GuiImageData * browserBg;
		GuiImageData * btnManage;
		GuiImageData * btnManageOver;
		GuiImageData * btnSettings;
		GuiImageData * btnSettingsOver;
		GuiImageData * btnHomebrew;
		GuiImageData * btnHomebrewOver;
		GuiImageData * btnHome;
		GuiImageData * btnHomeOver;
		GuiImageData * btnSdcard;
		GuiImageData * btnSdcardOver;
		GuiImageData * btnSdcardGray;
		
		GuiImage * manageBtnImg;
		GuiImage * manageBtnImgOver;
		GuiImage * settingsBtnImg;
		GuiImage * settingsBtnImgOver;
		GuiImage * homebrewBtnImg;
		GuiImage * homebrewBtnImgOver;
		GuiImage * homeBtnImg;
		GuiImage * homeBtnImgOver;
		GuiImage * sdcardBtnImg;
		GuiImage * sdcardBtnImgOver;
		GuiImage * sdcardBtnImgGray;
		
		GuiText * clockTimeBack;
		GuiText * clockTime;
		
		GuiButton * manageBtn;
		GuiButton * settingsBtn;
		GuiButton * homebrewBtn;
		GuiButton * homeBtn;
		GuiButton * sdcardBtn;
		GuiButton * BackInDirBtn;
		
		GuiTooltip * manageBtnTT;
		GuiTooltip * settingsBtnTT;
		GuiTooltip * homebrewBtnTT;
		GuiTooltip * homeBtnTT;
		GuiTooltip * sdcardBtnTT;
		
		TaskBar * taskbar;
		DeviceMenu * Device_Menu;
};

#endif
