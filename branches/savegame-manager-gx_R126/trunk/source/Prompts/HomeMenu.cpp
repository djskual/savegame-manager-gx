#include <unistd.h>

#include "../Language/gettext.h"
#include "../Themes/CTheme.h"
#include "../Settings/CSettings.h"
#include "../System/sys.h"
#include "../menu.h"
#include "PromptWindows.h"
#include "HomeMenu.h"

HomeMenu::HomeMenu()
	: GuiWindow(0, 0)
{
	choice = -1;

	this->SetPosition(0, 0);
	this->SetSize(screenwidth, screenheight);

	trigA = new SimpleGuiTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	trigB = new GuiTrigger();
	trigB->SetButtonOnlyTrigger(-1, WiiControls.BackButton | ClassicControls.BackButton << 16, GCControls.BackButton);
	trigHome = new GuiTrigger();
	trigHome->SetButtonOnlyTrigger(-1, WiiControls.HomeButton | ClassicControls.HomeButton << 16, GCControls.HomeButton);
	
	homein = new GuiSound(Resources::GetFile("menuin.ogg"), Resources::GetFileSize("menuin.ogg"), Settings.SFXVolume);
	homeout = new GuiSound(Resources::GetFile("menuout.ogg"), Resources::GetFileSize("menuout.ogg"), Settings.SFXVolume);
	
	bgMusic->Pause();
	homein->Play();
	
	TopBtnImgData = Resources::GetImageData("homemenu_top.png");
	TopBtnOverImgData = Resources::GetImageData("homemenu_top_over.png");
	BottomBtnImgData = Resources::GetImageData("homemenu_bottom.png");
	BottomBtnOverImgData = Resources::GetImageData("homemenu_bottom_over.png");
	CloseBtnImgData = Resources::GetImageData("homemenu_close.png");
	StandardBtnImgData = Resources::GetImageData("homemenu_button.png");
	WiimoteBtnImgData = Resources::GetImageData("homemenu_wiimote.png");

	BatteryImgData = Resources::GetImageData("homemenu_battery_white.png");
	BatteryBarImgData = Resources::GetImageData("homemenu_battery_bar_white.png");
	BatteryRedImgData = Resources::GetImageData("homemenu_battery_red.png");
	BatteryBarRedImgData = Resources::GetImageData("homemenu_battery_bar_red.png");

	TopBtnImg = new GuiImage(TopBtnImgData);
	TopBtnOverImg = new GuiImage(TopBtnOverImgData);
	BottomBtnImg = new GuiImage(BottomBtnImgData);
	BottomBtnOverImg = new GuiImage(BottomBtnOverImgData);
	CloseBtnImg = new GuiImage(CloseBtnImgData);
	ExitBtnImg = new GuiImage(StandardBtnImgData);
	ShutdownBtnImg = new GuiImage(StandardBtnImgData);
	WiimoteBtnImg = new GuiImage(WiimoteBtnImgData);

	TitleText = new GuiText(tr("HOME Menu"), 40, thColor("r=255 g=255 b=255 a=255 - homemenu title text color"));
	TitleText->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	TitleText->SetPosition(30, 40);
	TitleText->SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_IN, 50);

	TopBtn = new GuiButton(TopBtnImg->GetWidth(), TopBtnImg->GetHeight());
	TopBtn->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	TopBtn->SetImage(TopBtnImg);
	TopBtn->SetImageOver(TopBtnOverImg);
	TopBtn->SetLabel(TitleText);
	TopBtn->SetSoundClick(btnSoundClick);
	TopBtn->SetSoundOver(btnSoundOver);
	TopBtn->SetTrigger(trigA);
	TopBtn->SetTrigger(trigB);
	TopBtn->SetTrigger(trigHome);
	TopBtn->SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_IN, 50);

	BottomBtn = new GuiButton(BottomBtnImg->GetWidth(), BottomBtnImg->GetHeight());
	BottomBtn->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	BottomBtn->SetImage(BottomBtnImg);
	BottomBtn->SetImageOver(BottomBtnOverImg);
	BottomBtn->SetSoundClick(btnSoundClick);
	BottomBtn->SetSoundOver(btnSoundOver);
	BottomBtn->SetTrigger(trigA);
	BottomBtn->SetEffect(EFFECT_SLIDE_BOTTOM | EFFECT_SLIDE_IN, 50);
	BottomBtn->SetSelectable(true);

	CloseBtnText = new GuiText(tr("Close"), 28, thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));

	CloseBtn = new GuiButton(CloseBtnImg->GetWidth(), CloseBtnImg->GetHeight());
	CloseBtn->SetImage(CloseBtnImg);
	CloseBtn->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	CloseBtn->SetPosition(-20, 30);
	CloseBtn->SetLabel(CloseBtnText);
	CloseBtn->SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_IN, 50);

	ExitBtnText = new GuiText(tr("Back to Loader"), 28, thColor("r=0 g=0 b=0 a=255 - homemenu main button text color"));

	ExitBtn = new GuiButton(ExitBtnImg->GetWidth(), ExitBtnImg->GetHeight());
	ExitBtn->SetImage(ExitBtnImg);
	ExitBtn->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	ExitBtn->SetPosition(-140, 0);
	ExitBtn->SetLabel(ExitBtnText);
	ExitBtn->SetSoundClick(btnSoundClick);
	ExitBtn->SetSoundOver(btnSoundOver);
	ExitBtn->SetTrigger(trigA);
	ExitBtn->SetEffectGrow();
	ExitBtn->SetEffect(EFFECT_FADE, 50);

	ShutdownBtnText = new GuiText(tr("ShutDown Wii"), 28, thColor("r=0 g=0 b=0 a=255 - homemenu main button text color"));

	ShutdownBtn = new GuiButton(ShutdownBtnImg->GetWidth(), ShutdownBtnImg->GetHeight());
	ShutdownBtn->SetImage(ShutdownBtnImg);
	ShutdownBtn->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	ShutdownBtn->SetPosition(140, 0);
	ShutdownBtn->SetLabel(ShutdownBtnText);
	ShutdownBtn->SetSoundClick(btnSoundClick);
	ShutdownBtn->SetSoundOver(btnSoundOver);
	ShutdownBtn->SetTrigger(trigA);
	ShutdownBtn->SetEffectGrow();
	ShutdownBtn->SetEffect(EFFECT_FADE, 50);

	WiimoteBtn = new GuiButton(WiimoteBtnImg->GetWidth(), WiimoteBtnImg->GetHeight());
	WiimoteBtn->SetImage(WiimoteBtnImg);
	WiimoteBtn->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	WiimoteBtn->SetPosition(45, 232);
	WiimoteBtn->SetTrigger(trigA);
	WiimoteBtn->SetEffect(EFFECT_SLIDE_BOTTOM | EFFECT_SLIDE_IN, 50);

	Append(BottomBtn);
	Append(TopBtn);
	Append(CloseBtn);
	Append(ExitBtn);
	Append(ShutdownBtn);
	Append(WiimoteBtn);

	for (int i = 0; i < 4; i++)
	{
		char player[] = "P0";
		player[1] = i+'1';

		PlayerText[i] = new GuiText(player, 28, thColor("r=255 g=255 b=255 a=255 - homemenu battery text color"));
		PlayerText[i]->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
		PlayerText[i]->SetPosition(178 + i*108, -76);
		PlayerText[i]->SetEffect(EFFECT_SLIDE_BOTTOM | EFFECT_SLIDE_IN, 50);
		Append(PlayerText[i]);

		BatteryBarImg[i] = new GuiImage(BatteryBarImgData);
		BatteryBarImg[i]->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);

		BatteryImg[i] = new GuiImage(BatteryImgData);
		BatteryImg[i]->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
		BatteryImg[i]->SetPosition(2, -4);

		BatteryBarRedImg[i] = new GuiImage(BatteryBarRedImgData);
		BatteryBarRedImg[i]->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);

		BatteryRedImg[i] = new GuiImage(BatteryRedImgData);
		BatteryRedImg[i]->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
		BatteryRedImg[i]->SetPosition(2, -4);

		BatteryBtn[i] = new GuiButton(0,0);
		BatteryBtn[i]->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
		BatteryBtn[i]->SetPosition(214 + i*108, -80);
		BatteryBtn[i]->SetImage(BatteryBarImg[i]);
		BatteryBtn[i]->SetIcon(BatteryImg[i]);
		BatteryBtn[i]->SetEffect(EFFECT_SLIDE_BOTTOM | EFFECT_SLIDE_IN, 50);

		Append(BatteryBtn[i]);
	}
}

HomeMenu::~HomeMenu()
{
	ResumeGui();
	SetEffect(EFFECT_FADE, -50);
	while(this->GetEffect() > 0) usleep(100);

	HaltGui();
	if(parentElement)
		((GuiWindow *) parentElement)->Remove(this);

	RemoveAll();

	delete WiimoteBtn;
	delete ShutdownBtn;
	delete ExitBtn;
	delete CloseBtn;
	delete TopBtn;
	delete BottomBtn;

	delete TopBtnImg;
	delete TopBtnOverImg;
	delete BottomBtnImg;
	delete BottomBtnOverImg;
	delete CloseBtnImg;
	delete ExitBtnImg;
	delete ShutdownBtnImg;
	delete WiimoteBtnImg;

	delete TopBtnImgData;
	delete TopBtnOverImgData;
	delete BottomBtnImgData;
	delete BottomBtnOverImgData;
	delete CloseBtnImgData;
	delete StandardBtnImgData;
	delete WiimoteBtnImgData;
	delete BatteryImgData;
	delete BatteryBarImgData;
	delete BatteryRedImgData;
	delete BatteryBarRedImgData;

	delete TitleText;
	delete ShutdownBtnText;
	delete ExitBtnText;
	delete CloseBtnText;

	for (int i = 0; i < 4; i++)
	{
		delete PlayerText[i];
		delete BatteryBarImg[i];
		delete BatteryImg[i];
		delete BatteryBarRedImg[i];
		delete BatteryRedImg[i];
		delete BatteryBtn[i];
	}

	delete trigA;
	delete trigB;
	delete trigHome;

	homeout->Stop();
	delete homein;
	delete homeout;
	
	bgMusic->Play();
	ResumeGui();
}

void HomeMenu::FadeOut()
{
	homein->Stop();
	homeout->Play();
	
	TitleText->SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_OUT, 50);
	TopBtn->SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_OUT, 50);
	CloseBtn->SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_OUT, 50);
	ExitBtn->SetEffect(EFFECT_FADE, -50);
	ShutdownBtn->SetEffect(EFFECT_FADE, -50);
	BottomBtn->SetEffect(EFFECT_SLIDE_BOTTOM | EFFECT_SLIDE_OUT, 50);
	WiimoteBtn->SetEffect(EFFECT_SLIDE_BOTTOM | EFFECT_SLIDE_OUT, 50);

	for (int i = 0; i < 4; i++)
	{
		PlayerText[i]->SetEffect(EFFECT_SLIDE_BOTTOM | EFFECT_SLIDE_OUT, 50);
		BatteryBtn[i]->SetEffect(EFFECT_SLIDE_BOTTOM | EFFECT_SLIDE_OUT, 50);
	}
}

int HomeMenu::GetChoice()
{
	for (int i = 0; i < 4; i++)
	{
		if (WPAD_Probe(i, NULL) == WPAD_ERR_NONE)
		{
			int level = (WPAD_BatteryLevel(i) / 100.0) * 4;
			if (level > 4) level = 4;

			if (level <= 1) {
				BatteryBarImg[i]->SetImage(BatteryBarRedImgData);
				BatteryImg[i]->SetImage(BatteryRedImgData);
			} else {
				BatteryBarImg[i]->SetImage(BatteryBarImgData);
				BatteryImg[i]->SetImage(BatteryImgData);
			}

			BatteryImg[i]->SetTileHorizontal(level);
			BatteryBtn[i]->SetAlpha(255);
			PlayerText[i]->SetAlpha(255);
		}
		else
		{
			BatteryBarImg[i]->SetImage(BatteryBarImgData);
			BatteryImg[i]->SetTileHorizontal(0);
			BatteryBtn[i]->SetAlpha(130);
			PlayerText[i]->SetAlpha(100);
		}
	}

	if (TopBtn->GetState() == STATE_CLICKED)
	{
		TopBtn->ResetState();
		FadeOut();
		choice = 0;
	}
	else if (BottomBtn->GetState() == STATE_CLICKED)
	{
		BottomBtn->ResetState();
		FadeOut();
		choice = 0;
	}
	else if (ExitBtn->GetState() == STATE_CLICKED)
	{
		ExitBtn->ResetState();
		
		this->SetState(STATE_DISABLED);
		
		int ret = WindowPrompt(tr("Back to Loader"), 0, tr("Loader"), tr("Wii Menu"), tr("Cancel"));
		mainWindow->SetState(STATE_DISABLED);
		if (ret == 1)
			Sys_LoadHBC();
		else if(ret == 2)
			Sys_LoadMenu();
		
		this->SetState(STATE_DEFAULT);
	}
	else if (ShutdownBtn->GetState() == STATE_CLICKED)
	{
		ShutdownBtn->ResetState();
		
		this->SetState(STATE_DISABLED);
		
		int ret = WindowPrompt(tr("ShutDown Wii"), 0, tr("Full Shutdown"), tr("Idle Shutdown"), tr("Cancel"));
		mainWindow->SetState(STATE_DISABLED);
		if (ret == 1)
			Sys_ShutdownToStandby();
		else if (ret == 2)
			Sys_ShutdownToIdle();
		
		this->SetState(STATE_DEFAULT);
	}
	else if (BottomBtn->GetState() == STATE_SELECTED)
	{
		WiimoteBtn->SetPosition(WiimoteBtn->GetLeft(), 210);
	}
	else if (BottomBtn->GetState() != STATE_SELECTED)
	{
		WiimoteBtn->SetPosition(WiimoteBtn->GetLeft(), 232);
	}

	return choice;
}
