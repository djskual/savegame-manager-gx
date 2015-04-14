/****************************************************************************
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
 * ScreenSaver.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include <unistd.h>

#include "ScreenSaver.hpp"
#include "../Controls/WiiPointer.h"
#include "../Settings/CSettings.h"
#include "../Themes/CTheme.h"
#include "../menu.h"

ScreenSaver::ScreenSaver()
	: GuiWindow(0, 0)
{
	sr = false;
	
	this->SetPosition(0, 0);
	this->SetSize(screenwidth, screenheight);

	GXlogo = Resources::GetImageData("logo_SGMGX.png");
	GXlogoImg = new GuiImage(GXlogo);
	if (Settings.wsprompt) GXlogoImg->SetWidescreen(Settings.Widescreen);
	GXlogoImg->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	GXlogoImg->SetPosition(172, 152);
	
	BackgroundImg = new GuiImage(640, 480, thColor("r=0 g=0 b=0 a=255 - screensaver background color"));
	BackgroundImg->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	BackgroundImg->SetPosition(0, 0);
	
	Append(BackgroundImg);
	Append(GXlogoImg);

	mainWindow->SetState(STATE_DISABLED);
	this->SetState(STATE_DEFAULT);
	SetEffect(EFFECT_FADE, 50);
}

ScreenSaver::~ScreenSaver()
{
	ResumeGui();
	SetEffect(EFFECT_FADE, -50);
	while(this->GetEffect() > 0) usleep(100);

	HaltGui();
	if(parentElement)
		((GuiWindow *) parentElement)->Remove(this);

	RemoveAll();

	delete GXlogo;
	
	delete GXlogoImg;
	delete BackgroundImg;
	
	mainWindow->SetState(STATE_DEFAULT);
	ResumeGui();
}

bool ScreenSaver::GetButtonPressed()
{
	if ( !ControlActivityTimeout() )
		return true;
	
	if (!sr)
	{
		srand(time(0));
		sr=true;
	}
	
	//! Set random position
	GXlogoImg->SetPosition((rand() % 345), (rand() % 305));
	
	sleep(4);
	
	return false;
}
