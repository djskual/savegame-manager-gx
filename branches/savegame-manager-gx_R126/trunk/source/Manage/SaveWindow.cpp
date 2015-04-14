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
 * SaveWindow.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include <unistd.h>

#include "SaveWindow.hpp"
#include "../Saves/CSaveList.hpp"
#include "../EmuNand/CEmuList.hpp"
#include "../Themes/CTheme.h"
#include "../Tools/StringTools.h"
#include "../Settings/CSettings.h"
#include "../Prompts/SelectBrowser.h"
#include "../FileOperations/fileops.h"
#include "../FileOperations/Nand.hpp"
#include "../Language/gettext.h"
#include "../Menu/menus.h"

#define NONE            0
#define LEFT            1
#define RIGHT           2
#define IN              3
#define OUT             4

SaveWindow::SaveWindow(int Selected)
	: GuiWindow(572, 372)
{
	returnVal = -1;
	saveSelected = Selected;
	
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
	
	actionButtons = new ManageButtons(0,
									  0,
									  (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_MANAGE)) ? tr( "Extract" ) : NULL,
									  (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_MANAGE)) ? ((Settings.Sneek == OFF) ? tr( "Compress" ) : NULL) : NULL,
									  ((Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_MANAGE)) && !Settings.Sneek && Settings.EmuNand) ? tr( "Install" ) : NULL);
	actionButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	actionButtons->SetPosition(-6, -30);
	
	pathButtons = new ManageButtons(tr( "Extract to:" ), 0, tr( "Default" ), tr( "Browsed" ), tr( "Custom" ), tr( "Cancel" ));
	pathButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	pathButtons->SetPosition(-6, -30);
	
	confirmButtons = new ManageButtons(tr( "Extract to:" ), 0, tr( "OK" ), tr( "Cancel" ));
	confirmButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	confirmButtons->SetPosition(-6, -30);
	
	succesButtons = new ManageButtons(tr("Succesfully extracted"), 0, tr( "OK" ));
	succesButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	succesButtons->SetPosition(-6, -30);
	
	errorButtons = new ManageButtons(tr( "Error" ), 0, tr( "OK" ));
	errorButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	errorButtons->SetPosition(-6, -30);
	
	cancelButtons = new ManageButtons(tr("Extract cancelled"), 0, tr( "OK" ));
	cancelButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	cancelButtons->SetPosition(-6, -30);
	
	Append(dialogBoxImg);
	Append(btnCloseBoxImg);
	Append(btnClose);
	Append(btnLeft);
	Append(btnRight);
	Append(nameTxt);
	Append(subnameTxt);
	Append(idBox);
	Append(blockBox);
	Append(actionButtons);
	if (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_WIISAVE_DOWNLOAD))
	{
		Append(buttonBoxImg);
		Append(btnWifi);
	}
	
	ChangeSave(NONE);
	
	SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_IN, 50);
}

SaveWindow::~SaveWindow()
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
	
	delete btnClose;
	delete btnWifi;
	delete btnLeft;
	delete btnRight;
	
	delete saveBanner;
	
	delete idBox;
	delete blockBox;
	
	delete actionButtons;
	delete pathButtons;
	delete confirmButtons;
	delete succesButtons;
	delete errorButtons;
	delete cancelButtons;

	ResumeGui();
	
	mainWindow->SetState(STATE_DEFAULT);
}

void SaveWindow::LoadBannerImage(u64 tid)
{
	if(saveBanner)
		Remove(saveBanner);
	
	char path[ISFS_MAXPATH];
	u32 high = (u32)(tid >> 32);
	u32 low  = (u32)(tid & 0xFFFFFFFF);

	snprintf(path, sizeof(path),"/title/%08x/%08x/data/banner.bin", high, low);
	
	u8* buffer = NULL;
	u32 size = 0;
	Nand::LoadFile(path, &buffer, &size);

	saveBanner = new GuiSaveBanner(buffer);
	saveBanner->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	saveBanner->SetPosition(30,20);
	
	free(buffer);
	buffer = NULL;
	
	Append(saveBanner);
}

void SaveWindow::SetBoxes()
{
	if(idBox)
		Remove(idBox);
	if(blockBox)
		Remove(blockBox);
	
	idBox->SetText((SaveList.GetID(saveSelected)).c_str());
	idBox->SetPosition(260, 27);
	
	char blockTxt[10];
	snprintf(blockTxt, sizeof(blockTxt), "%0.f", ceil(SaveList.GetBlocks(saveSelected)));
	blockBox->SetText(blockTxt);
	blockBox->SetPosition((260+idBox->GetWidth()+20), 27);
	
	Append(idBox);
	Append(blockBox);
}

void SaveWindow::SetWindowEffect(int direction, int in_out)
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

void SaveWindow::ResetButtons()
{
	if(actionButtons)
		Remove(actionButtons);
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
	
	delete actionButtons;
	delete pathButtons;
	delete confirmButtons;
	delete succesButtons;
	delete errorButtons;
	delete cancelButtons;
	
	actionButtons = new ManageButtons(0,
									  0,
									  (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_MANAGE)) ? tr( "Extract" ) : NULL,
									  (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_MANAGE)) ? ((Settings.Sneek == OFF) ? tr( "Compress" ) : NULL) : NULL,
									  ((Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_MANAGE)) && !Settings.Sneek && Settings.EmuNand) ? tr( "Install" ) : NULL);
	actionButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	actionButtons->SetPosition(-6, -30);
	
	pathButtons = new ManageButtons(tr( "Extract to:" ), 0, tr( "Default" ), tr( "Browsed" ), tr( "Custom" ), tr( "Cancel" ));
	pathButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	pathButtons->SetPosition(-6, -30);
	
	confirmButtons = new ManageButtons(tr( "Extract to:" ), 0, tr( "OK" ), tr( "Cancel" ));
	confirmButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	confirmButtons->SetPosition(-6, -30);
	
	succesButtons = new ManageButtons(tr("Succesfully extracted"), 0, tr( "OK" ));
	succesButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	succesButtons->SetPosition(-6, -30);
	
	errorButtons = new ManageButtons(tr( "Error" ), 0, tr( "OK" ));
	errorButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	errorButtons->SetPosition(-6, -30);
	
	cancelButtons = new ManageButtons(tr("Extract cancelled"), 0, tr( "OK" ));
	cancelButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	cancelButtons->SetPosition(-6, -30);
	
	Append(actionButtons);
}

void SaveWindow::ChangeSave(int EffectDirection)
{
	SetWindowEffect(EffectDirection, OUT);
	
	HaltGui();
	
	LoadBannerImage(SaveList.GetTid(saveSelected));
	
	nameTxt->SetText((SaveList.GetName(saveSelected)).c_str());
	subnameTxt->SetText((SaveList.GetSubname(saveSelected)).c_str());
	
	SetBoxes();
	ResetButtons();
	
	EffectDirection = ((EffectDirection == LEFT) ? RIGHT : ((EffectDirection == RIGHT) ? LEFT : NONE));
	SetWindowEffect(EffectDirection, IN);
}

void SaveWindow::SetButtonsClickable(bool clickable)
{
	if ( !clickable )
	{
		btnClose->SetState(STATE_DISABLED);
		btnWifi->SetState(STATE_DISABLED);
		btnLeft->SetState(STATE_DISABLED);
		btnRight->SetState(STATE_DISABLED);
	}
	else
	{
		btnClose->SetState(STATE_DEFAULT);
		btnWifi->SetState(STATE_DEFAULT);
		btnLeft->SetState(STATE_DEFAULT);
		btnRight->SetState(STATE_DEFAULT);
	}
}

int SaveWindow::Show()
{
	int choice = -1;

	while(choice == -1)
	{
		usleep(100);
		
		choice = MainLoop();
	}

	return choice;
}

int SaveWindow::MainLoop()
{
	if (btnClose->GetState() == STATE_CLICKED) //! close
		returnVal = 0;

	else if (btnWifi->GetState() == STATE_CLICKED) //! download save
	{
		SetButtonsClickable(false);
		actionButtons->SetEffect(EFFECT_FADE, -20);
		
		while (actionButtons->GetEffect()) usleep(50);
		
		actionButtons->ResetState();
		HaltGui();
		Remove(actionButtons);
		
		Tools = new SaveManageTools(tr("Downloading saveslist:"), "www.wiisave.com");
		Tools->DownloadSave(SaveList.GetID(saveSelected));
		
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
		if (saveSelected >= SaveList.GetFilteredCount())
			saveSelected = 0;
		
		ChangeSave(LEFT);
		
		btnRight->ResetState();
	}

	else if (btnLeft->GetState() == STATE_CLICKED) //! previous save
	{
		saveSelected--;
		if (saveSelected < 0)
			saveSelected = SaveList.GetFilteredCount()-1;
		
		ChangeSave(RIGHT);
		
		btnLeft->ResetState();
	}
	
	else if (actionButtons->GetState() == STATE_CLICKED)
	{
		SetButtonsClickable(false);
		
		action = actionButtons->GetChoice();
		
		actionButtons->ResetState();
		
		actionButtons->SetEffect(EFFECT_FADE, -20);
		
		while (actionButtons->GetEffect()) usleep(50);
		
		HaltGui();
		
		Remove(actionButtons);
		
		if (action == EXTRACT_SAVE || action == COMPRESS_SAVE)
		{
			if (action == EXTRACT_SAVE) pathButtons->SetTitle(tr( "Extract to:" ));
			else if(action == COMPRESS_SAVE) pathButtons->SetTitle(tr( "Compress to:" ));
			pathButtons->SetEffect(EFFECT_FADE, 20);
			
			Append(pathButtons);
			ResumeGui();
			
			while (pathButtons->GetEffect()) usleep(50);
		}
		else if(action == INSTALL_EMU_SAVE)
		{
			confirmButtons->SetTitle(tr( "Install to:" ));
			confirmButtons->SetMessage(Settings.EmuNandPath);
			confirmButtons->SetEffect(EFFECT_FADE, 20);
			
			Append(confirmButtons);
			ResumeGui();
			
			while (confirmButtons->GetEffect()) usleep(50);
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
		
		if (action == EXTRACT_SAVE) confirmButtons->SetTitle(tr( "Extract to:" ));
		else if(action == COMPRESS_SAVE) confirmButtons->SetTitle(tr( "Compress to:" ));
		
		if (choice == 1) //! Default
		{
			if (action == EXTRACT_SAVE) Settings.TmpPath = Settings.SaveGamePath;
			else if(action == COMPRESS_SAVE) Settings.TmpPath = Settings.DataBinPath;
			confirmButtons->SetMessage(Settings.TmpPath.c_str());
			confirmButtons->SetEffect(EFFECT_FADE, 20);
			
			Append(confirmButtons);
			ResumeGui();
			
			while (confirmButtons->GetEffect()) usleep(50);
		}
		else if (choice == 2) //! Browsed
		{
			Settings.TmpPath = Settings.BrowserPath;
			confirmButtons->SetMessage(Settings.TmpPath.c_str());
			confirmButtons->SetEffect(EFFECT_FADE, 20);
			
			Append(confirmButtons);
			ResumeGui();
			
			while (confirmButtons->GetEffect()) usleep(50);
		}
		else if (choice == 3) //! Custom
		{
			this->SetState(STATE_DISABLED);
			
			int res = selectBrowser(((action == EXTRACT_SAVE) ? Settings.SaveGamePath : Settings.DataBinPath), PATH);
			if(res == 2)
			{
				confirmButtons->SetMessage(Settings.TmpPath.c_str());
				confirmButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(confirmButtons);
				ResumeGui();
				
				while (confirmButtons->GetEffect()) usleep(50);
				
				this->SetState(STATE_DEFAULT);
				SetButtonsClickable(false);
			}
			else
			{
				actionButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(actionButtons);
				ResumeGui();
				
				while (actionButtons->GetEffect()) usleep(50);
				
				this->SetState(STATE_DEFAULT);
				SetButtonsClickable(true);
			}
		}
		else if (choice == 4) //! Cancel
		{
			actionButtons->SetEffect(EFFECT_FADE, 20);
			
			Append(actionButtons);
			ResumeGui();
			
			while (actionButtons->GetEffect()) usleep(50);
			
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
		
		if (choice == 1 && action == EXTRACT_SAVE) //! Extract
		{
			Tools = new SaveManageTools(tr("Extracting..."));
			char* buffer = (char*)memalign(32, ISFS_MAXPATH);
			std::string nandPath;
			
			Settings.TmpPath += fmt("/%016llx", SaveList.GetTid(saveSelected));
			
			//! Set title UID
			if (ES_SetUID(SaveList.GetTid(saveSelected)) < 0)
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
			if (ES_GetDataDir(SaveList.GetTid(saveSelected), buffer) < 0)
			{
				Tools->StopProgress();
				
				errorButtons->SetMessage(tr("Unable to get NAND path"));
				errorButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(errorButtons);
				ResumeGui();
				
				while (errorButtons->GetEffect()) usleep(50);
				goto out;
			}
			nandPath = buffer;
			free(buffer);
			
			Tools->ExtractSave_Nand_Dev(nandPath, Settings.TmpPath);
			
			while (Tools->GetResult() == 0) usleep(50);
			
			Tools->StopProgress();
			
			if (Tools->GetResult() > 0)
			{
				succesButtons->SetTitle(tr( "Succesfully extracted" ));
				succesButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(succesButtons);
				ResumeGui();
				
				while (succesButtons->GetEffect()) usleep(50);
			}
			else if (Tools->GetResult() == -10)
			{
				cancelButtons->SetTitle(tr( "Extract cancelled" ));
				cancelButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(cancelButtons);
				ResumeGui();
				
				while (cancelButtons->GetEffect()) usleep(50);
				
				Settings.TmpPath += "/";
				RemoveDirectory(Settings.TmpPath.c_str());
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
		else if (choice == 1 && action == COMPRESS_SAVE) //! Compress
		{
			Tools = new SaveManageTools(tr("Compressing..."));
			
			Tools->CompressSave_Nand_Dev(SaveList.GetTid(saveSelected), Settings.TmpPath);
			
			while (Tools->GetResult() == 0) usleep(50);
			
			Tools->StopProgress();
			
			if (Tools->GetResult() > 0)
			{
				succesButtons->SetTitle(tr( "Succesfully compressed" ));
				succesButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(succesButtons);
				ResumeGui();
				
				while (succesButtons->GetEffect()) usleep(50);
			}
			else if (Tools->GetResult() == -10)
			{
				cancelButtons->SetTitle(tr( "Compression cancelled" ));
				cancelButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(cancelButtons);
				ResumeGui();
				
				while (cancelButtons->GetEffect()) usleep(50);
				
				Settings.TmpPath += "/";
				RemoveDirectory(Settings.TmpPath.c_str());
			}
			else if (Tools->GetResult() < 0 && Tools->GetResult() != -10)
			{
				errorButtons->SetMessage(tr("Unable to compress datas"));
				errorButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(errorButtons);
				ResumeGui();
				
				while (errorButtons->GetEffect()) usleep(50);
			}
			
			Tools->Reset();
			delete Tools;
		}
		else if (choice == 1 && action == INSTALL_EMU_SAVE) //! Install to emulated nand
		{
			Tools = new SaveManageTools(tr("Installing..."));
			
			Tools->InstallSave_Nand_Emu(SaveList.GetTid(saveSelected));
			
			while (Tools->GetResult() == 0) usleep(50);
			
			Tools->StopProgress();
			
			if (Tools->GetResult() > 0)
			{
				EmuList.SetRefresh();
				
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
				
				Settings.TmpPath += "/";
				RemoveDirectory(Settings.TmpPath.c_str());
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
			actionButtons->SetEffect(EFFECT_FADE, 20);
			Append(actionButtons);
			ResumeGui();
			
			while (actionButtons->GetEffect()) usleep(50);
			
			SetButtonsClickable(true);
		}
	}
	
	else if (succesButtons->GetState() == STATE_CLICKED)
	{
		succesButtons->SetEffect(EFFECT_FADE, -20);
		actionButtons->SetEffect(EFFECT_FADE, 20);
		
		while (succesButtons->GetEffect()) usleep(50);
		
		succesButtons->ResetState();
		
		HaltGui();
		
		Remove(succesButtons);
		Append(actionButtons);
		
		ResumeGui();
		
		while (actionButtons->GetEffect()) usleep(50);
		
		SetButtonsClickable(true);
	}
	
	else if (errorButtons->GetState() == STATE_CLICKED)
	{
		errorButtons->SetEffect(EFFECT_FADE, -20);
		actionButtons->SetEffect(EFFECT_FADE, 20);
		
		while (errorButtons->GetEffect()) usleep(50);
		
		errorButtons->ResetState();
		
		HaltGui();
		
		Remove(errorButtons);
		Append(actionButtons);
		
		ResumeGui();
		
		while (actionButtons->GetEffect()) usleep(50);
		
		SetButtonsClickable(true);
	}
	
	else if (cancelButtons->GetState() == STATE_CLICKED)
	{
		cancelButtons->SetEffect(EFFECT_FADE, -20);
		actionButtons->SetEffect(EFFECT_FADE, 20);
		
		while (cancelButtons->GetEffect()) usleep(50);
		
		cancelButtons->ResetState();
		
		HaltGui();
		
		Remove(cancelButtons);
		Append(actionButtons);
		
		ResumeGui();
		
		while (actionButtons->GetEffect()) usleep(50);
		
		SetButtonsClickable(true);
	}
	
	return returnVal;
}
