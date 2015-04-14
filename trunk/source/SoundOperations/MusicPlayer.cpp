/***************************************************************************
 * Copyright (C) 2010
 * by Dimok
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
 * for WiiXplorer 2010
 ***************************************************************************/
#include <sys/dir.h>
#include <unistd.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "../Controls/Application.h"
//#include "../Prompts/PromptWindows.h"
#include "../Themes/CTheme.h"
#include "MusicPlayer.h"
#include "../Language/gettext.h"
#include "../FileOperations/fileops.h"
//#include "../FileStartUp/FileExtensions.h"
#include "../Tools/StringTools.h"
#include "../System/sys.h"

MusicPlayer * MusicPlayer::instance = NULL;

extern const u8 bg_music_ogg[];
extern const u32 bg_music_ogg_size;

MusicPlayer::MusicPlayer()
	: GuiFrame(0, 0)
	, Thread(40, 16384)
{
	btnSoundOver = NULL;
	playerImgData = NULL;
	navi_defaultImgData = NULL;
	navi_upImgData = NULL;
	navi_downImgData = NULL;
	navi_leftImgData = NULL;
	navi_rightImgData = NULL;
	trigA = NULL;
	trigB = NULL;
	BackButton = NULL;
	PlayBtn = NULL;
	StopBtn = NULL;
	NextBtn = NULL;
	PreviousBtn = NULL;
	BackgroundImg = NULL;
	CircleImg = NULL;
	PlayTitle = NULL;

	LoopMode = 0;
	currentPlaying = 0;
	CircleImageDelay = 0;
	Stopped = false;
	PlaybackFinished = false;
	ExitRequested = false;

	MainSound = Resources::GetSound("bg_music.ogg");

	TitleList.LoadList();
	TitleList.ItemClicked.connect(this, &MusicPlayer::OnTitleListClick);

	currentPlaying = TitleList.FindFile(Settings.MusicPath);

	Play();

	startThread();
}

MusicPlayer::~MusicPlayer()
{
	ExitRequested = true;

	Hide();

	shutdownThread();

	if(MainSound)
		Resources::Remove(MainSound);
}

bool MusicPlayer::LoadStandard()
{
	strcpy(Settings.MusicPath, "");

	currentPlaying = -1;

	Title.assign(tr("SaveGameGX Default Music"));

	if(PlayTitle)
		PlayTitle->SetText(Title.c_str());

	MainSound->Load(bg_music_ogg, bg_music_ogg_size, false);
	MainSound->Play();
	Stopped = false;
	Paused = false;

	return true;
}

bool MusicPlayer::Load(const char * path, bool silent)
{
	if(!path)
	{
		LoadStandard();
		return false;
	}

	if(!CheckFile(path))
	{
		LoadStandard();
		return false;
	}

	loadPathThreaded = path;
	loadSilentThreaded = silent;

	return true;
}

void MusicPlayer::ThreadedLoadMusic(void)
{
	std::string path = loadPathThreaded;
	//bool silent = loadSilentThreaded;

	loadPathThreaded.clear();
	loadSilentThreaded = true;

	if(!MainSound->Load(path.c_str()))
	{
		LoadStandard();
		return;
	}

	char * filename = strrchr(path.c_str(), '/');
	if(filename)
		Title.assign(filename+1);

	if(PlayTitle)
		PlayTitle->SetText(Title.c_str());

	MainSound->Play();
	Stopped = false;
	Paused = false;
}

void MusicPlayer::Resume()
{
	if(IsStopped())
		return;

	MainSound->Play();
	Paused = false;
}

void MusicPlayer::Stop()
{
	Paused = false;
	Stopped = true;
	MainSound->Stop();
}

bool MusicPlayer::Play()
{
	return Play(currentPlaying);
}

bool MusicPlayer::Play(int pos)
{
	if(pos < 0 || pos >= (int) TitleList.size())
		return LoadStandard();

	currentPlaying = pos;

	snprintf(Settings.MusicPath, sizeof(Settings.MusicPath), "%s", TitleList.at(currentPlaying));

	return Load(Settings.MusicPath, true);
}

bool MusicPlayer::PlayNext()
{
	if(TitleList.size() == 0)
		return LoadStandard();

	currentPlaying++;
	if(currentPlaying >= (int) TitleList.size())
		currentPlaying = 0;

	snprintf(Settings.MusicPath, sizeof(Settings.MusicPath), "%s", TitleList.at(currentPlaying));

	return Load(Settings.MusicPath, true);
}

bool MusicPlayer::PlayPrevious()
{
	if(TitleList.size() == 0)
		return LoadStandard();

	currentPlaying--;
	if(currentPlaying < 0)
		currentPlaying = TitleList.size()-1;

	snprintf(Settings.MusicPath, sizeof(Settings.MusicPath), "%s", TitleList.at(currentPlaying));

	return Load(Settings.MusicPath, true);
}

bool MusicPlayer::PlayRandom()
{
	if(TitleList.size() == 0)
		return LoadStandard();

	srand(time(NULL));

	currentPlaying = rand() % TitleList.size();

	//just in case
	if(currentPlaying < 0)
		currentPlaying = TitleList.size()-1;
	else if(currentPlaying >= (int) TitleList.size())
		currentPlaying = 0;

	snprintf(Settings.MusicPath, sizeof(Settings.MusicPath), "%s", TitleList.at(currentPlaying));

	return Load(Settings.MusicPath, true);
}

void MusicPlayer::Show()
{
	//!To append on top let's put it out of the list first
	Application::Instance()->Remove(this);
	Application::Instance()->SetUpdateOnly(this);
	InternalSetup();
	SetEffect(EFFECT_SLIDE_RIGHT | EFFECT_SLIDE_IN, 40);
	Application::Instance()->Append(this);
}

void MusicPlayer::Hide()
{
	if(!Application::isClosing())
	{
		if(!ExitRequested && parentElement)
		{
			SetEffect(EFFECT_SLIDE_RIGHT | EFFECT_SLIDE_OUT, 40);
			while(this->GetEffect() > 0)
				Application::Instance()->updateEvents();
		}

		Application::Instance()->UnsetUpdateOnly(this);
	}

	this->DimBackground(false);
	this->SetVisible(false);

	RemoveAll();

	if(trigA)
		delete trigA;
	if(trigB)
		delete trigB;

	Resources::Remove(btnSoundOver);
	Resources::Remove(playerImgData);
	Resources::Remove(navi_defaultImgData);
	Resources::Remove(navi_upImgData);
	Resources::Remove(navi_downImgData);
	Resources::Remove(navi_leftImgData);
	Resources::Remove(navi_rightImgData);

	if(BackButton)
		delete BackButton;
	if(PlayBtn)
		delete PlayBtn;
	if(StopBtn)
		delete StopBtn;
	if(NextBtn)
		delete NextBtn;
	if(PreviousBtn)
		delete PreviousBtn;

	if(BackgroundImg)
		delete BackgroundImg;
	if(CircleImg)
		delete CircleImg;

	if(PlayTitle)
		delete PlayTitle;

	btnSoundOver = NULL;

	playerImgData = NULL;
	navi_defaultImgData = NULL;
	navi_upImgData = NULL;
	navi_downImgData = NULL;
	navi_leftImgData = NULL;
	navi_rightImgData = NULL;

	trigA = NULL;
	trigB = NULL;

	BackButton = NULL;
	PlayBtn = NULL;
	StopBtn = NULL;
	NextBtn = NULL;
	PreviousBtn = NULL;

	BackgroundImg = NULL;
	CircleImg = NULL;

	PlayTitle = NULL;
}

void MusicPlayer::OnButtonClick(GuiButton *sender, int pointer UNUSED, const POINT &p UNUSED)
{
	if(sender == BackButton)
	{
		Hide();
		TitleList.Hide();
	}
	else if(sender == PlayBtn)
	{
		CircleImg->SetImage(navi_upImgData);
		CircleImageDelay = 12;
		Play(TitleList.GetSelectedItem());
	}
	else if(sender == StopBtn)
	{
		CircleImg->SetImage(navi_downImgData);
		CircleImageDelay = 12;
		Stop();
	}
	else if(sender == PreviousBtn)
	{
		CircleImg->SetImage(navi_leftImgData);
		CircleImageDelay = 12;
		PlayPrevious();
	}
	else if(sender == NextBtn)
	{
		CircleImg->SetImage(navi_rightImgData);
		CircleImageDelay = 12;
		PlayNext();
	}
}

void MusicPlayer::OnTitleListClick(PlayList *list, int item)
{
	char * filename = strrchr(list->at(item), '/');
	if(filename)
		filename++;

	int ret = 0;//WindowPrompt(tr("How do you want to proceed?"), filename, tr("Play File"), tr("Remove from list"), tr("Clear List"), tr("Cancel"));
	if(ret == 1)
	{
		Play(item);
	}
	else if(ret == 2)
	{
		list->RemoveEntrie(item);
		currentPlaying = list->FindFile(Settings.MusicPath);
	}
	else if(ret == 3)
	{
		ClearList();
	}
}

void MusicPlayer::InternalSetup()
{
	this->SetVisible(true);
	this->DimBackground(true);

	trigA = new SimpleGuiTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	trigB = new GuiTrigger();
	trigB->SetButtonOnlyTrigger(-1, WiiControls.BackButton | ClassicControls.BackButton << 16, GCControls.BackButton);

	btnSoundOver = Resources::GetSound("button_over.wav");
	
	playerImgData = Resources::GetImageData("music_player.png");
	navi_defaultImgData = Resources::GetImageData("music_nav_default.png");
	navi_upImgData = Resources::GetImageData("music_nav_up.png");
	navi_downImgData = Resources::GetImageData("music_nav_down.png");
	navi_leftImgData = Resources::GetImageData("music_nav_left.png");
	navi_rightImgData = Resources::GetImageData("music_nav_right.png");

	width = playerImgData->GetWidth();
	height = playerImgData->GetHeight();

	BackButton = new GuiButton(35, 40);
	BackButton->SetPosition(404, 115);
	BackButton->SetSoundOver(btnSoundOver);
	BackButton->SetTrigger(trigA);
	BackButton->SetTrigger(trigB);
	BackButton->Clicked.connect(this, &MusicPlayer::OnButtonClick);

	PlayBtn = new GuiButton(68, 40);
	PlayBtn->SetPosition(77, 39);
	PlayBtn->SetTrigger(trigA);
	PlayBtn->Clicked.connect(this, &MusicPlayer::OnButtonClick);

	StopBtn = new GuiButton(68, 40);
	StopBtn->SetPosition(77, 124);
	StopBtn->SetTrigger(trigA);
	StopBtn->Clicked.connect(this, &MusicPlayer::OnButtonClick);

	PreviousBtn = new GuiButton(38, 60);
	PreviousBtn->SetPosition(44, 74);
	PreviousBtn->SetTrigger(trigA);
	PreviousBtn->Clicked.connect(this, &MusicPlayer::OnButtonClick);

	NextBtn = new GuiButton(38, 60);
	NextBtn->SetPosition(137, 74);
	NextBtn->SetTrigger(trigA);
	NextBtn->Clicked.connect(this, &MusicPlayer::OnButtonClick);

	BackgroundImg = new GuiImage(playerImgData);
	CircleImg = new GuiImage(navi_defaultImgData);
	CircleImg->SetPosition(39, 34);

	PlayTitle = new GuiText(Title.c_str(), 20, thColor("r=0 g=0 b=0 a=255 - MusicPlayer played title text color"));
	PlayTitle->SetPosition(220, 126);
	PlayTitle->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	PlayTitle->SetMaxWidth(135, SCROLL_HORIZONTAL);

	if(TitleList.IsMinimized())
		TitleList.SetPosition(90, 56);
	else
		TitleList.SetPosition(90, 56-18*15);
	TitleList.SetBounds(iRect(-200, -200, screenwidth+200, GetTop()-175+TitleList.GetHeight()));
	TitleList.Show();

	Append(&TitleList);
	Append(BackgroundImg);
	Append(CircleImg);
	Append(BackButton);
	Append(PlayBtn);
	Append(StopBtn);
	Append(NextBtn);
	Append(PreviousBtn);
	Append(PlayTitle);
}

void MusicPlayer::executeThread(void)
{
	while(!ExitRequested)
	{
		usleep(100000);

		//! we do this in the draw cycle, as the update cycle might not be called always
		if(PlaybackFinished && !Stopped && !Paused)
		{
			if(LoopMode > 0 && Settings.MusicPath[0] == 0)
			{
				//!Standard Music is always looped except on loop = 0
				Play();
			}
			else if(LoopMode == PLAYLIST_LOOP)
			{
				PlayNext();
			}
			else if(LoopMode == RANDOM_MUSIC)
			{
				PlayRandom();
			}

			PlaybackFinished = false;
		}

		if(!loadPathThreaded.empty()) {
			ThreadedLoadMusic();
		}
	}
}

void MusicPlayer::Draw()
{
	if(CircleImageDelay > 0)
	{
		--CircleImageDelay;
		if(CircleImageDelay == 0 && CircleImg != NULL)
			CircleImg->SetImage(navi_defaultImgData);
	}

	GuiFrame::Draw();
}
