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
 * manage.cpp
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
#include "saves/manage/manage.h"
#include "saves/wii_list.h"
#include "menu/MainWindow.h"
#include "main.h"

int selected;

/****************************************************************************
* Manage
*
* Shortcut function
***************************************************************************/
void Manage(int sel,
			short s)
{
    selected = sel;
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
					
					selected--;
					
					if(selected < 0)
						selected = 0;
					if(selected > browserD.numEntries-1)
						selected = browserD.numEntries-1;
					
					if(!browserDevice[selected].isSave) {
						selected++;
						if(selected > browserD.numEntries-1)
							selected = browserD.numEntries-1;
						
						if(!browserDevice[selected].isSave) {
							exitManage = true;
							goto exit;
						}
					}
				}
				
				else if(windowinfo.refreshType == ManageWindow::PREV)
				{
					do{
						selected--;
						if(selected < 0)
							selected = browserD.numEntries-1;
					}while(!browserDevice[selected].isSave);
					
					windowinfo.move = ManageWindow::MOVE_PREV_IN;
					navinfo.LastDeviceItemSelected = selected;
				}
				
				else if(windowinfo.refreshType == ManageWindow::NEXT)
				{
					do{
						selected++;
						if(selected > browserD.numEntries-1)
							selected = 0;
					}while(!browserDevice[selected].isSave);
					
					windowinfo.move = ManageWindow::MOVE_NEXT_IN;
					navinfo.LastDeviceItemSelected = selected;
				}
			}
			
			else if(source == ManageWindow::WII)
			{
				if(windowinfo.refreshType == ManageWindow::PREV)
				{
					selected--;
					if(selected < 0)
						selected = wiiSaveInfo.Cnt-1;
					
					windowinfo.move = ManageWindow::MOVE_PREV_IN;
					navinfo.LastWiiSaveSelected = selected;
				}
				
				else if(windowinfo.refreshType == ManageWindow::NEXT)
				{
					selected++;
					if(selected > wiiSaveInfo.Cnt-1)
						selected = 0;
					
					windowinfo.move = ManageWindow::MOVE_NEXT_IN;
					navinfo.LastWiiSaveSelected = selected;
				}
			}
			
			windowinfo.refreshType = ManageWindow::NO_TYPE;
			
		}
		
		char name[100];
		char subname[100];
		float block = 0;
		char ID[10];
		u64 tid = 0;
		int notSupported = 0;
		
		if(source == ManageWindow::DEVICE)
		{
			snprintf(name, sizeof(name), "%s", browserDevice[selected].displayname);
			snprintf(subname, sizeof(subname), "%s", browserDevice[selected].subname);
			snprintf(ID, sizeof(ID), "%s", browserDevice[selected].ID);
			block = browserDevice[selected].blockSize;
			tid = browserDevice[selected].tid;
			notSupported = browserDevice[selected].isNotSupported;
		}
		else if(source == ManageWindow::WII)
		{
			snprintf(name, sizeof(name), "%s", wiiSaveList[selected].name);
			snprintf(subname, sizeof(subname), "%s",  wiiSaveList[selected].subname);
			snprintf(ID, sizeof(ID), "%s",  wiiSaveList[selected].ID);
			block =  wiiSaveList[selected].blockSize;
			tid =  wiiSaveList[selected].tid;
			notSupported = 0;
		}
		
		ManageWindow Manage(name, subname, block, ID, tid, notSupported, selected);
		
		ManageButtons Buttons(0,
							  0,
							  source ? 0 : tr("Delete"),
							  source ? 0 : tr("Copy"),
							  source ? 0 : tr("Move"),
							  source ? tr("Extract") : (Manage.GetIfInstalled() ? tr("Install") : 0));
		
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
		MainWindow::Instance()->Append(&Manage);
		MainWindow::Instance()->ChangeFocus(&Manage);
		MainWindow::Instance()->Append(&Buttons);
		MainWindow::Instance()->ResumeGui();
		
		while(Manage.GetEffect() > 0) usleep(THREAD_SLEEP);
		
		while(!changeManage)
		{
			if(Manage.GetState() == STATE_CLICKED)
			{
				choice = Manage.GetChoice();
				
				if(choice == 1) { // close
					windowinfo.move = ManageWindow::MOVE_OUT;
					Manage.SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_OUT, 50);
					Buttons.SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_OUT, 50);
					exitManage = true;
				}
				else if(choice == 2) { // prev
					windowinfo.move = ManageWindow::MOVE_PREV_OUT;
					windowinfo.refreshType = ManageWindow::PREV;
					windowinfo.refresh = true;
					Manage.SetEffect(EFFECT_SLIDE_RIGHT | EFFECT_SLIDE_OUT, 50);
					Buttons.SetEffect(EFFECT_SLIDE_RIGHT | EFFECT_SLIDE_OUT, 50);
				}
				else if(choice == 3) { // next
					windowinfo.move = ManageWindow::MOVE_NEXT_OUT;
					windowinfo.refreshType = ManageWindow::NEXT;
					windowinfo.refresh = true;
					Manage.SetEffect(EFFECT_SLIDE_LEFT | EFFECT_SLIDE_OUT, 50);
					Buttons.SetEffect(EFFECT_SLIDE_LEFT | EFFECT_SLIDE_OUT, 50);
				}
				
				while(Manage.GetEffect() > 0) usleep(THREAD_SLEEP);
				
				Manage.ResetState();
				changeManage = true;
			}
			
			if(Buttons.GetState() == STATE_CLICKED)
			{
				choice = Buttons.GetChoice();
				Buttons.ResetState();
				
				Manage.Disabled();
				
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
									SaveDelete();
									
									windowinfo.move = ManageWindow::DEFAULT;
									
									Manage.StopEffect();
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
									SaveCopy();
									
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
									SaveMove();
									
									windowinfo.move = ManageWindow::DEFAULT;
									
									Manage.StopEffect();
									Buttons.StopEffect();
									
									goto out;//exit = true;
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
									
									SaveInstall();
									
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
					if(choice == 4)
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
									SaveExtract(name,tid, cfg.SaveGamePath);
									exit = true;
								}
								else if(choice == 2) {
									SaveExtract(name,tid,navinfo.LastBrowsedPath);
									exit = true;
								}
								else if(choice == 3)
								{
									char customPath[MAXPATHLEN];
									if(BrowseCustom(navinfo.LastBrowsedPath, customPath))
										SaveExtract(name,tid,customPath);
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
				
				Manage.Enabled();
				
				choice = 0;
			}
		}
		
	  out:
		
		MainWindow::Instance()->HaltGui();
		MainWindow::Instance()->Remove(&Buttons);
		MainWindow::Instance()->Remove(&Manage);
		MainWindow::Instance()->ResumeGui();
		
	}
	
  exit:
	
	if(windowinfo.refreshType == ManageWindow::NO_TYPE)
		windowinfo.move = ManageWindow::MOVE_IN;
}
