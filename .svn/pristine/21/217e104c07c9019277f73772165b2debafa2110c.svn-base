#include <unistd.h>

#include "menus.h"
#include "SaveGameMenu.hpp"
#include "../DeviceControls/DeviceHandler.hpp"
#include "../Network/networkops.h"
#include "../Network/update.h"
#include "../ManageAll/AllWindow.hpp"
#include "../Manage/SaveWindow.hpp"
#include "../Manage/EmuSaveWindow.hpp"
#include "../Manage/EmuMiiWindow.hpp"
#include "../Manage/FileSaveWindow.hpp"
#include "../Manage/FileMiiWindow.hpp"
#include "../Manage/MiiWindow.hpp"
#include "../Manage/DataBinWindow.hpp"
#include "../Saves/CSaveList.hpp"
#include "../EmuNand/CEmuList.hpp"
#include "../EmuNand/CEmuMiiList.hpp"
#include "../Miis/CMiiList.hpp"
#include "../Prompts/PromptWindows.h"
#include "../Themes/CTheme.h"
#include "../Tools/gecko.h"
#include "../Tools/ShowError.h"
#include "../Tools/PasswordCheck.h"
#include "../Settings/CSettings.h"
#include "../System/wpad.h"

static bool Exiting = false;

SaveGameMenu::SaveGameMenu()
	: GuiWindow(screenwidth, screenheight)
{
	returnMenu = MENU_NONE;
	lastrawtime = 0;
	Exiting = false;
	searchBar = NULL;
	show_searchwindow = false;
	saveIconBrowser = NULL;
	saveListBrowser = NULL;
	emuIconBrowser = NULL;
	emuListBrowser = NULL;
	miiIconBrowser = NULL;
	miiListBrowser = NULL;
	emuMiiIconBrowser = NULL;
	emuMiiListBrowser = NULL;
	storageIconBrowser = NULL;
	storageListBrowser = NULL;
	fileBrowser = NULL;
	Device_Menu = NULL;
	
	if(Settings.BrowserPath.size() == 0)
	{
		browserMode = Settings.BootBrowser;
		Settings.LastBrowser = browserMode;
		if (browserMode == BROWSE_NAND || browserMode == BROWSE_SDUSB)
			Settings.BrowserPath = Settings.SaveGamePath;
		else
			Settings.BrowserPath = Settings.SaveMiisPath; 
	}
	else
		browserMode = Settings.LastBrowser;
	
	if (browserMode == BROWSE_SDUSB)
		FileList.SetCurrentFilter((const wchar_t *)"\0");
	else
	{
		wString oldFilter(GetCurrentFilter());
		FilterList(oldFilter.c_str());
	}
	
	DeviceBrowser = new FileBrowser();
	NandSaveBrowser = new SaveBrowser();
	NandMiiBrowser = new MiiBrowser();
	EmuSaveBrowser = new EmuBrowser();
	EmulMiiBrowser = new EmuMiiBrowser();
	
	if (browserMode == BROWSE_SDUSB)
		CurBrowser = DeviceBrowser;
	else if (browserMode == BROWSE_NAND)
		CurBrowser = NandSaveBrowser;
	else if (browserMode == BROWSE_MII)
		CurBrowser = NandMiiBrowser;
	else if (browserMode == BROWSE_EMU)
		CurBrowser = EmuSaveBrowser;
	else if (browserMode == BROWSE_EMU_MII)
		CurBrowser = EmulMiiBrowser;
	
	taskbar = new TaskBar(NULL);
	taskbar->SetAlignment(ALIGN_CENTER, ALIGN_TOP);
	taskbar->SetPosition(thInt("0 - taskbar pos x"), thInt("13 - taskbar pos y"));

	trigA = new SimpleGuiTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	trigBackInDir = new GuiTrigger();
	trigBackInDir->SetButtonOnlyTrigger(-1, WiiControls.UpInDirectory | ClassicControls.UpInDirectory << 16, GCControls.UpInDirectory);
	trigHome = new GuiTrigger();
	trigHome->SetButtonOnlyTrigger(-1, WiiControls.HomeButton | ClassicControls.HomeButton << 16, GCControls.HomeButton);
	
	browserBg = Resources::GetImageData("browser.png");
	
	btnManage = Resources::GetImageData("menu_button_switch.png");
	btnManageOver = Resources::GetImageData("menu_button_switch_over.png");
	btnSettings = Resources::GetImageData("menu_button_settings.png");
	btnSettingsOver = Resources::GetImageData("menu_button_settings_over.png");
	btnHomebrew = Resources::GetImageData("menu_button_wii.png");
	btnHomebrewOver = Resources::GetImageData("menu_button_wii_over.png");
	btnHome = Resources::GetImageData("menu_button_home.png");
	btnHomeOver = Resources::GetImageData("menu_button_home_over.png");
	btnSdcard = Resources::GetImageData("menu_button_sdcard.png");
	btnSdcardOver = Resources::GetImageData("menu_button_sdcard_over.png");
	btnSdcardGray = Resources::GetImageData("menu_button_sdcard_gray.png");
	
	manageBtnTT = new GuiTooltip(tr( "Manage All" ));
	if (Settings.wsprompt) manageBtnTT->SetWidescreen(Settings.Widescreen);
	manageBtnTT->SetAlpha(thInt("255 - tooltip alpha"));
	manageBtnImg = new GuiImage(btnManage);
	manageBtnImg->SetWidescreen(Settings.Widescreen);
	manageBtnImgOver = new GuiImage(btnManageOver);
	manageBtnImgOver->SetWidescreen(Settings.Widescreen);

	manageBtn = new GuiButton(manageBtnImg, manageBtnImgOver,
							  ALIGN_LEFT, ALIGN_TOP,
							  thInt("16 - manageAll btn pos x"), thInt("355 - managAll btn pos y"),
							  trigA,
							  btnSoundOver, btnSoundClick,
							  1,
							  manageBtnTT, 24, -30, 0, 5);
	
	settingsBtnTT = new GuiTooltip(tr( "Settings" ));
	if (Settings.wsprompt) settingsBtnTT->SetWidescreen(Settings.Widescreen);
	settingsBtnTT->SetAlpha(thInt("255 - tooltip alpha"));
	settingsBtnImg = new GuiImage(btnSettings);
	settingsBtnImg->SetWidescreen(Settings.Widescreen);
	settingsBtnImgOver = new GuiImage(btnSettingsOver);
	settingsBtnImgOver->SetWidescreen(Settings.Widescreen);
	
	settingsBtn = new GuiButton(settingsBtnImg, settingsBtnImgOver,
								0, 3,
								thInt("64 - settings btn pos x"), thInt("371 - settings btn pos y"),
								trigA,
								btnSoundOver, btnSoundClick,
								1,
								settingsBtnTT, 65, -30, 0, 5);

	homebrewBtnTT = new GuiTooltip(tr( "Homebrew Launcher" ));
	if (Settings.wsprompt) homebrewBtnTT->SetWidescreen(Settings.Widescreen);
	homebrewBtnTT->SetAlpha(thInt("255 - tooltip alpha"));
	homebrewBtnImg = new GuiImage(btnHomebrew);
	homebrewBtnImg->SetWidescreen(Settings.Widescreen);
	homebrewBtnImgOver = new GuiImage(btnHomebrewOver);
	homebrewBtnImgOver->SetWidescreen(Settings.Widescreen);
	
	homebrewBtn = new GuiButton(homebrewBtnImg, homebrewBtnImgOver,
								0, 3,
								thInt("489 - homebrew btn pos x"), thInt("371 - homebrew btn pos y"),
								trigA,
								btnSoundOver,btnSoundClick,
								1,
								homebrewBtnTT, -10, -30, 1, 5);

	homeBtnTT = new GuiTooltip(tr( "Home Menu" ));
	if (Settings.wsprompt) homeBtnTT->SetWidescreen(Settings.Widescreen);
	homeBtnTT->SetAlpha(thInt("255 - tooltip alpha"));
	homeBtnImg = new GuiImage(btnHome);
	homeBtnImg->SetWidescreen(Settings.Widescreen);
	homeBtnImgOver = new GuiImage(btnHomeOver);
	homeBtnImgOver->SetWidescreen(Settings.Widescreen);
	
	homeBtn = new GuiButton(homeBtnImg, homeBtnImgOver,
							0, 3,
							thInt("576 - homemenu btn pos x"), thInt("355 - homemenu btn pos y"),
							trigA,
							btnSoundOver, btnSoundClick,
							1,
							homeBtnTT, 15, -30, 1, 5);
	homeBtn->RemoveSoundClick();
	homeBtn->SetTrigger(trigHome);
	
	sdcardBtnTT = new GuiTooltip(tr( "Reload SD" ));
	if (Settings.wsprompt) sdcardBtnTT->SetWidescreen(Settings.Widescreen);
	sdcardBtnTT->SetAlpha(thInt("255 - tooltip alpha"));
	sdcardBtnImg = new GuiImage(btnSdcard);
	sdcardBtnImgGray = new GuiImage(btnSdcardGray);
	sdcardBtnImgOver = new GuiImage(btnSdcardOver);
	sdcardBtnImg->SetWidescreen(Settings.Widescreen);
	sdcardBtnImgGray->SetWidescreen(Settings.Widescreen);
	sdcardBtnImgOver->SetWidescreen(Settings.Widescreen);
	
	sdcardBtn = new GuiButton(sdcardBtnImg, sdcardBtnImgOver,
							  0, 3,
							  thInt("160 - sd card btn pos x"), thInt("395 - sd card btn pos y"),
							  trigA,
							  btnSoundOver, btnSoundClick,
							  1,
							  sdcardBtnTT, 15, -30, 0, 5);
	
	BackInDirBtn = new GuiButton(0, 0);
	BackInDirBtn->SetTrigger(trigBackInDir);
	BackInDirBtn->SetSoundClick(btnSoundClick);

	GXColor clockColor = thColor("r=138 g=138 b=138 a=240 - clock color");
	clockTimeBack = new GuiText("88:88", 40, (GXColor) {clockColor.r, clockColor.g, clockColor.b, clockColor.a / 6});
	clockTimeBack->SetAlignment(thAlign("left - clock align hor"), thAlign("top - clock align ver"));
	clockTimeBack->SetPosition(thInt("275 - clock pos x"), thInt("340 - clock pos y"));
	clockTimeBack->SetFont(Resources::GetFile("clock.ttf"), Resources::GetFileSize("clock.ttf"));

	clockTime = new GuiText("", 40, clockColor);
	clockTime->SetAlignment(thAlign("left - clock align hor"), thAlign("top - clock align ver"));
	clockTime->SetPosition(thInt("275 - clock pos x"), thInt("340 - clock pos y"));
	clockTime->SetFont(Resources::GetFile("clock.ttf"), Resources::GetFileSize("clock.ttf"));

	ReloadBrowser();
}

SaveGameMenu::~SaveGameMenu()
{
	Settings.LastBrowser = browserMode;
	
	Exiting = true;
	ResumeGui();

	SetEffect(EFFECT_FADE, -20);
	while(parentElement && this->GetEffect() > 0) usleep(100);

	HaltGui();
	if(parentElement)
		((GuiWindow *) parentElement)->Remove(this);

	RemoveAll();
	
	delete trigA;
	delete trigHome;
	delete trigBackInDir;

	delete browserBg;
	delete btnManage;
	delete btnManageOver;
	delete btnHomebrew;
	delete btnHomebrewOver;
	delete btnSettings;
	delete btnSettingsOver;
	delete btnHome;
	delete btnHomeOver;
	delete btnSdcard;
	delete btnSdcardOver;
	
	delete manageBtnImg;
	delete manageBtnImgOver;
	delete settingsBtnImg;
	delete settingsBtnImgOver;
	delete homebrewBtnImg;
	delete homebrewBtnImgOver;
	delete homeBtnImg;
	delete homeBtnImgOver;
	delete sdcardBtnImg;
	delete sdcardBtnImgOver;
	
	delete clockTimeBack;
	delete clockTime;

	delete manageBtn;
	delete settingsBtn;
	delete homebrewBtn;
	delete homeBtn;
	delete sdcardBtn;
	delete BackInDirBtn;
	
	delete manageBtnTT;
	delete settingsBtnTT;
	delete homebrewBtnTT;
	delete homeBtnTT;
	delete sdcardBtnTT;
	
	if(storageIconBrowser)
		delete storageIconBrowser;
	if(storageListBrowser)
		delete storageListBrowser;
	delete DeviceBrowser;
	if(saveIconBrowser)
		delete saveIconBrowser;
	if(saveListBrowser)
		delete saveListBrowser;
	delete NandSaveBrowser;
	if(emuIconBrowser)
		delete emuIconBrowser;
	if(emuListBrowser)
		delete emuListBrowser;
	delete EmuSaveBrowser;
	if(miiIconBrowser)
		delete miiIconBrowser;
	if(saveListBrowser)
		delete miiListBrowser;
	delete NandMiiBrowser;
	if(emuMiiIconBrowser)
		delete emuMiiIconBrowser;
	if(emuMiiListBrowser)
		delete emuMiiListBrowser;
	delete EmulMiiBrowser;
	
	mainWindow->Remove(searchBar);
	delete searchBar;
	
	delete taskbar;
	if(Device_Menu)
		delete Device_Menu;
	
	ResumeGui();
}

void SaveGameMenu::ReloadBrowser()
{
	ResumeGui();

	SetEffect(EFFECT_FADE, -40);
	while(parentElement && this->GetEffect() > 0) usleep(100);

	HaltGui();
	RemoveAll();
	mainWindow->Remove(searchBar);
	
	if(storageIconBrowser)
		delete storageIconBrowser;
	if(storageListBrowser)
		delete storageListBrowser;
	if(saveIconBrowser)
		delete saveIconBrowser;
	if(saveListBrowser)
		delete saveListBrowser;
	if(emuIconBrowser)
		delete emuIconBrowser;
	if(emuListBrowser)
		delete emuListBrowser;
	if(miiIconBrowser)
		delete miiIconBrowser;
	if(miiListBrowser)
		delete miiListBrowser;
	if(emuMiiIconBrowser)
		delete emuMiiIconBrowser;
	if(emuMiiListBrowser)
		delete emuMiiListBrowser;
	if(Device_Menu)
		delete Device_Menu;
	delete searchBar;
	storageIconBrowser = NULL;
	storageListBrowser = NULL;
	saveIconBrowser = NULL;
	saveListBrowser = NULL;
	emuIconBrowser = NULL;
	emuListBrowser = NULL;
	miiIconBrowser = NULL;
	miiListBrowser = NULL;
	emuMiiIconBrowser = NULL;
	emuMiiListBrowser = NULL;
	searchBar = NULL;
	Device_Menu = NULL;
	
	if(browserMode == BROWSE_SDUSB)
	{
		if(Settings.BrowserMode == ICONMODE)
		{
			storageIconBrowser = new IconFileDeviceBrowser(CurBrowser, browserBg, browserBg->GetWidth(), browserBg->GetHeight());
			fileBrowser = storageIconBrowser;
		}
		else
		{
			storageListBrowser = new ListFileDeviceBrowser(CurBrowser, browserBg, browserBg->GetWidth(), browserBg->GetHeight());
			fileBrowser = storageListBrowser;
		}
		
		LoadDevicePath(Settings.BrowserPath);
		
		std::string dev = Settings.BrowserPath;
		dev = dev.substr(0, dev.find_first_of(":"));
		
		if(!dev.compare("sd"))
			taskbar->SetDeviceImage(TASKBAR_SD);
		else
			taskbar->SetDeviceImage(TASKBAR_USB);
	}
	else if (browserMode == BROWSE_NAND)
	{
		if(Settings.BrowserMode == ICONMODE)
		{
			saveIconBrowser = new IconSaveBrowser(CurBrowser, browserBg, browserBg->GetWidth(), browserBg->GetHeight());
			fileBrowser = saveIconBrowser;
		}
		else
		{
			saveListBrowser = new ListSaveBrowser(CurBrowser, browserBg, browserBg->GetWidth(), browserBg->GetHeight());
			fileBrowser = saveListBrowser;
		}
		
		NandSaveBrowser->GetSaveList();
		taskbar->SetDeviceImage(TASKBAR_WII);
	}
	else if (browserMode == BROWSE_EMU)
	{
		if(Settings.BrowserMode == ICONMODE)
		{
			emuIconBrowser = new IconEmuBrowser(CurBrowser, browserBg, browserBg->GetWidth(), browserBg->GetHeight());
			fileBrowser = emuIconBrowser;
		}
		else
		{
			emuListBrowser = new ListEmuBrowser(CurBrowser, browserBg, browserBg->GetWidth(), browserBg->GetHeight());
			fileBrowser = emuListBrowser;
		}
		
		EmuSaveBrowser->GetEmuList();
		taskbar->SetDeviceImage(TASKBAR_EMU);
	}
	else if (browserMode == BROWSE_MII)
	{
		if(Settings.BrowserMode == ICONMODE)
		{
			miiIconBrowser = new IconMiiBrowser(CurBrowser, browserBg, browserBg->GetWidth(), browserBg->GetHeight());
			fileBrowser = miiIconBrowser;
		}
		else
		{
			miiListBrowser = new ListMiiBrowser(CurBrowser, browserBg, browserBg->GetWidth(), browserBg->GetHeight());
			fileBrowser = miiListBrowser;
		}
		
		NandMiiBrowser->GetMiiList();
		taskbar->SetDeviceImage(TASKBAR_MII);
	}
	else if (browserMode == BROWSE_EMU_MII)
	{
		if(Settings.BrowserMode == ICONMODE)
		{
			emuMiiIconBrowser = new IconEmuMiiBrowser(CurBrowser, browserBg, browserBg->GetWidth(), browserBg->GetHeight());
			fileBrowser = emuMiiIconBrowser;
		}
		else
		{
			emuMiiListBrowser = new ListEmuMiiBrowser(CurBrowser, browserBg, browserBg->GetWidth(), browserBg->GetHeight());
			fileBrowser = emuMiiListBrowser;
		}
		
		EmulMiiBrowser->GetEmuMiiList();
		taskbar->SetDeviceImage(TASKBAR_EMU_MII);
	}
	
	taskbar->SetModeButton();
	
	fileBrowser->SetAlignment(thAlign("left - device browser align hor"), thAlign("middle - device browser align vert"));
	fileBrowser->SetPosition(thInt("30 - device browser pos x"), thInt("-50 - device browser pos y"));
	
	if (*GetCurrentFilter() && !show_searchwindow)
	{
		taskbar->SetSearchEffect(true);
	}
	else if(show_searchwindow)
	{
		taskbar->SetSearchEffect(false);
	}
	else
	{
		taskbar->StopSearchEffect();
	}
	
	if(DeviceHandler::Instance()->IsInserted(SD))
	{
		sdcardBtn->SetImage(sdcardBtnImg);
		sdcardBtn->SetState(STATE_DEFAULT);
	}
	else
	{
		sdcardBtn->SetImage(sdcardBtnImgGray);
		sdcardBtn->SetState(STATE_DISABLED);
	}

	Append(homeBtn);
	Append(manageBtn);
	Append(homebrewBtn);
	Append(settingsBtn);
	if (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_SD_RELOAD_BUTTON))
		Append(sdcardBtn);
	
	Append(BackInDirBtn);
	Append(taskbar);
	
	Append(fileBrowser);

	if ((Settings.Clock == CLOCK_HR12) || (Settings.Clock == CLOCK_HR24))
	{
		Append(clockTimeBack);
		Append(clockTime);
	}
	
	mainWindow->SetState(STATE_DEFAULT);
	
	if (show_searchwindow)
	{
		searchBar = new GuiSearchBar(GetAvailableSearchChars(), browserMode);
		mainWindow->Append(searchBar);
	}
	
	SetEffect(EFFECT_FADE, 40);
	ResumeGui();

	while(parentElement && this->GetEffect() > 0) usleep(100);
}

int SaveGameMenu::LoadDevicePath(std::string path)
{
	int len = path.size()-1;
	while(path[len] == '/')
	{
		path.erase(len, 1);
		len--;
	}
	if(path[len] == ':')
		path.erase(len, 1);
	
	int filecount = 0;
	if(!path.compare(DeviceName[SD]))
	{
		filecount = DeviceBrowser->BrowseDevice(SD);
		goto out;
	}
	for (int i = USB1; i <= USB8; i++)
	{
		if (!path.compare(DeviceName[i]))
		{
			filecount = DeviceBrowser->BrowseDevice(i);
			goto out;
		}
	}
	
	if(CurBrowser != DeviceBrowser)
		return -2;

	filecount = DeviceBrowser->BrowsePath(path);
	if(filecount < 0)
	{
		int choice = WindowPrompt(tr("Error:"),
		tr("Unable to load path."),
		tr("Retry"),
		tr("Close"));

		if(choice)
			return LoadDevicePath(path);
		else
			return -3;
	}

  out:
	
	DeviceBrowser->ResetMarker();
	Settings.BrowserPath.assign(DeviceBrowser->GetCurrentPath());
	return filecount;
}

int SaveGameMenu::Show()
{
	int menu = MENU_NONE;

	while(menu == MENU_NONE)
	{
		usleep(100);

		menu = MainLoop();
	}

	return menu;
}


int SaveGameMenu::MainLoop()
{
	UpdateClock();
	Receiver->CheckIncoming();
	
	if(SD_Port->StateChanged())
		ReloadBrowser();
	
	if (CheckTaskBarChanges())
		return MENU_SAVEGAME;
	if (CheckDeviceMenu())
		return MENU_SAVEGAME;
		
	if(browserMode == BROWSE_SDUSB)
		CheckDeviceBrowserChanges();
	else if (browserMode == BROWSE_NAND)
		CheckSaveBrowserChanges();
	else if (browserMode == BROWSE_MII)
		CheckMiiBrowserChanges();
	else if (browserMode == BROWSE_EMU)
		CheckEmuBrowserChanges();
	else if (browserMode == BROWSE_EMU_MII)
		CheckEmuMiiBrowserChanges();
	
	if (!Settings.updateChecked && Settings.AutoConnect && IsNetworkInit())
	{
		CheckForUpdate();
		
		Settings.updateChecked = true;
	}
	
	if (manageBtn->GetState() == STATE_CLICKED)
	{
		if (!Settings.GodMode && (Settings.ParentalBlocks & BLOCK_MANAGE_ALL))
		{
			WindowPrompt(tr( "Permission denied." ), tr( "Console must be unlocked for this option." ), tr( "OK" ));
			manageBtn->ResetState();
			return MENU_NONE;
		}
		
		AllWindow * allWindow = new AllWindow();
		
		mainWindow->SetState(STATE_DISABLED);
		mainWindow->Append(allWindow);
		
		allWindow->Show();
		
		if(allWindow->RefreshNeeded())
		{
			if (allWindow->RefreshNeeded() == 2)
			{
				wString oldFilter(GetCurrentFilter());
				FilterList(oldFilter.c_str());
			}
			
			CurBrowser->Refresh();
			fileBrowser->TriggerUpdate();
		}
		
		delete allWindow;
		mainWindow->SetState(STATE_DEFAULT);
		
		manageBtn->ResetState();
		fileBrowser->ResetState();
	}
	
	else if (BackInDirBtn->GetState() == STATE_CLICKED)
	{
		BackInDirBtn->ResetState();
		
		if (browserMode == BROWSE_SDUSB && !(*GetCurrentFilter()) && !show_searchwindow) BackInDirectory();
	}
	
	else if (settingsBtn->GetState() == STATE_CLICKED)
	{
		if (!Settings.GodMode && (Settings.ParentalBlocks & BLOCK_GLOBAL_SETTINGS))
		{
			WindowPrompt(tr( "Permission denied." ), tr( "Console must be unlocked for this option." ), tr( "OK" ));
			settingsBtn->ResetState();
			return MENU_NONE;
		}
		
		return MENU_SETTINGS;
	}
	
	else if (homebrewBtn->GetState() == STATE_CLICKED)
	{
		if (!Settings.GodMode && (Settings.ParentalBlocks & BLOCK_HBC_MENU))
		{
			WindowPrompt(tr( "Permission denied." ), tr( "Console must be unlocked for this option." ), tr( "OK" ));
			homebrewBtn->ResetState();
			return MENU_NONE;
		}
		
		return MENU_HOMEBREWBROWSE;
	}
	
	else if (homeBtn->GetState() == STATE_CLICKED)
	{
		homeBtn->ResetState();
		
		SetState(STATE_DISABLED);
		
		ShowHomeMenu();
		
		SetState(STATE_DEFAULT);
	}
	
	else if (sdcardBtn->GetState() == STATE_CLICKED)
	{
		HaltGui();
		bgMusic->Pause();
		
		//Settings.Save();
		DeviceHandler::Instance()->UnMountSD();
		DeviceHandler::Instance()->MountSD();
		
		gprintf("\tLoading config...%s\n", Settings.Load() ? "done" : "failed");
		gprintf("\tLoading language...%s\n", Settings.LoadLanguage(Settings.LanguageFile, CONSOLE_DEFAULT) ? "done" : "failed");
		gprintf("\tLoading font...%s\n", Theme::LoadFont(Settings.CustomFontPath) ? "done" : "failed (using default)");
		gprintf("\tLoading theme...%s\n", Theme::Load(Settings.Theme) ? "done" : "failed (using default)");
		bgMusic->Resume();
		
		Settings.LastBrowser = browserMode;
		if (browserMode == BROWSE_NAND || browserMode == BROWSE_SDUSB || browserMode == BROWSE_EMU)
			Settings.BrowserPath = Settings.SaveGamePath;
		else
			Settings.BrowserPath = Settings.SaveMiisPath;
		
		wString oldFilter(GetCurrentFilter());
		FilterList(oldFilter.c_str());
		ReloadBrowser();
		ResumeGui();
		sdcardBtn->ResetState();
	}
	
	else if (searchBar && (searchChar = searchBar->GetClicked()))
	{
		bool reload = true;
		if (searchChar > 27) //! Character clicked
		{
			int len = GetCurrentFilter() ? wcslen(GetCurrentFilter()) : 0;
			wchar_t newFilter[len + 2];
			if (GetCurrentFilter()) wcscpy(newFilter, GetCurrentFilter());
			newFilter[len] = searchChar;
			newFilter[len + 1] = 0;
			
			if (browserMode == BROWSE_SDUSB)
				FileList.SetCurrentFilter(newFilter);
			else
				FilterList(newFilter);
		}
		else if (searchChar == 6) //! Clear
		{
			if(*GetCurrentFilter())
			{
				if (browserMode == BROWSE_SDUSB)
					FileList.SetCurrentFilter((const wchar_t *)"\0");
				else
					FilterList((const wchar_t *)"\0");
			}
			else
				reload = false;
		}
		else if (searchChar == 7) //! Close
		{
			show_searchwindow = false;
			taskbar->StopSearchEffect();
		}
		else if (searchChar == 8) //! Backspace
		{
			int len = wcslen(GetCurrentFilter());
			wchar_t newFilter[len + 1];
			if (GetCurrentFilter()) wcscpy(newFilter, GetCurrentFilter());
			newFilter[len > 0 ? len - 1 : 0] = 0;
			
			if (browserMode == BROWSE_SDUSB)
				FileList.SetCurrentFilter(newFilter);
			else
				FilterList(newFilter);
		}
		if(reload) ReloadBrowser();
		return MENU_NONE;
	}
	
	if (Settings.Screensaver != 0 && ControlActivityTimeout())
		ShowScreenSaver();
	
	return returnMenu;
}

bool SaveGameMenu::CheckDeviceMenu()
{
    if(Device_Menu != NULL)
    {
        homeBtn->SetState(STATE_DISABLED);
		manageBtn->SetState(STATE_DISABLED);
		homebrewBtn->SetState(STATE_DISABLED);
		settingsBtn->SetState(STATE_DISABLED);
		sdcardBtn->SetState(STATE_DISABLED);
		BackInDirBtn->SetState(STATE_DISABLED);
        taskbar->SetState(STATE_DISABLED);
        fileBrowser->SetTriggerUpdate(false);
		
		int settings = Settings.Tooltips;
		Settings.Tooltips = 0;
		
		Append(Device_Menu);
		
        int device_choice = -1;
        while(device_choice == -1 && Device_Menu != NULL)
        {
            usleep(100);
			
            device_choice = Device_Menu->GetChoice();
        }
        delete Device_Menu;
        Device_Menu = NULL;
		
        if(device_choice >= 0)
        {
            if(device_choice == WII)
			{
				browserMode = BROWSE_NAND;
				Settings.LastBrowser = BROWSE_NAND;
			}
			else if(device_choice == MII)
			{
				browserMode = BROWSE_MII;
				Settings.LastBrowser = BROWSE_MII;
			}
			else if(device_choice == EMU)
			{
				browserMode = BROWSE_EMU;
				Settings.LastBrowser = BROWSE_EMU;
			}
			else if(device_choice == EMU_MII)
			{
				browserMode = BROWSE_EMU_MII;
				Settings.LastBrowser = BROWSE_EMU_MII;
			}
			else
			{
				browserMode = BROWSE_SDUSB;
				Settings.LastBrowser = BROWSE_SDUSB;
				Settings.BrowserPath = DeviceName[device_choice];
				Settings.BrowserPath += ":/";
			}
			
			Settings.Tooltips = settings;
			return true;
        }
        homeBtn->SetState(STATE_DEFAULT);
		manageBtn->SetState(STATE_DEFAULT);
		homebrewBtn->SetState(STATE_DEFAULT);
		settingsBtn->SetState(STATE_DEFAULT);
		sdcardBtn->SetState(STATE_DEFAULT);
		BackInDirBtn->SetState(STATE_DEFAULT);
        taskbar->SetState(STATE_DEFAULT);
        fileBrowser->SetTriggerUpdate(true);
		
		Settings.Tooltips = settings;
    }
	
	return false;
}

bool SaveGameMenu::CheckTaskBarChanges()
{
	bool clicked = false;
	
	if(taskbar->GetState() == STATE_CLICKED)
	{
		switch (taskbar->GetClicked())
		{
			case TASKBAR_DEVICE:
			{
				Device_Menu = new DeviceMenu(taskbar->GetLeft()-5-this->GetLeft(), taskbar->GetTop()+taskbar->GetHeight()-this->GetTop());
				break;
			}
			case TASKBAR_MODE:
			{
				if(Settings.BrowserMode == ICONMODE)
					Settings.BrowserMode = LISTMODE;
				else
					Settings.BrowserMode = ICONMODE;
					
				Settings.Save();
				
				clicked = true;
				break;
			}
			case TASKBAR_LOCK:
			{
				if (!Settings.GodMode)
				{
					//!password check to unlock
					mainWindow->SetState(STATE_DISABLED);
					int result = PasswordCheck(Settings.unlockCode);
					mainWindow->SetState(STATE_DEFAULT);
					if (result > 0)
					{
						if(result == 1)
							WindowPrompt( tr( "Correct Password" ), tr( "All the features of SaveGame Manager GX are unlocked." ), tr( "OK" ));
						Settings.GodMode = 1;
						Settings.Save();
					}
					else if(result < 0)
						WindowPrompt(tr( "Wrong Password" ), tr( "SaveGame Manager GX is protected" ), tr( "OK" ));
				}
				else
				{
					int choice = WindowPrompt(tr( "Lock Console" ), tr( "Are you sure?" ), tr( "Yes" ), tr( "No" ));
					if (choice == 1)
					{
						WindowPrompt(tr( "Console Locked" ), tr( "SaveGame Manager GX is protected" ), tr( "OK" ));
						Settings.GodMode = 0;
						Settings.Save();
					}
				}
				
				taskbar->SetLockButton();
				
				Remove(sdcardBtn);
				if (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_SD_RELOAD_BUTTON))
					Append(sdcardBtn);
				
				break;
			}
			case TASKBAR_SEARCH:
			default:
			{
				show_searchwindow = !show_searchwindow;
				wString oldFilter(GetCurrentFilter());
				FilterList(oldFilter.c_str());
				ReloadBrowser();
				break;
			}
		}
		
		taskbar->ResetState();
	}
	
	return clicked;
}

void SaveGameMenu::CheckSaveBrowserChanges()
{
	if(fileBrowser->GetState() == STATE_CLICKED)
	{
		SaveWindow * saveWindow = new SaveWindow(CurBrowser->GetSelIndex());
		
		mainWindow->SetState(STATE_DISABLED);
		mainWindow->Append(saveWindow);
		
		saveWindow->Show();
		
		delete saveWindow;
		
		fileBrowser->ResetState();
	}
}

void SaveGameMenu::CheckEmuBrowserChanges()
{
	if(fileBrowser->GetState() == STATE_CLICKED)
	{
		EmuSaveWindow * emuWindow = new EmuSaveWindow(CurBrowser->GetSelIndex());
		
		mainWindow->SetState(STATE_DISABLED);
		mainWindow->Append(emuWindow);
		
		emuWindow->Show();
		
		if(emuWindow->RefreshNeeded())
		{
			CurBrowser->Refresh();
			fileBrowser->TriggerUpdate();
		}
		
		delete emuWindow;
		
		fileBrowser->ResetState();
	}
}

void SaveGameMenu::CheckMiiBrowserChanges()
{
	if(fileBrowser->GetState() == STATE_CLICKED)
	{
		MiiWindow * miiWindow = new MiiWindow(CurBrowser->GetSelIndex());
		
		mainWindow->SetState(STATE_DISABLED);
		mainWindow->Append(miiWindow);
		
		miiWindow->Show();
		
		if(miiWindow->RefreshNeeded())
		{
			CurBrowser->Refresh();
			fileBrowser->TriggerUpdate();
		}
		
		delete miiWindow;
		
		fileBrowser->ResetState();
	}
}

void SaveGameMenu::CheckEmuMiiBrowserChanges()
{
	if(fileBrowser->GetState() == STATE_CLICKED)
	{
		EmuMiiWindow * emuMiiWindow = new EmuMiiWindow(CurBrowser->GetSelIndex());
		
		mainWindow->SetState(STATE_DISABLED);
		mainWindow->Append(emuMiiWindow);
		
		emuMiiWindow->Show();
		
		if(emuMiiWindow->RefreshNeeded())
		{
			CurBrowser->Refresh();
			fileBrowser->TriggerUpdate();
		}
		
		delete emuMiiWindow;
		
		fileBrowser->ResetState();
	}
}

void SaveGameMenu::CheckDeviceBrowserChanges()
{
	if(fileBrowser->GetState() == STATE_CLICKED)
	{
		fileBrowser->ResetState();
		//! check corresponding browser entry
		if(CurBrowser->IsCurrentDir() && !(*GetCurrentFilter()) && !show_searchwindow)
		{
			int res = CurBrowser->ChangeDirectory();
			if(res > 0)
			{
				CurBrowser->SetPageIndex(0);
				CurBrowser->ResetMarker();
				CurBrowser->Refresh();
				fileBrowser->ResetState();
				Settings.BrowserPath.assign(DeviceBrowser->GetCurrentPath());
			}
			else if(res == 0)
			{
				//!Archive file
				return;
			}
			else
			{
				ShowError(tr("Can't browse that path."));
			}
		}
		else if(!CurBrowser->IsCurrentDir())
		{
			if(CurBrowser->IsSave(CurBrowser->GetSelIndex()))
			{
				mainWindow->SetState(STATE_DISABLED);
				
				FileSaveWindow * fileSaveWindow = new FileSaveWindow(CurBrowser->GetSelIndex());
				
				mainWindow->Append(fileSaveWindow);
				
				fileSaveWindow->Show();
				
				if(fileSaveWindow->RefreshNeeded())
				{
					CurBrowser->Refresh();
					fileBrowser->TriggerUpdate();
				}
				
				delete fileSaveWindow;
			}
			else if(CurBrowser->IsDataBin(CurBrowser->GetSelIndex()))
			{
				mainWindow->SetState(STATE_DISABLED);
				
				DataBinWindow * dataBinWindow = new DataBinWindow(CurBrowser->GetSelIndex());
				
				mainWindow->Append(dataBinWindow);
				
				dataBinWindow->Show();
				
				if(dataBinWindow->RefreshNeeded())
				{
					CurBrowser->Refresh();
					fileBrowser->TriggerUpdate();
				}
				
				delete dataBinWindow;
			}
			else if(CurBrowser->IsMii(CurBrowser->GetSelIndex()))
			{
				mainWindow->SetState(STATE_DISABLED);
				
				FileMiiWindow * fileMiiWindow = new FileMiiWindow(CurBrowser->GetSelIndex());
				
				mainWindow->Append(fileMiiWindow);
				
				fileMiiWindow->Show();
				
				if(fileMiiWindow->RefreshNeeded())
				{
					CurBrowser->Refresh();
					fileBrowser->TriggerUpdate();
				}
				
				delete fileMiiWindow;
			}
			else
				gprintf("\n\t file selected");
			
			fileBrowser->ResetState();
		}
	}
}

void SaveGameMenu::BackInDirectory()
{
	CurBrowser->BackInDirectory();
	CurBrowser->SetPageIndex(0);
	CurBrowser->ResetMarker();
	CurBrowser->Refresh();
	fileBrowser->ResetState();
	Settings.BrowserPath.assign(DeviceBrowser->GetCurrentPath());
}

void SaveGameMenu::UpdateClock()
{
	if(Settings.Clock != CLOCK_HR12 && Settings.Clock != CLOCK_HR24)
		return;

	time_t rawtime = time(0);
	if(rawtime == lastrawtime) //! Only update every 1 second
		return;

	char theTime[50];
	theTime[0] = 0;

	lastrawtime = rawtime;
	struct tm * timeinfo = localtime(&rawtime);
	if (Settings.Clock == CLOCK_HR12)
	{
		if (rawtime & 1)
			strftime(theTime, sizeof(theTime), "%I:%M", timeinfo);
		else
			strftime(theTime, sizeof(theTime), "%I %M", timeinfo);
	}
	if (Settings.Clock == CLOCK_HR24)
	{
		if (rawtime & 1)
			strftime(theTime, sizeof(theTime), "%H:%M", timeinfo);
		else
			strftime(theTime, sizeof(theTime), "%H %M", timeinfo);
	}
	clockTime->SetText(theTime);
}

const wchar_t * SaveGameMenu::GetCurrentFilter()
{
	if (browserMode == BROWSE_SDUSB)
		return FileList.GetCurrentFilter();
	else if (browserMode == BROWSE_NAND)
		return SaveList.GetCurrentFilter();
	else if (browserMode == BROWSE_MII)
		return MiiList.GetCurrentFilter();
	else if (browserMode == BROWSE_EMU)
		return EmuList.GetCurrentFilter();
	else if (browserMode == BROWSE_EMU_MII)
		return EmuMiiList.GetCurrentFilter();
	
	return NULL;
}

const wchar_t * SaveGameMenu::GetAvailableSearchChars()
{
	if (browserMode == BROWSE_SDUSB)
		return FileList.GetAvailableSearchChars();
	else if (browserMode == BROWSE_NAND)
		return SaveList.GetAvailableSearchChars();
	else if (browserMode == BROWSE_MII)
		return MiiList.GetAvailableSearchChars();
	else if (browserMode == BROWSE_EMU)
		return EmuList.GetAvailableSearchChars();
	else if (browserMode == BROWSE_EMU_MII)
		return EmuMiiList.GetAvailableSearchChars();
	
	return NULL;
}

void SaveGameMenu::FilterList(const wchar_t * filter)
{
	if (browserMode == BROWSE_NAND)
		SaveList.FilterList(filter);
	else if (browserMode == BROWSE_MII)
		MiiList.FilterList(filter);
	else if (browserMode == BROWSE_EMU)
		EmuList.FilterList(filter);
	else if (browserMode == BROWSE_EMU_MII)
		EmuMiiList.FilterList(filter);
}
