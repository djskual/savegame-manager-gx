/***************************************************************************
 * Copyright (C) 2011
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
 * CreditWindow.cpp
 *
 * for SaveGame Manager 2011
 ***************************************************************************/
#include "CreditWindow.h"
#include "../Language/gettext.h"
#include "../Network/svnrev.h"
#include "../System/wpad.h"
#include "../Themes/CTheme.h"
#include "../Settings/CSettings.h"
#include "../System/IosLoader.h"
#include "../menu.h"

CreditWindow::CreditWindow()
	: GuiWindow(0, 0)
{
	angle = 0;

	this->SetPosition(0, 0);
	this->SetSize(screenwidth, screenheight);
	this->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	
	bgMusic->Pause();
	
	creditsMusic = new GuiSound(Resources::GetFile("credits_music.ogg"), Resources::GetFileSize("credits_music.ogg"), Settings.CreditsVolume);
	creditsMusic->SetVolume(Settings.CreditsVolume);
	creditsMusic->SetLoop(1);
	creditsMusic->Play();

	dialogBox = Resources::GetImageData("credits_bg.png");
	dialogBoxImg = new GuiImage(dialogBox);

	star = Resources::GetImageData("credits_little_star.png");
	starImg = new GuiImage(star);
	if(Settings.wsprompt)
		starImg->SetWidescreen(Settings.Widescreen);
	starImg->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	starImg->SetPosition(500,360);

	int i = 0;
	int y = 30;

	Entrie[i] = new GuiText(tr("Credits"), 28, thColor("r=255 g=255 b=255 a=255 - credits windows text color"));
	Entrie[i]->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	Entrie[i]->SetPosition(0, 22);
	i++;
	
	char Rev[100];
	sprintf(Rev, "%s %i IOS%u (%s%u) ", tr("Rev"), atoi(GetRev()), IOS_GetVersion(), tr("Rev"), IOS_GetRevision());
	
	char IosInfo[80] = "";
	iosinfo_t *info = new iosinfo_t();
	info = IosLoader::GetIOSInfo(IOS_GetVersion());
	if(info)
		snprintf(IosInfo, sizeof(IosInfo), "(%s v%i%s base%i) ", info->name, info->version, info->versionstring, info->baseios);
	
	Entrie[i] = new GuiText(Rev, 18, thColor("r=255 g=255 b=255 a=255 - credits windows text color"));
	Entrie[i]->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	Entrie[i]->SetPosition(0, info ? y-10 : y);
	i++;
	
	delete info;
	
	Entrie[i] = new GuiText(IosInfo, 18, thColor("r=255 g=255 b=255 a=255 - credits windows text color"));
	Entrie[i]->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	Entrie[i]->SetPosition(0, y+6);
	i++;
	y += 32;
	
	Entrie[i] = new GuiText("SaveGame Manager GX", 28, thColor("r=255 g=255 b=255 a=255 - credits windows text color"));
	Entrie[i]->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	Entrie[i]->SetPosition(0, y);
	i++;
	y += 30;

	Entrie[i] = new GuiText(tr("Official Site:"), 18, thColor("r=255 g=255 b=255 a=255 - credits windows text color"));
	Entrie[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	Entrie[i]->SetPosition(10, y);
	i++;

	Entrie[i] = new GuiText("http://www.savegamegx.net", 20, thColor("r=255 g=255 b=255 a=255 - credits windows text color"));
	Entrie[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	Entrie[i]->SetPosition(160, y);
	i++;
	y += 26;

	GuiText::SetPresets(18, thColor("r=255 g=255 b=255 a=255 - credits windows text color"), 3000, FTGX_JUSTIFY_LEFT | FTGX_ALIGN_TOP, ALIGN_LEFT, ALIGN_TOP);

	Entrie[i] = new GuiText(tr("Coding:"));
	Entrie[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	Entrie[i]->SetPosition(10, y);
	i++;

	Entrie[i] = new GuiText("Dj_Skual");
	Entrie[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	Entrie[i]->SetPosition(160, y);
	i++;
	y += 22;

	Entrie[i] = new GuiText("Giantpune");
	Entrie[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	Entrie[i]->SetPosition(160, y);
	i++;
	y += 26;

	Entrie[i] = new GuiText(tr("Design:"));
	Entrie[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	Entrie[i]->SetPosition(10,y);
	i++;

	Entrie[i] = new GuiText("Cyrex");
	Entrie[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	Entrie[i]->SetPosition(160,y);
	i++;
	y += 22;

	Entrie[i] = new GuiText("NeoRame");
	Entrie[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	Entrie[i]->SetPosition(160,y);
	i++;
	y += 22;
	
	Entrie[i] = new GuiText("Dj_Skual");
	Entrie[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	Entrie[i]->SetPosition(160,y);
	i++;
	y += 26;
	
	Entrie[i] = new GuiText(tr("Big thanks to:"));
	Entrie[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	Entrie[i]->SetPosition(10,y);
	i++;

	Entrie[i] = new GuiText(tr("All translaters for language files updates"));
	Entrie[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	Entrie[i]->SetPosition(160,y);
	i++;
	y += 22;

	char text[80];
	sprintf(text, "Lustar %s", tr("for GameTDB (GameTitleDataBase)"));
	Entrie[i] = new GuiText(text);
	Entrie[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	Entrie[i]->SetPosition(160,y);
	i++;
	y += 26;
	
	Entrie[i] = new GuiText(tr("Special thanks to:"));
	Entrie[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	Entrie[i]->SetPosition(10,y);
	i++;
	y += 22;

	sprintf(text, "USB Loader GX Team %s", tr("for design & sources"));
	Entrie[i] = new GuiText(text);
	Entrie[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	Entrie[i]->SetPosition(80,y);
	i++;
	y += 22;

	sprintf(text, "Waninkoko %s", tr("for the SaveGame Manager sources"));
	Entrie[i] = new GuiText(text);
	Entrie[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	Entrie[i]->SetPosition(80,y);
	i++;
	y += 22;

	sprintf(text, "Tantric %s", tr("for his awesome tool LibWiiGui"));
	Entrie[i] = new GuiText(text);
	Entrie[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	Entrie[i]->SetPosition(80,y);
	i++;
	y += 22;

	sprintf(text, "Spiffy360.com Team %s", tr("for Themes hosting"));
	Entrie[i] = new GuiText(text);
	Entrie[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	Entrie[i]->SetPosition(80,y);
	i++;
	y += 22;

	sprintf(text, "WiiSave.com Team %s", tr("for Saves hosting"));
	Entrie[i] = new GuiText(text);
	Entrie[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	Entrie[i]->SetPosition(80,y);
	i++;
	y += 22;

	sprintf(text, "WiiXplorer Team %s", tr("for sources"));
	Entrie[i] = new GuiText(text);
	Entrie[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	Entrie[i]->SetPosition(80,y);
	i++;
	y += 22;
	
	CreditEntries = i;

	Append(dialogBoxImg);
	for(int cnt = 0; cnt < CreditEntries; cnt++)
		Append(Entrie[cnt]);
	Append(starImg);

	mainWindow->SetState(STATE_DISABLED);
	this->SetState(STATE_DEFAULT);
	SetEffect(EFFECT_FADE, 40);
}

CreditWindow::~CreditWindow()
{
	ResumeGui();
	SetEffect(EFFECT_FADE, -40);
	while(this->GetEffect() > 0) usleep(100);

	HaltGui();
	if(parentElement)
		((GuiWindow *) parentElement)->Remove(this);

	creditsMusic->Stop();

	delete creditsMusic;

	RemoveAll();

	delete dialogBox;
	delete star;
	
	delete dialogBoxImg;
	delete starImg;
	
	for(int i = 0; i < CreditEntries; i++)
	{
		delete Entrie[i];
		Entrie[i] = NULL;
	}

	mainWindow->SetState(STATE_DEFAULT);
	ResumeGui();
	bgMusic->Play();
}

bool CreditWindow::ButtonPressed()
{
	angle++;
	if (angle > 360)
		angle = 0;
	usleep(12000);
	starImg->SetAngle(angle);

	if(ButtonsPressed() != 0)
		return true;

	return false;
}
