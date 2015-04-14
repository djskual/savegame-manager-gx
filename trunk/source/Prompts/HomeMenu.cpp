#include <unistd.h>

#include "../SoundOperations/MusicPlayer.h"
#include "../Themes/CTheme.h"
#include "../Settings/CSettings.h"
#include "../System/sys.h"
#include "../Controls/Application.h"
#include "PromptWindows.h"
#include "HomeMenu.h"

HomeMenu::HomeMenu()
	: GuiFrame(screenwidth, screenheight)
{
	settingsActive = false;
	
	this->SetPosition(0, 0);

	Application::Instance()->SetUpdateOnly(this);
	this->DimBackground(true);
	
	trigA.SetSimpleTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	trigB.SetButtonOnlyTrigger(-1, WiiControls.BackButton | ClassicControls.BackButton << 16, GCControls.BackButton);
	trigHome.SetButtonOnlyTrigger(-1, WiiControls.HomeButton | ClassicControls.HomeButton << 16, GCControls.HomeButton);
	
	HomeInSnd = Resources::GetSound("menuin.ogg");
	HomeOutSnd = Resources::GetSound("menuout.ogg");
	BtnSoundClick = Resources::GetSound("button_click.wav");
	BtnSoundOver = Resources::GetSound("button_over.wav");
	HomeInSnd->SetVolume(Settings.SFXVolume);
	HomeOutSnd->SetVolume(Settings.SFXVolume);
	BtnSoundClick->SetVolume(Settings.SFXVolume);
	BtnSoundOver->SetVolume(Settings.SFXVolume);
	
	TitleText = new GuiText(tr("HOME Menu"), thInt("40 - homemenu title text size"), thColor("r=255 g=255 b=255 a=255 - homemenu main text color"));
	TitleText->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	TitleText->SetPosition(30, 40);
	
	TopBtn = new PictureButton("homemenu_top.png", "homemenu_top_over.png", BtnSoundClick, BtnSoundOver);
	TopBtn->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	TopBtn->SetLabel(TitleText);
	TopBtn->SetTrigger(&trigA);
	TopBtn->SetTrigger(&trigB);
	TopBtn->SetTrigger(&trigHome);
	TopBtn->ResetEffects();
	TopBtn->SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_IN, 50);
	TopBtn->Clicked.connect(this, &HomeMenu::OnButtonClick);
	
	SettingsText = new GuiText(tr("Settings"), thInt("30 - homemenu main text size"), thColor("r=255 g=255 b=255 a=255 - homemenu main text color"));
	SettingsText->SetAlignment(ALIGN_CENTER | ALIGN_TOP);
	SettingsText->SetPosition(20, 50);
	
	BottomBtn = new PictureButton("homemenu_bottom.png", "homemenu_bottom_over.png", BtnSoundClick, BtnSoundOver);
	BottomBtn->SetAlignment(ALIGN_LEFT | ALIGN_BOTTOM);
	BottomBtn->SetLabel(SettingsText);
	BottomBtn->SetTrigger(&trigA);
	BottomBtn->ResetEffects();
	BottomBtn->SetEffect(EFFECT_SLIDE_BOTTOM | EFFECT_SLIDE_IN, 50);
	BottomBtn->Clicked.connect(this, &HomeMenu::OnButtonClick);
	BottomBtn->StateChanged.connect(this, &HomeMenu::OnStateChange);
	
	CloseBtnText = new GuiText(tr("Close"), thInt("28 - homemenu close text size"), thColor("r=0 g=0 b=0 a=255 - homemenu close text color"));

	CloseBtn = new PictureButton("homemenu_close.png", NULL, NULL, NULL);
	CloseBtn->SetAlignment(ALIGN_RIGHT | ALIGN_TOP);
	CloseBtn->SetPosition(-20, 30);
	CloseBtn->SetLabel(CloseBtnText);
	CloseBtn->SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_IN, 50);

	LeftBtnText = new GuiText(tr("Homebrew Channel"), thInt("26 - homemenu main button text size"), thColor("r=0 g=0 b=0 a=255 - homemenu main button text color"));
	if (Settings.HomeMenu == HOME_MENU_SYSTEM)
		LeftBtnText->SetText(tr("Wii Menu"));
	else if (Settings.HomeMenu == HOME_MENU_FULL)
		LeftBtnText->SetText(tr("Exit"));
	
	LeftBtn = new PictureButton("homemenu_button.png", NULL, BtnSoundClick, BtnSoundOver);
	LeftBtn->SetAlignment(ALIGN_CENTER | ALIGN_MIDDLE);
	LeftBtn->SetPosition(-140, 0);
	LeftBtn->SetLabel(LeftBtnText);
	LeftBtn->SetTrigger(&trigA);
	LeftBtn->SetEffectGrow();
	LeftBtn->SetEffect(EFFECT_FADE, 50);
	LeftBtn->Clicked.connect(this, &HomeMenu::OnButtonClick);

	RightBtnText = new GuiText(tr("Wii Menu"), thInt("26 - homemenu main button text size"), thColor("r=0 g=0 b=0 a=255 - homemenu main button text color"));
	if (Settings.HomeMenu == HOME_MENU_SYSTEM)
		RightBtnText->SetText(tr("Reset"));
	else if (Settings.HomeMenu == HOME_MENU_FULL)
		RightBtnText->SetText(tr("Shutdown Wii"));
	
	RightBtn = new PictureButton("homemenu_button.png", NULL, BtnSoundClick, BtnSoundOver);
	RightBtn->SetAlignment(ALIGN_CENTER | ALIGN_MIDDLE);
	RightBtn->SetPosition(140, 0);
	RightBtn->SetLabel(RightBtnText);
	RightBtn->SetTrigger(&trigA);
	RightBtn->SetEffectGrow();
	RightBtn->SetEffect(EFFECT_FADE, 50);
	RightBtn->Clicked.connect(this, &HomeMenu::OnButtonClick);

	WiimoteData = Resources::GetImageData("homemenu_wiimote.png");
	WiimoteImg = new GuiImage(WiimoteData);
	WiimoteImg->SetAlignment(ALIGN_LEFT | ALIGN_BOTTOM);
	WiimoteImg->SetPosition(45, 232);
	WiimoteImg->SetEffect(EFFECT_SLIDE_BOTTOM | EFFECT_SLIDE_IN, 50);
	
	SettingsBgData = Resources::GetImageData("homemenu_settings_bg.png");
	SettingsBgImg = new GuiImage(SettingsBgData);
	SettingsBgImg->SetAlignment(ALIGN_RIGHT | ALIGN_TOP);
	SettingsBgImg->SetPosition(133-screenwidth, 48);
	SettingsBgImg->SetBounds(iRect(133, 0, screenwidth, screenheight));
	
	SettingsBtnData = Resources::GetImageData("home_settings_button.png");
	SettingsBtnActiveData = Resources::GetImageData("home_settings_button_over.png");
	SettingsBtnImg = new GuiImage(SettingsBtnData);
	SettingsBtnActiveImg = new GuiImage(SettingsBtnActiveData);
	
	RumbleText = new GuiText(tr("Rumble"), thInt("30 - homemenu settings text size"), thColor("r=255 g=255 b=255 a=255 - homemenu settings text color"));
	RumbleText->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	RumbleText->SetPosition(153, 82);
	
	RumbleOnText = new GuiText(tr("Yes"), thInt("30 - homemenu settings button text size"), thColor("r=0 g=0 b=0 a=255 - homemenu settings button text color"));
	RumbleOnText->SetAlignment(ALIGN_CENTER | ALIGN_MIDDLE);
	RumbleOffText = new GuiText(tr("No"), thInt("30 - homemenu settings button text size"), thColor("r=0 g=0 b=0 a=255 - homemenu settings button text color"));
	RumbleOffText->SetAlignment(ALIGN_CENTER | ALIGN_MIDDLE);
	
	RumbleOnBtn = new GuiButton(SettingsBtnImg->GetWidth(), SettingsBtnImg->GetHeight());
	RumbleOnBtn->SetAlignment(ALIGN_RIGHT | ALIGN_TOP);
	RumbleOnBtn->SetPosition(-207, 59);
	RumbleOnBtn->SetLabel(RumbleOnText);
	RumbleOnBtn->SetSoundClick(BtnSoundClick);
	RumbleOnBtn->SetSoundOver(BtnSoundOver);
	RumbleOnBtn->SetTrigger(&trigA);
	RumbleOnBtn->SetEffectGrow();
	RumbleOnBtn->Clicked.connect(this, &HomeMenu::OnButtonClick);

	RumbleOffBtn = new GuiButton(SettingsBtnImg->GetWidth(), SettingsBtnImg->GetHeight());
	RumbleOffBtn->SetAlignment(ALIGN_RIGHT | ALIGN_TOP);
	RumbleOffBtn->SetPosition(-62, 59);
	RumbleOffBtn->SetLabel(RumbleOffText);
	RumbleOffBtn->SetSoundClick(BtnSoundClick);
	RumbleOffBtn->SetSoundOver(BtnSoundOver);
	RumbleOffBtn->SetTrigger(&trigA);
	RumbleOffBtn->SetEffectGrow();
	RumbleOffBtn->Clicked.connect(this, &HomeMenu::OnButtonClick);

	Append(BottomBtn);
	Append(TopBtn);
	Append(CloseBtn);
	Append(LeftBtn);
	Append(RightBtn);
	Append(SettingsBgImg);
	Append(WiimoteImg);
	
	BatteryBarDataRed = Resources::GetImageData("homemenu_battery_bar_red.png");
	BatteryBarDataWh = Resources::GetImageData("homemenu_battery_bar_white.png");
	BatteryDataRed = Resources::GetImageData("homemenu_battery_red.png");
	BatteryDataWh = Resources::GetImageData("homemenu_battery_white.png");
	
	char player[3];
	for (int i = 0; i < 4; i++)
	{
		sprintf(player, "P%d", i + 1);
		
		PlayerText[i] = new GuiText(player, thInt("22 - homemenu battery text size"), thColor("r=255 g=255 b=255 a=255 - homemenu battery text color"));
		PlayerText[i]->SetAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
		PlayerText[i]->SetAlpha(100);
		
		BatteryBarImg[i] = new GuiImage(BatteryBarDataWh);
		BatteryBarImg[i]->SetAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
		BatteryBarImg[i]->SetPosition(33, 0);
		
		BatteryImg[i] = new GuiImage(BatteryDataWh);
		BatteryImg[i]->SetAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
		BatteryImg[i]->SetPosition(36, 0);
		
		BatteryBtn[i] = new GuiButton(0,0);
		BatteryBtn[i]->SetLabel(PlayerText[i]);
		BatteryBtn[i]->SetImage(BatteryBarImg[i]);
		BatteryBtn[i]->SetIcon(BatteryImg[i]);
		BatteryBtn[i]->SetAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
		BatteryBtn[i]->SetPosition(180 + i*108, 149);
		BatteryBtn[i]->SetRumble(false);
		BatteryBtn[i]->SetEffect(EFFECT_SLIDE_BOTTOM | EFFECT_SLIDE_IN, 50);
		BatteryBtn[i]->SetAlpha(133);
		
		Append(BatteryBtn[i]);
	}
	
	if(!MusicPlayer::Instance()->IsStopped())
		MusicPlayer::Instance()->Pause();

	HomeInSnd->Play();
}

HomeMenu::~HomeMenu()
{
	RemoveAll();
	
	HomeOutSnd->Stop();
	
	if(!MusicPlayer::Instance()->IsStopped())
		MusicPlayer::Instance()->Resume();

	delete RightBtn;
	delete LeftBtn;
	delete CloseBtn;
	delete TopBtn;
	delete BottomBtn;
	delete RumbleOnBtn;
	delete RumbleOffBtn;

	delete WiimoteImg;
	delete SettingsBtnImg;
	delete SettingsBtnActiveImg;
	delete SettingsBgImg;
	
	Resources::Remove(WiimoteData);
	Resources::Remove(SettingsBgData);
	Resources::Remove(SettingsBtnData);
	Resources::Remove(SettingsBtnActiveData);
	
	delete TitleText;
	delete SettingsText;
	delete CloseBtnText;
	delete LeftBtnText;
	delete RightBtnText;
	delete RumbleText;
	delete RumbleOnText;
	delete RumbleOffText;
	
	for (int i = 0; i < 4; i++)
	{
		delete PlayerText[i];
		delete BatteryBarImg[i];
		delete BatteryImg[i];
		delete BatteryBtn[i];
	}
	
	Resources::Remove(BatteryBarDataRed);
	Resources::Remove(BatteryBarDataWh);
	Resources::Remove(BatteryDataRed);
	Resources::Remove(BatteryDataWh);
	
	Resources::Remove(HomeInSnd);
	Resources::Remove(HomeOutSnd);
	Resources::Remove(BtnSoundClick);
	Resources::Remove(BtnSoundOver);
}

void HomeMenu::quit()
{
	HomeInSnd->Stop();
	HomeOutSnd->Play();
	
	TopBtn->SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_OUT, 50);
	CloseBtn->SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_OUT, 50);
	LeftBtn->SetEffect(EFFECT_FADE, -50);
	RightBtn->SetEffect(EFFECT_FADE, -50);
	BottomBtn->SetEffect(EFFECT_SLIDE_BOTTOM | EFFECT_SLIDE_OUT, 50);
	WiimoteImg->SetEffect(EFFECT_SLIDE_BOTTOM | EFFECT_SLIDE_OUT, 50);
	
	for (int i = 0; i < 4; i++)
		BatteryBtn[i]->SetEffect(EFFECT_SLIDE_BOTTOM | EFFECT_SLIDE_OUT, 50);
	
	while(LeftBtn->IsAnimated()) Application::Instance()->updateEvents();
	
	this->DimBackground(false);
	this->Close();
}

bool HomeMenu::IsAnimated()
{
	return (GuiFrame::IsAnimated() ||
			TopBtn->IsAnimated() ||
			LeftBtn->IsAnimated() ||
			WiimoteImg->IsAnimated() ||
			SettingsBgImg->IsAnimated() ||
			HomeOutSnd->IsPlaying());
}

void HomeMenu::OnStateChange(GuiElement *sender UNUSED, int state, int stateChan UNUSED)
{
	if(settingsActive)
		return;
	
	if(state == STATE_SELECTED)
		WiimoteImg->SetPosition(WiimoteImg->GetLeft(), 210);
	else
		WiimoteImg->SetPosition(WiimoteImg->GetLeft(), 232);
}

void HomeMenu::OnButtonClick(GuiButton *sender, int pointer UNUSED, const POINT &p UNUSED)
{
	if(this->IsAnimated())
		return;
	
	if(sender == TopBtn)
	{
		if(!settingsActive)
			this->quit();
		else
			SettingsHide();
	}
	else if (sender == LeftBtn)
	{
		if (Settings.HomeMenu == HOME_MENU_SYSTEM)
			Sys_LoadMenu();
		else if (Settings.HomeMenu == HOME_MENU_DEFAULT)
			Sys_LoadHBC();
		else if (Settings.HomeMenu == HOME_MENU_FULL)
		{
			int ret = WindowPrompt(tr( "Exit to where?" ), 0, tr( "Homebrew Channel" ), tr( "Wii Menu" ), tr( "Reset" ), tr( "Cancel" ));
			if (ret == 1)
				Sys_LoadHBC();
			else if(ret == 2)
				Sys_LoadMenu();
			else if(ret == 3)
				RebootApp();
		}
	}
	else if (sender == RightBtn)
	{
		if (Settings.HomeMenu == HOME_MENU_SYSTEM)
			RebootApp();
		else if (Settings.HomeMenu == HOME_MENU_DEFAULT)
			Sys_LoadMenu();
		else if (Settings.HomeMenu == HOME_MENU_FULL)
		{
			int ret = WindowPrompt(tr( "How to Shutdown?" ), 0, tr( "Full shutdown" ), tr( "Standby" ), tr("Cancel"));
			if (ret == 1)
				Sys_ShutdownToStandby();
			else if (ret == 2)
				Sys_ShutdownToIdle();
		}
	}
	else if(sender == BottomBtn)
	{
		if(!settingsActive)
			SettingsShow();
		else
			SettingsHide();
	}
	else if(sender == RumbleOnBtn || sender == RumbleOffBtn)
	{
		Settings.Rumble = !Settings.Rumble;
		SettingsBtnUpdate();
	}
}

void HomeMenu::SettingsShow()
{
	settingsActive = true;
	
	LeftBtn->SetEffect(EFFECT_SLIDE_RIGHT | EFFECT_SLIDE_OUT, 50);
	RightBtn->SetEffect(EFFECT_SLIDE_RIGHT | EFFECT_SLIDE_OUT, 50);
	TopBtn->SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_OUT, 50);
	CloseBtn->SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_OUT, 50);
	WiimoteImg->SetEffect(EFFECT_MOVE | EFFECT_MOVE_VERT, 50, -BottomBtn->GetHeight());
	SettingsText->SetText(tr("Quit Settings"));
	
	while(LeftBtn->IsAnimated()) Application::Instance()->updateEvents();
	
	SettingsAnim(true);
}

void HomeMenu::SettingsHide()
{
	SettingsAnim(false);
	
	WiimoteImg->SetEffect(EFFECT_MOVE | EFFECT_MOVE_VERT, 50, 210);
	LeftBtn->SetEffect(EFFECT_SLIDE_RIGHT | EFFECT_SLIDE_IN, 50);
	RightBtn->SetEffect(EFFECT_SLIDE_RIGHT | EFFECT_SLIDE_IN, 50);
	TopBtn->SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_IN, 50);
	CloseBtn->SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_IN, 50);
	SettingsText->SetText(tr("Settings"));
	
	while(CloseBtn->IsAnimated()) Application::Instance()->updateEvents();
	
	
	settingsActive = false;
}

void HomeMenu::SettingsAnim(bool expand)
{
	if(expand)
    {
		SettingsBgImg->SetEffect(EFFECT_MOVE | EFFECT_MOVE_HOR, 50, 133-screenwidth+SettingsBgImg->GetWidth());
		while(SettingsBgImg->IsAnimated()) Application::Instance()->updateEvents();
		
		SettingsBtnUpdate();
		RumbleText->SetEffect(EFFECT_FADE, 50);
		RumbleOffBtn->SetEffect(EFFECT_FADE, 50);
		RumbleOnBtn->SetEffect(EFFECT_FADE, 50);
		Append(RumbleText);
		Append(RumbleOffBtn);
		Append(RumbleOnBtn);
		while(RumbleOnBtn->IsAnimated()) Application::Instance()->updateEvents();
	}
	else
    {
		RumbleText->SetEffect(EFFECT_FADE, -50);
		RumbleOffBtn->SetEffect(EFFECT_FADE, -50);
		RumbleOnBtn->SetEffect(EFFECT_FADE, -50);
		while(RumbleOnBtn->IsAnimated()) Application::Instance()->updateEvents();
		Remove(RumbleText);
		Remove(RumbleOffBtn);
		Remove(RumbleOnBtn);
		
		SettingsBgImg->SetEffect(EFFECT_MOVE | EFFECT_MOVE_HOR, 50, 133-screenwidth);
		while(SettingsBgImg->IsAnimated()) Application::Instance()->updateEvents();
	}
}

void HomeMenu::SettingsBtnUpdate()
{
	if(Settings.Rumble == ON)
	{
		RumbleOnBtn->SetImage(SettingsBtnActiveImg);
		RumbleOnBtn->ResetEffects();
		RumbleOnBtn->SetState(STATE_DISABLED);
		RumbleOffBtn->SetImage(SettingsBtnImg);
		RumbleOffBtn->SetEffectGrow();
		RumbleOffBtn->SetState(STATE_DEFAULT);
	}
	else
	{
		RumbleOnBtn->SetImage(SettingsBtnImg);
		RumbleOnBtn->SetEffectGrow();
		RumbleOnBtn->SetState(STATE_DEFAULT);
		RumbleOffBtn->SetImage(SettingsBtnActiveImg);
		RumbleOffBtn->ResetEffects();
		RumbleOffBtn->SetState(STATE_DISABLED);
	}
}

void HomeMenu::Draw()
{
	//! check if a WiiMote was connected every sec
	if(frameCount %(Settings.PAL50 ? 50 : 60) == 0)
	{
		for (int i = 0; i < 4; i++)
		{
			if (WPAD_Probe(i, NULL) == WPAD_ERR_NONE)
			{
				int level = (WPAD_BatteryLevel(i) / 100.0) * 4;
				if (level > 4) level = 4;
				
				if (level <= 1)
				{
					BatteryBarImg[i]->SetImage(BatteryBarDataRed);
					BatteryImg[i]->SetImage(BatteryDataRed);
				}
				else
				{
					BatteryBarImg[i]->SetImage(BatteryBarDataWh);
					BatteryImg[i]->SetImage(BatteryDataWh);
				}
				
				BatteryImg[i]->SetTileHorizontal(level);
				BatteryBtn[i]->SetAlpha(255);
				PlayerText[i]->SetAlpha(255);
			}
			else
			{
				BatteryBarImg[i]->SetImage(BatteryBarDataWh);
				BatteryImg[i]->SetTileHorizontal(0);
				BatteryBtn[i]->SetAlpha(130);
				PlayerText[i]->SetAlpha(100);
			}
		}
	}

	GuiFrame::Draw();
}