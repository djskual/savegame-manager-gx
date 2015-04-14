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
#include <time.h>

#include "Clock.h"
#include "../Controls/Application.h"
#include "../Themes/CTheme.h"
#include "../System/Video.h"

Clock::Clock()
	: GuiFrame(screenwidth, screenheight)
{
	GXColor clockColor = thColor("r=138 g=138 b=138 a=240 - clock color");
	float clockFontScaleFactor = 0;
	if(thFloat("1.0 - Overrided clock scale factor. 1.0=allow user setting") != 1.0f)
		clockFontScaleFactor = thFloat("1.0 - Overrided clock scale factor. 1.0=allow user setting");
	else
		clockFontScaleFactor = Settings.ClockFontScaleFactor;
	
 	timeTxtBack = new GuiText("88:88", 40 / Settings.FontScaleFactor * clockFontScaleFactor, (GXColor) {clockColor.r, clockColor.g, clockColor.b, clockColor.a / 6}); 
	timeTxtBack->SetAlignment(thAlign("left - clock align hor") | thAlign("top - clock align ver"));
	timeTxtBack->SetPosition(thInt("275 - clock pos x"), thInt("340 - clock pos y"));
	timeTxtBack->SetFont(Resources::GetFile("clock.ttf"), Resources::GetFileSize("clock.ttf"));
	
	timeTxt = new GuiText("", 40 / Settings.FontScaleFactor * clockFontScaleFactor, clockColor);
	timeTxt->SetAlignment(thAlign("left - clock align hor") | thAlign("top - clock align ver"));
	timeTxt->SetPosition(thInt("275 - clock pos x"), thInt("340 - clock pos y"));
	timeTxt->SetFont(Resources::GetFile("clock.ttf"), Resources::GetFileSize("clock.ttf"));

	Append(timeTxtBack);
	Append(timeTxt);
}

Clock::~Clock()
{
	RemoveAll();
	
	delete timeTxt;
	delete timeTxtBack;
}

void Clock::Draw()
{
	if(!this->IsVisible() || (Settings.Clock != CLOCK_HR12 && Settings.Clock != CLOCK_HR24))
		return;

	if(frameCount %(Settings.PAL50 ? 50 : 60) == 0) //! Update time value every sec
	{
		char timetxt[20];
		timetxt[0] = 0;
		time_t currenttime = time(0);
		struct tm * timeinfo = localtime(&currenttime);
		
		if(Settings.Clock == CLOCK_HR24)
		{
			if(currenttime & 1)
				strftime(timetxt, sizeof(timetxt), "%H %M", timeinfo);
			else
				strftime(timetxt, sizeof(timetxt), "%H:%M", timeinfo);
		}
		else
		{
			if(currenttime & 1)
				strftime(timetxt, sizeof(timetxt), "%I %M", timeinfo);
			else
				strftime(timetxt, sizeof(timetxt), "%I:%M", timeinfo);
		}
		
		timeTxt->SetText(timetxt);
	}

	GuiFrame::Draw();
}
