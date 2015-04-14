/***************************************************************************
 * Copyright (C) 2012
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
 * CreditsWindow.cpp
 *
 * for SaveGame Manager GX 2012
 ***************************************************************************/
#include "CreditsWindow.h"
#include "../Themes/CTheme.h"
#include "../SoundOperations/MusicPlayer.h"
#include "../Controls/Application.h"
#include "../Network/svnrev.h"
#include "../System/IosLoader.h"
#include "../System/input.h"

CreditsWindow::CreditsWindow()
	: GuiFrame(screenwidth, screenheight)
	, CreditEntries(0)
	, angle(0)
	, oldFontScale(Settings.FontScaleFactor)
{
	Settings.FontScaleFactor = 1.0f;

	creditsMusic = new GuiSound(Resources::GetFile("credits_music.ogg"), Resources::GetFileSize("credits_music.ogg"));
	creditsMusic->SetVolume(Settings.CreditsVolume);
	creditsMusic->SetLoop(1);
	
	if(!MusicPlayer::Instance()->IsStopped())
		MusicPlayer::Instance()->Pause();
	creditsMusic->Play();
	
	bg = Resources::GetImageData("credits_bg.png");
	bgImg = new GuiImage(bg);

	star = Resources::GetImageData("credits_little_star.png");
	starImg = new GuiImage(star);
	starImg->SetPosition(500,360);

	int i = 0;
	int y = 30;
	
	Entrie[i] = new GuiText(tr("Credits"), 28, thColor("r=255 g=255 b=255 a=255 - credits windows text color"));
	Entrie[i]->SetAlignment(ALIGN_CENTER | ALIGN_TOP);
	Entrie[i]->SetPosition(0, 22);
	i++;
	
	char SvnRev[100];
	snprintf(SvnRev, sizeof(SvnRev), "Rev%s  IOS%u (Rev %u) ", GetRev(), IOS_GetVersion(), IOS_GetRevision());

	char IosInfo[80] = "";
	iosinfo_t * info = IosLoader::GetIOSInfo(IOS_GetVersion());
	if(info)
		snprintf(IosInfo, sizeof(IosInfo), "(%s v%i%s base%i) ", info->name, info->version, info->versionstring, info->baseios);
	
	Entrie[i] = new GuiText(SvnRev, 16, thColor("r=255 g=255 b=255 a=255 - credits windows text color"));
	Entrie[i]->SetAlignment(ALIGN_RIGHT | ALIGN_TOP);
	Entrie[i]->SetPosition(0, info ? y-10 : y);
	i++;
	
	Entrie[i] = new GuiText(IosInfo, 16, thColor("r=255 g=255 b=255 a=255 - credits windows text color"));
	Entrie[i]->SetAlignment(ALIGN_RIGHT | ALIGN_TOP);
	Entrie[i]->SetPosition(0, y+6);
	i++;
	y += 32;
	
	Entrie[i] = new GuiText("SaveGame Manager GX", 28, thColor("r=255 g=255 b=255 a=255 - credits windows text color"));
	Entrie[i]->SetAlignment(ALIGN_CENTER | ALIGN_TOP);
	Entrie[i]->SetPosition(0, y);
	i++;
	y += 30;

	Entrie[i] = new GuiText(tr("Official Site:"), 18, thColor("r=255 g=255 b=255 a=255 - credits windows text color"));
	Entrie[i]->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	Entrie[i]->SetPosition(10, y);
	i++;

	Entrie[i] = new GuiText("http://www.savegamegx.net", 20, thColor("r=255 g=255 b=255 a=255 - credits windows text color"));
	Entrie[i]->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	Entrie[i]->SetPosition(160, y);
	i++;
	y += 26;

	GuiText::SetPresets(18, thColor("r=255 g=255 b=255 a=255 - credits windows text color") , 0, ALIGN_LEFT | ALIGN_TOP);

	Entrie[i] = new GuiText(tr("Coding:"));
	Entrie[i]->SetPosition(10, y);
	i++;

	Entrie[i] = new GuiText("Dj_Skual");
	Entrie[i]->SetPosition(160, y);
	i++;
	y += 22;

	Entrie[i] = new GuiText("Giantpune");
	Entrie[i]->SetPosition(160, y);
	i++;
	y += 26;

	Entrie[i] = new GuiText(tr("Design:"));
	Entrie[i]->SetPosition(10,y);
	i++;

	Entrie[i] = new GuiText("Cyrex");
	Entrie[i]->SetPosition(160,y);
	i++;
	y += 22;

	Entrie[i] = new GuiText("NeoRame");
	Entrie[i]->SetPosition(160,y);
	i++;
	y += 22;
	
	Entrie[i] = new GuiText("Dj_Skual");
	Entrie[i]->SetPosition(160,y);
	i++;
	y += 26;
	
	Entrie[i] = new GuiText(tr("Big thanks to:"));
	Entrie[i]->SetPosition(10,y);
	i++;

	Entrie[i] = new GuiText(tr("All translaters for language files updates"));
	Entrie[i]->SetPosition(160,y);
	i++;
	y += 22;

	char text[80];
	sprintf(text, "Lustar %s", tr("for GameTDB (GameTitleDataBase)"));
	Entrie[i] = new GuiText(text);
	Entrie[i]->SetPosition(160,y);
	i++;
	y += 26;
	
	Entrie[i] = new GuiText(tr("Special thanks to:"));
	Entrie[i]->SetPosition(10,y);
	i++;
	y += 22;

	sprintf(text, "USB Loader GX Team %s", tr("for design & sources"));
	Entrie[i] = new GuiText(text);
	Entrie[i]->SetPosition(80,y);
	i++;
	y += 22;

	sprintf(text, "Waninkoko %s", tr("for the SaveGame Manager sources"));
	Entrie[i] = new GuiText(text);
	Entrie[i]->SetPosition(80,y);
	i++;
	y += 22;

	sprintf(text, "Tantric %s", tr("for his awesome tool LibWiiGui"));
	Entrie[i] = new GuiText(text);
	Entrie[i]->SetPosition(80,y);
	i++;
	y += 22;

	sprintf(text, "Spiffy360.com Team %s", tr("for Themes hosting"));
	Entrie[i] = new GuiText(text);
	Entrie[i]->SetPosition(80,y);
	i++;
	y += 22;

	sprintf(text, "WiiSave.com Team %s", tr("for Saves hosting"));
	Entrie[i] = new GuiText(text);
	Entrie[i]->SetPosition(80,y);
	i++;
	y += 22;

	sprintf(text, "WiiXplorer Team %s", tr("for sources"));
	Entrie[i] = new GuiText(text);
	Entrie[i]->SetPosition(80,y);
	i++;
	y += 22;
	
	CreditEntries = i;
	
	const u8 *creditsFont = Resources::GetFile("font.ttf");
	u32 creditsFontSize = Resources::GetFileSize("font.ttf");
	
	Append(bgImg);
	for(int j = 0; j < CreditEntries; j++)
	{
		Entrie[j]->SetFont(creditsFont, creditsFontSize);
		Append(Entrie[j]);
	}
	Append(starImg);

	SetEffect(EFFECT_FADE, 40);
	Application::Instance()->SetUpdateOnly(this);
}

CreditsWindow::~CreditsWindow()
{
	creditsMusic->Stop();

	if(!MusicPlayer::Instance()->IsStopped())
		MusicPlayer::Instance()->Resume();
	
	RemoveAll();

	delete creditsMusic;
	
	delete bgImg;
	delete starImg;
	
	Resources::Remove(bg);
	Resources::Remove(star);
	
	for(int i = 0; i < CreditEntries; i++)
	{
		delete Entrie[i];
		Entrie[i] = NULL;
	}
	
	Settings.FontScaleFactor = oldFontScale;
}

void CreditsWindow::Update(GuiTrigger * t)
{
	if(!t || state == STATE_DISABLED)
		return;
	
	if(angle++ > 360) angle = 0;
	usleep(12000);
	starImg->SetAngle(angle);

	if(ButtonPressed())
		Clicked(this);

	GuiFrame::Update(t);
}
