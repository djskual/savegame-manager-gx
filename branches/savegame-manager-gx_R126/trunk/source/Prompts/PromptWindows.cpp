#include <gccore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "../Language/gettext.h"
#include "../Settings/CSettings.h"
#include "../System/wpad.h"
#include "../Network/networkops.h"
#include "../Themes/CTheme.h"
#include "../Tools/gecko.h"
#include "../Tools/StringTools.h"
#include "../Tools/ShowError.h"
#include "../Text/Text.hpp"
#include "ScreenSaver.hpp"
#include "ProgressWindow.h"
#include "PromptWindow.hpp"
#include "PromptWindows.h"
#include "CreditWindow.h"
#include "HomeMenu.h"
#include "../menu.h"

static bool AbortRequested = false;

static void AbortCallback(void)
{
	AbortRequested = true;
}

/****************************************************************************
 * WindowPrompt
 *
 * Displays a prompt window to user, with information, an error message, or
 * presenting a user with a choice of up to 4 Buttons.
 *
 * Give him 1 Title, 1 Subtitle and 4 Buttons
 * If title/subtitle or one of the buttons is not needed give him a 0 on that
 * place.
 ***************************************************************************/
int WindowPrompt(const char *title,
				 const char *msg,
				 const char *btn1Label,
				 const char *btn2Label,
				 const char *btn3Label,
				 const char *btn4Label,
				 int wait)
{
	int choice = -1;
	int count = wait;
	gprintf("\n\tWindowPrompt( %s, %s, %s, %s, %s, %s, %i ): ", title, msg, btn1Label, btn2Label, btn3Label, btn4Label, wait);

	PromptWindow *Window = new PromptWindow;
	Window->SetTitle(title);
	Window->SetMessageText(msg);
	if(btn1Label)
		Window->AddButton(btn1Label);
	if(btn2Label)
		Window->AddButton(btn2Label);
	if(btn3Label)
		Window->AddButton(btn3Label);
	if(btn4Label)
		Window->AddButton(btn4Label);

	mainWindow->SetState(STATE_DISABLED);
	mainWindow->Append(Window);
	ResumeGui();

	while (choice == -1)
	{
		VIDEO_WaitVSync();
		
		choice = Window->GetChoice();
		
		if (count > 0) count--;
		if (count == 0) choice = 1;
	}

	delete Window;

	mainWindow->SetState(STATE_DEFAULT);
	gprintf(" %i", choice);

	return choice;
}

/****************************************************************************
 * OnScreenNumpad
 *
 * Opens an on-screen numpad window, with the data entered being stored
 * into the specified variable.
 ***************************************************************************/
int OnScreenNumpad(char * var, u32 maxlen)
{
	int save = -1;

	GuiNumpad numpad(var, maxlen);

	GuiImageData btnOutline(Resources::GetFile("button.png"), Resources::GetFileSize("button.png"));

	GuiTrigger trigA;
	trigA.SetSimpleTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	GuiTrigger trigB;
	trigB.SetSimpleTrigger(-1, WiiControls.BackButton | ClassicControls.BackButton << 16, GCControls.BackButton);

	GuiText okBtnTxt(tr( "OK" ), 22, thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
	GuiImage okBtnImg(&btnOutline);
	if (Settings.wsprompt)
	{
		okBtnTxt.SetWidescreen(Settings.Widescreen);
		okBtnImg.SetWidescreen(Settings.Widescreen);
	}
	GuiButton okBtn(&okBtnImg, &okBtnImg, 0, 4, 5, -15, &trigA, btnSoundOver, btnSoundClick, 1);
	okBtn.SetLabel(&okBtnTxt);
	GuiText cancelBtnTxt(tr( "Cancel" ), 22, thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
	GuiImage cancelBtnImg(&btnOutline);
	if (Settings.wsprompt)
	{
		cancelBtnTxt.SetWidescreen(Settings.Widescreen);
		cancelBtnImg.SetWidescreen(Settings.Widescreen);
	}
	GuiButton cancelBtn(&cancelBtnImg, &cancelBtnImg, 1, 4, -5, -15, &trigA, btnSoundOver, btnSoundClick, 1);
	cancelBtn.SetLabel(&cancelBtnTxt);
	cancelBtn.SetTrigger(&trigB);

	numpad.Append(&okBtn);
	numpad.Append(&cancelBtn);

	HaltGui();
	mainWindow->SetState(STATE_DISABLED);
	mainWindow->Append(&numpad);
	ResumeGui();

	while (save == -1)
	{
		VIDEO_WaitVSync();

		if (okBtn.GetState() == STATE_CLICKED)
			save = 1;
		else if (cancelBtn.GetState() == STATE_CLICKED) save = 0;
	}

	if (save == 1)
	{
		snprintf(var, maxlen, "%s", numpad.kbtextstr);
	}

	HaltGui();
	mainWindow->Remove(&numpad);
	mainWindow->SetState(STATE_DEFAULT);
	ResumeGui();
	gprintf("\t%s", (save == 1 ? "saved" : "discarded"));
	return save;
}

/****************************************************************************
 * OnScreenKeyboard
 *
 * Opens an on-screen keyboard window, with the data entered being stored
 * into the specified variable.
 ***************************************************************************/
int OnScreenKeyboard(char * var, u32 maxlen, int min)
{

	int save = -1;

	gprintf("\nOnScreenKeyboard(%s, %i, %i) \n\tkeyset = %i", var, maxlen, min, Settings.keyset);

	GuiKeyboard keyboard(var, maxlen, min, Settings.keyset);

	GuiImageData btnOutline(Resources::GetFile("button.png"), Resources::GetFileSize("button.png"));

	GuiTrigger trigA;
	trigA.SetSimpleTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	GuiTrigger trigB;
	trigB.SetSimpleTrigger(-1, WiiControls.BackButton | ClassicControls.BackButton << 16, GCControls.BackButton);

	GuiText okBtnTxt(tr( "OK" ), 22, thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
	GuiImage okBtnImg(&btnOutline);
	if (Settings.wsprompt)
	{
		okBtnTxt.SetWidescreen(Settings.Widescreen);
		okBtnImg.SetWidescreen(Settings.Widescreen);
	}
	GuiButton okBtn(&okBtnImg, &okBtnImg, 0, 4, 5, 15, &trigA, btnSoundOver, btnSoundClick, 1);
	okBtn.SetLabel(&okBtnTxt);
	GuiText cancelBtnTxt(tr( "Cancel" ), 22, thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
	GuiImage cancelBtnImg(&btnOutline);
	if (Settings.wsprompt)
	{
		cancelBtnTxt.SetWidescreen(Settings.Widescreen);
		cancelBtnImg.SetWidescreen(Settings.Widescreen);
	}
	GuiButton cancelBtn(&cancelBtnImg, &cancelBtnImg, 1, 4, -5, 15, &trigA, btnSoundOver, btnSoundClick, 1);
	cancelBtn.SetLabel(&cancelBtnTxt);
	cancelBtn.SetTrigger(&trigB);

	keyboard.Append(&okBtn);
	keyboard.Append(&cancelBtn);

	HaltGui();
	mainWindow->SetState(STATE_DISABLED);
	mainWindow->Append(&keyboard);
	ResumeGui();

	while (save == -1)
	{
		VIDEO_WaitVSync();

		if (okBtn.GetState() == STATE_CLICKED)
			save = 1;
		else if (cancelBtn.GetState() == STATE_CLICKED) save = 0;
	}

	if (save)
	{
		snprintf(var, maxlen, "%s", keyboard.kbtextstr);
	}

	HaltGui();
	mainWindow->Remove(&keyboard);
	mainWindow->SetState(STATE_DEFAULT);
	ResumeGui();
	gprintf("\t%s", (save ? "saved" : "discarded"));
	return save;
}

/****************************************************************************
 * NetworkInitPrompt
 ***************************************************************************/
bool NetworkInitPrompt()
{
	if (!IsNetworkInit())
	{
		int choice = WindowPrompt(tr("No network connection"),tr("Do you want to connect?"),tr("Yes"),tr("Cancel"));
		if(choice == 1)
		{
			
			ProgressSetAbortCallback(AbortCallback);
			ShowProgress(tr("Initializing Network"), 0, 0, 0, 1, false, true, THROBBER);
			
			Initialize_Network();
			
			if(AbortRequested)
			{
				usleep(20000);
				ProgressStop();
				DeinitNetwork();
				ShowError(tr("Action cancelled."));
				return false;
			}
			
			ProgressStop();
			if (!IsNetworkInit())
			{
				ShowError(tr("Could not initialize network!"));
				return false;
			}
		}
	}
	
	return true;
}

/****************************************************************************
 * ShowCredits
 * Display Credits
 ***************************************************************************/
void ShowCredits()
{
	CreditWindow *Credits = new CreditWindow();

	mainWindow->Append(Credits);
	
	while(Credits->ButtonPressed() == false) usleep(100);
	
	delete Credits;
	Credits = NULL;
}

/****************************************************************************
 * ShowHomeMenu
 * Display HomeMenu
 ***************************************************************************/
int ShowHomeMenu()
{
	HomeMenu *hm = new HomeMenu();

	mainWindow->Append(hm);
	
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
 * ShowScreenSaver
 * Display ScreenSaver
 ***************************************************************************/
void ShowScreenSaver()
{
	ScreenSaver *screenSaver = new ScreenSaver();

	mainWindow->Append(screenSaver);
	
	while (screenSaver->GetButtonPressed() == false) usleep(100);

	delete screenSaver;
	screenSaver = NULL;
}
