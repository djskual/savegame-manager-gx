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
 * FileSaveWindow.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include <unistd.h>

#include "FileSaveWindow.hpp"
#include "../Files/CFileList.hpp"
#include "../Themes/CTheme.h"
#include "../Settings/CSettings.h"
#include "../FileOperations/fileops.h"
#include "../Prompts/SelectBrowser.h"
#include "../EmuNand/CEmuList.hpp"
#include "../Saves/CSaveList.hpp"
#include "../Saves/SaveTools.h"
#include "../Language/gettext.h"
#include "../Menu/menus.h"

#define NONE            0
#define LEFT            1
#define RIGHT           2
#define IN              3
#define OUT             4

FileSaveWindow::FileSaveWindow(int Selected)
	: GuiWindow(572, 372)
{
	returnVal = -1;
	saveSelected = Selected;
	refreshNeeded = false;
	action = 0;
	install_type = 0;
	
	saveBanner = NULL;
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
	imgLeft = Resources::GetImageData("arrow_left.png");
	imgRight = Resources::GetImageData("arrow_right.png");

	trigA = new SimpleGuiTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	trigB = new GuiTrigger();
	trigB->SetButtonOnlyTrigger(-1, WiiControls.BackButton | ClassicControls.BackButton << 16, GCControls.BackButton);
	trigHome = new GuiTrigger();
	trigHome->SetButtonOnlyTrigger(-1, WiiControls.HomeButton | ClassicControls.HomeButton << 16, GCControls.HomeButton);
	trigL = new GuiTrigger();
	trigL->SetButtonOnlyTrigger(-1, WiiControls.LeftButton | ClassicControls.LeftButton << 16, GCControls.LeftButton);
	trigR = new GuiTrigger();
	trigR->SetButtonOnlyTrigger(-1, WiiControls.RightButton | ClassicControls.RightButton << 16, GCControls.RightButton);
	trigPlus = new GuiTrigger();
	trigPlus->SetButtonOnlyTrigger(-1, WiiControls.NextButton | ClassicControls.NextButton << 16, 0);
	trigMinus = new GuiTrigger();
	trigMinus->SetButtonOnlyTrigger(-1, WiiControls.PrevButton | ClassicControls.PrevButton << 16, 0);
	
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

	btnLeftImg = new GuiImage(imgLeft);
	btnLeft = new GuiButton(btnLeftImg, btnLeftImg, 0, 5, 10, 0, trigA, btnSoundOver, btnSoundClick, 1);
	btnLeft->SetTrigger(trigL);
	btnLeft->SetTrigger(trigMinus);

	btnRightImg = new GuiImage(imgRight);
	btnRight = new GuiButton(btnRightImg, btnRightImg, 1, 5, -22, 0, trigA, btnSoundOver, btnSoundClick, 1);
	btnRight->SetTrigger(trigR);
	btnRight->SetTrigger(trigPlus);
	
	not_installed = FileList.GetIsNotInstalled(saveSelected);
	display_install = false;
	if(Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_MANAGE))
	{
		if(!not_installed || (!Settings.Sneek && Settings.EmuNand))
			display_install = true;
	}
	actionsButtons = new ManageButtons(0,
									   0,
									   display_install ? tr( "Install" ) : NULL,
									   (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_MANAGE)) ? tr( "Delete" ) : NULL,
									   (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_MANAGE)) ? tr( "Move" ) : NULL,
									   (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_MANAGE)) ? tr( "Copy" ) : NULL);
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
	
	pathButtons = new ManageButtons(tr( "Move to:" ), 0, tr( "Default" ), tr( "Browsed" ), tr( "Custom" ), tr( "Cancel" ));
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
	Append(btnLeft);
	Append(btnRight);
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
	
	ChangeSave(NONE);
	
	SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_IN, 50);
}

FileSaveWindow::~FileSaveWindow()
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
	delete trigL;
	delete trigR;
	delete trigPlus;
	delete trigMinus;
	delete trigHome;

	delete imgDialogBox;
	delete imgButtonBox;
	delete imgClose;
	delete imgCloseOver;
	delete imgWifi;
	delete imgWifiOver;
	delete imgLeft;
	delete imgRight;

	delete dialogBoxImg;
	delete buttonBoxImg;
	delete btnCloseBoxImg;
	delete btnCloseImg;
	delete btnCloseImgOver;
	delete btnWifiImg;
	delete btnWifiImgOver;
	delete btnLeftImg;
	delete btnRightImg;
	
	delete nameTxt;
	delete subnameTxt;
	delete warningTxt;
	
	delete btnClose;
	delete btnWifi;
	delete btnLeft;
	delete btnRight;
	
	if (saveBanner)
		delete saveBanner;
	
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

void FileSaveWindow::LoadBannerImage(u64 tid)
{
	if(saveBanner)
		Remove(saveBanner);
	
	char path[ISFS_MAXPATH];
	snprintf(path, sizeof(path),"%s/%016llx/banner.bin", Settings.BrowserPath.c_str(), tid);
	
	u8* buffer = NULL;
	u64 size = 0;
	LoadFileToMem(path, &buffer, &size);
	
	saveBanner = new GuiSaveBanner(buffer);
	saveBanner->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	saveBanner->SetPosition(30,20);
	
	free(buffer);
	buffer = NULL;
	
	Append(saveBanner);
}

void FileSaveWindow::SetBoxes()
{
	if(idBox)
		Remove(idBox);
	if(blockBox)
		Remove(blockBox);
	
	u64 tid = StrToHex64(FileList.GetFilename(saveSelected).c_str());
	ID = titleText((u32)(tid>> 32),(u32)(tid & 0xFFFFFFFF));
	idBox->SetText(ID.c_str());
	idBox->SetPosition(260, 27);
	
	u64 foldersize = 0;
	u32 filenumber = 0;
	char savepath[ISFS_MAXPATH];
	snprintf(savepath, sizeof(savepath),"%s/%016llx", Settings.BrowserPath.c_str(), tid);
	GetFolderSize(savepath, &foldersize, &filenumber);
	
	char blockTxt[10];
	snprintf(blockTxt, sizeof(blockTxt), "%0.f", ceil((float)((float)foldersize/(float)NAND_BLOCK_SIZE)));
	blockBox->SetText(blockTxt);
	blockBox->SetPosition((260+idBox->GetWidth()+20), 27);
	
	Append(idBox);
	Append(blockBox);
}

void FileSaveWindow::SetWindowEffect(int direction, int in_out)
{
	if(direction == LEFT) {
		if(in_out == IN)
			SetEffect(EFFECT_SLIDE_LEFT | EFFECT_SLIDE_IN, 50);
		else
			SetEffect(EFFECT_SLIDE_LEFT | EFFECT_SLIDE_OUT, 50);
	}
	else if(direction == RIGHT)
	{
		if(in_out == IN)
			SetEffect(EFFECT_SLIDE_RIGHT | EFFECT_SLIDE_IN, 50);
		else
			SetEffect(EFFECT_SLIDE_RIGHT | EFFECT_SLIDE_OUT, 50);
	}
	
	ResumeGui();
	
	while(parentElement && (this->GetEffect() > 0 || nameTxt->GetEffect() > 0)) usleep(100);
}

void FileSaveWindow::ResetButtons()
{
	if(actionsButtons)
		Remove(actionsButtons);
	if(installButtons)
		Remove(installButtons);
	if(pathButtons)
		Remove(pathButtons);
	if(confirmButtons)
		Remove(confirmButtons);
	if(succesButtons)
		Remove(succesButtons);
	if(errorButtons)
		Remove(errorButtons);
	if(cancelButtons)
		Remove(cancelButtons);
	if(warningTxt)
		Remove(warningTxt);
	
	delete actionsButtons;
	delete installButtons;
	delete pathButtons;
	delete confirmButtons;
	delete succesButtons;
	delete errorButtons;
	delete cancelButtons;
	
	not_installed = FileList.GetIsNotInstalled(saveSelected);
	display_install = false;
	if(Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_MANAGE))
	{
		if(!not_installed || (!Settings.Sneek && Settings.EmuNand))
			display_install = true;
	}
	actionsButtons = new ManageButtons(0,
									   0,
									   display_install ? tr( "Install" ) : NULL,
									   (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_MANAGE)) ? tr( "Delete" ) : NULL,
									   (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_MANAGE)) ? tr( "Move" ) : NULL,
									   (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_MANAGE)) ? tr( "Copy" ) : NULL);
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
	
	pathButtons = new ManageButtons(tr( "Move to:" ), 0, tr( "Default" ), tr( "Browsed" ), tr( "Custom" ), tr( "Cancel" ));
	pathButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	pathButtons->SetPosition(-6, -30);
	
	confirmButtons = new ManageButtons(tr( "Install from:" ), 0, tr( "OK" ), tr( "Cancel" ));
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
	
	Append(actionsButtons);
	if(not_installed)
		Append(warningTxt);
}

void FileSaveWindow::RemoveEntry()
{
	FileList.RemoveEntry(saveSelected);
	
	saveSelected--;
	if (saveSelected < 0)
		saveSelected = FileList.GetFilteredCount()-1;
	
	int i = 0;
	while(!FileList.GetIsSave(saveSelected) && (i < FileList.GetFilteredCount()))
	{
		i++;
		saveSelected--;
		if (saveSelected < 0)
			saveSelected = FileList.GetFilteredCount()-1;
	}
	
	if (i >= FileList.GetFilteredCount())
	{
		mainWindow->SetState(STATE_DEFAULT);
		returnVal = 0;
		return;
	}
	
	ChangeSave(RIGHT);
}

void FileSaveWindow::ChangeSave(int EffectDirection)
{
	SetWindowEffect(EffectDirection, OUT);
	
	HaltGui();
	
	u64 tid = StrToHex64(FileList.GetFilename(saveSelected).c_str());
	LoadBannerImage(tid);
	
	nameTxt->SetText(FileList.GetDisplayname(saveSelected));
	
	char path[MAXPATHLEN];
	snprintf(path, sizeof(path),"%s/%016llx", Settings.BrowserPath.c_str(), tid);
	subnameTxt->SetText(GetDeviceSubname(path));
	
	SetBoxes();
	ResetButtons();
	
	EffectDirection = ((EffectDirection == LEFT) ? RIGHT : ((EffectDirection == RIGHT) ? LEFT : NONE));
	SetWindowEffect(EffectDirection, IN);
}

void FileSaveWindow::SetButtonsClickable(bool clickable)
{
	if ( !clickable )
	{
		btnClose->SetState(STATE_DISABLED);
		btnWifi->SetState(STATE_DISABLED);
		btnLeft->SetState(STATE_DISABLED);
		btnRight->SetState(STATE_DISABLED);
		warningTxt->SetVisible(false);
	}
	else
	{
		btnClose->SetState(STATE_DEFAULT);
		btnWifi->SetState(STATE_DEFAULT);
		btnLeft->SetState(STATE_DEFAULT);
		btnRight->SetState(STATE_DEFAULT);
		warningTxt->SetVisible(true);
	}
}

int FileSaveWindow::Show()
{
	int choice = -1;

	while(choice == -1)
	{
		usleep(100);
		
		choice = MainLoop();
	}

	return choice;
}

int FileSaveWindow::MainLoop()
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
		
		Tools = new SaveManageTools(tr("Downloading saveslist:"), "www.wiisave.com");
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

	else if (btnRight->GetState() == STATE_CLICKED) //! next save
	{
		saveSelected++;
		if (saveSelected >= FileList.GetFilteredCount())
			saveSelected = 0;
		
		int i = 0;
		while(!FileList.GetIsSave(saveSelected) && (i < FileList.GetFilteredCount()))
		{
			i++;
			saveSelected++;
			if (saveSelected >= FileList.GetFilteredCount())
				saveSelected = 0;
		}
		
		ChangeSave(LEFT);
		
		btnRight->ResetState();
	}

	else if (btnLeft->GetState() == STATE_CLICKED) //! previous save
	{
		saveSelected--;
		if (saveSelected < 0)
			saveSelected = FileList.GetFilteredCount()-1;
		
		int i = 0;
		while(!FileList.GetIsSave(saveSelected) && (i < FileList.GetFilteredCount()))
		{
			i++;
			saveSelected--;
			if (saveSelected < 0)
				saveSelected = FileList.GetFilteredCount()-1;
		}
		
		ChangeSave(RIGHT);
		
		btnLeft->ResetState();
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
		
		if (action == MOVE_SAVE || action == COPY_SAVE)
		{
			this->SetState(STATE_DISABLED);
			
			int res = selectBrowser(Settings.BrowserPath.c_str(), PATH);
			if( res == 2)
			{
				if (strcmp(Settings.BrowserPath.c_str(), Settings.TmpPath.c_str()))
				{
					if (Settings.TmpPath[Settings.TmpPath.size()-1] != '/')
						Settings.TmpPath += "/";
					
					if (action == MOVE_SAVE) confirmButtons->SetTitle(tr( "Move to:" ));
					else if (action == COPY_SAVE) confirmButtons->SetTitle(tr( "Copy to:" ));
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
		else if (action == DELETE_SAVE)
		{
			confirmButtons->SetTitle(tr( "Delete from:" ));
			confirmButtons->SetMessage(Settings.BrowserPath.c_str());
			confirmButtons->SetEffect(EFFECT_FADE, 20);
			
			Append(confirmButtons);
			
			ResumeGui();
			
			while (confirmButtons->GetEffect()) usleep(50);
		}
		else if (action == INSTALL_SAVE)
		{
			confirmButtons->SetTitle(tr( "Install to:" ));
			
			if(!Settings.Sneek && Settings.EmuNand)
			{
				if(not_installed)
				{
					install_type = EMU_INSTALL;
					
					confirmButtons->SetMessage(Settings.EmuNandPath);
					confirmButtons->SetEffect(EFFECT_FADE, 20);
					
					Append(confirmButtons);
					
					ResumeGui();
					
					while (confirmButtons->GetEffect()) usleep(50);
				}
				else
				{
					installButtons->SetEffect(EFFECT_FADE, 20);
					
					Append(installButtons);
					
					ResumeGui();
					
					while (installButtons->GetEffect()) usleep(50);
				}
				
			}
			else
			{
				install_type = NAND_INSTALL;
				
				confirmButtons->SetMessage(tr( "Wii" ));
				confirmButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(confirmButtons);
				
				ResumeGui();
				
				while (confirmButtons->GetEffect()) usleep(50);
			}
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
		
		if (install_type == NAND_INSTALL) //! nand
		{
			confirmButtons->SetMessage(tr( "Wii" ));
			confirmButtons->SetEffect(EFFECT_FADE, 20);
			
			Append(confirmButtons);
			
			ResumeGui();
			
			while (confirmButtons->GetEffect()) usleep(50);
		}
		else if (install_type == EMU_INSTALL) //! emu
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
	
	else if (confirmButtons->GetState() == STATE_CLICKED)
	{
		int choice = confirmButtons->GetChoice();
		
		confirmButtons->SetEffect(EFFECT_FADE, -20);
		
		while (confirmButtons->GetEffect()) usleep(50);
		
		confirmButtons->ResetState();
		HaltGui();
		Remove(confirmButtons);
		
		if (choice == 1 && action == INSTALL_SAVE)
		{
			Tools = new SaveManageTools(tr("Installing..."));
			u64 tid = StrToHex64(FileList.GetFilename(saveSelected).c_str());
			
			//! Generate device path
			devPath = Settings.BrowserPath;
			if (devPath[devPath.size()-1] != '/')
				devPath += "/";
			devPath += FileList.GetFilename(saveSelected);
			
			if(install_type == NAND_INSTALL)
			{
				char* buffer = (char*)memalign(32, ISFS_MAXPATH);
				
				//! Set title UID
				if (ES_SetUID(tid) < 0)
				{
					Tools->StopProgress();
					
					errorButtons->SetMessage(tr("Unable to SetUID"));
					errorButtons->SetEffect(EFFECT_FADE, 20);
					
					Append(errorButtons);
					ResumeGui();
					
					while (errorButtons->GetEffect()) usleep(50);
					goto out;
				}
				
				//! Get NAND path
				if (ES_GetDataDir(tid, buffer) < 0)
				{
					Tools->StopProgress();
					
					errorButtons->SetMessage(tr("Unable to get NAND path"));
					errorButtons->SetEffect(EFFECT_FADE, 20);
					
					Append(errorButtons);
					ResumeGui();
					
					while (errorButtons->GetEffect()) usleep(50);
					goto out;
				}
				std::string nandPath = buffer;
				free(buffer);
				
				Tools->InstallSave_Dev_Nand(devPath, nandPath);
			}
			else
				Tools->InstallSave_Dev_Emu(devPath, tid);
			
			while (Tools->GetResult() == 0) usleep(50);
			
			if (Tools->GetResult() > 0)
			{
				if(install_type == EMU_INSTALL)
					EmuList.SetRefresh();
				else
					SaveList.Get();
			}
			
			Tools->StopProgress();
			
			if (Tools->GetResult() > 0)
			{
				succesButtons->SetTitle(tr( "Succesfully installed" ));
				succesButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(succesButtons);
				ResumeGui();
				
				while (succesButtons->GetEffect()) usleep(50);
				
				
			}
			else if (Tools->GetResult() == -10)
			{
				cancelButtons->SetTitle(tr( "Install cancelled" ));
				cancelButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(cancelButtons);
				ResumeGui();
				
				while (cancelButtons->GetEffect()) usleep(50);
			}
			else if (Tools->GetResult() < 0 && Tools->GetResult() != -10)
			{
				errorButtons->SetMessage(tr("Unable to copy datas"));
				errorButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(errorButtons);
				ResumeGui();
				
				while (errorButtons->GetEffect()) usleep(50);
			}
			
		  out:
			
			Tools->Reset();
			delete Tools;
		}
		else if (choice == 1 && action == DELETE_SAVE)
		{
			Tools = new SaveManageTools(tr("Deleting..."));
			
			devPath = Settings.BrowserPath;
			if (devPath[devPath.size()-1] != '/')
				devPath += "/";
			devPath += FileList.GetFilename(saveSelected);
			devPath += "/";
			
			Tools->DeleteSave_Dev(devPath);
			
			while (Tools->GetResult() == 0) usleep(50);
			
			Tools->StopProgress();
			
			if (Tools->GetResult() > 0)
			{
				refreshNeeded = true;
				
				succesButtons->SetTitle(tr( "Succesfully deleted" ));
				succesButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(succesButtons);
				ResumeGui();
				
				while (succesButtons->GetEffect()) usleep(50);
			}
			else if (Tools->GetResult() == -10)
			{
				cancelButtons->SetTitle(tr( "Delete cancelled" ));
				cancelButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(cancelButtons);
				ResumeGui();
				
				while (cancelButtons->GetEffect()) usleep(50);
			}
			else if (Tools->GetResult() < 0 && Tools->GetResult() != -10)
			{
				errorButtons->SetMessage(tr("Unable to delete datas"));
				errorButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(errorButtons);
				ResumeGui();
				
				while (errorButtons->GetEffect()) usleep(50);
			}
			
			Tools->Reset();
			delete Tools;
		}
		else if (choice == 1 && action == MOVE_SAVE)
		{
			Tools = new SaveManageTools(tr("Moving..."));
			
			devPath = Settings.BrowserPath;
			if (devPath[devPath.size()-1] != '/')
				devPath += "/";
			devPath += FileList.GetFilename(saveSelected);
			
			Tools->MoveSave(devPath, Settings.TmpPath);
			
			while (Tools->GetResult() == 0) usleep(50);
			
			Tools->StopProgress();
			
			if (Tools->GetResult() > 0)
			{
				refreshNeeded = true;
				
				succesButtons->SetTitle(tr( "Succesfully moved" ));
				succesButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(succesButtons);
				ResumeGui();
				
				while (succesButtons->GetEffect()) usleep(50);
			}
			else if (Tools->GetResult() == -10)
			{
				cancelButtons->SetTitle(tr( "Move cancelled" ));
				cancelButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(cancelButtons);
				ResumeGui();
				
				while (cancelButtons->GetEffect()) usleep(50);
			}
			else if (Tools->GetResult() < 0 && Tools->GetResult() != -10)
			{
				errorButtons->SetMessage(tr("Unable to move datas"));
				errorButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(errorButtons);
				ResumeGui();
				
				while (errorButtons->GetEffect()) usleep(50);
			}
			
			Tools->Reset();
			delete Tools;
		}
		else if (choice == 1 && action == COPY_SAVE)
		{
			Tools = new SaveManageTools(tr("Copying..."));
			
			devPath = Settings.BrowserPath;
			if (devPath[devPath.size()-1] != '/')
				devPath += "/";
			devPath += FileList.GetFilename(saveSelected);
			
			Tools->CopySave(devPath, Settings.TmpPath);
			
			while (Tools->GetResult() == 0) usleep(50);
			
			Tools->StopProgress();
			
			if (Tools->GetResult() > 0)
			{
				succesButtons->SetTitle(tr( "Succesfully copied" ));
				succesButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(succesButtons);
				ResumeGui();
				
				while (succesButtons->GetEffect()) usleep(50);
			}
			else if (Tools->GetResult() == -10)
			{
				cancelButtons->SetTitle(tr( "Copy cancelled" ));
				cancelButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(cancelButtons);
				ResumeGui();
				
				while (cancelButtons->GetEffect()) usleep(50);
			}
			else if (Tools->GetResult() < 0 && Tools->GetResult() != -10)
			{
				errorButtons->SetMessage(tr("Unable to copy datas"));
				errorButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(errorButtons);
				ResumeGui();
				
				while (errorButtons->GetEffect()) usleep(50);
			}
			
			Tools->Reset();
			delete Tools;
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
		if (!refreshNeeded)
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
			RemoveEntry();
		
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
