/***************************************************************************
 * Copyright (C) 2009
 * by r-win & Dimok
 * Modified by Dj_Skual
 * for SaveGame Manager GX 2011
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
 * MainWindow.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/

#include <unistd.h>

#include "prompts/promptwindows.h"
#include "file/fileops.h"
#include "boot_homebrew/BootHomebrew.h"
#include "network/networkops.h"
#include "manage_all/ManageAllProgress.h"
#include "manage_all/ManageMiiProgress.h"
#include "settings/settingsprompts.h"
#include "settings/settings.h"
#include "saves/manage/ManageProgress.h"
#include "saves/wii_list.h"
#include "mii/mii_list.h"
#include "system/video.h"
#include "system/sys.h"
#include "system/Resources.h"
#include "menu/MainWindow.h"
#include "menu/menu.h"
#include "main.h"

extern u8 shutdown;
extern u8 reset;

MainWindow *MainWindow::instance = NULL;

MainWindow::MainWindow()
	: GuiWindow(screenwidth, screenheight)
{
	guihalt = true;
	exitApplication = false;
	guithread = LWP_THREAD_NULL;
	boothomebrew = false;
	
	//!Initialize main GUI handling thread
	LWP_CreateThread (&guithread, UpdateGUI, this, NULL, 0, 70);

	//!Initalize the foldersize thread
	InitGetSizeThread();
	StopSizeGain();

    //!Initalize the manageprogress thread
	InitManageProgressThread();
	StopManageProgress();
	
	//!Initalize the manageall saves progress thread
	InitManageAllProgressThread();
	StopManageAllProgress();
	
	//!Initalize the manageall mii progress thread
	InitManageMiiProgressThread();
	StopManageMiiProgress();
	
	//!Initialize network thread if selected
    InitNetworkThread();
    HaltNetworkThread();
	
	if (cfg.AutoConnect)
        ResumeNetworkThread();
	
	pointer[0] = Resources::GetImageData(player1_point_png, player1_point_png_size);
	pointer[1] = Resources::GetImageData(player2_point_png, player2_point_png_size);
	pointer[2] = Resources::GetImageData(player3_point_png, player3_point_png_size);
	pointer[3] = Resources::GetImageData(player4_point_png, player4_point_png_size);

	int cios = IOS_GetVersion();
	int rev = IOS_GetRevision();
	
	gprintf("\ncIOS %i (Rev %i) loaded\n", cios, rev);
	
	if (cios < 249 || cios > 250)
	{
		WindowPrompt(tr("Error"), tr("You need to install a cIOS 249 from Waninkoko to use this app."), tr("Exit"));
		exit(0);
	}
	else if (cios >= 249 && cios <= 250)
	{
		if (rev < 9 || rev > 65535)
		{
			WindowPrompt(tr("Error"), tr("You need to install a cIOS 249 rev 9 or higher to use this app."), tr("Exit"));
			exit(0);
		}
	}

	int mii = CreateMiiList();
	int save = CreateWiiSaveList();
	
	/* Get the MiiList struct */
	if (mii < 0)
		WindowPrompt(tr("Error"), tr("Unable to create Mii list."), tr("OK"));
	else if (mii == 0)
		WindowPrompt(0, tr("No Mii found in Wii memory."), tr("OK"));
	
	/* Get the SaveList struct */
	if (save < 0)
		WindowPrompt(tr("Error"), tr("Unable to create Wii Saves list."), tr("OK"));
	else if (save == 0)
		WindowPrompt(0, tr("No SaveGame found in Wii."), tr("OK"));
	
	bg = Resources::GetImageData(cfg.Widescreen? wbackground_png : background_png,
								 cfg.Widescreen? wbackground_png_size : background_png_size);
	bgImg = new GuiImage(bg);
	Append(bgImg);
}

MainWindow::~MainWindow()
{
	Quit();
	RemoveAll();

	for (int i = 0; i < 4; i++)
	{
	    Resources::Remove(pointer[i]);
	}
	
	delete bgImg;
	Resources::Remove(bg);
}

MainWindow * MainWindow::Instance()
{
	if (instance == NULL)
	{
		instance = new MainWindow();
	}
	return instance;
}

void MainWindow::DestroyInstance()
{
    if(instance)
    {
        delete instance;
    }
    instance = NULL;
}

bool MainWindow::GetBootHomebrew()
{
	return boothomebrew;
}

void MainWindow::SetBootHomebrew(bool boot)
{
	boothomebrew = boot;
}

void MainWindow::Quit()
{
	if (!exitApplication)
	{
		// Exit the application if this function is called...
		exitApplication = true;

		ResumeGui(); // Resume the gui, or the thread won't exit
		LWP_JoinThread(guithread, NULL);
		guithread = LWP_THREAD_NULL;

		// Fade out...
		for(int i = 0; i <= 255; i += 15)
		{
			Draw();
            Menu_DrawRectangle(0,0,screenwidth,screenheight,(GXColor){0, 0, 0, i},1);
			Menu_Render();
		}
	}
}

void MainWindow::Show()
{
    ResumeGui();
	int currentMenu = MENU_SAVEGAME;

	while(currentMenu != MENU_EXIT)
	{
	    switch(currentMenu)
        {
            case MENU_SETTINGS:
				currentMenu = MenuSettings();
				break;
			case MENU_SAVEGAME:
				currentMenu = SaveGameBrowser();
				break;
			default:
				currentMenu = SaveGameBrowser();
				break;
        }
	}

	ResumeGui();

	if(boothomebrew)
        BootHomebrew();
	
	AppExit();
}

void MainWindow::ResumeGui()
{
	guihalt = false;
	LWP_ResumeThread (guithread);
}

void MainWindow::HaltGui()
{
	guihalt = true;

	while(!LWP_ThreadIsSuspended (guithread))
		usleep(THREAD_SLEEP);
}

void *MainWindow::UpdateGUI(void *arg)
{
	((MainWindow *) arg)->InternalUpdateGUI();
	return NULL;
}

void MainWindow::InternalUpdateGUI()
{
	while (!exitApplication)
	{
		if(guihalt)
		{
			LWP_SuspendThread(guithread);
			usleep(THREAD_SLEEP);
			continue;
		}

        if (shutdown == 1)
			Sys_Shutdown();

		else if (reset == 1)
			Sys_Reboot();
		
		UpdatePads();
		Draw();

		if (cfg.Tooltips && GetState() != STATE_DISABLED)
			DrawTooltip();
		
		for (int i = 3; i >= 0; i--)
		{
			if (userInput[i].wpad->ir.valid)
			{
				Menu_DrawImg(userInput[i].wpad->ir.x-48,
							userInput[i].wpad->ir.y-48,
							96,
							96,
							pointer[i]->GetImage(),
							userInput[i].wpad->ir.angle,
							cfg.Widescreen? 0.8 : 1,
							1,
							255);
			}
			if (cfg.Rumble)
				DoRumble(i);
		}

		Menu_Render();

		for (int i = 0; i < 4; i++)
		{
			Update(&userInput[i]);
		}
		
		if(bgMusic) bgMusic->UpdateState();
		
		switch (cfg.Screensaver)
		{
			case 1:
				WPad_SetIdleTime(60);
				break;
			case 2:
				WPad_SetIdleTime(180);
				break;
			case 3:
				WPad_SetIdleTime(300);
				break;
			case 4:
				WPad_SetIdleTime(600);
				break;
			case 5:
				WPad_SetIdleTime(1200);
				break;
			case 6:
				WPad_SetIdleTime(1800);
				break;
			case 7:
				WPad_SetIdleTime(3600);
				break;
		}
	}
}
