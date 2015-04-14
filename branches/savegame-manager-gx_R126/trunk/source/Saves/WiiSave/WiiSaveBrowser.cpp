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
 * WiiSaveBrowser.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include <unistd.h>

#include "WiiSaveBrowser.hpp"
#include "../../Themes/CTheme.h"
#include "../../Tools/StringTools.h"
#include "../../Settings/CSettings.h"
#include "../../Language/gettext.h"
#include "../../Menu/menus.h"

#define NONE			0
#define CHANGE			1
#define IN              2
#define OUT             3

WiiSaveBrowser::WiiSaveBrowser(WiiSave_List * savelist)
	: GuiWindow(400, 190)
{
	returnVal = -2;
	saveSelected = 0;
	SaveList = savelist;
	
	SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	SetPosition(0, 65);

	imgDialogBox = Resources::GetImageData("browser.png");
	imgLine = Resources::GetImageData("browser_separator.png");
	imgButtonBox = Resources::GetImageData("manage_save_button_box.png");
	imgClose = Resources::GetImageData("button_close.png");
	imgCloseOver = Resources::GetImageData("button_close_over.png");
	imgDownload = Resources::GetImageData("button_valid.png");
	imgDownloadOver = Resources::GetImageData("button_valid_over.png");
	imgNext = Resources::GetImageData("button_plus.png");
	imgNextOver = Resources::GetImageData("button_plus_over.png");
	imgPrevious = Resources::GetImageData("button_minus.png");
	imgPreviousOver = Resources::GetImageData("button_minus_over.png");

	trigA = new SimpleGuiTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	trigB = new GuiTrigger();
	trigB->SetButtonOnlyTrigger(-1, WiiControls.BackButton | ClassicControls.BackButton << 16, GCControls.BackButton);
	trigU = new GuiTrigger();
	trigU->SetButtonOnlyTrigger(-1, WiiControls.UpButton | ClassicControls.UpButton << 16, GCControls.UpButton);
	trigD = new GuiTrigger();
	trigD->SetButtonOnlyTrigger(-1, WiiControls.DownButton | ClassicControls.DownButton << 16, GCControls.DownButton);
	trigL = new GuiTrigger();
	trigL->SetButtonOnlyTrigger(-1, WiiControls.LeftButton | ClassicControls.LeftButton << 16, GCControls.LeftButton);
	trigR = new GuiTrigger();
	trigR->SetButtonOnlyTrigger(-1, WiiControls.RightButton | ClassicControls.RightButton << 16, GCControls.RightButton);
	trigPlus = new GuiTrigger();
	trigPlus->SetButtonOnlyTrigger(-1, WiiControls.NextButton | ClassicControls.NextButton << 16, 0);
	trigMinus = new GuiTrigger();
	trigMinus->SetButtonOnlyTrigger(-1, WiiControls.PrevButton | ClassicControls.PrevButton << 16, 0);
	

	dialogBoxImg = new GuiImage(imgDialogBox);
	dialogBoxImg->SetSkew(0, 0, -224, 0, -224, -86, 0, -86);
	
	lineImg = new GuiImage(imgLine);
	lineImg->SetSkew(0, 0, 0, 0, 0, 60, 0, 60);
	lineImg->SetAngle(-90);
	lineImg->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	lineImg->SetPosition(138, -100);
	
	nameTxt = new GuiText("", 22, thColor("r=0 g=0 b=0 a=255 - download save window text color"));
	nameTxt->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	nameTxt->SetPosition(-20, 10);
	nameTxt->SetMaxWidth(330, SCROLL_HORIZONTAL);
	
	synopsisTxt = new Text((char*)NULL, 18, thColor("r=0 g=0 b=0 a=255 - download save window text color"));
	synopsisTxt->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	synopsisTxt->SetPosition(10, 46);
	synopsisTxt->SetMaxWidth(340);
	synopsisTxt->SetLinesToDraw(6);
	synopsisTxt->Refresh();

	numberTxt = new GuiText("", 18, thColor("r=255 g=255 b=255 a=255 - manage windows text color"));
	numberTxt->SetAlignment(ALIGN_LEFT, ALIGN_BOTTOM);
	numberTxt->SetPosition(370, -4);
	
	btnUp = new GuiButton(0, 0);
	btnUp->SetTrigger(trigU);
	btnDown = new GuiButton(0, 0);
	btnDown->SetTrigger(trigD);
	
	closeBoxImg = new GuiImage(imgButtonBox);
	closeBoxImg->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	closeBoxImg->SetPosition(0, 0);
	downloadBoxImg = new GuiImage(imgButtonBox);
	downloadBoxImg->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	downloadBoxImg->SetPosition(0, 40);
	previousBoxImg = new GuiImage(imgButtonBox);
	previousBoxImg->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	previousBoxImg->SetPosition(0, 80);
	nextBoxImg = new GuiImage(imgButtonBox);
	nextBoxImg->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	nextBoxImg->SetPosition(0, 120);
	
	btnCloseImg = new GuiImage(imgClose);
	btnCloseImgOver = new GuiImage(imgCloseOver);
	btnClose = new GuiButton(btnCloseImg, btnCloseImgOver, ALIGN_RIGHT, ALIGN_TOP, -3, 3, trigA, btnSoundOver, btnSoundClick, 1);
	btnClose->SetTrigger(trigB);
	
	btnDownloadImg = new GuiImage(imgDownload);
	btnDownloadImgOver = new GuiImage(imgDownloadOver);
	btnDownload = new GuiButton(btnDownloadImg, btnDownloadImgOver, ALIGN_RIGHT, ALIGN_TOP, -3, 43, trigA, btnSoundOver, btnSoundClick, 1);
	
	btnPreviousImg = new GuiImage(imgPrevious);
	btnPreviousImgOver = new GuiImage(imgPreviousOver);
	btnPrevious = new GuiButton(btnPreviousImg, btnPreviousImgOver, ALIGN_RIGHT, ALIGN_TOP, -3, 83, trigA, btnSoundOver, btnSoundClick, 1);
	btnPrevious->SetTrigger(trigL);
	btnPrevious->SetTrigger(trigMinus);
	
	btnNextImg = new GuiImage(imgNext);
	btnNextImgOver = new GuiImage(imgNextOver);
	btnNext = new GuiButton(btnNextImg, btnNextImgOver, ALIGN_RIGHT, ALIGN_TOP, -3, 123, trigA, btnSoundOver, btnSoundClick, 1);
	btnNext->SetTrigger(trigR);
	btnNext->SetTrigger(trigPlus);

	Append(dialogBoxImg);
	Append(lineImg);
	Append(closeBoxImg);
	Append(btnClose);
	Append(btnUp);
	Append(btnDown);
	Append(downloadBoxImg);
	Append(btnDownload);
	Append(nameTxt);
	Append(synopsisTxt);
	Append(numberTxt);
	if (SaveList->GetFilteredCount() > 1)
	{
		Append(previousBoxImg);
		Append(btnPrevious);
		Append(nextBoxImg);
		Append(btnNext);
	}
	
	ChangeSave(NONE);
	
	SetEffect(EFFECT_FADE, 20);
}

WiiSaveBrowser::~WiiSaveBrowser()
{
	StopEffect();
	SetEffect(EFFECT_FADE, -20);
	ResumeGui();

	while(parentElement && this->GetEffect() > 0) usleep(100);

	HaltGui();

	if(parentElement)
		((GuiWindow * ) parentElement)->Remove(this);

	RemoveAll();

	delete trigA;
	delete trigB;
	delete trigL;
	delete trigR;
	delete trigU;
	delete trigD;
	delete trigPlus;
	delete trigMinus;

	delete imgDialogBox;
	delete imgLine;
	delete imgButtonBox;
	delete imgClose;
	delete imgCloseOver;
	delete imgDownload;
	delete imgDownloadOver;
	delete imgNext;
	delete imgNextOver;
	delete imgPrevious;
	delete imgPreviousOver;

	delete dialogBoxImg;
	delete lineImg;
	delete closeBoxImg;
	delete btnCloseImg;
	delete btnCloseImgOver;
	delete downloadBoxImg;
	delete btnDownloadImg;
	delete btnDownloadImgOver;
	delete nextBoxImg;
	delete btnNextImg;
	delete btnNextImgOver;
	delete previousBoxImg;
	delete btnPreviousImg;
	delete btnPreviousImgOver;
	
	delete nameTxt;
	delete numberTxt;
	delete synopsisTxt;
	
	delete btnClose;
	delete btnDownload;
	delete btnNext;
	delete btnPrevious;
	delete btnUp;
	delete btnDown;
	
	ResumeGui();
}

void WiiSaveBrowser::SetWindowEffect(int change)
{
	if(change == CHANGE)
	{
		nameTxt->StopEffect();
		synopsisTxt->StopEffect();
		numberTxt->StopEffect();
	}
	
	ResumeGui();
	
	while(parentElement && (this->GetEffect() > 0 || nameTxt->GetEffect() > 0)) usleep(100);
}

void WiiSaveBrowser::ChangeSave(int Change)
{
	SetWindowEffect(Change);
	
	HaltGui();
	
	nameTxt->SetText(SaveList->GetFilteredTitle(saveSelected));
	numberTxt->SetText(fmt("%d/%d", saveSelected+1, SaveList->GetFilteredCount()));
	synopsisTxt->SetText(SaveList->GetFilteredSynopsis(saveSelected).c_str());
	synopsisTxt->Refresh();
	
	SetWindowEffect(Change);
}

int WiiSaveBrowser::Show()
{
	int choice = -2;

	while(choice == -2)
	{
		usleep(100);
		
		choice = MainLoop();
	}

	return choice;
}

int WiiSaveBrowser::MainLoop()
{
	if (btnClose->GetState() == STATE_CLICKED) //! close
	{
		saveSelected = -1;
		returnVal = -1;
	}

	else if (btnNext->GetState() == STATE_CLICKED) //! next save
	{
		saveSelected++;
		if (saveSelected >= SaveList->GetFilteredCount())
			saveSelected = 0;
		
		ChangeSave(CHANGE);
		
		btnNext->ResetState();
	}

	else if (btnPrevious->GetState() == STATE_CLICKED) //! previous save
	{
		saveSelected--;
		if (saveSelected < 0)
			saveSelected = SaveList->GetFilteredCount()-1;
		
		ChangeSave(CHANGE);
		
		btnPrevious->ResetState();
	}
	
	else if (btnUp->GetState() == STATE_CLICKED) //! previous line in synopsys text
	{
		btnUp->ResetState();
		
		synopsisTxt->PreviousLine();
	}
	
	else if (btnDown->GetState() == STATE_CLICKED) //! next line in synopsys text
	{
		btnDown->ResetState();
		
		synopsisTxt->NextLine();
	}
	
	else if (btnDownload->GetState() == STATE_CLICKED) //! download save
	{
		returnVal = saveSelected;
	}
	
	return returnVal;
}
