/****************************************************************************
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
 * DataBinWindow.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include <unistd.h>

#include "DataBinWindow.hpp"
#include "ManageProgress.h"
#include "../Files/CFileList.hpp"
#include "../EmuNand/CEmuList.hpp"
#include "../Themes/CTheme.h"
#include "../Settings/CSettings.h"
#include "../FileOperations/fileops.h"
#include "../Prompts/ProgressWindow.h"
#include "../Prompts/SelectBrowser.h"
#include "../Saves/SaveTools.h"
#include "../Language/gettext.h"
#include "../Menu/menus.h"

DataBinWindow::DataBinWindow(int Selected)
	: GuiWindow(572, 372)
{
	returnVal = -1;
	dataBinSelected = Selected;
	filePath = "";
	refreshNeeded = false;
	action = 0;
	install_type = 0;
	
	dataBinBanner = NULL;
	idBox = NULL;
	blockBox = NULL;
	
	SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	SetPosition(6, 0);

	imgDialogBox = Resources::GetImageData("manage_save_bg.png");
	imgButtonBox = Resources::GetImageData("manage_save_button_box.png");
	imgClose = Resources::GetImageData("button_close.png");
	imgCloseOver = Resources::GetImageData("button_close_over.png");
	imgWifi = Resources::GetImageData("button_wifi.png");
	imgWifiOver = Resources::GetImageData("button_wifi_over.png");
	
	trigA = new SimpleGuiTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	trigB = new GuiTrigger();
	trigB->SetButtonOnlyTrigger(-1, WiiControls.BackButton | ClassicControls.BackButton << 16, GCControls.BackButton);
	trigHome = new GuiTrigger();
	trigHome->SetButtonOnlyTrigger(-1, WiiControls.HomeButton | ClassicControls.HomeButton << 16, GCControls.HomeButton);
	
	dialogBoxImg = new GuiImage(imgDialogBox);

	nameTxt = new GuiText("", 35, thColor("r=255 g=255 b=255 a=255 - manage windows text color"));
	if (Settings.wsprompt) nameTxt->SetWidescreen(Settings.Widescreen);
	nameTxt->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	nameTxt->SetPosition(30, 85);
	nameTxt->SetMaxWidth(500, SCROLL_HORIZONTAL);
	
	subnameTxt = new GuiText((char*) NULL, 22, thColor("r=255 g=255 b=255 a=255 - manage windows text color"));
	subnameTxt->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	subnameTxt->SetPosition(30, 123);
	subnameTxt->SetMaxWidth(500, SCROLL_HORIZONTAL);
	
	idBox = new GuiBlockBox("");
	idBox->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	
	blockBox = new GuiBlockBox("");
	blockBox->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	
	btnCloseBoxImg = new GuiImage(imgButtonBox);
	btnCloseBoxImg->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	btnCloseBoxImg->SetPosition(-18, 6);

	btnCloseImg = new GuiImage(imgClose);
	btnCloseImgOver = new GuiImage(imgCloseOver);
	btnClose = new GuiButton(btnCloseImg, btnCloseImgOver, 1, 5, 0, 0, trigA, btnSoundOver, btnSoundClick, 1);
	btnClose->SetTrigger(trigB);
	btnClose->SetTrigger(trigHome);
	btnClose->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	btnClose->SetPosition(-21, 8);

	buttonBoxImg = new GuiImage(imgButtonBox);
	buttonBoxImg->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	buttonBoxImg->SetPosition(-60, 6);

	btnWifiImg = new GuiImage(imgWifi);
	btnWifiImgOver = new GuiImage(imgWifiOver);
	btnWifi = new GuiButton(btnWifiImg, btnWifiImgOver, 1, 5, 0, 0, trigA, btnSoundOver, btnSoundClick, 1);
	btnWifi->SetTrigger(trigB);
	btnWifi->SetTrigger(trigHome);
	btnWifi->SetAlignment(ALIGN_RIGHT, ALIGN_TOP); 
	btnWifi->SetPosition(-63, 8);

	not_installed = FileList.GetIsNotInstalled(dataBinSelected);
	display_install = false;
	if(Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_MANAGE_DATABIN))
	{
		if(!not_installed || (!Settings.Sneek && Settings.EmuNand))
			display_install = true;
	}
	actionsButtons = new ManageButtons(0,
									   0,
									   (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_MANAGE_DATABIN)) ? tr( "Decompress" ) : NULL,
									   display_install ? tr( "Install" ) : NULL,
									   (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_MANAGE_DATABIN)) ? tr( "Delete" ) : NULL,
									   (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_MANAGE_DATABIN)) ? tr( "Copy" ) : NULL);
	actionsButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	actionsButtons->SetPosition(-6, -30);
	if(not_installed)
		actionsButtons->SetPosition(-6, -55);
	
	installButtons = new ManageButtons(tr( "Install to:" ),
									   0,
									   !not_installed ? tr( "Wii" ) : NULL,
									   (!Settings.Sneek && Settings.EmuNand) ? tr( "Emu" ) : NULL,
									   NULL,
									   tr( "Cancel" ));
	installButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	installButtons->SetPosition(-6, -30);
	
	pathButtons = new ManageButtons(tr( "Copy to:" ), 0, tr( "Default" ), tr( "Browsed" ), tr( "Custom" ), tr( "Cancel" ));
	pathButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	pathButtons->SetPosition(-6, -30);
	
	confirmButtons = new ManageButtons(tr( "Install to:" ), 0, tr( "OK" ), tr( "Cancel" ));
	confirmButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	confirmButtons->SetPosition(-6, -30);
	
	succesButtons = new ManageButtons(tr( "Succesfully installed" ), 0, tr( "OK" ));
	succesButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	succesButtons->SetPosition(-6, -30);
	
	errorButtons = new ManageButtons(tr( "Error" ), 0, tr( "OK" ));
	errorButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	errorButtons->SetPosition(-6, -30);
	
	cancelButtons = new ManageButtons(tr( "Install cancelled" ), 0, tr( "OK" ));
	cancelButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	cancelButtons->SetPosition(-6, -30);
	
	warningTxt = new GuiText(tr("WARNING: You need to have a save already installed in the Wii to install this one!!!"), 20, thColor("r=255 g=255 b=255 a=255 - manage windows text color"));
	if (Settings.wsprompt) warningTxt->SetWidescreen(Settings.Widescreen);
	warningTxt->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	warningTxt->SetPosition(-6, -24);
	warningTxt->SetMaxWidth(450, SCROLL_WIDE);
	
	Append(dialogBoxImg);
	Append(btnCloseBoxImg);
	Append(btnClose);
	Append(nameTxt);
	Append(subnameTxt);
	Append(idBox);
	Append(blockBox);
	Append(actionsButtons);
	if (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_WIISAVE_DOWNLOAD))
	{
		Append(buttonBoxImg);
		Append(btnWifi);
	}
	if(not_installed)
		Append(warningTxt);
	
	LoadDataBin();
	
	SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_IN, 50);
	
	ResumeGui();
	
	while(parentElement && (this->GetEffect() > 0 || nameTxt->GetEffect() > 0)) usleep(100);
}

DataBinWindow::~DataBinWindow()
{
	StopEffect();
	SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_OUT, 50);
	ResumeGui();

	while(parentElement && this->GetEffect() > 0) usleep(100);

	HaltGui();

	if(parentElement)
		((GuiWindow * ) parentElement)->Remove(this);

	RemoveAll();

	delete trigA;
	delete trigB;
	delete trigHome;

	delete imgDialogBox;
	delete imgButtonBox;
	delete imgClose;
	delete imgCloseOver;
	delete imgWifi;
	delete imgWifiOver;
	
	delete dialogBoxImg;
	delete buttonBoxImg;
	delete btnCloseBoxImg;
	delete btnCloseImg;
	delete btnCloseImgOver;
	delete btnWifiImg;
	delete btnWifiImgOver;
	
	delete nameTxt;
	delete subnameTxt;
	delete warningTxt;
	
	delete btnClose;
	delete btnWifi;
	
	if (save)
		delete save;
	if (dataBinBanner)
		delete dataBinBanner;
	
	delete idBox;
	delete blockBox;
	
	delete actionsButtons;
	delete installButtons;
	delete pathButtons;
	delete confirmButtons;
	delete succesButtons;
	delete errorButtons;
	delete cancelButtons;

	ResumeGui();
	
	mainWindow->SetState(STATE_DEFAULT);
}

void DataBinWindow::LoadBannerImage(u64 tid)
{
	if(dataBinBanner)
		Remove(dataBinBanner);
	
	u8* buffer = NULL;
	int i = 0;
	for(i = 0; i < save->GetEntriesCount(); i++)
	{
		if(!save->GetEntrie(i).compare("/banner.bin"))
			break;
	}
	
	dataBinBanner = new GuiSaveBanner(save->GetData(i));
	dataBinBanner->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	dataBinBanner->SetPosition(30,20);
	
	free(buffer);
	buffer = NULL;
	
	Append(dataBinBanner);
}

void DataBinWindow::SetBoxes()
{
	if(idBox)
		Remove(idBox);
	if(blockBox)
		Remove(blockBox);
	
	char ID[5];
	snprintf(ID, sizeof(ID), "%s", save->GetID().c_str());
	idBox->SetText(ID);
	idBox->SetPosition(260, 27);
	
	u64 datasize = save->GetDecryptedSize();
	char blockTxt[10];
	snprintf(blockTxt, sizeof(blockTxt), "%0.f", ceil((float)((float)datasize/(float)NAND_BLOCK_SIZE)));
	blockBox->SetText(blockTxt);
	blockBox->SetPosition((260+idBox->GetWidth()+20), 27);
	
	Append(idBox);
	Append(blockBox);
}

void DataBinWindow::LoadDataBin()
{
	DataBinPath = Settings.BrowserPath;
	DataBinPath += "/";
	DataBinPath += FileList.GetFilename(dataBinSelected);
	
	ShowProgress(tr("Loading Data.bin Structure"), 0, 0, 0, 1, false, false, THROBBER);
	
	save = new SaveDataBin(DataBinPath);
	if(!save->IsOk())
	{
		delete save;
		mainWindow->SetState(STATE_DEFAULT);
		returnVal = 0;
	}
	
	
	ProgressStop();
	mainWindow->SetState(STATE_DISABLED);
	
	HaltGui();
	
	LoadBannerImage(save->GetTid());
	
	nameTxt->SetText(save->GetName());
	subnameTxt->SetText(save->GetSubname());
	
	SetBoxes();
}

void DataBinWindow::SetButtonsClickable(bool clickable)
{
	if ( !clickable )
	{
		btnClose->SetState(STATE_DISABLED);
		btnWifi->SetState(STATE_DISABLED);
		warningTxt->SetVisible(false);
	}
	else
	{
		btnClose->SetState(STATE_DEFAULT);
		btnWifi->SetState(STATE_DEFAULT);
		warningTxt->SetVisible(true);
	}
}

int DataBinWindow::Show()
{
	int choice = -1;

	while(choice == -1)
	{
		usleep(100);
		
		choice = MainLoop();
	}

	return choice;
}

int DataBinWindow::MainLoop()
{
	if (btnClose->GetState() == STATE_CLICKED) //! close
		returnVal = 0;

	else if (btnWifi->GetState() == STATE_CLICKED) //! download save
	{
		SetButtonsClickable(false);
		actionsButtons->SetEffect(EFFECT_FADE, -20);
		
		while (actionsButtons->GetEffect()) usleep(50);
		
		actionsButtons->ResetState();
		HaltGui();
		Remove(actionsButtons);
		
		char ID[5];
		snprintf(ID, sizeof(ID), "%s", save->GetID().c_str());
		
		SaveManageTools * Tools = new SaveManageTools(tr("Downloading saveslist:"), "www.wiisave.com");
		Tools->DownloadSave(ID);
		
		while (Tools->GetResult() == 0) usleep(50);
		
		int ret = Tools->GetResult();
		if (ret > 0)
		{
			succesButtons->SetTitle(tr( "Succesfully downloaded" ));
			succesButtons->SetEffect(EFFECT_FADE, 20);
			
			Append(succesButtons);
			ResumeGui();
			
			while (succesButtons->GetEffect()) usleep(50);
		}
		else if (ret == -10)
		{
			cancelButtons->SetTitle(tr( "Download cancelled" ));
			cancelButtons->SetEffect(EFFECT_FADE, 20);
			
			Append(cancelButtons);
			ResumeGui();
			
			while (cancelButtons->GetEffect()) usleep(50);
		}
		else if (ret < 0 && ret != -10)
		{
			if (ret == -1)
				errorButtons->SetMessage(tr("Bad game ID"));
			else if (ret == -2)
				errorButtons->SetMessage(tr("Could not initialize network!"));
			else if (ret == -3)
				errorButtons->SetMessage(tr("Connection to server timed out."));
			else if ((ret == -4) || (ret == -5))
				errorButtons->SetMessage(tr("No saves found on the site."));
			else if (ret == -6)
				errorButtons->SetMessage(tr("Unable to download file."));
			else if (ret == -7)
				errorButtons->SetMessage(tr("Unable to find supported files."));
			
			errorButtons->SetEffect(EFFECT_FADE, 20);
			Append(errorButtons);
			ResumeGui();
			
			while (errorButtons->GetEffect()) usleep(50);
		}
		
		Tools->Reset();
		delete Tools;
	}

	else if (actionsButtons->GetState() == STATE_CLICKED)
	{
		SetButtonsClickable(false);
		
		action = actionsButtons->GetChoice();
		
		actionsButtons->SetEffect(EFFECT_FADE, -20);
		
		while (actionsButtons->GetEffect()) usleep(50);
		
		actionsButtons->ResetState();
		HaltGui();
		
		Remove(actionsButtons);
		
		if (action == DECOMPRESS_DATABIN)
		{
			pathButtons->SetTitle(tr( "Decompress to:" ));
			pathButtons->SetEffect(EFFECT_FADE, 20);
			
			Append(pathButtons);
			
			ResumeGui();
			
			while (pathButtons->GetEffect()) usleep(50);
		}
		else if (action == COPY_DATABIN)
		{
			this->SetState(STATE_DISABLED);
			
			int res = selectBrowser(Settings.BrowserPath.c_str(), PATH);
			if( res == 2)
			{
				if (strcmp(Settings.BrowserPath.c_str(), Settings.TmpPath.c_str()))
				{
					if (Settings.TmpPath[Settings.TmpPath.size()-1] != '/')
						Settings.TmpPath += "/";
					
					confirmButtons->SetTitle(tr( "Copy to:" ));
					confirmButtons->SetMessage(Settings.TmpPath.c_str());
					confirmButtons->SetEffect(EFFECT_FADE, 20);
					
					Append(confirmButtons);
					ResumeGui();
					
					while (confirmButtons->GetEffect()) usleep(50);
				}
				else
				{
					errorButtons->SetMessage(tr("Source and destination folders must be different"));
					errorButtons->SetEffect(EFFECT_FADE, 20);
					
					Append(errorButtons);
					ResumeGui();
					
					while (errorButtons->GetEffect()) usleep(50);
				}
				this->SetState(STATE_DEFAULT);
				SetButtonsClickable(false);
			}
			else
			{
				actionsButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(actionsButtons);
				ResumeGui();
				
				while (actionsButtons->GetEffect()) usleep(50);
				
				this->SetState(STATE_DEFAULT);
				SetButtonsClickable(true);
			}
		}
		else if (action == DELETE_DATABIN)
		{
			confirmButtons->SetTitle(tr( "Delete from:" ));
			confirmButtons->SetMessage(Settings.BrowserPath.c_str());
			confirmButtons->SetEffect(EFFECT_FADE, 20);
			
			Append(confirmButtons);
			
			ResumeGui();
			
			while (confirmButtons->GetEffect()) usleep(50);
		}
		else if (action == INSTALL_DATABIN)
		{
			confirmButtons->SetTitle(tr( "Install to:" ));
			installButtons->SetEffect(EFFECT_FADE, 20);
			
			Append(installButtons);
			
			ResumeGui();
			
			while (installButtons->GetEffect()) usleep(50);
		}
	}
	
	else if (installButtons->GetState() == STATE_CLICKED)
	{
		install_type = installButtons->GetChoice();
		
		installButtons->SetEffect(EFFECT_FADE, -20);
		
		while (installButtons->GetEffect()) usleep(50);
		
		installButtons->ResetState();
		HaltGui();
		Remove(installButtons);
		
		if (install_type == NAND_DATABIN) //! nand
		{
			confirmButtons->SetMessage(tr( "Wii" ));
			confirmButtons->SetEffect(EFFECT_FADE, 20);
			
			Append(confirmButtons);
			
			ResumeGui();
			
			while (confirmButtons->GetEffect()) usleep(50);
		}
		else if (install_type == EMU_DATABIN) //! emu
		{
			confirmButtons->SetMessage(Settings.EmuNandPath);
			confirmButtons->SetEffect(EFFECT_FADE, 20);
			
			Append(confirmButtons);
			
			ResumeGui();
			
			while (confirmButtons->GetEffect()) usleep(50);
		}
		else //! Cancel
		{
			actionsButtons->SetEffect(EFFECT_FADE, 20);
			
			Append(actionsButtons);
			ResumeGui();
			
			while (actionsButtons->GetEffect()) usleep(50);
			
			SetButtonsClickable(true);
		}
	}
	
	else if (pathButtons->GetState() == STATE_CLICKED)
	{
		int choice = pathButtons->GetChoice();
		
		pathButtons->SetEffect(EFFECT_FADE, -20);
		
		while (pathButtons->GetEffect()) usleep(50);
		
		pathButtons->ResetState();
		HaltGui();
		Remove(pathButtons);
		
		if (choice == 1) //! Default
		{
			Settings.TmpPath = Settings.SaveGamePath;
			confirmButtons->SetTitle(tr( "Decompress to:" ));
			confirmButtons->SetMessage(Settings.TmpPath.c_str());
			confirmButtons->SetEffect(EFFECT_FADE, 20);
			
			Append(confirmButtons);
			ResumeGui();
			
			while (confirmButtons->GetEffect()) usleep(50);
		}
		else if (choice == 2) //! Browsed
		{
			Settings.TmpPath = Settings.BrowserPath;
			confirmButtons->SetTitle(tr( "Decompress to:" ));
			confirmButtons->SetMessage(Settings.TmpPath.c_str());
			confirmButtons->SetEffect(EFFECT_FADE, 20);
			
			Append(confirmButtons);
			ResumeGui();
			
			while (confirmButtons->GetEffect()) usleep(50);
		}
		else if (choice == 3) //! Custom
		{
			delete save;
			save = NULL;
			
			int res = selectBrowser(Settings.SaveGamePath, PATH);
			save = new SaveDataBin(DataBinPath);
			if(res == 2)
			{
				confirmButtons->SetTitle(tr( "Decompress to:" ));
				confirmButtons->SetMessage(Settings.TmpPath.c_str());
				confirmButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(confirmButtons);
				ResumeGui();
				
				while (confirmButtons->GetEffect()) usleep(50);
			}
			else
			{
				actionsButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(actionsButtons);
				ResumeGui();
				
				while (actionsButtons->GetEffect()) usleep(50);
				
				SetButtonsClickable(true);
			}
		}
		else if (choice == 4) //! Cancel
		{
			actionsButtons->SetEffect(EFFECT_FADE, 20);
			
			Append(actionsButtons);
			ResumeGui();
			
			while (actionsButtons->GetEffect()) usleep(50);
			
			SetButtonsClickable(true);
		}
	}
	
	else if (confirmButtons->GetState() == STATE_CLICKED)
	{
		int choice = confirmButtons->GetChoice();
		
		confirmButtons->SetEffect(EFFECT_FADE, -20);
		
		while (confirmButtons->GetEffect()) usleep(50);
		
		confirmButtons->ResetState();
		HaltGui();
		Remove(confirmButtons);
		
		if (choice == 1 && action == DECOMPRESS_DATABIN)
		{
			InitManageProgressThread();
			StartManageProgress(tr("Decompressing..."));
			
			bool result = save->WriteDecryptedDataBin(Settings.TmpPath);
			
			ManageProgressStop();
			ExitManageProgressThread();
			
			if (result)
			{
				refreshNeeded = true;
				
				succesButtons->SetTitle(tr( "Succesfully decompressed" ));
				succesButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(succesButtons);
				ResumeGui();
				
				while (succesButtons->GetEffect()) usleep(50);
			}
			else
			{
				errorButtons->SetMessage(tr("Unable to decompress datas"));
				errorButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(errorButtons);
				ResumeGui();
				
				while (errorButtons->GetEffect()) usleep(50);
			}
		}
		else if (choice == 1 && action == INSTALL_DATABIN)
		{
			InitManageProgressThread();
			StartManageProgress(tr("Installing..."));
			
			bool result = 0;
			if(install_type == NAND_DATABIN)
				result = save->InstallDecryptedDataBin();
			else
			{
				result = save->EmuInstallDecryptedDataBin();
				if(result)
					EmuList.SetRefresh();
			}
			
			ManageProgressStop();
			ExitManageProgressThread();
			
			if (result)
			{
				succesButtons->SetTitle(tr( "Succesfully installed" ));
				succesButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(succesButtons);
				ResumeGui();
				
				while (succesButtons->GetEffect()) usleep(50);
			}
			else
			{
				errorButtons->SetMessage(tr("Unable to copy datas"));
				errorButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(errorButtons);
				ResumeGui();
				
				while (errorButtons->GetEffect()) usleep(50);
			}
		}
		else if (choice == 1 && action == COPY_DATABIN)
		{
			InitManageProgressThread();
			StartManageProgress(tr("Copying..."));
			
			filePath = Settings.BrowserPath;
			if (filePath[filePath.size()-1] != '/')
				filePath += "/";
			filePath += FileList.GetFilename(dataBinSelected);
			
			char ID[5];
			snprintf(ID, sizeof(ID), "%s", save->GetID().c_str());
			
			bool result = CopyDataBin(ID, filePath, Settings.TmpPath);
			
			ManageProgressStop();
			ExitManageProgressThread();
			
			if (result)
			{
				refreshNeeded = true;
				
				succesButtons->SetTitle(tr( "Succesfully copied" ));
				succesButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(succesButtons);
				ResumeGui();
				
				while (succesButtons->GetEffect()) usleep(50);
			}
			else
			{
				errorButtons->SetMessage(tr("Unable to copy datas"));
				errorButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(errorButtons);
				ResumeGui();
				
				while (errorButtons->GetEffect()) usleep(50);
			}
		}
		else if (choice == 1 && action == DELETE_DATABIN)
		{
			InitManageProgressThread();
			StartManageProgress(tr("Deleting..."));
			
			filePath = Settings.BrowserPath;
			if (filePath[filePath.size()-1] != '/')
				filePath += "/";
			filePath += FileList.GetFilename(dataBinSelected);
			
			bool result = RemoveFile(filePath.c_str());
			
			sleep(2);
			
			ManageProgressStop();
			ExitManageProgressThread();
			
			if (result)
			{
				refreshNeeded = true;
				
				succesButtons->SetTitle(tr( "Succesfully deleted" ));
				succesButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(succesButtons);
				ResumeGui();
				
				while (succesButtons->GetEffect()) usleep(50);
			}
			else
			{
				errorButtons->SetMessage(tr("Unable to delete datas"));
				errorButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(errorButtons);
				ResumeGui();
				
				while (errorButtons->GetEffect()) usleep(50);
			}
		}
		else //! Cancel
		{
			actionsButtons->SetEffect(EFFECT_FADE, 20);
			Append(actionsButtons);
			ResumeGui();
			
			while (actionsButtons->GetEffect()) usleep(50);
			
			SetButtonsClickable(true);
		}
	}
	
	else if (succesButtons->GetState() == STATE_CLICKED)
	{
		if (action != DELETE_DATABIN)
		{
			succesButtons->SetEffect(EFFECT_FADE, -20);
			actionsButtons->SetEffect(EFFECT_FADE, 20);
			
			while (succesButtons->GetEffect()) usleep(50);
			
			succesButtons->ResetState();
			
			HaltGui();
			
			Remove(succesButtons);
			Append(actionsButtons);
			
			ResumeGui();
			
			while (actionsButtons->GetEffect()) usleep(50);
		}
		else
		{
			mainWindow->SetState(STATE_DEFAULT);
			returnVal = 0;
		}
		
		SetButtonsClickable(true);
	}
	
	else if (errorButtons->GetState() == STATE_CLICKED)
	{
		errorButtons->SetEffect(EFFECT_FADE, -20);
		actionsButtons->SetEffect(EFFECT_FADE, 20);
		
		while (errorButtons->GetEffect()) usleep(50);
		
		errorButtons->ResetState();
		
		HaltGui();
		
		Remove(errorButtons);
		Append(actionsButtons);
		
		ResumeGui();
		
		while (actionsButtons->GetEffect()) usleep(50);
		
		SetButtonsClickable(true);
	}
	
	else if (cancelButtons->GetState() == STATE_CLICKED)
	{
		cancelButtons->SetEffect(EFFECT_FADE, -20);
		actionsButtons->SetEffect(EFFECT_FADE, 20);
		
		while (cancelButtons->GetEffect()) usleep(50);
		
		cancelButtons->ResetState();
		
		HaltGui();
		
		Remove(cancelButtons);
		Append(actionsButtons);
		
		ResumeGui();
		
		while (actionsButtons->GetEffect()) usleep(50);
		
		SetButtonsClickable(true);
	}
	
	return returnVal;
}
