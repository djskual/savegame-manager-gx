/***************************************************************************
 * Copyright (C) 2010
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
 * manage_mii.cpp
 *
 * for SaveGame Manager 2010
 ***************************************************************************/

#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

#include "libwiigui/gui.h"
#include "prompts/browsers/browsers.h"
#include "saves/manage/ManageButtons.h"
#include "mii/manage_mii/manage_mii.h"
#include "mii/manage_mii/ManageMiiWindow.h"
#include "mii/mii_list.h"
#include "mii/mii_tools.h"
#include "menu/MainWindow.h"
#include "main.h"

int selectedMii;

/****************************************************************************
* Manage
*
* Shortcut function
***************************************************************************/
void ManageMii(int sel,
			short s)
{
    selectedMii = sel;
	int source = s; //! Wii = 1, Device = Default
	int choice = 0;
	bool changeManage = false;
	bool exitManage = false;
	bool exit = false;
	
	MainWindow::Instance()->SetState(STATE_DISABLED);
	
	while(!exitManage)
	{
		changeManage = false;
		
		if(windowinfo.refresh == true) {
			windowinfo.refresh = false;
			
			if(source == ManageWindow::DEVICE)
			{
				if(windowinfo.refreshType == ManageWindow::DELETE) {
					
					navinfo.firstdevice = true;
					BrowseDevice(navinfo.LastBrowsedPath);
					
					selectedMii--;
					
					if(selectedMii < 0)
						selectedMii = 0;
					if(selectedMii > browserD.numEntries-1)
						selectedMii = browserD.numEntries-1;
					
					if(!browserDevice[selectedMii].isMii) {
						selectedMii++;
						if(selectedMii > browserD.numEntries-1)
							selectedMii = browserD.numEntries-1;
						
						if(!browserDevice[selectedMii].isMii) {
							exitManage = true;
							goto exit;
						}
					}
				}
				
				else if(windowinfo.refreshType == ManageWindow::PREV)
				{
					do{
						selectedMii--;
						if(selectedMii < 0)
							selectedMii = browserD.numEntries-1;
					}while(!browserDevice[selectedMii].isMii);
					
					windowinfo.move = ManageWindow::MOVE_PREV_IN;
					navinfo.LastDeviceItemSelected = selectedMii;
				}
				
				else if(windowinfo.refreshType == ManageWindow::NEXT)
				{
					do{
						selectedMii++;
						if(selectedMii > browserD.numEntries-1)
							selectedMii = 0;
					}while(!browserDevice[selectedMii].isMii);
					
					windowinfo.move = ManageWindow::MOVE_NEXT_IN;
					navinfo.LastDeviceItemSelected = selectedMii;
				}
			}
			
			else if(source == ManageWindow::WII)
			{
				if(windowinfo.refreshType == ManageWindow::DELETE) {
					
					CreateMiiList();
					
					if(!miiListInfo.Cnt) {
						exitManage = true;
						goto exit;
					}
					
					selectedMii--;
					
					if(selectedMii < 0)
						selectedMii = 0;
					if(selectedMii > miiListInfo.Cnt-1)
						selectedMii = miiListInfo.Cnt-1;
				}
				
				if(windowinfo.refreshType == ManageWindow::PREV)
				{
					selectedMii--;
					if(selectedMii < 0)
						selectedMii = miiListInfo.Cnt-1;
					
					windowinfo.move = ManageWindow::MOVE_PREV_IN;
					navinfo.LastMiiSelected = selectedMii;
				}
				
				else if(windowinfo.refreshType == ManageWindow::NEXT)
				{
					selectedMii++;
					if(selectedMii > miiListInfo.Cnt-1)
						selectedMii = 0;
					
					windowinfo.move = ManageWindow::MOVE_NEXT_IN;
					navinfo.LastMiiSelected = selectedMii;
				}
			}
			
			windowinfo.refreshType = ManageWindow::NO_TYPE;
			
		}
		
		char name[31];
		char creator[31];
		int day = 0;
		int month = 0;
		int female = 0;
		int favColor = 0;
		
		if(source == ManageWindow::DEVICE)
		{
			snprintf(name, sizeof(name), "%s", browserDevice[selectedMii].displayname);
			snprintf(creator, sizeof(creator), "%s", browserDevice[selectedMii].miiCreator);			
			day = browserDevice[selectedMii].miiDay;
			month = browserDevice[selectedMii].miiMonth;
			female = browserDevice[selectedMii].miiFemale;
			favColor = browserDevice[selectedMii].miiFavColor;
			
		}
		else if(source == ManageWindow::WII)
		{
			snprintf(name, sizeof(name), "%s", miiList[selectedMii].displayname);
			snprintf(creator, sizeof(creator), "%s",  miiList[selectedMii].creator);
			day = miiList[selectedMii].day;
			month = miiList[selectedMii].month;
			female = miiList[selectedMii].female;
			favColor = miiList[selectedMii].favColor;
			
		}
		
		ManageMiiWindow ManageMii(name, creator, day, month, female, favColor);
		
		ManageButtons Buttons(0,
							  0,
							  tr("Delete"),
							  source ? 0 : tr("Copy"),
							  source ? 0 : tr("Move"),
							  source ? tr("Extract") : tr("Install"));
		
		windowinfo.move = ManageWindow::NO_MOVE_IN;
		
		ManageButtons Confirm(0,
							  tr("Are you sure ?"),
							  tr("Yes"),
							  tr("Go Back"),
							  0,
							  0);
		
		ManageButtons Path(tr("Extract to:"),
						   0,
						   tr("Default"),
						   tr("Browsed"),
						   tr("Custom"),
						   tr("Go Back"));
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Append(&ManageMii);
		MainWindow::Instance()->ChangeFocus(&ManageMii);
		MainWindow::Instance()->Append(&Buttons);
		MainWindow::Instance()->ResumeGui();
		
		while(ManageMii.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		while(!changeManage)
		{
			if(ManageMii.GetState() == STATE_CLICKED)
			{
				choice = ManageMii.GetChoice();
				
				if(choice == 1) { // close
					windowinfo.move = ManageWindow::MOVE_OUT;
					ManageMii.SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_OUT, 50);
					Buttons.SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_OUT, 50);
					exitManage = true;
				}
				else if(choice == 2) { // prev
					windowinfo.move = ManageWindow::MOVE_PREV_OUT;
					windowinfo.refreshType = ManageWindow::PREV;
					windowinfo.refresh = true;
					ManageMii.SetEffect(EFFECT_SLIDE_RIGHT | EFFECT_SLIDE_OUT, 50);
					Buttons.SetEffect(EFFECT_SLIDE_RIGHT | EFFECT_SLIDE_OUT, 50);
				}
				else if(choice == 3) { // next
					windowinfo.move = ManageWindow::MOVE_NEXT_OUT;
					windowinfo.refreshType = ManageWindow::NEXT;
					windowinfo.refresh = true;
					ManageMii.SetEffect(EFFECT_SLIDE_LEFT | EFFECT_SLIDE_OUT, 50);
					Buttons.SetEffect(EFFECT_SLIDE_LEFT | EFFECT_SLIDE_OUT, 50);
				}
				
				while(ManageMii.GetEffect() > 0) usleep(THREAD_SLEEP);
				
				ManageMii.ResetState();
				changeManage = true;
			}
			
			if(Buttons.GetState() == STATE_CLICKED)
			{
				choice = Buttons.GetChoice();
				Buttons.ResetState();
				
				ManageMii.Disabled();
				
				while(Buttons.GetEffect() > 0) usleep(THREAD_SLEEP);
				
				MainWindow::Instance()->HaltGui();
				MainWindow::Instance()->Remove(&Buttons);
				MainWindow::Instance()->ResumeGui();
				
				windowinfo.move = ManageWindow::NO_MOVE_IN;
				
				if(source == ManageWindow::DEVICE)
				{
					if(choice == 1)
					{
						Confirm.SetTitle(tr("Delete"));
						
						MainWindow::Instance()->HaltGui();
						MainWindow::Instance()->Append(&Confirm);
						MainWindow::Instance()->ResumeGui();
						
						while(Confirm.GetEffect() > 0) usleep(THREAD_SLEEP);
						
						exit = false;
						while(!exit)
						{
							if(Confirm.GetState() == STATE_CLICKED) {
								choice = Confirm.GetChoice();
								Confirm.ResetState();
								
								while(Confirm.GetEffect() > 0) usleep(THREAD_SLEEP);
								
								MainWindow::Instance()->HaltGui();
								MainWindow::Instance()->Remove(&Confirm);
								MainWindow::Instance()->ResumeGui();
								
								windowinfo.move = ManageWindow::NO_MOVE_IN;
								
								if(choice == 1)
								{
									DeleteDeviceMii();
									
									ManageMii.StopEffect();
									Buttons.StopEffect();
									
									goto out;
								}
								else if(choice == 2) {
									
									exit = true;
								}
							}
						}
					}
					else if(choice == 2)
					{
						Confirm.SetTitle(tr("Copy"));
						
						MainWindow::Instance()->HaltGui();
						MainWindow::Instance()->Append(&Confirm);
						MainWindow::Instance()->ResumeGui();
						
						while(Confirm.GetEffect() > 0) usleep(THREAD_SLEEP);
						
						exit = false;
						while(!exit)
						{
							if(Confirm.GetState() == STATE_CLICKED) {
								choice = Confirm.GetChoice();
								Confirm.ResetState();
								
								while(Confirm.GetEffect() > 0) usleep(THREAD_SLEEP);
								
								MainWindow::Instance()->HaltGui();
								MainWindow::Instance()->Remove(&Confirm);
								MainWindow::Instance()->ResumeGui();
								
								windowinfo.move = ManageWindow::NO_MOVE_IN;
								
								if(choice == 1)
								{
									CopyMii();
									
									exit = true;
								}
								else if(choice == 2) {
									
									exit = true;
								}
							}
						}
					}
					else if(choice == 3)
					{
						Confirm.SetTitle(tr("Move"));
						
						MainWindow::Instance()->HaltGui();
						MainWindow::Instance()->Append(&Confirm);
						MainWindow::Instance()->ResumeGui();
						
						while(Confirm.GetEffect() > 0) usleep(THREAD_SLEEP);
						
						exit = false;
						while(!exit)
						{
							if(Confirm.GetState() == STATE_CLICKED) {
								choice = Confirm.GetChoice();
								Confirm.ResetState();
								
								while(Confirm.GetEffect() > 0) usleep(THREAD_SLEEP);
								
								MainWindow::Instance()->HaltGui();
								MainWindow::Instance()->Remove(&Confirm);
								MainWindow::Instance()->ResumeGui();
								
								windowinfo.move = ManageWindow::NO_MOVE_IN;
								
								if(choice == 1)
								{
									MoveMii();
									
									ManageMii.StopEffect();
									Buttons.StopEffect();
									
									goto out;
								}
								else if(choice == 2) {
									
									exit = true;
								}
							}
						}
					}
					else if(choice == 4)
					{
						Confirm.SetTitle(tr("Install"));
						
						MainWindow::Instance()->HaltGui();
						MainWindow::Instance()->Append(&Confirm);
						MainWindow::Instance()->ResumeGui();
						
						while(Confirm.GetEffect() > 0) usleep(THREAD_SLEEP);
						
						exit = false;
						while(!exit)
						{
							if(Confirm.GetState() == STATE_CLICKED)
							{
								choice = Confirm.GetChoice();
								Confirm.ResetState();
								
								while(Confirm.GetEffect() > 0) usleep(THREAD_SLEEP);
								
								MainWindow::Instance()->HaltGui();
								MainWindow::Instance()->Remove(&Confirm);
								MainWindow::Instance()->ResumeGui();
								
								windowinfo.move = ManageWindow::NO_MOVE_IN;
								
								if(choice == 1) {
									
									InstallMii();
									
									exit = true;
								}
								else if(choice == 2) {
									
									exit = true;
								}
							}
						}
					}
				}
				else if(source == ManageWindow::WII)
				{
					if(choice == 1)
					{
						Confirm.SetTitle(tr("Delete"));
						
						MainWindow::Instance()->HaltGui();
						MainWindow::Instance()->Append(&Confirm);
						MainWindow::Instance()->ResumeGui();
						
						while(Confirm.GetEffect() > 0) usleep(THREAD_SLEEP);
						
						exit = false;
						while(!exit)
						{
							if(Confirm.GetState() == STATE_CLICKED) {
								choice = Confirm.GetChoice();
								Confirm.ResetState();
								
								while(Confirm.GetEffect() > 0) usleep(THREAD_SLEEP);
								
								MainWindow::Instance()->HaltGui();
								MainWindow::Instance()->Remove(&Confirm);
								MainWindow::Instance()->ResumeGui();
								
								windowinfo.move = ManageWindow::NO_MOVE_IN;
								
								if(choice == 1)
								{
									DeleteNandMii();
									
									ManageMii.StopEffect();
									Buttons.StopEffect();
									
									goto out;
								}
								else if(choice == 2) {
									
									exit = true;
								}
							}
						}
					}
					else if(choice == 4)
					{
						MainWindow::Instance()->HaltGui();
						MainWindow::Instance()->Append(&Path);
						MainWindow::Instance()->ResumeGui();
						
						while(Path.GetEffect() > 0) usleep(THREAD_SLEEP);
						
						exit = false;
						while(!exit)
						{
							if(Path.GetState() == STATE_CLICKED) {
								choice = Path.GetChoice();
								Path.ResetState();
								
								while(Path.GetEffect() > 0) usleep(THREAD_SLEEP);
								
								MainWindow::Instance()->HaltGui();
								MainWindow::Instance()->Remove(&Path);
								MainWindow::Instance()->ResumeGui();
								
								windowinfo.move = ManageWindow::NO_MOVE_IN;
								
								if(choice == 1) {
									ExtractMii(cfg.SaveMiisPath);
									exit = true;
								}
								else if(choice == 2) {
									ExtractMii(navinfo.LastBrowsedPath);
									exit = true;
								}
								else if(choice == 3)
								{
									char customPath[MAXPATHLEN];
									if(BrowseCustom(navinfo.LastBrowsedPath, customPath))
										ExtractMii(customPath);
									exit = true;
								}
								else if(choice == 4) {
									exit = true;
								}
							}
						}
					}
				}
				MainWindow::Instance()->HaltGui();
				MainWindow::Instance()->Append(&Buttons);
				MainWindow::Instance()->ResumeGui();
				
				while(Buttons.GetEffect() > 0) usleep(THREAD_SLEEP);
				
				ManageMii.Enabled();
				
				choice = 0;
			}
		}
		
	  out:
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&Buttons);
		MainWindow::Instance()->Remove(&ManageMii);
		MainWindow::Instance()->ResumeGui();
		
	}
	
  exit:
	
	if(windowinfo.refreshType == ManageWindow::NO_TYPE)
		windowinfo.move = ManageWindow::MOVE_IN;
}
