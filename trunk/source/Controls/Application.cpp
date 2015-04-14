/****************************************************************************
 * Copyright (C) 2009-2011 Dimok
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
#include "Application.h"
#include "../Themes/CTheme.h"
#include "../System/input.h"
#include "../System/sys.h"
#include "../System/video.h"
#include "../SoundOperations/MusicPlayer.h"
#include "../Settings/menus/SettingsMenu.h"
#include "../Prompts/ProgressWindow.h"
#include "../Prompts/ScreenSaver.hpp"
#include "../Prompts/HomeMenu.h"
#include "../Menu/MainMenu.h"
#include "../stdafx.h"

Application *Application::instance = NULL;
bool Application::exitApplication = false;
bool Application::screenSaverEnabled = true;
short Application::reloadTheme = 0;
bool Application::returnToTheme = true;
bool Application::reloadLanguage = false;

Application::Application()
	: GuiFrame(screenwidth, screenheight)
{
	
}

Application::~Application()
{
	RemoveAll();
	delete clock;
	delete bgImg;
	delete btnHome;

	for (int i = 0; i < 4; i++)
		delete pointer[i];

	//! remove still outstanding deletes without any dependencies
	for(u32 i = 0; i < deleteList.size(); i++)
		delete deleteList[i];

	LWP_MutexDestroy(m_mutex);
}

void Application::FadeOut()
{
	for(int i = 0; i <= 255; i += 15)
	{
		Draw();
		GXColor fadeoutColor = (GXColor){0, 0, 0, i};
		Menu_DrawRectangle(0, 0, 100.0f, screenwidth, screenheight, &fadeoutColor, false, true);
		Menu_Render();
	}
}

void Application::quit()
{
	FadeOut();
	
	exitApplication = true;
}

void Application::hide()
{
	RemoveAll();
}

void Application::show()
{
	Append(bgImg);
	Append(clock);
	Append(MusicPlayer::Instance());
	Append(ProgressWindow::Instance());
	
	Append(MainMenu::Instance());
}

void Application::exec()
{
	while(!exitApplication)
	{
		updateEvents();
	}

	ExitApp();
}

void Application::updateEvents()
{
	if(exitApplication)
		return;

	if(shutdown)
		Sys_Shutdown();
	else if(reset)
		Sys_Reboot();
	
	//! check if theme or lang reload is needed
	ReloadTheme();
	ReloadLanguage();

	//! first update the inputs
	UpdatePads();
	for (int i = 3; i >= 0; i--)
		pointer[i]->Update(&userInput[i]);
	
	//! update the gui elements with new inputs
	for (int i = 0; i < 4; i++)
	{
		if(!updateOnlyElement.empty())
			updateOnlyElement.back()->Update(&userInput[i]);
		else
			Update(&userInput[i]);
		
		//! always update the home menu, everywhere
		btnHome->Update(&userInput[i]);
	}

	//! render everything
	Draw();
	for (int i = 3; i >= 0; i--)
		pointer[i]->Draw();
	
	Menu_Render();

	//! delete elements that were queued for delete after the rendering is done
	if(!deleteList.empty())
		ProcessDeleteQueue();
	
	//! ScreenSaver
	if(screenSaverEnabled && Settings.Screensaver != 0 && ControlActivityTimeout())
		ShowScreenSaver();
}

void Application::ProcessDeleteQueue(void)
{
	LWP_MutexLock(m_mutex);
	for(u32 i = 0; i < deleteList.size(); ++i)
	{
		GuiElement *parent = deleteList[i]->GetParent();
		//! Only remove elements when they are currently not animated
		//! otherwise wait till animation is finished
		if(!parent || !deleteList[i]->IsAnimated())
		{
			//! since i am lazy and don't remove them usually, this is where i remove it
			if(parent)
			{
				GuiFrame *parentFrame = dynamic_cast<GuiFrame *>(parent);
				if(parentFrame)
					parentFrame->Remove(deleteList[i]);
			}
			
			UnsetUpdateOnly(deleteList[i]);
			delete deleteList[i];
			deleteList.erase(deleteList.begin() + i);
			i--;
		}
	}
	LWP_MutexUnlock(m_mutex);
}

void Application::PushForDelete(GuiElement *e)
{
	if(!e)
		return;

	//! elements that are queued for delete are only rendered and not updated with buttons
	e->SetState(STATE_DISABLED);

	LWP_MutexLock(m_mutex);
	deleteList.push_back(e);
	LWP_MutexUnlock(m_mutex);
}

/***********************************************************
 * Main application initialization.
 *
 * Called only once at the beginning of the application.
 ***********************************************************/
void Application::init(void)
{
	background = Resources::GetImageData(Settings.Widescreen ? "menu_wbackground.png" : "menu_background.png");
	bgImg = new GuiImage(background);
	
	//! Setup WiiMote Pointers
	pointer[0] = new WiiPointer("player1_point.png");
	pointer[1] = new WiiPointer("player2_point.png");
	pointer[2] = new WiiPointer("player3_point.png");
	pointer[3] = new WiiPointer("player4_point.png");
	
	LWP_MutexInit(&m_mutex, true);

	//! Setup the music player
	MusicPlayer::Instance()->SetVolume(Settings.MusicVolume);
	MusicPlayer::Instance()->SetLoop(Settings.BGMLoopMode);
	MusicPlayer::Instance()->SetAlignment(ALIGN_RIGHT | ALIGN_TOP);
	MusicPlayer::Instance()->SetPosition(30, 230);
	
	//! setup the home menu button
	trigHome.SetButtonOnlyTrigger(-1, WiiControls.HomeButton | ClassicControls.HomeButton << 16, GCControls.HomeButton);

	btnHome = new GuiButton(0, 0);
	btnHome->SetTrigger(&trigHome);
	btnHome->Clicked.connect(this, &Application::OnHomeButtonClick);
	
	clock = new Clock();
	
	ProgressWindow::Instance();
	MainMenu::Instance()->show();
}

void Application::OnHomeButtonClick(GuiButton *sender UNUSED, int pointer UNUSED, const POINT &p3 UNUSED)
{
	btnHome->SetClickable(false);

	HomeMenu *homeMenu = new HomeMenu();
	homeMenu->Closing.connect(this, &Application::OnHomeMenuClosing);
	this->Append(homeMenu);
}

void Application::OnHomeMenuClosing(GuiFrame *menu UNUSED)
{
	btnHome->SetClickable(true);
}

void Application::ShowScreenSaver()
{
	btnHome->SetClickable(false);

	ScreenSaver *saver = new ScreenSaver();
	saver->Closing.connect(this, &Application::OnHomeMenuClosing);
	this->Append(saver);
}

void Application::ReloadTheme()
{
	if(!reloadTheme)
		return;
	
	bool defaultTheme = ((reloadTheme == 1) ? true : false);
	reloadTheme = 0;
	
	Theme::Reload(defaultTheme, returnToTheme ? SettingsMenu::THEME : -1);
	
	returnToTheme = true;
}

void Application::ReloadLanguage()
{
	if(!reloadLanguage)
		return;
	
	reloadLanguage = false;
	
	Settings.LoadLanguage(Settings.LanguageFile);
	Theme::Reload(false, SettingsMenu::GUI);
}
