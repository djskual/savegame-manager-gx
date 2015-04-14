/***************************************************************************
 * Copyright (C) 2009
 * by Dimok for WiiXplorer
 * modified by dj_skual
 * for SaveGame Manager GX 2012
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
 * DeviceMenu.cpp
 *
 ***************************************************************************/
#include "DeviceMenu.h"
#include "../Themes/CTheme.h"
#include "../Language/gettext.h"
#include "../Settings/CSettings.h"
#include "../menu.h"

DeviceMenu::DeviceMenu(int x, int y)
	:GuiWindow(0, 0)
{
	int PositionY = 19;
	int FontSize = thInt("17 - device menu text size");
	deviceCount = 0;
	choice = -1;

	//! Menu imagedata
	device_choose_right_Data = Resources::GetImageData("device_choose_right.png");
	device_choose_left_Data = Resources::GetImageData("device_choose_left.png");
	device_choose_center_Data = Resources::GetImageData("device_choose_center.png");
	menu_select = Resources::GetImageData("deviceselection.png");

	//! Device imagedata
	sd_ImgData = Resources::GetImageData("device_choose_sd.png");
	usb_ImgData = Resources::GetImageData("device_choose_usb.png");
	wii_ImgData = Resources::GetImageData("device_choose_wii.png");
	mii_ImgData = Resources::GetImageData("device_choose_mii.png");
	emu_ImgData = Resources::GetImageData("device_choose_emu.png");
	emu_mii_ImgData = Resources::GetImageData("device_choose_emu_mii.png");

	//! Menu images
	centerImg = new GuiImage(device_choose_center_Data);
	leftImg = new GuiImage(device_choose_left_Data);
	rightImg = new GuiImage(device_choose_right_Data);

	trigA = new SimpleGuiTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);
	trigB = new GuiTrigger();
	trigB->SetButtonOnlyTrigger(-1, WiiControls.BackButton | ClassicControls.BackButton << 16, GCControls.BackButton);
	trigHome = new GuiTrigger();
	trigHome->SetButtonOnlyTrigger(-1, WiiControls.HomeButton | ClassicControls.HomeButton << 16, GCControls.HomeButton);
	
	for(int i = 0; i < MAXDEVICES; i++)
	{
		deviceBtn[i] = NULL;
		deviceImgs[i] = NULL;
		deviceImgOver[i] = NULL;
		deviceText[i] = NULL;
		deviceSelection[i] = -1;
	}

	int PositionX = leftImg->GetWidth();

	if(DeviceHandler::Instance()->IsInserted(SD))
	{
		deviceText[deviceCount] = new GuiText(DeviceName[SD], FontSize, thColor("r=0 g=0 b=0 a=255 - device menu text color"));
		deviceText[deviceCount]->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
		deviceText[deviceCount]->SetPosition(0, 2);
		
		deviceImgs[deviceCount] = new GuiImage(sd_ImgData);
		deviceImgs[deviceCount]->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
		deviceImgOver[deviceCount] = new GuiImage(menu_select);
		deviceImgOver[deviceCount]->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
		
		deviceBtn[deviceCount] = new GuiButton(deviceImgs[deviceCount]->GetWidth(), deviceImgs[deviceCount]->GetHeight()+FontSize);
		deviceBtn[deviceCount]->SetLabel(deviceText[deviceCount]);
		deviceBtn[deviceCount]->SetSoundClick(btnSoundClick);
		deviceBtn[deviceCount]->SetIcon(deviceImgs[deviceCount]);
		deviceBtn[deviceCount]->SetImageOver(deviceImgOver[deviceCount]);
		deviceBtn[deviceCount]->SetTrigger(trigA);
		deviceBtn[deviceCount]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
		deviceBtn[deviceCount]->SetPosition(PositionX, PositionY);
		deviceBtn[deviceCount]->Clicked.connect(this, &DeviceMenu::OnButtonClick);
		PositionX += deviceImgs[deviceCount]->GetWidth()+10;
		
		deviceSelection[deviceCount] = SD;
		
		deviceCount++;
	}
	
	int j = 0;
	for(j = USB1; j <= USB8; j++)
	{
		if(DeviceHandler::Instance()->IsInserted(j))
		{
			deviceText[deviceCount] = new GuiText(DeviceName[j], FontSize, thColor("r=0 g=0 b=0 a=255 - device menu text color"));
			deviceText[deviceCount]->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
			deviceText[deviceCount]->SetPosition(0, 2);
			
			deviceImgs[deviceCount] = new GuiImage(usb_ImgData);
			deviceImgs[deviceCount]->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
			deviceImgOver[deviceCount] = new GuiImage(menu_select);
			deviceImgOver[deviceCount]->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
			
			deviceBtn[deviceCount] = new GuiButton(deviceImgs[deviceCount]->GetWidth(), deviceImgs[deviceCount]->GetHeight()+FontSize);
			deviceBtn[deviceCount]->SetLabel(deviceText[deviceCount]);
			deviceBtn[deviceCount]->SetSoundClick(btnSoundClick);
			deviceBtn[deviceCount]->SetIcon(deviceImgs[deviceCount]);
			deviceBtn[deviceCount]->SetImageOver(deviceImgOver[deviceCount]);
			deviceBtn[deviceCount]->SetTrigger(trigA);
			deviceBtn[deviceCount]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
			deviceBtn[deviceCount]->SetPosition(PositionX, PositionY);
			deviceBtn[deviceCount]->Clicked.connect(this, &DeviceMenu::OnButtonClick);
			PositionX += deviceImgs[deviceCount]->GetWidth()+10;
			
			deviceSelection[deviceCount] = j;
			
			deviceCount++;
		}
	}

	deviceText[deviceCount] = new GuiText(tr("Wii"), FontSize, thColor("r=0 g=0 b=0 a=255 - device menu text color"));
	deviceText[deviceCount]->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	deviceText[deviceCount]->SetPosition(0, 2);
	
	deviceImgs[deviceCount] = new GuiImage(wii_ImgData);
	deviceImgs[deviceCount]->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	deviceImgOver[deviceCount] = new GuiImage(menu_select);
	deviceImgOver[deviceCount]->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	
	deviceBtn[deviceCount] = new GuiButton(deviceImgs[deviceCount]->GetWidth(), deviceImgs[deviceCount]->GetHeight()+FontSize);
	deviceBtn[deviceCount]->SetLabel(deviceText[deviceCount]);
	deviceBtn[deviceCount]->SetSoundClick(btnSoundClick);
	deviceBtn[deviceCount]->SetIcon(deviceImgs[deviceCount]);
	deviceBtn[deviceCount]->SetImageOver(deviceImgOver[deviceCount]);
	deviceBtn[deviceCount]->SetTrigger(trigA);
	deviceBtn[deviceCount]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	deviceBtn[deviceCount]->SetPosition(PositionX, PositionY);
	deviceBtn[deviceCount]->Clicked.connect(this, &DeviceMenu::OnButtonClick);
	PositionX += deviceImgs[deviceCount]->GetWidth()+10;
	
	deviceSelection[deviceCount] = WII;
	
	deviceCount++;
	
	deviceText[deviceCount] = new GuiText(tr("Mii"), FontSize, thColor("r=0 g=0 b=0 a=255 - device menu text color"));
	deviceText[deviceCount]->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
	deviceText[deviceCount]->SetPosition(0, 2);
	
	deviceImgs[deviceCount] = new GuiImage(mii_ImgData);
	deviceImgs[deviceCount]->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	deviceImgOver[deviceCount] = new GuiImage(menu_select);
	deviceImgOver[deviceCount]->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	
	deviceBtn[deviceCount] = new GuiButton(deviceImgs[deviceCount]->GetWidth(), deviceImgs[deviceCount]->GetHeight()+FontSize);
	deviceBtn[deviceCount]->SetLabel(deviceText[deviceCount]);
	deviceBtn[deviceCount]->SetSoundClick(btnSoundClick);
	deviceBtn[deviceCount]->SetIcon(deviceImgs[deviceCount]);
	deviceBtn[deviceCount]->SetImageOver(deviceImgOver[deviceCount]);
	deviceBtn[deviceCount]->SetTrigger(trigA);
	deviceBtn[deviceCount]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	deviceBtn[deviceCount]->SetPosition(PositionX, PositionY);
	deviceBtn[deviceCount]->Clicked.connect(this, &DeviceMenu::OnButtonClick);
	PositionX += deviceImgs[deviceCount]->GetWidth()+10;
	
	deviceSelection[deviceCount] = MII;
	
	deviceCount++;
	
	if(!Settings.Sneek && Settings.EmuNand)
	{
		deviceText[deviceCount] = new GuiText(tr("Emu"), FontSize, thColor("r=0 g=0 b=0 a=255 - device menu text color"));
		deviceText[deviceCount]->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
		deviceText[deviceCount]->SetPosition(0, 2);
		
		deviceImgs[deviceCount] = new GuiImage(emu_ImgData);
		deviceImgs[deviceCount]->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
		deviceImgOver[deviceCount] = new GuiImage(menu_select);
		deviceImgOver[deviceCount]->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
		
		deviceBtn[deviceCount] = new GuiButton(deviceImgs[deviceCount]->GetWidth(), deviceImgs[deviceCount]->GetHeight()+FontSize);
		deviceBtn[deviceCount]->SetLabel(deviceText[deviceCount]);
		deviceBtn[deviceCount]->SetSoundClick(btnSoundClick);
		deviceBtn[deviceCount]->SetIcon(deviceImgs[deviceCount]);
		deviceBtn[deviceCount]->SetImageOver(deviceImgOver[deviceCount]);
		deviceBtn[deviceCount]->SetTrigger(trigA);
		deviceBtn[deviceCount]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
		deviceBtn[deviceCount]->SetPosition(PositionX, PositionY);
		deviceBtn[deviceCount]->Clicked.connect(this, &DeviceMenu::OnButtonClick);
		PositionX += deviceImgs[deviceCount]->GetWidth()+10;
		
		deviceSelection[deviceCount] = EMU;
		
		deviceCount++;
		
		deviceText[deviceCount] = new GuiText(tr("EmuMii"), FontSize, thColor("r=0 g=0 b=0 a=255 - device menu text color"));
		deviceText[deviceCount]->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
		deviceText[deviceCount]->SetPosition(0, 2);
		
		deviceImgs[deviceCount] = new GuiImage(emu_mii_ImgData);
		deviceImgs[deviceCount]->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
		deviceImgOver[deviceCount] = new GuiImage(menu_select);
		deviceImgOver[deviceCount]->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
		
		deviceBtn[deviceCount] = new GuiButton(deviceImgs[deviceCount]->GetWidth(), deviceImgs[deviceCount]->GetHeight()+FontSize);
		deviceBtn[deviceCount]->SetLabel(deviceText[deviceCount]);
		deviceBtn[deviceCount]->SetSoundClick(btnSoundClick);
		deviceBtn[deviceCount]->SetIcon(deviceImgs[deviceCount]);
		deviceBtn[deviceCount]->SetImageOver(deviceImgOver[deviceCount]);
		deviceBtn[deviceCount]->SetTrigger(trigA);
		deviceBtn[deviceCount]->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
		deviceBtn[deviceCount]->SetPosition(PositionX, PositionY);
		deviceBtn[deviceCount]->Clicked.connect(this, &DeviceMenu::OnButtonClick);
		PositionX += deviceImgs[deviceCount]->GetWidth()+10;
		
		deviceSelection[deviceCount] = EMU_MII;
		
		deviceCount++;
	}
	
	//! Set image position and tile
	tile = (PositionX-leftImg->GetWidth())/centerImg->GetWidth();
	
	leftImg->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	leftImg->SetPosition(0, 0);
	
	centerImg->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	centerImg->SetPosition(leftImg->GetLeft()+leftImg->GetWidth(), 0);
	centerImg->SetTileHorizontal(tile);
	
	rightImg->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	rightImg->SetPosition(leftImg->GetWidth() + tile * centerImg->GetWidth(), 0);
	
	width = leftImg->GetWidth() + tile * centerImg->GetWidth() + rightImg->GetWidth();
	height = leftImg->GetHeight();
	
	NoBtn = new GuiButton(screenwidth, screenheight);
	NoBtn->SetPosition(-x, -y);
	NoBtn->SetTrigger(trigA);
	NoBtn->SetTrigger(trigB);
	NoBtn->SetTrigger(trigHome);
	NoBtn->Clicked.connect(this, &DeviceMenu::OnButtonClick);
	
	Append(NoBtn);
	Append(leftImg);
	Append(centerImg);
	Append(rightImg);
	for(int i = 0; i < deviceCount; i++)
		Append(deviceBtn[i]);
	
	SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	SetPosition(x, y);
	SetEffect(EFFECT_FADE, 25);
}

DeviceMenu::~DeviceMenu()
{
	ResumeGui();
	SetEffect(EFFECT_FADE, -30);
	while(this->GetEffect() > 0) usleep(100);
	
	HaltGui();
	if(parentElement)
		((GuiWindow *) parentElement)->Remove(this);
	
	RemoveAll();
	
	delete device_choose_right_Data;
	delete device_choose_left_Data;
	delete device_choose_center_Data;
	delete sd_ImgData;
	delete usb_ImgData;
	delete wii_ImgData;
	delete mii_ImgData;
	delete emu_ImgData;
	delete emu_mii_ImgData;
	delete menu_select;
	
	for(int i = 0; i < MAXDEVICES; i++)
	{
		if(deviceBtn[i]) {
			delete deviceBtn[i];
			deviceBtn[i] = NULL;
		}
		if(deviceImgs[i]) {
			delete deviceImgs[i];
			deviceImgs[i] = NULL;
		}
		if(deviceImgOver[i]) {
			delete deviceImgOver[i];
			deviceImgOver[i] = NULL;
		}
		if(deviceText[i]) {
			delete deviceText[i];
			deviceText[i] = NULL;
		}
	}
	
	delete centerImg;
	delete leftImg;
	delete rightImg;
	
	delete NoBtn;
	
	delete trigA;
	delete trigB;
	
	ResumeGui();
}

int DeviceMenu::GetChoice()
{
	return choice;
}

void DeviceMenu::OnButtonClick(GuiButton *sender, int pointer, const POINT &p)
{
	sender->ResetState();
	
	if(sender == NoBtn)
	{
		choice = -2;
		return;
	}
	
	int i = 0;
	for(i = 0; i < deviceCount; i++)
	{
		if(sender == deviceBtn[i])
		{
			choice = deviceSelection[i];
			break;
		}
	}
}
