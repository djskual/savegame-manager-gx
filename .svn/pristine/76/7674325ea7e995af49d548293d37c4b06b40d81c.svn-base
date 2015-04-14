/****************************************************************************
 * Copyright (C) 2011
 * by Dimok
 * modified by dj_skual
 * for SaveGame Manager GX 2011
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
 ***************************************************************************/
#include <unistd.h>

#include "HomebrewPrompt.hpp"
#include "../Themes/gettheme.h"
#include "../Themes/Resources.h"
#include "../Settings/CSettings.h"
#include "../Language/gettext.h"
#include "../Tools/StringTools.h"
#include "../menu.h"

HomebrewPrompt::HomebrewPrompt(const char *name,
							   const char *coder,
							   const char *version,
							   const char *release_date,
							   const char *long_description,
							   GuiImageData * iconImgData,
							   u64 filesize)
		: PromptWindow(NULL, NULL)
{
	SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	SetPosition(0, 6);

	trigU = new GuiTrigger();
	trigU->SetButtonOnlyTrigger(-1, WiiControls.UpButton | ClassicControls.UpButton << 16, GCControls.UpButton);
	trigD = new GuiTrigger();
	trigD->SetButtonOnlyTrigger(-1, WiiControls.DownButton | ClassicControls.DownButton << 16, GCControls.DownButton);
	trigL = new GuiTrigger();
	trigL->SetButtonOnlyTrigger(-1, WiiControls.LeftButton | ClassicControls.LeftButton << 16, GCControls.LeftButton);
	trigR = new GuiTrigger();
	trigR->SetButtonOnlyTrigger(-1, WiiControls.RightButton | ClassicControls.RightButton << 16, GCControls.RightButton);
	
	btnUp = new GuiButton(0, 0);
	btnUp->SetTrigger(trigU);
	btnDown = new GuiButton(0, 0);
	btnDown->SetTrigger(trigD);
	btnLeft = new GuiButton(0, 0);
	btnLeft->SetTrigger(trigL);
	btnRight = new GuiButton(0, 0);
	btnRight->SetTrigger(trigR);
	
	whiteBox = Resources::GetImageData("browser.png");
	
	iconImg = new GuiImage(iconImgData);
	iconImg->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	iconImg->SetPosition(45, 10);

	dialogBoxImg->SetSkew(0, -80, 0, -80, 0, 50, 0, 50);
	dialogBoxImg->SetWidescreen(false);

	whiteBoxImg = new GuiImage(whiteBox);
	whiteBoxImg->SetPosition(30, 110);
	whiteBoxImg->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	whiteBoxImg->SetSkew(0, 0, -210, 0, -210, -114, 0, -114);

	nameTxt = new GuiText(name, 30, thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
	nameTxt->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	nameTxt->SetPosition(0, -25);
	nameTxt->SetMaxWidth(430, SCROLL_HORIZONTAL);

	coderTxt = new GuiText(fmt(tr( "Coded by: %s" ), coder), 16, thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
	coderTxt->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	coderTxt->SetPosition(180, 30);
	coderTxt->SetMaxWidth(280);

	versionTxt = new GuiText(fmt(tr( "Version: %s" ), version), 16, thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
	versionTxt->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	versionTxt->SetPosition(40, 65);
	versionTxt->SetMaxWidth(430);

	release_dateTxt = new GuiText(release_date, 16, thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
	release_dateTxt->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	release_dateTxt->SetPosition(40, 85);
	release_dateTxt->SetMaxWidth(430);

	const int pagesize = 6;
	long_descriptionTxt = new Text(long_description, 20, thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
	long_descriptionTxt->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	long_descriptionTxt->SetPosition(46, 117);
	long_descriptionTxt->SetMaxWidth(358);
	long_descriptionTxt->SetLinesToDraw(pagesize);
	long_descriptionTxt->Refresh();

	//!convert filesize from u64 to char and put unit of measurement after it
	char filesizeCH[15];
	if (filesize <= 1024.0)
		snprintf(filesizeCH, sizeof(filesizeCH), "%lld B", filesize);
	if (filesize > 1024.0)
		snprintf(filesizeCH, sizeof(filesizeCH), "%0.2f KB", filesize / 1024.0);
	if (filesize > 1048576.0)
		snprintf(filesizeCH, sizeof(filesizeCH), "%0.2f MB", filesize / 1048576.0);

	filesizeTxt = new GuiText(filesizeCH, 16, thColor("r=0 g=0 b=0 a=255 - prompt windows text color"));
	filesizeTxt->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	filesizeTxt->SetPosition(-40, 12);

	int total_lines = long_descriptionTxt->GetTotalLinesCount();
	scrollBar = new Scrollbar(150, LISTMODE);
	scrollBar->SetParent(this);
	scrollBar->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	scrollBar->SetPosition(-28, 115);
	scrollBar->SetEntrieCount(total_lines-pagesize+1);
	scrollBar->SetPageSize(pagesize);
	scrollBar->SetRowSize(0);
	scrollBar->SetScrollSpeed(5);
	if (total_lines)
		scrollBar->SetListChanged(true);
	else
	{
		scrollBar->SetListChanged(false);
		scrollBar->SetState(STATE_DISABLED);
	}
	
	Append(whiteBoxImg);
	Append(scrollBar);
	Append(btnUp);
	Append(btnDown);
	Append(btnLeft);
	Append(btnRight);

	if(strcmp(name, "") != 0) Append(nameTxt);
	if(strcmp(version, "") != 0) Append(versionTxt);
	if(strcmp(coder, "") != 0) Append(coderTxt);
	if(strcmp(release_date, "") != 0) Append(release_dateTxt);
	if(strcmp(long_description, "") != 0) Append(long_descriptionTxt);
	Append(filesizeTxt);
	Append(iconImg);

	AddButton(tr( "Load" ));
	AddButton(tr( "Back" ));

	Button[0]->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	Button[0]->SetPosition(40, 2);
	Button[1]->SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
	Button[1]->SetPosition(-40, 2);
}

HomebrewPrompt::~HomebrewPrompt()
{
	ResumeGui();

	SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_OUT, 50);
	while(parentElement && this->GetEffect() > 0) usleep(100);

	if(parentElement)
		((GuiWindow *) parentElement)->Remove(this);
	parentElement = NULL;

	RemoveAll();
	
	delete btnUp;
	delete btnDown;
	delete btnLeft;
	delete btnRight;
	delete trigU;
	delete trigD;
	delete trigL;
	delete trigR;

	delete whiteBox;
	delete iconImg;
	delete whiteBoxImg;
	delete nameTxt;
	delete coderTxt;
	delete versionTxt;
	delete release_dateTxt;
	delete long_descriptionTxt;
	delete filesizeTxt;
	delete scrollBar;
}

void HomebrewPrompt::onListChange()
{
	if(btnUp->GetState() == STATE_CLICKED)
	{
		btnUp->ResetState();
		
		long_descriptionTxt->PreviousLine();
	}
	
	if(btnDown->GetState() == STATE_CLICKED)
	{
		btnDown->ResetState();
		
		long_descriptionTxt->NextLine();
	}
	
	if(btnLeft->GetState() == STATE_CLICKED)
	{
		btnLeft->ResetState();
		
		for(int i = 0; i < 6; i++)
			long_descriptionTxt->PreviousLine();
	}
	
	if(btnRight->GetState() == STATE_CLICKED)
	{
		btnRight->ResetState();
		
		for(int i = 0; i < 6; i++)
			long_descriptionTxt->NextLine();
	}
	
	if(scrollBar->ListChanged())
		long_descriptionTxt->SetTextLine(scrollBar->GetSelectedItem()+scrollBar->GetSelectedIndex());
	
	scrollBar->SetSelectedItem(0);
	scrollBar->SetSelectedIndex(long_descriptionTxt->GetCurrPos());
}

int HomebrewPrompt::MainLoop()
{
	int choice = -1;

	while (choice == -1)
	{
		usleep(100);
		
		choice = GetChoice();
		if(choice == -1) onListChange();
	}

	return choice;
}
