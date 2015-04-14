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
#include "ProgressBar.h"
#include "../Themes/CTheme.h"

ProgressBar::ProgressBar()
	: GuiFrame(400, 40)
{
	barOutline = Resources::GetImageData("progress_bar_outline.png");
	barEmpty = Resources::GetImageData("progress_bar_empty.png");
	bar = Resources::GetImageData("progress_bar.png");
	
	barOutlineImg = new GuiImage(barOutline);
	barEmptyImg = new GuiImage(barEmpty);
	barImg = new GuiImage(bar);
	
	barImg->SetTileHorizontal(0);
	barEmptyImg->SetTileHorizontal(100);

	//int OutLineMiddle = barOutline->GetHeight()/2;

	prTxt = new GuiText("0.00", thInt("20 - progressbar text size"), thColor("r=0 g=0 b=0 a=255 - progressbar text color"));
	prTxt->SetAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
	prTxt->SetPosition(180, 0);
	/*prTxt->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	prTxt->SetPosition(0.45f*width, OutLineMiddle-9);*/

	prsTxt = new GuiText("%", thInt("20 - progressbar text size"), thColor("r=0 g=0 b=0 a=255 - progressbar text color"));
	prsTxt->SetAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
	prsTxt->SetPosition(240, 0);
	/*prsTxt->SetAlignment(ALIGN_LEFT | ALIGN_TOP);
	prsTxt->SetPosition(0.6f*width, OutLineMiddle-9);*/

	Append(barEmptyImg);
	Append(barImg);
	Append(barOutlineImg);
	Append(prsTxt);
	Append(prTxt);
}

ProgressBar::~ProgressBar()
{
	RemoveAll();
	
	Resources::Remove(barOutline);
	Resources::Remove(barEmpty);
	Resources::Remove(bar);
	
	delete barOutlineImg;
	delete barEmptyImg;
	delete barImg;
	
	delete prTxt;
	delete prsTxt;
}

void ProgressBar::SetPercent(float Percent)
{
	if(Percent < 0.0f)
	{
		prTxt->SetText("0.00");
		barImg->SetTileHorizontal(0);
	}
	else if(Percent < 100.0f)
	{
		prTxt->SetTextf("%0.2f", Percent);
		barImg->SetTileHorizontal(Percent);
	}
	else
	{
		prTxt->SetText("100");
		barImg->SetTileHorizontal(100);
	}
}
