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
#include "Explorer.h"
#include "MainMenu.h"
#include "../FileOperations/fileops.h"
#include "../FileOperations/FileStartUp.h"
#include "../Files/IconFileBrowser.hpp"
#include "../Files/ListFileBrowser.hpp"
#include "../Controls/Application.h"
#include "../Themes/CTheme.h"
#include "../Prompts/PromptWindows.h"
#include "../Settings/CSettings.h"
#include "../System/sys.h"

Explorer::Explorer(GuiFrame *p, std::string path)
	: GuiFrame(0, 0, p)
{
	this->Init();
	if(!path.empty())
		this->LoadPath(path);
}

Explorer::~Explorer()
{
	RemoveAll();

	Resources::Remove(Background);
	
	delete BackgroundImg;
	
	delete BackInDirBtn;

	delete trigA;
	delete trigBackInDir;

	if(fileBrowser != curBrowser)
		delete fileBrowser;

	delete curBrowser;
	delete guiBrowser;
}

void Explorer::Init()
{
	guiBrowser = NULL;
	curBrowser = NULL;

	fileBrowser = new FileBrowser();
	curBrowser = fileBrowser;

	SetAlignment(ALIGN_CENTER | ALIGN_TOP);
	SetPosition(0, 50);

	trigA = new SimpleGuiTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	trigBackInDir = new GuiTrigger();
	trigBackInDir->SetButtonOnlyTrigger(-1, WiiControls.UpInDirectory | ClassicControls.UpInDirectory << 16, GCControls.UpInDirectory);

	Background = Resources::GetImageData("browser.png");
	width = Background->GetWidth();
	height = Background->GetHeight();
	BackgroundImg = new GuiImage(Background);

	guiBrowserType = Settings.BrowserMode;

	if(guiBrowserType == ICONBROWSER)
		guiBrowser = new IconFileBrowser(curBrowser, width, height);
	else
		guiBrowser = new ListFileBrowser(curBrowser, width, height);

	guiBrowser->Clicked.connect(this, &Explorer::OnBrowserChanges);

	BackInDirBtn = new GuiButton(0, 0);
	BackInDirBtn->SetTrigger(trigBackInDir);
	BackInDirBtn->Clicked.connect(this, &Explorer::BackInDirectory);

	Append(BackgroundImg);
	Append(BackInDirBtn);
	Append(guiBrowser);
	
	show();
}

void Explorer::show()
{
	SetEffect(EFFECT_FADE, 50);

	if(parentElement)
		((GuiFrame *) parentElement)->Append(this);

	if(guiBrowserType != Settings.BrowserMode)
	{
		Remove(guiBrowser);
		delete guiBrowser;
		guiBrowserType = Settings.BrowserMode;
		
		if(guiBrowserType == ICONBROWSER)
			guiBrowser = new IconFileBrowser(curBrowser, width, height);
		else
			guiBrowser = new ListFileBrowser(curBrowser, width, height);
		
		guiBrowser->Clicked.connect(this, &Explorer::OnBrowserChanges);
		Append(guiBrowser);
	}
}

void Explorer::hide()
{
	if(!Application::isClosing())
	{
		SetEffect(EFFECT_FADE, -50);
		while(this->GetEffect() > 0)
			Application::Instance()->updateEvents();
	}

	if(parentElement)
		((GuiFrame *) parentElement)->Remove(this);
}

int Explorer::LoadPath(std::string path)
{
	int filecount = curBrowser->BrowsePath(path);
	
	//curBrowser->ResetMarker();
	Settings.LastUsedPath.assign(curBrowser->GetCurrentPath());
	return filecount;
}

void Explorer::OnBrowserChanges(int index UNUSED)
{
	if(MainMenu::Instance()->SearchWindowDisplayed())
		return;
	
	//! check corresponding browser entry
	if(curBrowser->IsCurrentDir())
	{
		MainMenu::Instance()->SetFilterOn(false);
		TaskBar::Instance()->ResetSearchBtn();
		int result = curBrowser->ChangeDirectory();
		if(result >= 0)
		{
			guiBrowser->SetSelected(0);
			curBrowser->SetPageIndex(0);
			//curBrowser->ResetMarker();
			guiBrowser->Refresh();
			if(fileBrowser == curBrowser)
				Settings.LastUsedPath.assign(curBrowser->GetCurrentPath());
		}
		/*else if(result == CLOSE_ARCHIVE && fileBrowser != curBrowser)
		{
			delete curBrowser;
			curBrowser = fileBrowser;
			guiBrowser->SetBrowser(curBrowser);
			guiBrowser->Refresh();
		}*/
		else
		{
			ShowError(tr("Can't browse that path."));
		}
	}
	else
	{
		int result = 0;
		
		SetState(STATE_DISABLED);
		
		result = curBrowser->ExecuteFile(curBrowser->GetCurrentSelectedFilepath());
		
		SetState(STATE_DEFAULT);
		
		if(result == REFRESH_BROWSER || result == RELOAD_BROWSER)
			guiBrowser->Refresh();
		/*else if(result == ARCHIVE)
		{
			if(fileBrowser != curBrowser)
				delete curBrowser;
			
			curBrowser = new ArchiveBrowser(curBrowser->GetCurrentSelectedFilepath());
			guiBrowser->SetBrowser(curBrowser);
		}*/
	}
}

void Explorer::BackInDirectory(GuiButton *sender, int pointer UNUSED, const POINT &p UNUSED)
{
	if(MainMenu::Instance()->SearchWindowDisplayed())
		return;
	
	MainMenu::Instance()->SetFilterOn(false);
	TaskBar::Instance()->ResetSearchBtn();
	curBrowser->BackInDirectory();
	guiBrowser->SetSelected(0);
	curBrowser->SetPageIndex(0);
	//curBrowser->ResetMarker();
	guiBrowser->Refresh();

	sender->ResetState();
}

void Explorer::Refresh()
{
	guiBrowser->SetSelected(0);
	curBrowser->SetPageIndex(0);
	//curBrowser->ResetMarker();
	guiBrowser->Refresh();
}
