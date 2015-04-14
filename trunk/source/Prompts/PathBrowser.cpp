/****************************************************************************
 * Copyright (C) 2009-2012 Dimok
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
#include "PathBrowser.h"
#include "../Themes/CTheme.h"
#include "../Controls/Application.h"
#include "../Language/UpdateLanguage.h"
#include "../DeviceControls/DeviceHandler.hpp"
#include "../Files/CustomFileBrowser.hpp"
#include "../FileOperations/fileops.h"
#include "../Prompts/PromptWindows.h"

PathBrowser::PathBrowser(const std::string &path, u8 Filter)
	: GuiFrame(0, 0)
{
	filter = Filter;
	currentDevice = SD;
	
	btnSoundClick = Resources::GetSound("button_click.wav");
	btnSoundOver = Resources::GetSound("button_over.wav");
	btnSoundClick->SetVolume(Settings.SFXVolume);
	btnSoundOver->SetVolume(Settings.SFXVolume);
	
	trigA.SetSimpleTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	trigB.SetButtonOnlyTrigger(-1, WiiControls.BackButton | ClassicControls.BackButton << 16, GCControls.BackButton);

	browserImgData = Resources::GetImageData("browser_select.png");
	browserImg = new GuiImage(browserImgData);
	width = browserImgData->GetWidth();
	height = browserImgData->GetHeight();
	
	btnBoxImgData = Resources::GetImageData("browser_select_button_box.png");
	btnCloseBoxImg = new GuiImage(btnBoxImgData);
	btnCloseBoxImg->SetAlignment(ALIGN_RIGHT | ALIGN_TOP);
	btnCloseBoxImg->SetPosition(-18, 15);
	btnWifiValBoxImg = new GuiImage(btnBoxImgData);
	btnWifiValBoxImg->SetAlignment(ALIGN_RIGHT | ALIGN_TOP);
	btnWifiValBoxImg->SetPosition(-58, 15);
	
	sdImgData = Resources::GetImageData("storage_sd.png");
	usbImgData = Resources::GetImageData("storage_usb.png");
	folderImgData = Resources::GetImageData("icon_brows_folder.png");
	deviceImg = NULL;
	
	folderImg = new GuiImage(folderImgData);
	folderImg->SetScale(0.667);
	folderImg->SetPosition(0,-7);
	folderBtn = new GuiButton(folderImg->GetWidth()*0.667, folderImg->GetHeight()*0.667);
	folderBtn->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	folderBtn->SetPosition(10, 15);
	folderBtn->SetImage(folderImg);
	folderBtn->SetSoundClick(btnSoundClick);
	folderBtn->SetSoundOver(btnSoundOver);
	folderBtn->SetTrigger(&trigA);
	folderBtn->SetEffectGrow();
	folderBtn->Clicked.connect(this, &PathBrowser::OnFolderButtonClick);
	
	devSwitchBtn = new PictureButton("storage_sd.png", "storage_sd.png", btnSoundClick, btnSoundOver);
	devSwitchBtn->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	devSwitchBtn->SetPosition(58, 17);
	devSwitchBtn->SetTrigger(&trigA);
	devSwitchBtn->Clicked.connect(this, &PathBrowser::OnDevButtonClick);
	
	closeBtn = new PictureButton("button_close.png", "button_close_over.png", btnSoundClick, btnSoundOver);
	closeBtn->SetAlignment(ALIGN_RIGHT | ALIGN_TOP);
	closeBtn->SetPosition(-21, 17);
	closeBtn->SetTrigger(&trigA);
	closeBtn->Clicked.connect(this, &PathBrowser::OnButtonClick);
	
	wifiBtn = new PictureButton("button_wifi.png", "button_wifi_over.png", btnSoundClick, btnSoundOver);
	wifiBtn->SetAlignment(ALIGN_RIGHT | ALIGN_TOP);
	wifiBtn->SetPosition(-61, 17);
	wifiBtn->SetTrigger(&trigA);
	wifiBtn->Clicked.connect(this, &PathBrowser::OnWifiButtonClick);
	
	validBtn = new PictureButton("button_valid.png", "button_valid_over.png", btnSoundClick, btnSoundOver);
	validBtn->SetAlignment(ALIGN_RIGHT | ALIGN_TOP);
	validBtn->SetPosition(-61, 17);
	validBtn->SetTrigger(&trigA);
	validBtn->Clicked.connect(this, &PathBrowser::OnButtonClick);
	
	backInDirBtn = new GuiButton(0, 0);
	backInDirBtn->SetTrigger(&trigB);
	backInDirBtn->Clicked.connect(this, &PathBrowser::OnBackInDirClick);
	
	guiBrowser = NULL;
	curBrowser = NULL;

	fileBrowser = new CustomBrowser();
	curBrowser = fileBrowser;
	guiBrowser = new CustomFileBrowser(curBrowser, 315, 270);
	guiBrowser->SetPosition(30, 56);
	guiBrowser->Clicked.connect(this, &PathBrowser::OnBrowserChanges);

	fileBrowser->SetFilter(filter);
	fileBrowser->BrowsePath(path);
	
	adressText = new GuiText(curBrowser->GetCurrentPath().c_str(), 18, thColor("r=0 g=0 b=0 a=255 - select browser adress text color"));
	adressText->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	adressText->SetPosition(76, 17);
	adressText->SetMaxWidth(237, SCROLL_HORIZONTAL);
	
	Append(browserImg);
	Append(folderBtn);
	Append(devSwitchBtn);
	Append(btnCloseBoxImg);
	Append(closeBtn);
	if (filter != FILTER_FONT)
		Append(btnWifiValBoxImg);
	if (filter == FILTER_LANG)
		Append(wifiBtn);
	else if (filter == FILTER_PATH)
		Append(validBtn);
	Append(adressText);
	Append(guiBrowser);
	Append(backInDirBtn);
	
	this->DimBackground(true);
	SetAlignment(ALIGN_CENTER | ALIGN_MIDDLE);
	SetEffect(EFFECT_FADE, 20);
	
	Application::Instance()->SetUpdateOnly(this);
	Application::Instance()->Append(this);
}

PathBrowser::~PathBrowser()
{
	Application::Instance()->Remove(this);
	RemoveAll();

	Resources::Remove(browserImgData);
	Resources::Remove(sdImgData);
	Resources::Remove(usbImgData);
	Resources::Remove(folderImgData);
	Resources::Remove(btnBoxImgData);

	Resources::Remove(btnSoundClick);
	Resources::Remove(btnSoundOver);

	delete browserImg;
	delete btnCloseBoxImg;
	delete btnWifiValBoxImg;
	delete folderImg;
	if(deviceImg)
		delete deviceImg;
	
	delete adressText;
	
	delete devSwitchBtn;
	delete closeBtn;
	delete wifiBtn;
	delete validBtn;
	delete folderBtn;
	delete backInDirBtn;

	delete fileBrowser;
	delete guiBrowser;
}

void PathBrowser::SetDeviceImage()
{
	if(deviceImg)
		delete deviceImg;
	
	if(currentDevice == SD)
		deviceImg = new GuiImage(sdImgData);
	else
		deviceImg = new GuiImage(usbImgData);
	
	devSwitchBtn->SetImage(deviceImg);
	devSwitchBtn->SetImageOver(deviceImg);
}

void PathBrowser::OnButtonClick(GuiButton *sender, int pointer UNUSED, const POINT &p UNUSED)
{
	ButtonClick(this, filter, sender == validBtn, curBrowser->GetCurrentPath());
}

void PathBrowser::OnFolderButtonClick(GuiButton *sender UNUSED, int pointer UNUSED, const POINT &p UNUSED)
{
	char entered[MAXPATHLEN];
	snprintf(entered, sizeof(entered), "%s", curBrowser->GetCurrentPath().c_str());
	if(OnScreenKeyboard(entered, sizeof(entered)))
	{
		while(entered[strlen(entered)-1] == '/' && entered[strlen(entered)-2] != ':')
			entered[strlen(entered)-1] = 0;
		
		CreateSubfolder(entered);
		fileBrowser->BrowsePath(entered);
		adressText->SetText(curBrowser->GetCurrentPath());
	}
}

void PathBrowser::OnDevButtonClick(GuiButton *sender UNUSED, int pointer UNUSED, const POINT &p UNUSED)
{
	do
	{
		currentDevice++;
		if(currentDevice >= MAXDEVICES)
			currentDevice = SD;
	}
	while(!DeviceHandler::Instance()->IsInserted(currentDevice));
	
	fileBrowser->BrowsePath(fmt("%s:/", DeviceName[currentDevice]));
	adressText->SetText(curBrowser->GetCurrentPath());
	
	SetDeviceImage();
}

void PathBrowser::OnWifiButtonClick(GuiButton *sender UNUSED, int pointer UNUSED, const POINT &p UNUSED)
{
	std::string langPath = curBrowser->GetCurrentPath();
	
	int res = WindowPrompt(tr( "Download/Update Languages Files to:" ), langPath.c_str(), tr( "All Files" ), tr( "Installed Files" ), tr( "Select Files" ), tr( "Cancel" ));
	if(res == 1) //! Download All Files available
	{
		DownloadAllLanguageFiles(langPath);
		
		guiBrowser->Refresh();
	}
	else if(res == 2) //! Update already installed files
	{
		if(!UpdateLanguageFiles(langPath))
		{
			int choice = WindowPrompt(tr( "No Language File found" ), tr( "Download" ), tr( "All Files" ), tr( "Select Files" ), tr( "Cancel" ));
			if(choice == 1) //! Download All Files available
				DownloadAllLanguageFiles(langPath);
			else if(choice == 2) //! Select files to download
				SelectLanguageFiles(langPath);
		}
		
		guiBrowser->Refresh();
	}
	else if(res == 3) //! Select files to download/update
	{
		SelectLanguageFiles(langPath);
		
		guiBrowser->Refresh();
	}
}

void PathBrowser::OnBackInDirClick(GuiButton *sender UNUSED, int pointer UNUSED, const POINT &p UNUSED)
{
	curBrowser->BackInDirectory();
	guiBrowser->SetSelected(0);
	curBrowser->SetPageIndex(0);
	guiBrowser->Refresh();
	adressText->SetText(curBrowser->GetCurrentPath());
}

void PathBrowser::OnBrowserChanges(int index UNUSED)
{
	if(curBrowser->IsCurrentDir())
	{
		int result = curBrowser->ChangeDirectory();
		if(result >= 0)
		{
			guiBrowser->SetSelected(0);
			curBrowser->SetPageIndex(0);
			guiBrowser->Refresh();
			adressText->SetText(curBrowser->GetCurrentPath());
		}
		else
		{
			ShowError(tr("Can't browse that path."));
		}
	}
	else
	{
		ButtonClick(this, filter, true, curBrowser->GetCurrentSelectedFilepath());
	}
}
