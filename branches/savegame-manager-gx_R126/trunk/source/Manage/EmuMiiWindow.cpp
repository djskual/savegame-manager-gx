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
 * EmuMiiWindow.cpp
 *
 * for SaveGame Manager GX 2012
 ***************************************************************************/
#include <unistd.h>

#include "EmuMiiWindow.hpp"
#include "../EmuNand/CEmuMiiList.hpp"
#include "../Miis/CMiiList.hpp"
#include "../Themes/CTheme.h"
#include "../Settings/CSettings.h"
#include "../Prompts/SelectBrowser.h"
#include "../FileOperations/fileops.h"
#include "../Language/gettext.h"
#include "../Menu/menus.h"

#define NONE            0
#define LEFT            1
#define RIGHT           2
#define IN              3
#define OUT             4

EmuMiiWindow::EmuMiiWindow(int Selected)
	: GuiWindow(572, 372)
{
	returnVal = -1;
	miiSelected = Selected;
	refreshNeeded = false;
	action = 0;
	
	SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	SetPosition(6, 0);

	imgDialogBox = Resources::GetImageData("manage_save_bg.png");
	imgButtonBox = Resources::GetImageData("manage_save_button_box.png");
	imgClose = Resources::GetImageData("button_close.png");
	imgCloseOver = Resources::GetImageData("button_close_over.png");
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
	nameTxt->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	nameTxt->SetPosition(-6, 10);
	nameTxt->SetMaxWidth(500, SCROLL_HORIZONTAL);
	
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

	btnLeftImg = new GuiImage(imgLeft);
	btnLeft = new GuiButton(btnLeftImg, btnLeftImg, 0, 5, 10, 0, trigA, btnSoundOver, btnSoundClick, 1);
	btnLeft->SetTrigger(trigL);
	btnLeft->SetTrigger(trigMinus);

	btnRightImg = new GuiImage(imgRight);
	btnRight = new GuiButton(btnRightImg, btnRightImg, 1, 5, -22, 0, trigA, btnSoundOver, btnSoundClick, 1);
	btnRight->SetTrigger(trigR);
	btnRight->SetTrigger(trigPlus);
	
	actionsButtons = new ManageButtons(0,
									   0,
									   (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_MANAGE_EMUNAND)) ? tr( "Extract" ) : NULL,
									   (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_MANAGE_EMUNAND)) ? tr( "Install" ) : NULL,
									   (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_MANAGE_EMUNAND)) ? tr( "Delete" ) : NULL);
	actionsButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	actionsButtons->SetPosition(-6, -30);
	
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
	
	overWriteButtons = new ManageButtons(tr( "is already installed." ), 0, tr( "OverWrite" ), tr( "Duplicate" ), tr( "Cancel"));
	overWriteButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	overWriteButtons->SetPosition(-6, -30);
	
	miiInfos = new MiiInfos(miiSelected, BROWSE_EMU_MII);
	miiInfos->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	miiInfos->SetPosition(-6, 65);
	
	Append(dialogBoxImg);
	Append(btnCloseBoxImg);
	Append(btnClose);
	Append(btnLeft);
	Append(btnRight);
	Append(nameTxt);
	Append(miiInfos);
	Append(actionsButtons);
	
	ChangeMii(NONE);
	
	SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_IN, 50);
}

EmuMiiWindow::~EmuMiiWindow()
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
	delete imgLeft;
	delete imgRight;

	delete dialogBoxImg;
	delete btnCloseBoxImg;
	delete btnCloseImg;
	delete btnCloseImgOver;
	delete btnLeftImg;
	delete btnRightImg;
	
	delete nameTxt;
	
	delete btnClose;
	delete btnLeft;
	delete btnRight;
	
	delete miiInfos;

	delete actionsButtons;
	delete pathButtons;
	delete confirmButtons;
	delete succesButtons;
	delete errorButtons;
	delete cancelButtons;
	delete overWriteButtons;
	
	ResumeGui();
	
	mainWindow->SetState(STATE_DEFAULT);
}

void EmuMiiWindow::SetWindowEffect(int direction, int in_out)
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

void EmuMiiWindow::ResetButtons()
{
	if(actionsButtons)
		Remove(actionsButtons);
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
	if(overWriteButtons)
		Remove(overWriteButtons);
	if(miiInfos)
		Remove(miiInfos);
	
	delete actionsButtons;
	delete pathButtons;
	delete confirmButtons;
	delete succesButtons;
	delete errorButtons;
	delete cancelButtons;
	delete overWriteButtons;
	delete miiInfos;
	
	actionsButtons = new ManageButtons(0,
									   0,
									   (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_MANAGE_EMUNAND)) ? tr( "Extract" ) : NULL,
									   (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_MANAGE_EMUNAND)) ? tr( "Install" ) : NULL,
									   (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_MANAGE_EMUNAND)) ? tr( "Delete" ) : NULL);
	actionsButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	actionsButtons->SetPosition(-6, -30);
	
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
	
	overWriteButtons = new ManageButtons(tr( "is already installed." ), 0, tr( "OverWrite" ), tr( "Duplicate" ), tr( "Cancel"));
	overWriteButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	overWriteButtons->SetPosition(-6, -30);
	
	miiInfos = new MiiInfos(miiSelected, BROWSE_EMU_MII);
	miiInfos->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	miiInfos->SetPosition(-6, 65);
	
	Append(actionsButtons);
	Append(miiInfos);
}

void EmuMiiWindow::RemoveEntry()
{
	EmuMiiList.RemoveEntry(miiSelected);
	
	if (EmuMiiList.GetFilteredCount() == 0)
	{
		mainWindow->SetState(STATE_DEFAULT);
		returnVal = 0;
		return;
	}
	
	miiSelected--;
	if (miiSelected < 0)
		miiSelected = EmuMiiList.GetFilteredCount()-1;
	
	ChangeMii(RIGHT);
}

void EmuMiiWindow::ChangeMii(int EffectDirection)
{
	SetWindowEffect(EffectDirection, OUT);
	
	HaltGui();
	
	nameTxt->SetText(EmuMiiList.GetName(miiSelected).c_str());
	
	ResetButtons();
	
	EffectDirection = ((EffectDirection == LEFT) ? RIGHT : ((EffectDirection == RIGHT) ? LEFT : NONE));
	SetWindowEffect(EffectDirection, IN);
}

void EmuMiiWindow::SetButtonsClickable(bool clickable)
{
	if ( !clickable )
	{
		btnClose->SetState(STATE_DISABLED);
		btnLeft->SetState(STATE_DISABLED);
		btnRight->SetState(STATE_DISABLED);
	}
	else
	{
		btnClose->SetState(STATE_DEFAULT);
		btnLeft->SetState(STATE_DEFAULT);
		btnRight->SetState(STATE_DEFAULT);
	}
}

int EmuMiiWindow::Show()
{
	int choice = -1;

	while(choice == -1)
	{
		usleep(100);
		
		choice = MainLoop();
	}
	
	if(refreshNeeded)
	{
		wString oldFilter(EmuMiiList.GetCurrentFilter());
		EmuMiiList.FilterList(oldFilter.c_str());
	}
	
	return choice;
}

int EmuMiiWindow::MainLoop()
{
	if (btnClose->GetState() == STATE_CLICKED) //! close
		returnVal = 0;

	else if (btnRight->GetState() == STATE_CLICKED) //! next save
	{
		miiSelected++;
		if (miiSelected >= EmuMiiList.GetFilteredCount())
			miiSelected = 0;
		
		ChangeMii(LEFT);
		
		btnRight->ResetState();
	}

	else if (btnLeft->GetState() == STATE_CLICKED) //! previous save
	{
		miiSelected--;
		if (miiSelected < 0)
			miiSelected = EmuMiiList.GetFilteredCount()-1;
		
		ChangeMii(RIGHT);
		
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
		
		if (action == EXTRACT_EMU_MII)
		{
			pathButtons->SetEffect(EFFECT_FADE, 20);
			
			Append(pathButtons);
			
			ResumeGui();
			
			while (pathButtons->GetEffect()) usleep(50);
		}
		else if (action == INSTALL_EMU_MII)
		{
			confirmButtons->SetTitle(tr( "Install from:" ));
			confirmButtons->SetMessage(Settings.EmuNandPath);
			confirmButtons->SetEffect(EFFECT_FADE, 20);
			
			Append(confirmButtons);
			ResumeGui();
			
			while (confirmButtons->GetEffect()) usleep(50);
		}
		else if (action == DELETE_EMU_MII)
		{
			confirmButtons->SetTitle(tr( "Delete from Emu Nand" ));
			confirmButtons->SetMessage(NULL);
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
		
		if (choice == 1) //! Default
		{
			Settings.TmpPath = Settings.SaveMiisPath;
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
			
			int res = selectBrowser(Settings.SaveMiisPath, PATH);
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
				actionsButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(actionsButtons);
				ResumeGui();
				
				while (actionsButtons->GetEffect()) usleep(50);
				
				this->SetState(STATE_DEFAULT);
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
		
		if (choice == 1 && action == EXTRACT_EMU_MII)
		{
			Tools = new MiiManageTools(tr("Extracting..."));
			
			Tools->ExtractMii_Emu_Dev(Settings.TmpPath, miiSelected);
			
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
		else if (choice == 1 && action == INSTALL_EMU_MII)
		{
			Tools = new MiiManageTools(tr("Installing..."));
			
			int choice = 0;
			int position = 0; //! position to the place of mii in wii file
			bool overwriteOk = false;
			for(int n = 0; n < MiiList.GetFullCount(); n++)
			{
				if(!MiiList.GetFullName(n).compare(EmuMiiList.GetName(miiSelected)))
				{
					Tools->StopProgress();
					Tools->Reset();
					delete Tools;
					
					overWriteButtons->SetEffect(EFFECT_FADE, 20);
					
					Append(overWriteButtons);
					ResumeGui();
					
					while (overWriteButtons->GetEffect()) usleep(50);
					
					while (choice == 0) choice = overWriteButtons->GetChoice();
					
					overWriteButtons->SetEffect(EFFECT_FADE, -20);
					
					overWriteButtons->ResetState();
					HaltGui();
					Remove(overWriteButtons);
					
					if(choice == 1)
					{
						overwriteOk = true;
						position = MiiList.GetFullPosition(n);
					}
					else if(choice == 3)
					{
						actionsButtons->SetEffect(EFFECT_FADE, 20);
						Append(actionsButtons);
						ResumeGui();
						
						while (actionsButtons->GetEffect()) usleep(50);
						SetButtonsClickable(true);
						goto out;
					}
					
					Tools = new MiiManageTools(tr( "Installing..." ));
					n = MiiList.GetFullCount();
				}
			}
			
			if(!overwriteOk)
				position = Tools->FindFirstFree(true);
			Tools->InstallMii_Emu_Nand(EmuMiiList.GetPosition(miiSelected), position);
			
			while (Tools->GetResult() == 0) usleep(50);
			
			Tools->StopProgress();
			
			if (Tools->GetResult() > 0)
			{
				MiiList.SetRefresh();
				
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
			
			Tools->Reset();
			delete Tools;
			
		  out: usleep(1);
		}
		else if (choice == 1 && action == DELETE_EMU_MII)
		{
			Tools = new MiiManageTools(tr( "Deleting..." ));
			
			Tools->DeleteMii_Emu(EmuMiiList.GetPosition(miiSelected));
			
			while (Tools->GetResult() == 0) usleep(50);
			
			Tools->StopProgress();
			
			if (Tools->GetResult() > 0)
			{
				refreshNeeded = true;
				EmuMiiList.SetRefresh();
				
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
