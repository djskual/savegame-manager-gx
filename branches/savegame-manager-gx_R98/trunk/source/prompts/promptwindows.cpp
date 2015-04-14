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
 * promptwindows.cpp
 *
 * for SaveGame Manager 2011
 ***************************************************************************/

#include <gccore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <wiiuse/wpad.h>

#include "libwiigui/gui.h"
#include "prompts/promptwindows.h"
#include "prompts/PromptWindow.h"
#include "prompts/ProgressWindow.h"
#include "prompts/CreditWindow.h"
#include "prompts/ShortCuts.h"
#include "prompts/HomeMenu.h"
#include "network/networkops.h"
#include "menu/MainWindow.h"
#include "main.h"

extern bool actioncanceled;

/****************************************************************************
 * OnScreenKeyboard by Tantric 2009
 *
 * Opens an on-screen keyboard window, with the data entered being stored
 * into the specified variable.
 ***************************************************************************/
int OnScreenKeyboard(char * var, u16 maxlen, int stealth)
{
	int save = -1;

	GuiKeyboard keyboard(var, maxlen);

	GuiSound btnSoundOver(button_over_wav, button_over_wav_size, cfg.SFXVolume);
	GuiSound btnSoundClick(button_click_wav, button_click_wav_size, cfg.SFXVolume);
	
	SimpleGuiTrigger trigA(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

	GuiText okBtnTxt(tr("OK"), 22, (GXColor){0, 0, 0, 255});
	GuiText cancelBtnTxt(tr("Cancel"), 22, (GXColor){0, 0, 0, 255});
	
	GuiImageData btnOutline(button_png, button_png_size);
	
	GuiImage okBtnImg(&btnOutline);
	GuiButton okBtn(btnOutline.GetWidth(), btnOutline.GetHeight());
	okBtn.SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	okBtn.SetPosition(25, -25);
	okBtn.SetLabel(&okBtnTxt);
	okBtn.SetImage(&okBtnImg);
	okBtn.SetImageOver(&okBtnImg);
	okBtn.SetSoundOver(&btnSoundOver);
	okBtn.SetSoundClick(&btnSoundClick);
	okBtn.SetTrigger(&trigA);
	okBtn.SetEffectGrow();

	GuiImage cancelBtnImg(&btnOutline);
	GuiButton cancelBtn(btnOutline.GetWidth(), btnOutline.GetHeight());
	cancelBtn.SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
	cancelBtn.SetPosition(-25, -25);
	cancelBtn.SetLabel(&cancelBtnTxt);
	cancelBtn.SetImage(&cancelBtnImg);
	cancelBtn.SetImageOver(&cancelBtnImg);
	cancelBtn.SetSoundOver(&btnSoundOver);
	cancelBtn.SetSoundClick(&btnSoundClick);
	cancelBtn.SetTrigger(&trigA);
	cancelBtn.SetEffectGrow();
	
	keyboard.Append(&okBtn);
	keyboard.Append(&cancelBtn);
	
	MainWindow::Instance()->HaltGui();
	if(!stealth)
		MainWindow::Instance()->SetState(STATE_DISABLED);
	MainWindow::Instance()->Append(&keyboard);
	MainWindow::Instance()->ChangeFocus(&keyboard);
	MainWindow::Instance()->ResumeGui();

	while(save == -1)
	{
		usleep(THREAD_SLEEP);

		if(okBtn.GetState() == STATE_CLICKED)
			save = 1;
		
		else if(cancelBtn.GetState() == STATE_CLICKED)
			save = 0;
	}

	if(save)
	{
		snprintf(var, maxlen, "%s", keyboard.kbtextstr);
	}

	MainWindow::Instance()->HaltGui();
	MainWindow::Instance()->Remove(&keyboard);
	if(!stealth)
		MainWindow::Instance()->SetState(STATE_DEFAULT);
	MainWindow::Instance()->ResumeGui();
	return save;
}

/****************************************************************************
* WindowPrompt
*
* Shortcut function
***************************************************************************/
int
WindowPrompt(const char *title, const char *msg, const char *btn1Label,
const char *btn2Label, const char *btn3Label,
const char *btn4Label, bool resetstate)
{
    PromptWindow * Prompt = new PromptWindow(title, msg, btn1Label, btn2Label, btn3Label, btn4Label/*, resetstate*/);

    if (resetstate)
		MainWindow::Instance()->SetState(STATE_DISABLED);
	MainWindow::Instance()->Append(Prompt);
	
    int choice = -1;
	while(choice == -1)
    {
        usleep(100);
		
        choice = Prompt->GetChoice();
    }

    delete Prompt;
    Prompt = NULL;

    if (resetstate)
		MainWindow::Instance()->SetState(STATE_DEFAULT);
	
	return choice;
}

/****************************************************************************
 * ShowHomeMenu
 * Display HomeMenu
 ***************************************************************************/
int ShowHomeMenu()
{
	HomeMenu *hm = new HomeMenu();

	MainWindow::Instance()->Append(hm);
	
	int choice = -1;
	while (choice == -1)
	{
		VIDEO_WaitVSync();
		
		choice = hm->GetChoice();
	}

	delete hm;
	hm = NULL;

	return choice;	
}

/****************************************************************************
 * ShowCredits
 * Display Credits
 ***************************************************************************/
void ShowCredits()
{
    
	
	CreditWindow *Credits = new CreditWindow();

	MainWindow::Instance()->Append(Credits);
	
    while(Credits->ButtonPressed() == false) usleep(100);
    
	delete Credits;
    Credits = NULL;
	
}

/****************************************************************************
 * ShowShortcuts
 * Display Shortcuts
 ***************************************************************************/
void ShowShortcuts()
{
	Shortcuts *sc = new Shortcuts();

	MainWindow::Instance()->Append(sc);
	
	int choice = -1;
	while(choice == -1)
	{
		usleep(100);
		
		choice = sc->GetChoice();
		
		if(choice == 1) //mii
		{
			snprintf(navinfo.LastBrowsedPath , sizeof(navinfo.LastBrowsedPath), cfg.SaveMiisPath);
			
			if (device.BrowsedDevice == 3) {
				device.LastBrowsedWii = 3;
				
			}
			device.BrowsedDevice = 4;
			
		}
		else if(choice == 2) //saves
		{
			snprintf(navinfo.LastBrowsedPath , sizeof(navinfo.LastBrowsedPath), cfg.SaveGamePath);
			
			if (device.BrowsedDevice == 4) {
				device.LastBrowsedWii = 4;
			}
			
			device.BrowsedDevice = 3;
		}
		
		if(choice > 0)
			navinfo.refresh = true;
	}
 
	delete sc;
	sc = NULL;
}

/****************************************************************************
 * WindowScreensaver
 * Display screensaver
 ***************************************************************************/
int WindowScreensaver() {
	
	int i = 0;
	bool exit = false;
	// initialize random seed: //
	srand ( time(NULL) );

	GuiImageData logo(SaveGameManagerGX_png, SaveGameManagerGX_png_size);
	GuiImage logoImg(&logo);
	logoImg.SetPosition(172,152);
	logoImg.SetAlignment(ALIGN_LEFT,ALIGN_TOP);

	GuiImage BackgroundImg(640,480,(GXColor) {0, 0, 0, 255});
	BackgroundImg.SetPosition(0,0);
	BackgroundImg.SetAlignment(ALIGN_LEFT,ALIGN_TOP);

	GuiWindow screensaverWindow(screenwidth,screenheight);
	screensaverWindow.Append(&BackgroundImg);
	screensaverWindow.Append(&logoImg);
	
	MainWindow::Instance()->HaltGui();
	MainWindow::Instance()->Append(&screensaverWindow);
	MainWindow::Instance()->ResumeGui();

    while (!exit) {
        i++;
        if (IsWpadConnected()) {
            exit = true;
        }
        // Set position only every 400000th loop //
        if ((i % 8000000) == 0) {
            // Set random position //
            logoImg.SetPosition((rand() % 345), (rand() % 305));
        }
    }
	
	MainWindow::Instance()->HaltGui();
    MainWindow::Instance()->Remove(&screensaverWindow);
    MainWindow::Instance()->ResumeGui();
	return 1;
}

/****************************************************************************
 * NetworkInitPrompt
 ***************************************************************************/
int NetworkInitPrompt() {
    
    if (!IsNetworkInit()) {
        int choice = WindowPrompt(tr("No network connection"),tr("Do you want to connect?"),tr("Yes"),tr("Cancel"));
		if(choice == 1) {
			StartProgress(tr("Initializing Network"), CONNECT, true);
			ShowProgress(0, 1);
			Initialize_Network();
			if(actioncanceled)
			{
				usleep(20000);
				StopProgress();
				DeInit_Network();
				WindowPrompt(tr("Error"), tr("Action cancelled."), tr("OK"));
				return 0;
			}
			StopProgress();
			if (!IsNetworkInit())
				WindowPrompt(tr("Could not initialize network!"), 0, tr("OK"));
		}
	}
	if (IsNetworkInit()) {
		return 1;
	}
	return 0;
}

