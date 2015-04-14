/****************************************************************************
 * USB Loader GX Team
 * modified by dj_skual
 * for SaveGame Manager GX 2011
 * 
 * libwiigui Template
 * by Tantric 2009
 *
 * menu.cpp
 * Menu flow routines - handles all menu logic
 ***************************************************************************/
#include <unistd.h>

#include "Miis/MiiInstaller.h"
#include "Network/networkops.h"
#include "Settings/CSettings.h"
#include "Settings/settingsprompts.h"
#include "Prompts/ProgressWindow.h"
#include "HomebrewBoot/HomebrewBrowser.hpp"
#include "Themes/ThemeDownloader.h"
#include "Themes/ThemeMenu.h"
#include "Themes/CTheme.h"
#include "System/sys.h"
#include "Menu/menus.h"
#include "menu.h"
#include "main.h"

/*** Variables that are also used extern ***/
GuiWindow * mainWindow = NULL;
WiiPointer * pointer[4] = { NULL, NULL, NULL, NULL };
GuiImage * bgImg = NULL;
GuiImageData * background = NULL;
GuiBGM * bgMusic = NULL;
GuiSound * btnSoundClick = NULL;
GuiSound * btnSoundOver = NULL;
HomebrewReceiver * Receiver = NULL;
SdPort * SD_Port = NULL;

extern u8 shutdown;
extern u8 reset;

int currentMenu = 0;
u8 mountMethod = 0;

static lwp_t guithread = LWP_THREAD_NULL;
static bool guiHalt = true;
static bool ExitRequested = false;

/****************************************************************************
 * ResumeGui
 *
 * Signals the GUI thread to start, and resumes the thread. This is called
 * after finishing the removal/insertion of new elements, and after initial
 * GUI setup.
 ***************************************************************************/
void ResumeGui()
{
	guiHalt = false;
	LWP_ResumeThread(guithread);
}

/****************************************************************************
 * HaltGui
 *
 * Signals the GUI thread to stop, and waits for GUI thread to stop
 * This is necessary whenever removing/inserting new elements into the GUI.
 * This eliminates the possibility that the GUI is in the middle of accessing
 * an element that is being changed.
 ***************************************************************************/
void HaltGui()
{
	if (guiHalt) return;
	guiHalt = true;
	
	//! wait for thread to finish
	while (!LWP_ThreadIsSuspended(guithread))
		usleep(100);
}

/****************************************************************************
 * UpdateGUI
 *
 * Primary thread to allow GUI to respond to state changes, and draws GUI
 ***************************************************************************/
static void * UpdateGUI(void *arg)
{
	int i;
	
	while (!ExitRequested)
	{
		if (guiHalt)
		{
			LWP_SuspendThread(guithread);
			continue;
		}
		
		if (shutdown)
			Sys_Shutdown();
		else if (reset)
			Sys_Reboot();
		
		UpdatePads();
		
		mainWindow->Draw();
		
		if (Settings.Tooltips && Theme::ShowTooltips && mainWindow->GetState() != STATE_DISABLED)
			mainWindow->DrawTooltip();
		
		//! Pointer modifies wpad data struct for easy implementation of "virtual pointer" with PAD-Sticks
		//! That is why it has to be called right before updating other gui elements with the triggers
		for (i = 3; i >= 0; i--)
			pointer[i]->Draw(&userInput[i]);
		
		for (i = 0; i < 4; i++)
			mainWindow->Update(&userInput[i]);
		
		Menu_Render();
		
		if (bgMusic) bgMusic->UpdateState();
	}

	ShutoffRumble();
	
	for (i = 5; i < 255; i += 10)
	{
		mainWindow->Draw();
		Menu_DrawRectangle(0, 0, screenwidth, screenheight, (GXColor) {0, 0, 0, i}, 1);
		Menu_Render();
	}

	mainWindow->RemoveAll();
	
	return NULL;
}

/****************************************************************************
 * InitGUIThread
 *
 * Startup GUI threads
 ***************************************************************************/
void InitGUIThreads()
{
	ExitRequested = false;
	
	if(guithread == LWP_THREAD_NULL)
		LWP_CreateThread(&guithread, UpdateGUI, NULL, NULL, 65536, LWP_PRIO_HIGHEST);
}

void ExitGUIThreads()
{
	ExitRequested = true;
	
	if(guithread != LWP_THREAD_NULL)
	{
		ResumeGui();
		LWP_JoinThread(guithread, NULL);
		guithread = LWP_THREAD_NULL;
	}
}

/****************************************************************************
 * MainMenu
 ***************************************************************************/
int MainMenu(int menu)
{
	currentMenu = menu;
	
	InitGUIThreads();
	
	InitProgressThread();
	InitNetworkThread();
	
	if (Settings.AutoConnect)
		ResumeNetworkThread();
	
	btnSoundClick = new GuiSound(Resources::GetFile("button_click.wav"), Resources::GetFileSize("button_click.wav"), Settings.SFXVolume);
	btnSoundOver = new GuiSound(Resources::GetFile("button_over.wav"), Resources::GetFileSize("button_over.wav"), Settings.SFXVolume);
	
	pointer[0] = new WiiPointer("player1_point.png");
	pointer[1] = new WiiPointer("player2_point.png");
	pointer[2] = new WiiPointer("player3_point.png");
	pointer[3] = new WiiPointer("player4_point.png");
	
	mainWindow = new GuiWindow(screenwidth, screenheight);
	
	background = Resources::GetImageData(Settings.Widescreen ? "menu_wbackground.png" : "menu_background.png");
	bgImg = new GuiImage(background);
	mainWindow->Append(bgImg);
	
	ResumeGui();
	
	bgMusic = new GuiBGM(Resources::GetFile("bg_music.ogg"), Resources::GetFileSize("bg_music.ogg"), Settings.MusicVolume);
	bgMusic->SetLoop(Settings.BgMusicLoop);
	bgMusic->Load(Settings.BgMusicPath);
	bgMusic->Play();
	
	if (startError == true)
	{
		WindowPrompt(tr("Error:"), tr("No Storage Device Found"), tr( "OK" ));
		currentMenu = MENU_EXIT;
	}
	
	Receiver = new HomebrewReceiver();
	
	////////////////////////////////////
	SD_Port = new SdPort();
	///////////////////////////////////
	
	SetWPADTimeout();
	
	Mii::Fix_DB();
	
	while (currentMenu != MENU_EXIT)
	{
		switch (currentMenu)
		{
			case MENU_SETTINGS:
				currentMenu = MenuSettings();
				break;
			case MENU_THEMEMENU:
				currentMenu = ThemeMenu::Run();
				break;
			case MENU_THEMEDOWNLOADER:
				currentMenu = ThemeDownloader::Run();
				break;
			case MENU_HOMEBREWBROWSE:
				currentMenu = HomebrewBrowser::Run();
				break;
			case MENU_SAVEGAME:
			default:
				currentMenu = MenuSaveGame();
				break;
		}
	}
	
	//! THIS SHOULD NEVER HAPPEN ANYMORE
	ExitApp();
	
	return -1;
}
