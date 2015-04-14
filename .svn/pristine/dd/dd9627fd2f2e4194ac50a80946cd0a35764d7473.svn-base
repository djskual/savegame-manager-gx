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
 * AllWindow.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include <unistd.h>

#include "AllWindow.hpp"
#include "CErrorLog.h"
#include "../Themes/CTheme.h"
#include "../Settings/CSettings.h"
#include "../Prompts/SelectBrowser.h"
#include "../Language/gettext.h"
#include "../Menu/menus.h"
#include "../Miis/CMiiList.hpp"
#include "../EmuNand/CEmuList.hpp"
#include "../EmuNand/CEmuMiiList.hpp"

AllWindow::AllWindow()
	: GuiWindow(572, 372)
{
	returnVal = -1;
	source = 0;
	type = 0;
	action = 0;
	final_action = 0;
	exit = true;
	refreshNeeded = 0;
	
	SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	SetPosition(6, 0);

	imgDialogBox = Resources::GetImageData("manage_save_bg.png");
	imgButtonBox = Resources::GetImageData("manage_save_button_box.png");
	imgClose = Resources::GetImageData("button_close.png");
	imgCloseOver = Resources::GetImageData("button_close_over.png");
	
	trigA = new SimpleGuiTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	trigB = new GuiTrigger();
	trigB->SetButtonOnlyTrigger(-1, WiiControls.BackButton | ClassicControls.BackButton << 16, GCControls.BackButton);
	trigHome = new GuiTrigger();
	trigHome->SetButtonOnlyTrigger(-1, WiiControls.HomeButton | ClassicControls.HomeButton << 16, GCControls.HomeButton);
	
	dialogBoxImg = new GuiImage(imgDialogBox);

	titleTxt = new GuiText(tr( "Manage All" ), 35, thColor("r=255 g=255 b=255 a=255 - manage windows text color"));
	if (Settings.wsprompt) titleTxt->SetWidescreen(Settings.Widescreen);
	titleTxt->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	titleTxt->SetPosition(-6, 10);
	titleTxt->SetMaxWidth(500, SCROLL_HORIZONTAL);
	
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

	sourceButtons = new AllButtons(tr( "Select a source:" ), 0, tr( "Wii" ), tr( "Device" ), (!Settings.Sneek && Settings.EmuNand) ? tr( "Emu" ) : NULL);
	sourceButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	sourceButtons->SetPosition(-6, -30);
	
	typeButtons = new AllButtons(tr( "Select a type:" ), 0, tr( "Saves" ), tr( "Miis" ));
	typeButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	typeButtons->SetPosition(-6, -30);
	
	actionButtons = new AllButtons(tr( "Wii" ), 0, tr( "Extract to Device" ), (!Settings.Sneek && Settings.EmuNand) ? tr( "Install to EmuNand" ) : NULL);
	actionButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	actionButtons->SetPosition(-6, -30);
	
	pathButtons = new AllButtons(0, 0, tr( "Default" ), tr( "Browsed" ), tr( "Custom" ));
	pathButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	pathButtons->SetPosition(-6, -30);
	
	confirmButtons = new AllButtons(0, 0, tr( "OK" ), tr( "Cancel" ));
	confirmButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	confirmButtons->SetPosition(-6, -30);
	
	succesButtons = new AllButtons(0, 0, tr( "OK" ));
	succesButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	succesButtons->SetPosition(-6, -30);
	
	cancelButtons = new AllButtons(0, 0, tr( "OK" ));
	cancelButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	cancelButtons->SetPosition(-6, -30);
	
	errorButtons = new AllButtons(tr( "Error" ), tr("Show error log?"), tr( "Yes" ), tr( "No" ));
	errorButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	errorButtons->SetPosition(-6, -30);
	
	Append(dialogBoxImg);
	Append(btnCloseBoxImg);
	Append(btnClose);
	Append(titleTxt);
	Append(sourceButtons);
	
	SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_IN, 50);
}

AllWindow::~AllWindow()
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

	delete dialogBoxImg;
	delete btnCloseBoxImg;
	delete btnCloseImg;
	delete btnCloseImgOver;
	
	delete titleTxt;
	
	delete btnClose;
	
	delete actionButtons;
	delete sourceButtons;
	delete pathButtons;
	delete confirmButtons;
	delete succesButtons;
	delete cancelButtons;
	delete errorButtons;

	ResumeGui();
}

void AllWindow::ResetButtons()
{
	if(sourceButtons)
		Remove(sourceButtons);
	if(typeButtons)
		Remove(typeButtons);
	if(actionButtons)
		Remove(actionButtons);
	if(pathButtons)
		Remove(pathButtons);
	if(confirmButtons)
		Remove(confirmButtons);
	if(succesButtons)
		Remove(succesButtons);
	if(cancelButtons)
		Remove(cancelButtons);
	if(errorButtons)
		Remove(errorButtons);
	
	delete sourceButtons;
	delete typeButtons;
	delete actionButtons;
	delete pathButtons;
	delete confirmButtons;
	delete succesButtons;
	delete cancelButtons;
	delete errorButtons;
	
	sourceButtons = new AllButtons(tr( "Select a source:" ), 0, tr( "Wii" ), tr( "Device" ), (!Settings.Sneek && Settings.EmuNand) ? tr( "Emu" ) : NULL);
	sourceButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	sourceButtons->SetPosition(-6, -30);
	
	typeButtons = new AllButtons(tr( "Select a type:" ), 0, tr( "Saves" ), tr( "Miis" ));
	typeButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	typeButtons->SetPosition(-6, -30);
	
	actionButtons = new AllButtons(tr( "Select an action:" ), 0, tr( "Extract to Device" ), (!Settings.Sneek && Settings.EmuNand) ? tr( "Install to EmuNand" ) : NULL);
	actionButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	actionButtons->SetPosition(-6, -30);
	
	pathButtons = new AllButtons(0, 0, tr( "Default" ), tr( "Browsed" ), tr( "Custom" ));
	pathButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	pathButtons->SetPosition(-6, -30);
	
	confirmButtons = new AllButtons(0, 0, tr( "OK" ), tr( "Cancel" ));
	confirmButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	confirmButtons->SetPosition(-6, -30);
	
	succesButtons = new AllButtons(0, 0, tr( "OK" ));
	succesButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	succesButtons->SetPosition(-6, -30);
	
	cancelButtons = new AllButtons(0, 0, tr( "OK" ));
	cancelButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	cancelButtons->SetPosition(-6, -30);
	
	errorButtons = new AllButtons(tr( "Error" ), tr("Show error log?"), tr( "Yes" ), tr( "No" ));
	errorButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	errorButtons->SetPosition(-6, -30);
	
	titleTxt->SetText(tr( "Manage All" ));
	sourceButtons->SetEffect(EFFECT_FADE, 20);
	
	Append(sourceButtons);
	
	exit = true;
}

void AllWindow::SetActionButton()
{
	if(actionButtons)
		Remove(actionButtons);
	delete actionButtons;
	
	if (type == SAVES)
	{
		titleTxt->SetText(tr( "Manage All Saves" ));
		
		if (source == WII_NAND)
		{
			actionButtons = new AllButtons (tr( "Select an action:" ),
											0,
											tr( "Extract to Device" ),
											(!Settings.Sneek && Settings.EmuNand) ? tr( "Install to EmuNand" ) : NULL,
											NULL);
		}
		else if (source == DEVICES)
		{
			actionButtons = new AllButtons (tr( "Select an action:" ),
											0,
											tr( "Install to Wii" ),
											(!Settings.Sneek && Settings.EmuNand) ? tr( "Install to EmuNand" ) : NULL,
											tr( "Delete All" ));
		}
		else if (source == EMU_NAND)
		{
			actionButtons = new AllButtons (tr( "Select an action:" ),
											0,
											tr( "Extract to Device" ),
											tr( "Install to Wii" ),
											tr( "Delete All" ));
		}
	}
	else if (type == MIIS)
	{
		titleTxt->SetText(tr( "Manage All Miis" ));
		
		if (source == WII_NAND)
		{
			actionButtons = new AllButtons (tr( "Select an action:" ),
											0,
											tr( "Extract to Device" ),
											(!Settings.Sneek && Settings.EmuNand) ? tr( "Install to EmuNand" ) : NULL,
											tr( "Delete All" ));
		}
		else if (source == DEVICES)
		{
			actionButtons = new AllButtons (tr( "Select an action:" ),
											0,
											tr( "Install to Wii" ),
											(!Settings.Sneek && Settings.EmuNand) ? tr( "Install to EmuNand" ) : NULL,
											tr( "Delete All" ));
		}
		else if (source == EMU_NAND)
		{
			actionButtons = new AllButtons (tr( "Select an action:" ),
											0,
											tr( "Extract to Device" ),
											tr( "Install to Wii" ),
											tr( "Delete All" ));
		}
	}
	
	actionButtons->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	actionButtons->SetPosition(-6, -30);
	actionButtons->SetEffect(EFFECT_FADE, 20);
	
	Append(actionButtons);
	
	ResumeGui();
	
	while (actionButtons->GetEffect()) usleep(50);
}

void AllWindow::SetFinalAction()
{
	if (type == SAVES)
	{
		if (source == WII_NAND)
		{
			if (action == EXTRACT)
				final_action = EXTRACT_NAND_SAVES_TO_DEV;
			else if (action == INSTALL)
				final_action = INSTALL_NAND_SAVES_TO_EMU;
		}
		else if (source == DEVICES)
		{
			if (action == EXTRACT)
				final_action = INSTALL_DEV_SAVES_TO_NAND;
			else if (action == INSTALL)
				final_action = INSTALL_DEV_SAVES_TO_EMU;
			else if (action == DELETE)
				final_action = DELETE_DEV_SAVES;
		}
		else if (source == EMU_NAND)
		{
			if (action == EXTRACT)
				final_action = EXTRACT_EMU_SAVES_TO_DEV;
			else if (action == INSTALL)
				final_action = INSTALL_EMU_SAVES_TO_NAND;
			else if (action == DELETE)
				final_action = DELETE_EMU_SAVES;
		}
	}
	else if (type == MIIS)
	{
		if (source == WII_NAND)
		{
			if (action == EXTRACT)
				final_action = EXTRACT_NAND_MIIS_TO_DEV;
			else if (action == INSTALL)
				final_action = INSTALL_NAND_MIIS_TO_EMU;
			else if (action == DELETE)
				final_action = DELETE_NAND_MIIS;
		}
		else if (source == DEVICES)
		{
			if (action == EXTRACT)
				final_action = INSTALL_DEV_MIIS_TO_NAND;
			else if (action == INSTALL)
				final_action = INSTALL_DEV_MIIS_TO_EMU;
			else if (action == DELETE)
				final_action = DELETE_DEV_MIIS;
		}
		else if (source == EMU_NAND)
		{
			if (action == EXTRACT)
				final_action = EXTRACT_EMU_MIIS_TO_DEV;
			else if (action == INSTALL)
				final_action = INSTALL_EMU_MIIS_TO_NAND;
			else if (action == DELETE)
				final_action = DELETE_EMU_MIIS;
		}
	}
}

int AllWindow::Show()
{
	int result = -1;

	while(result == -1)
	{
		usleep(100);
		
		result = MainLoop();
	}

	return result;
}

int AllWindow::MainLoop()
{
	if (btnClose->GetState() == STATE_CLICKED) //! close
	{
		btnClose->ResetState();
		
		if (exit)
			returnVal = 0;
		else
			ResetButtons();
	}

	else if (sourceButtons->GetState() == STATE_CLICKED)
	{
		source = sourceButtons->GetChoice();
		exit = false;
		
		sourceButtons->SetEffect(EFFECT_FADE, -20);
		
		while (sourceButtons->GetEffect()) usleep(50);
		
		sourceButtons->ResetState();
		HaltGui();
		Remove(sourceButtons);
		
		typeButtons->SetEffect(EFFECT_FADE, 20);
		Append(typeButtons);
		
		ResumeGui();
		
		while (typeButtons->GetEffect()) usleep(50);
	}
	
	else if (typeButtons->GetState() == STATE_CLICKED)
	{
		type = typeButtons->GetChoice();
		
		typeButtons->SetEffect(EFFECT_FADE, -20);
		
		while (typeButtons->GetEffect()) usleep(50);
		
		typeButtons->ResetState();
		HaltGui();
		Remove(typeButtons);
		
		SetActionButton();
	}
	
	else if (actionButtons->GetState() == STATE_CLICKED)
	{
		action = actionButtons->GetChoice();
		
		actionButtons->SetEffect(EFFECT_FADE, -20);
		
		while (actionButtons->GetEffect()) usleep(50);
		
		actionButtons->ResetState();
		HaltGui();
		Remove(actionButtons);
		
		SetFinalAction();
		
		if (final_action == EXTRACT_NAND_SAVES_TO_DEV ||
			final_action == EXTRACT_EMU_SAVES_TO_DEV ||
			final_action == EXTRACT_NAND_MIIS_TO_DEV ||
			final_action == EXTRACT_EMU_MIIS_TO_DEV)
		{
			pathButtons->SetEffect(EFFECT_FADE, 20);
			pathButtons->SetTitle(tr( "Extract to:" ));
			Append(pathButtons);
			
			ResumeGui();
			
			while (pathButtons->GetEffect()) usleep(50);
		}
		else if(final_action == INSTALL_NAND_SAVES_TO_EMU ||
				final_action == INSTALL_EMU_SAVES_TO_NAND ||
				final_action == INSTALL_NAND_MIIS_TO_EMU ||
				final_action == INSTALL_EMU_MIIS_TO_NAND)
		{
			if (final_action == INSTALL_NAND_SAVES_TO_EMU ||
				final_action == INSTALL_NAND_MIIS_TO_EMU)
				confirmButtons->SetTitle(tr( "Install to:" ));
			else if(final_action == INSTALL_EMU_SAVES_TO_NAND ||
					final_action == INSTALL_EMU_MIIS_TO_NAND)
				confirmButtons->SetTitle(tr( "Install from:" ));
			
			confirmButtons->SetMessage(Settings.EmuNandPath);
			confirmButtons->SetEffect(EFFECT_FADE, 20);
			Append(confirmButtons);
			
			ResumeGui();
			
			while (confirmButtons->GetEffect()) usleep(50);
		}
		else if(final_action == INSTALL_DEV_SAVES_TO_NAND ||
				final_action == INSTALL_DEV_SAVES_TO_EMU ||
				final_action == INSTALL_DEV_MIIS_TO_NAND ||
				final_action == INSTALL_DEV_MIIS_TO_EMU)
		{
			pathButtons->SetEffect(EFFECT_FADE, 20);
			pathButtons->SetTitle(tr( "Install from:" ));
			Append(pathButtons);
			
			ResumeGui();
			
			while (pathButtons->GetEffect()) usleep(50);
		}
		else if(final_action == DELETE_DEV_SAVES ||
				final_action == DELETE_DEV_MIIS)
		{
			pathButtons->SetEffect(EFFECT_FADE, 20);
			pathButtons->SetTitle(tr( "Delete from:" ));
			Append(pathButtons);
			
			ResumeGui();
			
			while (pathButtons->GetEffect()) usleep(50);
		}
		else if(final_action == DELETE_NAND_MIIS ||
				final_action == DELETE_EMU_SAVES ||
				final_action == DELETE_EMU_MIIS)
		{
			confirmButtons->SetTitle(tr( "Delete from:" ));
			
			if(final_action == DELETE_NAND_MIIS)
				confirmButtons->SetMessage(tr( "Wii" ));
			if (final_action == DELETE_EMU_MIIS ||
				final_action == DELETE_EMU_SAVES)
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
		
		if (final_action == EXTRACT_NAND_SAVES_TO_DEV ||
			final_action == EXTRACT_EMU_SAVES_TO_DEV ||
			final_action == EXTRACT_NAND_MIIS_TO_DEV ||
			final_action == EXTRACT_EMU_MIIS_TO_DEV)
			confirmButtons->SetTitle(tr( "Extract to:" ));
		else if(final_action == INSTALL_DEV_SAVES_TO_NAND ||
				final_action == INSTALL_DEV_SAVES_TO_EMU ||
				final_action == INSTALL_DEV_MIIS_TO_NAND ||
				final_action == INSTALL_DEV_MIIS_TO_EMU)
			confirmButtons->SetTitle(tr( "Install from:" ));
		else
			confirmButtons->SetTitle(tr( "Delete from:" ));
		
		if (choice == 1) //! Default
		{
			if (final_action == EXTRACT_NAND_SAVES_TO_DEV ||
				final_action == EXTRACT_EMU_SAVES_TO_DEV ||
				final_action == INSTALL_DEV_SAVES_TO_NAND ||
				final_action == INSTALL_DEV_SAVES_TO_EMU ||
				final_action == DELETE_DEV_SAVES)
				Settings.TmpPath = Settings.SaveGamePath;
			else
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
			
			if (final_action == EXTRACT_NAND_SAVES_TO_DEV ||
				final_action == EXTRACT_EMU_SAVES_TO_DEV ||
				final_action == INSTALL_DEV_SAVES_TO_NAND ||
				final_action == INSTALL_DEV_SAVES_TO_EMU ||
				final_action == DELETE_DEV_SAVES)
				Settings.TmpPath = Settings.SaveGamePath;
			else
				Settings.TmpPath = Settings.SaveMiisPath;
			
			int res = selectBrowser(Settings.TmpPath.c_str(), PATH);
			
			this->SetState(STATE_DEFAULT);
			
			if(res == 2)
			{
				confirmButtons->SetMessage(Settings.TmpPath.c_str());
				confirmButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(confirmButtons);
				ResumeGui();
				
				while (confirmButtons->GetEffect()) usleep(50);
			}
			else
			{
				ResetButtons();
				ResumeGui();
				
				while (sourceButtons->GetEffect()) usleep(50);
			}
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
		
		btnClose->SetState(STATE_DISABLED);
		
		if (choice == 1 && final_action == EXTRACT_NAND_SAVES_TO_DEV)
		{
			saveTools = new SaveAllTools();
			ErrorLog.Reset();
			
			saveTools->ExtractAll_Saves_Nand_Dev(Settings.TmpPath);
			
			while (saveTools->GetResult() == 0) usleep(50);
			
			if (saveTools->GetResult() > 0)
			{
				succesButtons->SetTitle(tr("Succesfully extracted"));
				succesButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(succesButtons);
				ResumeGui();
				
				while (succesButtons->GetEffect()) usleep(50);
			}
			else if (saveTools->GetResult() == -10)
			{
				cancelButtons->SetTitle(tr("Extract cancelled"));
				cancelButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(cancelButtons);
				ResumeGui();
				
				while (cancelButtons->GetEffect()) usleep(50);
			}
			else if (saveTools->GetResult() < 0 && saveTools->GetResult() != -10)
			{
				errorButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(errorButtons);
				ResumeGui();
				
				while (errorButtons->GetEffect()) usleep(50);
			}
			
			saveTools->Reset();
			delete saveTools;
			
			if (!Settings.BrowserPath.compare(Settings.TmpPath)  && Settings.LastBrowser == BROWSE_SDUSB)
				refreshNeeded = 1;
		}
		
		else if (choice == 1 && final_action == INSTALL_NAND_SAVES_TO_EMU)
		{
			saveTools = new SaveAllTools();
			ErrorLog.Reset();
			
			saveTools->InstallAll_Saves_Nand_Emu();
			
			while (saveTools->GetResult() == 0) usleep(50);
			
			if (saveTools->GetResult() > 0)
			{
				succesButtons->SetTitle(tr("Succesfully installed"));
				succesButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(succesButtons);
				ResumeGui();
				
				while (succesButtons->GetEffect()) usleep(50);
			}
			else if (saveTools->GetResult() == -10)
			{
				cancelButtons->SetTitle(tr("Install cancelled"));
				cancelButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(cancelButtons);
				ResumeGui();
				
				while (cancelButtons->GetEffect()) usleep(50);
			}
			else if (saveTools->GetResult() < 0 && saveTools->GetResult() != -10)
			{
				errorButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(errorButtons);
				ResumeGui();
				
				while (errorButtons->GetEffect()) usleep(50);
			}
			
			saveTools->Reset();
			delete saveTools;
			
			EmuList.SetRefresh();
			
			if (Settings.LastBrowser == BROWSE_EMU)
				refreshNeeded = 2;
		}
		
		else if (choice == 1 && final_action == INSTALL_DEV_SAVES_TO_NAND)
		{
			saveTools = new SaveAllTools();
			ErrorLog.Reset();
			
			saveTools->InstallAll_Saves_Dev_Nand(Settings.TmpPath);
			
			while (saveTools->GetResult() == 0) usleep(50);
			
			if (saveTools->GetResult() > 0)
			{
				succesButtons->SetTitle(tr("Succesfully installed"));
				succesButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(succesButtons);
				ResumeGui();
				
				while (succesButtons->GetEffect()) usleep(50);
			}
			else if (saveTools->GetResult() == -10)
			{
				cancelButtons->SetTitle(tr("Install cancelled"));
				cancelButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(cancelButtons);
				ResumeGui();
				
				while (cancelButtons->GetEffect()) usleep(50);
			}
			else if (saveTools->GetResult() < 0 && saveTools->GetResult() != -10)
			{
				errorButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(errorButtons);
				ResumeGui();
				
				while (errorButtons->GetEffect()) usleep(50);
			}
			
			saveTools->Reset();
			delete saveTools;
		}
		
		else if (choice == 1 && final_action == INSTALL_DEV_SAVES_TO_EMU)
		{
			saveTools = new SaveAllTools();
			ErrorLog.Reset();
			
			saveTools->InstallAll_Saves_Dev_Emu(Settings.TmpPath);
			
			while (saveTools->GetResult() == 0) usleep(50);
			
			if (saveTools->GetResult() > 0)
			{
				succesButtons->SetTitle(tr("Succesfully installed"));
				succesButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(succesButtons);
				ResumeGui();
				
				while (succesButtons->GetEffect()) usleep(50);
			}
			else if (saveTools->GetResult() == -10)
			{
				cancelButtons->SetTitle(tr("Install cancelled"));
				cancelButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(cancelButtons);
				ResumeGui();
				
				while (cancelButtons->GetEffect()) usleep(50);
			}
			else if (saveTools->GetResult() < 0 && saveTools->GetResult() != -10)
			{
				errorButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(errorButtons);
				ResumeGui();
				
				while (errorButtons->GetEffect()) usleep(50);
			}
			
			saveTools->Reset();
			delete saveTools;
			
			EmuList.SetRefresh();
			
			if (Settings.LastBrowser == BROWSE_EMU)
				refreshNeeded = 2;
		}
		
		else if (choice == 1 && final_action == EXTRACT_EMU_SAVES_TO_DEV)
		{
			saveTools = new SaveAllTools();
			ErrorLog.Reset();
			
			saveTools->ExtractAll_Saves_Emu_Dev(Settings.TmpPath);
			
			while (saveTools->GetResult() == 0) usleep(50);
			
			if (saveTools->GetResult() > 0)
			{
				succesButtons->SetTitle(tr("Succesfully extracted"));
				succesButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(succesButtons);
				ResumeGui();
				
				while (succesButtons->GetEffect()) usleep(50);
			}
			else if (saveTools->GetResult() == -10)
			{
				cancelButtons->SetTitle(tr("Extract cancelled"));
				cancelButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(cancelButtons);
				ResumeGui();
				
				while (cancelButtons->GetEffect()) usleep(50);
			}
			else if (saveTools->GetResult() < 0 && saveTools->GetResult() != -10)
			{
				errorButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(errorButtons);
				ResumeGui();
				
				while (errorButtons->GetEffect()) usleep(50);
			}
			
			saveTools->Reset();
			delete saveTools;
			
			if (!Settings.BrowserPath.compare(Settings.TmpPath)  && Settings.LastBrowser == BROWSE_SDUSB)
				refreshNeeded = 1;
		}
		
		else if (choice == 1 && final_action == INSTALL_EMU_SAVES_TO_NAND)
		{
			saveTools = new SaveAllTools();
			ErrorLog.Reset();
			
			saveTools->InstallAll_Saves_Emu_Nand();
			
			while (saveTools->GetResult() == 0) usleep(50);
			
			if (saveTools->GetResult() > 0)
			{
				succesButtons->SetTitle(tr("Succesfully installed"));
				succesButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(succesButtons);
				ResumeGui();
				
				while (succesButtons->GetEffect()) usleep(50);
			}
			else if (saveTools->GetResult() == -10)
			{
				cancelButtons->SetTitle(tr("Install cancelled"));
				cancelButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(cancelButtons);
				ResumeGui();
				
				while (cancelButtons->GetEffect()) usleep(50);
			}
			else if (saveTools->GetResult() < 0 && saveTools->GetResult() != -10)
			{
				errorButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(errorButtons);
				ResumeGui();
				
				while (errorButtons->GetEffect()) usleep(50);
			}
			
			saveTools->Reset();
			delete saveTools;
		}
		
		else if (choice == 1 && final_action == DELETE_DEV_SAVES)
		{
			saveTools = new SaveAllTools();
			ErrorLog.Reset();
			
			saveTools->DeleteAll_Saves_Dev(Settings.TmpPath);
			
			while (saveTools->GetResult() == 0) usleep(50);
			
			if (saveTools->GetResult() > 0)
			{
				succesButtons->SetTitle(tr("Succesfully deleted"));
				succesButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(succesButtons);
				ResumeGui();
				
				while (succesButtons->GetEffect()) usleep(50);
			}
			else if (saveTools->GetResult() == -10)
			{
				cancelButtons->SetTitle(tr("Delete cancelled"));
				cancelButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(cancelButtons);
				ResumeGui();
				
				while (cancelButtons->GetEffect()) usleep(50);
			}
			else if (saveTools->GetResult() < 0 && saveTools->GetResult() != -10)
			{
				errorButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(errorButtons);
				ResumeGui();
				
				while (errorButtons->GetEffect()) usleep(50);
			}
			
			saveTools->Reset();
			delete saveTools;
			
			if (!Settings.BrowserPath.compare(Settings.TmpPath)  && Settings.LastBrowser == BROWSE_SDUSB)
				refreshNeeded = 1;
		}
		
		else if (choice == 1 && final_action == DELETE_EMU_SAVES)
		{
			saveTools = new SaveAllTools();
			ErrorLog.Reset();
			
			saveTools->DeleteAll_Saves_Emu();
			
			while (saveTools->GetResult() == 0) usleep(50);
			
			if (saveTools->GetResult() > 0)
			{
				succesButtons->SetTitle(tr("Succesfully deleted"));
				succesButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(succesButtons);
				ResumeGui();
				
				while (succesButtons->GetEffect()) usleep(50);
			}
			else if (saveTools->GetResult() == -10)
			{
				cancelButtons->SetTitle(tr("Delete cancelled"));
				cancelButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(cancelButtons);
				ResumeGui();
				
				while (cancelButtons->GetEffect()) usleep(50);
			}
			else if (saveTools->GetResult() < 0 && saveTools->GetResult() != -10)
			{
				errorButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(errorButtons);
				ResumeGui();
				
				while (errorButtons->GetEffect()) usleep(50);
			}
			
			saveTools->Reset();
			delete saveTools;
			
			EmuList.SetRefresh();
			
			if (Settings.LastBrowser == BROWSE_EMU)
				refreshNeeded = 2;
		}
		
		else if (choice == 1 && final_action == EXTRACT_NAND_MIIS_TO_DEV)
		{
			miiTools = new MiiAllTools();
			ErrorLog.Reset();
			
			miiTools->ExtractAll_Miis_Nand_Dev(Settings.TmpPath);
			
			while (miiTools->GetResult() == 0) usleep(50);
			
			if (miiTools->GetResult() > 0)
			{
				succesButtons->SetTitle(tr("Succesfully extracted"));
				succesButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(succesButtons);
				ResumeGui();
				
				while (succesButtons->GetEffect()) usleep(50);
			}
			else if (miiTools->GetResult() == -10)
			{
				cancelButtons->SetTitle(tr("Extract cancelled"));
				cancelButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(cancelButtons);
				ResumeGui();
				
				while (cancelButtons->GetEffect()) usleep(50);
			}
			else if (miiTools->GetResult() < 0 && miiTools->GetResult() != -10)
			{
				errorButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(errorButtons);
				ResumeGui();
				
				while (errorButtons->GetEffect()) usleep(50);
			}
			
			miiTools->Reset();
			delete miiTools;
			
			if (!Settings.BrowserPath.compare(Settings.TmpPath) && Settings.LastBrowser == BROWSE_SDUSB)
				refreshNeeded = 1;
		}
		
		else if (choice == 1 && final_action == INSTALL_NAND_MIIS_TO_EMU)
		{
			miiTools = new MiiAllTools();
			ErrorLog.Reset();
			
			miiTools->InstallAll_Miis_Nand_Emu();
			
			while (miiTools->GetResult() == 0) usleep(50);
			
			if (miiTools->GetResult() > 0)
			{
				succesButtons->SetTitle(tr("Succesfully installed"));
				succesButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(succesButtons);
				ResumeGui();
				
				while (succesButtons->GetEffect()) usleep(50);
			}
			else if (miiTools->GetResult() == -10)
			{
				cancelButtons->SetTitle(tr("Install cancelled"));
				cancelButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(cancelButtons);
				ResumeGui();
				
				while (cancelButtons->GetEffect()) usleep(50);
			}
			else if (miiTools->GetResult() < 0 && miiTools->GetResult() != -10)
			{
				errorButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(errorButtons);
				ResumeGui();
				
				while (errorButtons->GetEffect()) usleep(50);
			}
			
			miiTools->Reset();
			delete miiTools;
			
			EmuMiiList.SetRefresh();
			
			if (Settings.LastBrowser == BROWSE_EMU_MII)
				refreshNeeded = 2;
		}
		
		else if (choice == 1 && final_action == INSTALL_DEV_MIIS_TO_NAND)
		{
			miiTools = new MiiAllTools();
			ErrorLog.Reset();
			
			miiTools->InstallAll_Miis_Dev_Nand(Settings.TmpPath);
			
			while (miiTools->GetResult() == 0) usleep(50);
			
			if (miiTools->GetResult() > 0)
			{
				succesButtons->SetTitle(tr("Succesfully installed"));
				succesButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(succesButtons);
				ResumeGui();
				
				while (succesButtons->GetEffect()) usleep(50);
			}
			else if (miiTools->GetResult() == -10)
			{
				cancelButtons->SetTitle(tr("Install cancelled"));
				cancelButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(cancelButtons);
				ResumeGui();
				
				while (cancelButtons->GetEffect()) usleep(50);
			}
			else if (miiTools->GetResult() < 0 && miiTools->GetResult() != -10)
			{
				errorButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(errorButtons);
				ResumeGui();
				
				while (errorButtons->GetEffect()) usleep(50);
			}
			
			miiTools->Reset();
			delete miiTools;
			
			MiiList.SetRefresh();
			
			if (Settings.LastBrowser == BROWSE_MII)
				refreshNeeded = 2;
		}
		
		else if (choice == 1 && final_action == INSTALL_DEV_MIIS_TO_EMU)
		{
			miiTools = new MiiAllTools();
			ErrorLog.Reset();
			
			miiTools->InstallAll_Miis_Dev_Emu(Settings.TmpPath);
			
			while (miiTools->GetResult() == 0) usleep(50);
			
			if (miiTools->GetResult() > 0)
			{
				succesButtons->SetTitle(tr("Succesfully installed"));
				succesButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(succesButtons);
				ResumeGui();
				
				while (succesButtons->GetEffect()) usleep(50);
			}
			else if (miiTools->GetResult() == -10)
			{
				cancelButtons->SetTitle(tr("Install cancelled"));
				cancelButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(cancelButtons);
				ResumeGui();
				
				while (cancelButtons->GetEffect()) usleep(50);
			}
			else if (miiTools->GetResult() < 0 && miiTools->GetResult() != -10)
			{
				errorButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(errorButtons);
				ResumeGui();
				
				while (errorButtons->GetEffect()) usleep(50);
			}
			
			miiTools->Reset();
			delete miiTools;
			
			EmuMiiList.SetRefresh();
			
			if (Settings.LastBrowser == BROWSE_EMU_MII)
				refreshNeeded = 2;
		}
		
		else if (choice == 1 && final_action == INSTALL_EMU_MIIS_TO_NAND) //in progress
		{
			miiTools = new MiiAllTools();
			ErrorLog.Reset();
			
			miiTools->InstallAll_Miis_Emu_Nand();
			
			while (miiTools->GetResult() == 0) usleep(50);
			
			if (miiTools->GetResult() > 0)
			{
				succesButtons->SetTitle(tr("Succesfully installed"));
				succesButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(succesButtons);
				ResumeGui();
				
				while (succesButtons->GetEffect()) usleep(50);
			}
			else if (miiTools->GetResult() == -10)
			{
				cancelButtons->SetTitle(tr("Install cancelled"));
				cancelButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(cancelButtons);
				ResumeGui();
				
				while (cancelButtons->GetEffect()) usleep(50);
			}
			else if (miiTools->GetResult() < 0 && miiTools->GetResult() != -10)
			{
				errorButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(errorButtons);
				ResumeGui();
				
				while (errorButtons->GetEffect()) usleep(50);
			}
			
			miiTools->Reset();
			delete miiTools;
			
			MiiList.SetRefresh();
			
			if (Settings.LastBrowser == BROWSE_MII)
				refreshNeeded = 2;
		}
		
		else if (choice == 1 && final_action == EXTRACT_EMU_MIIS_TO_DEV)
		{
			miiTools = new MiiAllTools();
			ErrorLog.Reset();
			
			miiTools->ExtractAll_Miis_Emu_Dev(Settings.TmpPath);
			
			while (miiTools->GetResult() == 0) usleep(50);
			
			if (miiTools->GetResult() > 0)
			{
				succesButtons->SetTitle(tr("Succesfully extracted"));
				succesButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(succesButtons);
				ResumeGui();
				
				while (succesButtons->GetEffect()) usleep(50);
			}
			else if (miiTools->GetResult() == -10)
			{
				cancelButtons->SetTitle(tr("Extract cancelled"));
				cancelButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(cancelButtons);
				ResumeGui();
				
				while (cancelButtons->GetEffect()) usleep(50);
			}
			else if (miiTools->GetResult() < 0 && miiTools->GetResult() != -10)
			{
				errorButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(errorButtons);
				ResumeGui();
				
				while (errorButtons->GetEffect()) usleep(50);
			}
			
			miiTools->Reset();
			delete miiTools;
			
			if (!Settings.BrowserPath.compare(Settings.TmpPath) && Settings.LastBrowser == BROWSE_SDUSB)
				refreshNeeded = 1;
		}
		
		else if (choice == 1 && final_action == DELETE_NAND_MIIS)
		{
			miiTools = new MiiAllTools();
			ErrorLog.Reset();
			
			miiTools->DeleteAll_Miis_Nand();
			
			while (miiTools->GetResult() == 0) usleep(50);
			
			if (miiTools->GetResult() > 0)
			{
				succesButtons->SetTitle(tr("Succesfully deleted"));
				succesButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(succesButtons);
				ResumeGui();
				
				while (succesButtons->GetEffect()) usleep(50);
			}
			else if (miiTools->GetResult() == -10)
			{
				cancelButtons->SetTitle(tr("Delete cancelled"));
				cancelButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(cancelButtons);
				ResumeGui();
				
				while (cancelButtons->GetEffect()) usleep(50);
			}
			else if (miiTools->GetResult() < 0 && miiTools->GetResult() != -10)
			{
				errorButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(errorButtons);
				ResumeGui();
				
				while (errorButtons->GetEffect()) usleep(50);
			}
			
			miiTools->Reset();
			delete miiTools;
			
			MiiList.SetRefresh();
			
			if (Settings.LastBrowser == BROWSE_MII)
				refreshNeeded = 2;
		}
		
		else if (choice == 1 && final_action == DELETE_DEV_MIIS)
		{
			miiTools = new MiiAllTools();
			ErrorLog.Reset();
			
			miiTools->DeleteAll_Miis_Dev(Settings.TmpPath);
			
			while (miiTools->GetResult() == 0) usleep(50);
			
			if (miiTools->GetResult() > 0)
			{
				succesButtons->SetTitle(tr("Succesfully deleted"));
				succesButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(succesButtons);
				ResumeGui();
				
				while (succesButtons->GetEffect()) usleep(50);
			}
			else if (miiTools->GetResult() == -10)
			{
				cancelButtons->SetTitle(tr("Delete cancelled"));
				cancelButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(cancelButtons);
				ResumeGui();
				
				while (cancelButtons->GetEffect()) usleep(50);
			}
			else if (miiTools->GetResult() < 0 && miiTools->GetResult() != -10)
			{
				errorButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(errorButtons);
				ResumeGui();
				
				while (errorButtons->GetEffect()) usleep(50);
			}
			
			miiTools->Reset();
			delete miiTools;
			
			if (!Settings.BrowserPath.compare(Settings.TmpPath) && Settings.LastBrowser == BROWSE_SDUSB)
				refreshNeeded = 1;
		}
		
		else if (choice == 1 && final_action == DELETE_EMU_MIIS)
		{
			miiTools = new MiiAllTools();
			ErrorLog.Reset();
			
			miiTools->DeleteAll_Miis_Emu();
			
			while (miiTools->GetResult() == 0) usleep(50);
			
			if (miiTools->GetResult() > 0)
			{
				succesButtons->SetTitle(tr("Succesfully deleted"));
				succesButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(succesButtons);
				ResumeGui();
				
				while (succesButtons->GetEffect()) usleep(50);
			}
			else if (miiTools->GetResult() == -10)
			{
				cancelButtons->SetTitle(tr("Delete cancelled"));
				cancelButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(cancelButtons);
				ResumeGui();
				
				while (cancelButtons->GetEffect()) usleep(50);
			}
			else if (miiTools->GetResult() < 0 && miiTools->GetResult() != -10)
			{
				errorButtons->SetEffect(EFFECT_FADE, 20);
				
				Append(errorButtons);
				ResumeGui();
				
				while (errorButtons->GetEffect()) usleep(50);
			}
			
			miiTools->Reset();
			delete miiTools;
			
			EmuMiiList.SetRefresh();
			
			if (Settings.LastBrowser == BROWSE_EMU_MII)
				refreshNeeded = 2;
		}
		
		else //! Cancel
		{
			ResetButtons();
			ResumeGui();
			while (sourceButtons->GetEffect()) usleep(50);
		}
		
		btnClose->SetState(STATE_DEFAULT);
	}
	
	else if (succesButtons->GetState() == STATE_CLICKED)
	{
		succesButtons->SetEffect(EFFECT_FADE, -20);
		
		while (succesButtons->GetEffect()) usleep(50);
		
		succesButtons->ResetState();
		
		HaltGui();
		
		ResetButtons();
		ResumeGui();
		
		while (sourceButtons->GetEffect()) usleep(50);
	}
	
	else if (cancelButtons->GetState() == STATE_CLICKED)
	{
		cancelButtons->SetEffect(EFFECT_FADE, -20);
		
		while (cancelButtons->GetEffect()) usleep(50);
		
		cancelButtons->ResetState();
		
		HaltGui();
		
		ResetButtons();
		ResumeGui();
		
		while (sourceButtons->GetEffect()) usleep(50);
	}
	
	else if (errorButtons->GetState() == STATE_CLICKED)
	{
		int choice = errorButtons->GetChoice();
		
		errorButtons->SetEffect(EFFECT_FADE, -20);
		
		while (errorButtons->GetEffect()) usleep(50);
		
		errorButtons->ResetState();
		
		if (choice == 1) //! Show ErrorLog
		{
			ErrorLog.Show();
			
			mainWindow->SetState(STATE_DISABLED);
			this->SetState(STATE_DEFAULT);
		}
		
		HaltGui();
		ResetButtons();
		ResumeGui();
		
		while (sourceButtons->GetEffect()) usleep(50);
	}
	
	return returnVal;
}
