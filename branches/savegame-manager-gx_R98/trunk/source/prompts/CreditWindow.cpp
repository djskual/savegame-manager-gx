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
#include "prompts/CreditWindow.h"
#include "network/svnrev.h"
#include "menu/MainWindow.h"
#include "main.h"

CreditWindow::CreditWindow()
    : GuiWindow(0, 0)
{
    angle = 0;

    this->SetPosition(0, 0);
	this->SetSize(screenwidth, screenheight);
	this->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	
	bgMusic->Pause();
	
	creditsMusic = Resources::GetSound(credits_music_ogg, credits_music_ogg_size, cfg.CreditsVolume);
    creditsMusic->SetLoop(1);
    creditsMusic->Play();

    dialogBox = Resources::GetImageData(credits_bg_png, credits_bg_png_size);
    dialogBoxImg = new GuiImage(dialogBox);

    star = Resources::GetImageData(little_star_png, little_star_png_size);
    starImg = new GuiImage(star);
	starImg->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	starImg->SetPosition(505,350);

    int i = 0;
    int y = 30;

    Entrie[i] = new GuiText(tr("Credits"), 28, (GXColor) {255, 255, 255, 255});
    Entrie[i]->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
    Entrie[i]->SetPosition(0, 22);
    i++;
    
	char Rev[50];
	sprintf(Rev, "%s %i ", tr("Rev"), atoi(SvnRev()));

	Entrie[i] = new GuiText(Rev, 18, (GXColor) {255, 255, 255, 255});
	Entrie[i]->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	Entrie[i]->SetPosition(0, y);
	i++;
	y += 32;

    Entrie[i] = new GuiText("SaveGame Manager GX", 26, (GXColor) {255, 255, 255, 255});
    Entrie[i]->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
    Entrie[i]->SetPosition(0, y);
    i++;
    y += 38;

    Entrie[i] = new GuiText(tr("Official Site:"), 20, (GXColor) {255, 255, 255, 255});
    Entrie[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
    Entrie[i]->SetPosition(10, y);
    i++;

    Entrie[i] = new GuiText("http://code.google.com/p/savegame-manager-gx/", 20, (GXColor) {255, 255, 255, 255});
    Entrie[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
    Entrie[i]->SetPosition(160, y);
    i++;
    y += 28;

    GuiText::SetPresets(18, (GXColor) {255, 255, 255,  255}, 0, GuiText::WRAP,FTGX_JUSTIFY_LEFT | FTGX_ALIGN_TOP, ALIGN_LEFT, ALIGN_TOP);

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
    y += 28;

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
    y += 28;
	
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
    sprintf(text, "Lustar %s", tr("for WiiTDB (WiiTitleDataBase)"));
    Entrie[i] = new GuiText(text);
    Entrie[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
    Entrie[i]->SetPosition(160,y);
    y += 28;
	
	Entrie[i] = new GuiText(tr("Special thanks to:"));
    Entrie[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
    Entrie[i]->SetPosition(10,y);
    i++;
    y += 22;

    sprintf(text, "USB Loader GX Team %s", tr("for design & sources"));
    Entrie[i] = new GuiText(text);
    Entrie[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
    Entrie[i]->SetPosition(60,y);
    i++;
    y += 22;

    sprintf(text, "Waninkoko %s", tr("for the SaveGame Manager sources"));
    Entrie[i] = new GuiText(text);
    Entrie[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
    Entrie[i]->SetPosition(60,y);
    i++;
    y += 22;

    sprintf(text, "Tantric %s", tr("for his awesome tool LibWiiGui"));
    Entrie[i] = new GuiText(text);
    Entrie[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
    Entrie[i]->SetPosition(60,y);
    i++;
    y += 22;

    sprintf(text, "WiiXplorer Team %s", tr("for sources"));
    Entrie[i] = new GuiText(text);
    Entrie[i]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
    Entrie[i]->SetPosition(60,y);
    i++;
    y += 22;
	
	CreditEntries = i;

    Append(dialogBoxImg);
    for(int i = 0; i < CreditEntries; i++)
        Append(Entrie[i]);
    Append(starImg);

    MainWindow::Instance()->SetState(STATE_DISABLED);
    this->SetState(STATE_DEFAULT);
    SetEffect(EFFECT_FADE, 40);
}

CreditWindow::~CreditWindow()
{
    MainWindow::Instance()->ResumeGui();
    SetEffect(EFFECT_FADE, -40);
    while(this->GetEffect() > 0) usleep(100);

    MainWindow::Instance()->HaltGui();
    if(parentElement)
        ((GuiWindow *) parentElement)->Remove(this);

    creditsMusic->Stop();

    Resources::Remove(creditsMusic);

    RemoveAll();

    Resources::Remove(dialogBox);
    Resources::Remove(star);
    
	delete dialogBoxImg;
    delete starImg;
    
	for(int i = 0; i < CreditEntries; i++)
    {
        delete Entrie[i];
        Entrie[i] = NULL;
    }

    MainWindow::Instance()->SetState(STATE_DEFAULT);
	MainWindow::Instance()->ResumeGui();
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
	{
		return true;
	}

	return false;
}
