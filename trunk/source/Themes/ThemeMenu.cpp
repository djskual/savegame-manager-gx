/***************************************************************************
 * Copyright (C) 2012 Dj_Skual
 * for SaveGame Manager GX 2012
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
 * ThemeMenu.cpp
 *
 ***************************************************************************/
#include "ThemeMenu.h"
#include "../Controls/Application.h"

ThemeMenu::ThemeMenu(int Type)
	: GuiFrame(screenwidth, screenheight)
{
	type = Type;
	
	Downloader = NULL;
	//Loader = NULL;
	
	BtnSoundClick = Resources::GetSound("button_click.wav");
	BtnSoundOver = Resources::GetSound("button_over.wav");
	BtnSoundClick->SetVolume(Settings.SFXVolume);
	BtnSoundOver->SetVolume(Settings.SFXVolume);
	
	trigA.SetSimpleTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	trigB.SetButtonOnlyTrigger(-1, WiiControls.BackButton | ClassicControls.BackButton << 16, GCControls.BackButton);
	
	BgImgData = Resources::GetImageData("settings_background.png");
	BgImg = new GuiImage(BgImgData);
	
	Title = new GuiText((type == DOWNLOADER) ? tr("Theme Downloader") : tr("Theme Menu"), 28, thColor("r=0 g=0 b=0 a=255 - settings menu title text color"));
	Title->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	Title->SetPosition(170, 38);
	Title->SetMaxWidth(310, SCROLL_HORIZONTAL);
	
	BackBtnTxt = new GuiText(tr( "Back" ), 22, thColor("r=0 g=0 b=0 a=255 - settings menu back button text color"));
	BackBtn = new PictureButton("settings_menu_button.png", "settings_menu_button.png", BtnSoundClick, BtnSoundOver);
	BackBtn->SetAlignment(thAlign("center - settings menu back button align hor") | thAlign("top - settings menu back button align ver"));
	BackBtn->SetPosition(thInt("-195 - settings menu back button pos x"), thInt("400 - settings menu back button pos y"));
	BackBtn->SetLabel(BackBtnTxt);
	BackBtn->SetTrigger(&trigA);
	BackBtn->SetTrigger(&trigB);
	BackBtn->Clicked.connect(this, &ThemeMenu::OnBackButtonClick);
	
	Append(BgImg);
	Append(Title);
	Append(BackBtn);
	
	SetEffect(EFFECT_FADE, 20);
	
	Application::Instance()->SetUpdateOnly(this);
	Application::Instance()->Append(this);
	while(BackBtn->IsAnimated())
		Application::Instance()->updateEvents();
	
	if (type == DOWNLOADER)
	{
		Downloader = new ThemeDownloader();
		Append(Downloader);
		Downloader->show();
	}
	else
	{
		//Loader = new ThemeLoader();
		//Append(Loader);
		//Loader->show();
	}
}

ThemeMenu::~ThemeMenu()
{
	RemoveAll();
	
	if(Downloader)
		delete Downloader;
	//if(Loader)
	//	delete Loader;
	
	delete Title;
	
	delete BgImg;
	Resources::Remove(BgImgData);
	
	delete BackBtnTxt;
	delete BackBtn;
	
	Resources::Remove(BtnSoundClick);
	Resources::Remove(BtnSoundOver);
}

void ThemeMenu::quit()
{
	if(Downloader)
		Downloader->FadeOut();
	//else
	//	Loader->FadeOut();
	
	SetEffect(EFFECT_FADE, -20);
	while(this->IsAnimated())
		Application::Instance()->updateEvents();
	
	Application::Instance()->PushForDelete(this);
	Application::Instance()->updateEvents();
	
	Closed();
}

void ThemeMenu::OnBackButtonClick(GuiButton *sender UNUSED, int pointer UNUSED, const POINT &p UNUSED)
{
	quit();
}
