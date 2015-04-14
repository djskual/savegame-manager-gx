/***************************************************************************
 * Copyright (C) 2011
 * by Dj_Skual
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
 *
 * Explorer Class
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include "SelectBrowser.h"
#include "../menu.h"
#include "../Themes/CTheme.h"
#include "../Tools/StringTools.h"
#include "../Settings/CSettings.h"
#include "../Tools/ShowError.h"
#include "PromptWindows.h"
#include "ProgressWindow.h"
#include "../DeviceControls/DeviceHandler.hpp"
#include "../Language/gettext.h"
#include "../Language/UpdateLanguage.h"
#include "../FileOperations/FileSelect.h"
#include "../FileOperations/fileops.h"
#include "../System/sys.h"

SelectBrowser::SelectBrowser(short Type)
	:GuiWindow(0, 0)
{
	this->Setup(Type);
}

SelectBrowser::SelectBrowser(const char * path, short Type)
	:GuiWindow(0, 0)
{
	this->Setup(Type);
	
	char Path[1024];
	if (type == LANGBROWSER)
		sprintf(Path, "%s", Settings.LanguagePath);
	else
		sprintf(Path, "%s", path);
	
	this->LoadPath(Path);
}

SelectBrowser::~SelectBrowser()
{
	ResumeGui();
	SetEffect(EFFECT_FADE, -50);
	while(this->GetEffect() > 0) usleep(100);
	
	HaltGui();
	if(parentElement)
		((GuiWindow *) parentElement)->Remove(this);
	
	RemoveAll();
	
	delete BrowserImg;
	delete ButtonBoxImgClose;
	delete ButtonBoxImgWifiVal;
	delete DeviceImg;
	delete FolderImg;
	delete CloseImg;
	delete CloseOverImg;
	delete WifiImg;
	delete WifiOverImg;
	delete ValidImg;
	delete ValidOverImg;
	
	delete BrowserImgData;
	delete ButtonBoxImgData;
	delete CloseImgData;
	delete CloseOverImgData;
	delete WifiImgData;
	delete WifiOverImgData;
	delete ValidImgData;
	delete ValidOverImgData;
	delete SDImgData;
	delete USBImgData;
	delete FolderImgData;
	
	delete AdressText;
	
	delete DeviceSwitchBtn;
	delete FolderBtn;
	delete CloseBtn;
	delete WifiBtn;
	delete ValidBtn;
	delete BackInDirBtn;
	
	delete trigA;
	delete trigHome;
	delete trigBackInDir;
	
	delete listBrowser;
	
	delete DeviceBrowser;
}

void SelectBrowser::Setup(short Type)
{
	type = Type;
	result = -1;
	currentDevice = SD;
	listBrowser = NULL;
	fileBrowser = NULL;
	
	DeviceBrowser = new FileCustomBrowser();
	CurBrowser = DeviceBrowser;
	
	SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	SetPosition(0, 0);
	
	trigA = new SimpleGuiTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	trigBackInDir = new GuiTrigger();
	trigBackInDir->SetButtonOnlyTrigger(-1, WiiControls.UpInDirectory | ClassicControls.UpInDirectory << 16, GCControls.UpInDirectory);
	trigHome = new GuiTrigger();
	trigHome->SetButtonOnlyTrigger(-1, WiiControls.HomeButton | ClassicControls.HomeButton << 16, GCControls.HomeButton);
	
	BrowserImgData = Resources::GetImageData("browser_select.png");
	ButtonBoxImgData = Resources::GetImageData("browser_select_button_box.png");
	
	CloseImgData = Resources::GetImageData("button_close.png");
	CloseOverImgData = Resources::GetImageData("button_close_over.png");
	WifiImgData = Resources::GetImageData("button_wifi.png");
	WifiOverImgData = Resources::GetImageData("button_wifi_over.png");
	ValidImgData = Resources::GetImageData("button_valid.png");
	ValidOverImgData = Resources::GetImageData("button_valid_over.png");
	
	SDImgData = Resources::GetImageData("storage_sd.png");
	USBImgData = Resources::GetImageData("storage_usb.png");
	FolderImgData = Resources::GetImageData("icon_brows_folder.png");
	
	width = BrowserImgData->GetWidth();
	height = BrowserImgData->GetHeight();
	BrowserImg = new GuiImage(BrowserImgData);
	
	ButtonBoxImgClose = new GuiImage(ButtonBoxImgData);
	ButtonBoxImgClose->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	ButtonBoxImgClose->SetPosition(177, -161);
	ButtonBoxImgWifiVal = new GuiImage(ButtonBoxImgData);
	ButtonBoxImgWifiVal->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	ButtonBoxImgWifiVal->SetPosition(138, -161);
	
	DeviceImg = new GuiImage(SDImgData);
	DeviceSwitchBtn = new GuiButton(DeviceImg->GetWidth(), DeviceImg->GetHeight());
	DeviceSwitchBtn->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	DeviceSwitchBtn->SetPosition(55, -164);
	DeviceSwitchBtn->SetImage(DeviceImg);
	DeviceSwitchBtn->SetSoundClick(btnSoundClick);
	DeviceSwitchBtn->SetSoundOver(btnSoundOver);
	DeviceSwitchBtn->SetTrigger(trigA);
	DeviceSwitchBtn->SetEffectGrow();
	DeviceSwitchBtn->SetState(STATE_DISABLED);
	if(DeviceHandler::Instance()->GetDeviceCount() >= 2)
		DeviceSwitchBtn->SetState(STATE_DEFAULT);
	
	AdressText = new GuiText((char *) NULL, 18, thColor("r=0 g=0 b=0 a=255 - select browser adress text color"));
	AdressText->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	AdressText->SetPosition(75, -165);
	AdressText->SetMaxWidth(255-17, SCROLL_HORIZONTAL);
	
	FolderImg = new GuiImage(FolderImgData);
	FolderImg->SetScale(0.667);
	FolderImg->SetPosition(0,-7);
	FolderBtn = new GuiButton(FolderImg->GetWidth()*0.667, FolderImg->GetHeight()*0.667);
	FolderBtn->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	FolderBtn->SetPosition(-178, -164);
	FolderBtn->SetImage(FolderImg);
	FolderBtn->SetSoundClick(btnSoundClick);
	FolderBtn->SetSoundOver(btnSoundOver);
	FolderBtn->SetTrigger(trigA);
	FolderBtn->SetEffectGrow();
	
	CloseImg = new GuiImage(CloseImgData);
	CloseOverImg = new GuiImage(CloseOverImgData);
	CloseBtn = new GuiButton(CloseImg->GetWidth(), CloseImg->GetHeight());
	CloseBtn->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	CloseBtn->SetPosition(176, -160);
	CloseBtn->SetImage(CloseImg);
	CloseBtn->SetImageOver(CloseOverImg);
	CloseBtn->SetSoundOver(btnSoundOver);
	CloseBtn->SetSoundClick(btnSoundClick);
	CloseBtn->SetTrigger(trigA);
	CloseBtn->SetTrigger(trigHome);
	CloseBtn->SetEffectGrow();
	
	WifiImg = new GuiImage(WifiImgData);
	WifiOverImg = new GuiImage(WifiOverImgData);
	WifiBtn = new GuiButton(WifiImg->GetWidth(), WifiImg->GetHeight());
	WifiBtn->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	WifiBtn->SetPosition(137, -160);
	WifiBtn->SetImage(WifiImg);
	WifiBtn->SetImageOver(WifiOverImg);
	WifiBtn->SetSoundOver(btnSoundOver);
	WifiBtn->SetSoundClick(btnSoundClick);
	WifiBtn->SetTrigger(trigA);
	WifiBtn->SetEffectGrow();
	
	ValidImg = new GuiImage(ValidImgData);
	ValidOverImg = new GuiImage(ValidOverImgData);
	ValidBtn = new GuiButton(ValidImg->GetWidth(), ValidImg->GetHeight());
	ValidBtn->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	ValidBtn->SetPosition(137, -160);
	ValidBtn->SetImage(ValidImg);
	ValidBtn->SetImageOver(ValidOverImg);
	ValidBtn->SetSoundOver(btnSoundOver);
	ValidBtn->SetSoundClick(btnSoundClick);
	ValidBtn->SetTrigger(trigA);
	ValidBtn->SetEffectGrow();
	
	BackInDirBtn = new GuiButton(0, 0);
	BackInDirBtn->SetTrigger(trigBackInDir);
	
	listBrowser = new ListFileCustomBrowser(CurBrowser, 322, 276);
	fileBrowser = listBrowser;
	
	fileBrowser->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	fileBrowser->SetPosition(26, 54);
	
	Append(BrowserImg);
	Append(ButtonBoxImgClose);
	if (type != FONTBROWSER && type != SOUNDBROWSER)
		Append(ButtonBoxImgWifiVal);
	Append(AdressText);
	Append(DeviceSwitchBtn);
	Append(FolderBtn);
	Append(CloseBtn);
	if (type == LANGBROWSER)
		Append(WifiBtn);
	if (type != FONTBROWSER && type != SOUNDBROWSER && type != LANGBROWSER)
		Append(ValidBtn);
	Append(BackInDirBtn);
	Append(fileBrowser);
	
	//! filters
	if (type != FONTBROWSER && type != SOUNDBROWSER && type != LANGBROWSER)
		SetFilter(FILTER_FILES);
	else
		DeviceBrowser->SetFilterCustom(type);
	
	if (type == THEMEPATHBROWSER)
		DeviceBrowser->SetFilterCustom(type);
	
	SetEffect(EFFECT_FADE, 50);
}

int SelectBrowser::LoadPath(const char * path)
{
	if(CurBrowser != DeviceBrowser)
		return -2;

	int filecount = DeviceBrowser->BrowsePath(path);
	if(filecount < 0)
	{
		int choice = WindowPrompt(tr("Error:"),
		tr("Unable to load path."),
		tr("Retry"),
		tr("Close"));
		
		if(choice)
			return LoadPath(path);
		else
			return -3;
	}
	
	std::string currentroot = path;
	currentroot.erase(currentroot.find_first_of(':'));
	for (int i = SD; i <= USB8; i++)
	{
		if (!currentroot.compare(DeviceName[i]))
			currentDevice = i;
	}
	
	DeviceBrowser->ResetMarker();
	AdressText->SetText(DeviceBrowser->GetCurrentPath());
	SetDeviceImage();
	
	return filecount;
}

int SelectBrowser::GetMenuChoice()
{
	CheckBrowserChanges();
	
	if (CloseBtn->GetState() == STATE_CLICKED)
	{
		CloseBtn->ResetState();
		
		result = 0;
	}
	else if (BackInDirBtn->GetState() == STATE_CLICKED)
	{
		BackInDirBtn->ResetState();
		
		BackInDirectory();
	}
	else if (WifiBtn && WifiBtn->GetState() == STATE_CLICKED)
	{
		WifiBtn->ResetState();
		
		char langPath[1024];
		strcpy(langPath, CurBrowser->GetCurrentPath().c_str());
		
		int res = WindowPrompt(tr( "Download/Update Languages Files to:" ), langPath, tr( "All Files" ), tr( "Installed Files" ), tr( "Select Files" ), tr( "Cancel" ));
		if ( res == 1 ) //! Download All Files available
		{
			DownloadAllLanguageFiles(langPath);
			
			LoadPath(langPath);
		}
		else if ( res == 2 ) //! Update already installed files
		{
			if (UpdateLanguageFiles(langPath) == 0)
			{
				int choice = WindowPrompt(tr( "No Language File found" ), tr( "Download" ), tr( "All Files" ), tr( "Select Files" ), tr( "Cancel" ));
				if ( choice == 1 ) //! Download All Files available
					DownloadAllLanguageFiles(langPath);
				else if ( choice == 2 ) //! Select files to download
					SelectLanguageFiles(langPath);
			}
			
			LoadPath(langPath);
		}
		else if ( res == 3 ) //! Select files to download/update
		{
			SelectLanguageFiles(langPath);
			
			LoadPath(langPath);
		}
		
		mainWindow->SetState(STATE_DISABLED);
		SetState(STATE_DEFAULT);
	}
	else if (ValidBtn && ValidBtn->GetState() == STATE_CLICKED)
	{
		ValidBtn->ResetState();
		
		char Path[1024];
		strcpy(Path, CurBrowser->GetCurrentPath().c_str());
		
		if(type == SAVEPATHBROWSER)
			snprintf(Settings.SaveGamePath, sizeof(Settings.SaveGamePath), "%s", Path);
		else if(type == DATABINPATHBROWSER)
			snprintf(Settings.DataBinPath, sizeof(Settings.DataBinPath), "%s", Path);
		else if(type == MIIPATHBROWSER)
			snprintf(Settings.SaveMiisPath, sizeof(Settings.SaveMiisPath), "%s", Path);
		else if(type == UPDATEPATHBROWSER)
			snprintf(Settings.UpdatePath, sizeof(Settings.UpdatePath), "%s", Path);
		else if(type == GAMETDBPATHBROWSER)
			snprintf(Settings.GameTDBPath, sizeof(Settings.GameTDBPath), "%s", Path);
		else if(type == THEMEPATHBROWSER)
			snprintf(Settings.ThemePath, sizeof(Settings.ThemePath), "%s", Path);
		else if(type == HOMEBREWPATHBROWSER)
			snprintf(Settings.HomebrewPath, sizeof(Settings.HomebrewPath), "%s", Path);
		else if(type == SCREENSHOTPATHBROWSER)
			snprintf(Settings.ScreenshotPath, sizeof(Settings.ScreenshotPath), "%s", Path);
		else if(type == EMUNANDPATHBROWSER)
			snprintf(Settings.EmuNandPath, sizeof(Settings.EmuNandPath), "%s", Path);
		else if(type == PATH)
			Settings.TmpPath = Path;
		
		if (type != PATH)
			WindowPrompt(tr("Path changed."), 0, tr("OK"));
		
		result = 2; //! 2 = Succes
	}
	else if(FolderBtn->GetState() == STATE_CLICKED)
	{
		FolderBtn->ResetState();
		
		char entered[1024];
		strcpy(entered, CurBrowser->GetCurrentPath().c_str());
		
		if (type == LANGBROWSER)
		{
			if(CreateLangPath(entered) == 1)
				LoadPath(entered);
		}
		else
		{
			if(CreatePath(entered))
				LoadPath(entered);
		}
	}
	else if (DeviceSwitchBtn->GetState() == STATE_CLICKED)
	{
		DeviceSwitchBtn->ResetState();
		
		currentDevice++;
		while(!DeviceHandler::Instance()->IsInserted(currentDevice))
		{
			currentDevice++;
			if(currentDevice >= MAXDEVICES)
				currentDevice = SD;
		}
		LoadPath(fmt("%s:/", DeviceName[currentDevice]));
	}
	
	return result;
}

void SelectBrowser::CheckBrowserChanges()
{
	if(fileBrowser->GetState() == STATE_CLICKED)
	{
		fileBrowser->ResetState();
		//! check corresponding browser entry
		if(CurBrowser->IsCurrentDir())
		{
			int res = CurBrowser->ChangeDirectory();
			if(res > 0)
			{
				fileBrowser->SetSelected(0);
				CurBrowser->SetPageIndex(0);
				CurBrowser->ResetMarker();
				fileBrowser->TriggerUpdate();
				AdressText->SetText(CurBrowser->GetCurrentPath());
			}
			else if(res == 0)
			{
				//!Archive file
				return;
			}
			else
			{
				ShowError(tr("Can't browse that path."));
				result = 0;
			}
		}
		else
		{
			char filepath[MAXPATHLEN];
			int res = 0;
			
			SetState(STATE_DISABLED);
			fileBrowser->SetTriggerUpdate(false);
			
			if(CurBrowser == DeviceBrowser)
				snprintf(filepath, sizeof(filepath), "%s", CurBrowser->GetCurrentSelectedFilepath().c_str());
			
			if(res >= 0)
				res = FileSelect(filepath, type);
			
			SetState(STATE_DEFAULT);
			fileBrowser->SetTriggerUpdate(true);
			
			if(res == REFRESH_BROWSER)
			{
				CurBrowser->Refresh();
				fileBrowser->TriggerUpdate();
			}
			else if(res == EXIT_BROWSER)
			{
				result = 0;
			}
			else if(res == RELOAD_BROWSER)
			{
				result = 1;
			}
			else if(res == EXIT_SUCCES)
			{
				result = 2;
			}
			else if(res == SOUND_SUCCES)
			{
				result = 5;
			}
		}
	}
}

void SelectBrowser::SetDeviceImage()
{
	if(CurBrowser != DeviceBrowser)
		return;
	
	if(currentDevice == SD)
		DeviceImg->SetImage(SDImgData);
	else
		DeviceImg->SetImage(USBImgData);
}

void SelectBrowser::BackInDirectory()
{
	CurBrowser->BackInDirectory();
	fileBrowser->SetSelected(0);
	CurBrowser->SetPageIndex(0);
	CurBrowser->ResetMarker();
	CurBrowser->Refresh();
	fileBrowser->TriggerUpdate();
	AdressText->SetText(CurBrowser->GetCurrentPath());
}

int SelectBrowser::CreateLangPath(char * Path)
{
	HaltGui();
	mainWindow->SetState(STATE_DISABLED);
	SetState(STATE_DISABLED);
	ResumeGui();
	
	int result = OnScreenKeyboard(Path, 149, 0);
	if(result == 1)
	{
		char dirnoslash[strlen(Path)+1];
		strcpy(dirnoslash, Path);
		
		int pos = strlen(dirnoslash)-1;
		while(dirnoslash[pos] == '/')
		{
			dirnoslash[pos] = '\0';
			pos--;
		}
		strcpy(Path, dirnoslash);
		CreateSubfolder(Path);
		
		WindowPrompt(tr("Language Path changed."), 0, tr("OK"));
		snprintf(Settings.LanguagePath, sizeof(Settings.LanguagePath), "%s", Path);
	}
	
	HaltGui();
	mainWindow->SetState(STATE_DISABLED);
	SetState(STATE_DEFAULT);
	ResumeGui();
	
	return result;
}

int SelectBrowser::CreatePath(char * Path)
{
	HaltGui();
	mainWindow->SetState(STATE_DISABLED);
	SetState(STATE_DISABLED);
	ResumeGui();
	
	int result = OnScreenKeyboard(Path, 149, 0);
	if(result == 1)
	{
		char dirnoslash[strlen(Path)+1];
		strcpy(dirnoslash, Path);
		
		int pos = strlen(dirnoslash)-1;
		while(dirnoslash[pos] == '/')
		{
			dirnoslash[pos] = '\0';
			pos--;
		}
		
		if (CreateSubfolder(dirnoslash) == false)
			result = 0;
	}
	
	HaltGui();
	mainWindow->SetState(STATE_DISABLED);
	SetState(STATE_DEFAULT);
	ResumeGui();
	
	return result;
}

int selectBrowser(const char * enterPath, short Type)
{
	int ret = -1;
	SelectBrowser * sb = new SelectBrowser(enterPath, Type);
	
	mainWindow->Append(sb);
	ResumeGui();
	
	while(ret == -1)
	{
		usleep(100);
		
		ret = sb->GetMenuChoice();
	}
	
	delete sb;
	sb = NULL;
	ResumeGui();
	
	return ret;
}
