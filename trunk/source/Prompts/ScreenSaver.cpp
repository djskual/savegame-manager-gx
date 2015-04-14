/****************************************************************************
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
 * ScreenSaver.cpp
 *
 * for SaveGame Manager GX 2012
 ***************************************************************************/
#include <unistd.h>

#include "ScreenSaver.hpp"
#include "../Controls/Application.h"
#include "../Settings/CSettings.h"
#include "../Themes/CTheme.h"
#include "../System/input.h"

ScreenSaver::ScreenSaver()
	: GuiFrame(screenwidth, screenheight)
{
	srand(0);
	
	GXlogo = Resources::GetImageData("logo_SGMGX.png");
	GXlogoImg = new GuiImage(GXlogo);
	GXlogoImg->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	GXlogoImg->SetPosition(172, 152);
	
	BackgroundImg = new GuiImage(screenwidth, screenheight, thColor("r=0 g=0 b=0 a=255 - screensaver background color"));
	BackgroundImg->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	
	this->Append(BackgroundImg);
	this->Append(GXlogoImg);
	
	Application::Instance()->SetUpdateOnly(this);
	Application::Instance()->SetScreenSaver(false);
	SetEffect(EFFECT_FADE, 50);
}

ScreenSaver::~ScreenSaver()
{
	RemoveAll();

	delete GXlogoImg;
	delete BackgroundImg;
	
	Resources::Remove(GXlogo);
	
	Application::Instance()->SetScreenSaver(true);
}

void ScreenSaver::quit()
{
	SetEffect(EFFECT_FADE, -50);
	while(this->IsAnimated()) Application::Instance()->updateEvents();
	
	this->Close();
}

void ScreenSaver::Draw()
{
	if (!ControlActivityTimeout() && !this->IsAnimated())
	{
		quit();
	}
	else
	{
		if(timer.elapsed() > 4)
		{
			GXlogoImg->SetPosition((rand() % 345), (rand() % 305));
			timer.reset();
		}
	}
	
	GuiFrame::Draw();
}