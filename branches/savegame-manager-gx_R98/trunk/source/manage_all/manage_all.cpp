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
 * manage_all.cpp
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
#include "manage_all/manage_all.h"
#include "manage_all/manage_all_tools.h"
#include "manage_all/ManageAllWindow.h"
#include "manage_all/ManageAllButtons.h"
#include "manage_all/Buttons.h"
#include "menu/MainWindow.h"
#include "main.h"

/****************************************************************************
* ManageAll
*
* Shortcut function
***************************************************************************/
void ManageAll()
{
    int choice = 0;
	bool exitManageAll = false;
	bool exitManageAllFiles = false;
	bool exit = false;
	
	MainWindow::Instance()->SetState(STATE_DISABLED);
	
	ManageAllWindow Window(tr("Manage All"));
	
	ManageAllButtons WhatButtons(tr("Saves"),
								 tr("Miis"),
								 1,
								 0,
								 0);
	WhatButtons.SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_IN, 50);
	
	ManageAllButtons AllSaveButtons(tr("Install All"),
									tr("Extract All"),
									0,
									0,
									0);
	
	ManageAllButtons AllMiiButtons(tr("Install All"),
								   tr("Extract All"),
								   0,
								   1,
								   0);
	
	ManageAllButtons AllDeleteButtons(tr("Nand"),
									  tr("Devices"),
									  0,
									  0,
									  1);
	
	Buttons Path(0,
				 0,
				 0,
				 tr("Default"),
				 tr("Browsed"),
				 tr("Custom"),
				 tr("Go Back"));
		
	Buttons Confirm(0,
					navinfo.LastBrowsedPath,
					0,
					tr("Confirm"),
					tr("Cancel"),
					0,
					0);
	
	Buttons SuccesAll(0,
					  tr("Succesfully done"),
					  0,
					  tr("OK"),
					  0,
					  0,
					  0);
	
	MainWindow::Instance()->HaltGui();
	MainWindow::Instance()->Append(&Window);
	MainWindow::Instance()->ChangeFocus(&Window);
	MainWindow::Instance()->Append(&WhatButtons);
	MainWindow::Instance()->ResumeGui();
	
	while(Window.GetEffect() > 0) usleep(THREAD_SLEEP);
	
	while(!exitManageAll)
	{
		if(Window.GetState() == STATE_CLICKED)
		{
			Window.ResetState();
			exitManageAll = true;
		}
		if(WhatButtons.GetState() == STATE_CLICKED)
		{
			choice = WhatButtons.GetChoice();
			WhatButtons.ResetState();
			
			WhatButtons.SetEffect(EFFECT_FADE, -20);
			
			while(WhatButtons.GetEffect() > 0) usleep(THREAD_SLEEP);
			
			MainWindow::Instance()->HaltGui();
			MainWindow::Instance()->Remove(&WhatButtons);
			MainWindow::Instance()->ResumeGui();
			
			while(WhatButtons.GetEffect() > 0) usleep(THREAD_SLEEP);
	
			if(choice == 1)
			{
				Window.SetTitle(tr("Manage All Saves"));
				AllSaveButtons.SetEffect(EFFECT_FADE, 20);
				
				MainWindow::Instance()->HaltGui();
				MainWindow::Instance()->Append(&AllSaveButtons);
				MainWindow::Instance()->ResumeGui();
				
				while(AllSaveButtons.GetEffect() > 0) usleep(THREAD_SLEEP);
	
				while(!exitManageAllFiles)
				{
					if(Window.GetState() == STATE_CLICKED)
					{
						Window.ResetState();
						exitManageAllFiles = true;
					}
					if(AllSaveButtons.GetState() == STATE_CLICKED)
					{
						Window.Disabled();
						
						choice = AllSaveButtons.GetChoice();
						AllSaveButtons.ResetState();
						
						AllSaveButtons.SetEffect(EFFECT_FADE, -20);
						
						while(AllSaveButtons.GetEffect() > 0) usleep(THREAD_SLEEP);
						
						MainWindow::Instance()->HaltGui();
						MainWindow::Instance()->Remove(&AllSaveButtons);
						MainWindow::Instance()->ResumeGui();
							
						if(choice == 1)
						{
							Window.SetTitle(tr("Install All Saves"));
							Path.SetTitle(tr("From:"));
							
							Path.SetEffect(EFFECT_FADE, 20);
							
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
									
									Path.SetEffect(EFFECT_FADE, -20);
							
									MainWindow::Instance()->HaltGui();
									MainWindow::Instance()->Append(&Path);
									MainWindow::Instance()->ResumeGui();
									
									while(Path.GetEffect() > 0) usleep(THREAD_SLEEP);
									
									MainWindow::Instance()->HaltGui();
									MainWindow::Instance()->Remove(&Path);
									MainWindow::Instance()->ResumeGui();
									
									char sourcePath[MAXPATHLEN];
									if(choice == 1) {
										snprintf(sourcePath, sizeof(sourcePath), "%s", cfg.SaveGamePath);
									}
									else if(choice == 2) {
										snprintf(sourcePath, sizeof(sourcePath), "%s", navinfo.LastBrowsedPath);
									}
									else if(choice == 3)
									{
										if(!BrowseCustom(navinfo.LastBrowsedPath, sourcePath))
											exit = true;
									}
									else if(choice == 4) {
										exit = true;
									}
									
									choice = 0;
									
									if(exit == false) {
										
										Confirm.SetTitle(tr("From:"));
										Confirm.SetMessage(sourcePath);
										Confirm.SetWarning(tr("WARNING: All existing saves in Wii will be overwritten by installed saves !!!"));
										
										Confirm.SetEffect(EFFECT_FADE, 20);
										
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
												
												Confirm.SetEffect(EFFECT_FADE, -20);
												
												while(Confirm.GetEffect() > 0) usleep(THREAD_SLEEP);
												
												MainWindow::Instance()->HaltGui();
												MainWindow::Instance()->Remove(&Confirm);
												MainWindow::Instance()->ResumeGui();
												
												if(choice == 1)
												{
													Window.SetTitle(tr("Installing All..."));
													choice = AllSaveInstall(sourcePath);
													if(choice == 1)
													{
														Window.SetTitle(tr("Install All Saves"));
														SuccesAll.SetEffect(EFFECT_FADE, 20);
														
														
														MainWindow::Instance()->HaltGui();
														MainWindow::Instance()->Append(&SuccesAll);
														MainWindow::Instance()->ResumeGui();
														
														while(SuccesAll.GetEffect() > 0) usleep(THREAD_SLEEP);
														
														exit = false;
														while(!exit)
														{
															if(SuccesAll.GetState() == STATE_CLICKED) {
																SuccesAll.ResetState();
																exit = true;
															}
														}
														
														SuccesAll.SetEffect(EFFECT_FADE, -20);
														
														while(SuccesAll.GetEffect() > 0) usleep(THREAD_SLEEP);
														
														MainWindow::Instance()->HaltGui();
														MainWindow::Instance()->Remove(&SuccesAll);
														MainWindow::Instance()->ResumeGui();
														
													}
													exit = true;
													choice = 0;
												}
												else if(choice == 2)
												{
													exit = true;
													choice = 0;
												}
											}
										}
									}
								}
							}
						}
						else if(choice == 2)
						{
							Window.SetTitle(tr("Extract All Saves"));
							Path.SetTitle(tr("To:"));
							
							Path.SetEffect(EFFECT_FADE, 20);
							
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
									
									Path.SetEffect(EFFECT_FADE, -20);
							
									MainWindow::Instance()->HaltGui();
									MainWindow::Instance()->Append(&Path);
									MainWindow::Instance()->ResumeGui();
									
									while(Path.GetEffect() > 0) usleep(THREAD_SLEEP);
									
									MainWindow::Instance()->HaltGui();
									MainWindow::Instance()->Remove(&Path);
									MainWindow::Instance()->ResumeGui();
									
									char destPath[MAXPATHLEN];
									if(choice == 1) {
										snprintf(destPath, sizeof(destPath), "%s", cfg.SaveGamePath);
									}
									else if(choice == 2) {
										snprintf(destPath, sizeof(destPath), "%s", navinfo.LastBrowsedPath);
									}
									else if(choice == 3)
									{
										if(!BrowseCustom(navinfo.LastBrowsedPath, destPath))
											exit = true;
									}
									else if(choice == 4) {
										exit = true;
									}
									
									choice = 0;
									
									if(exit == false) {
										
										Confirm.SetTitle(tr("To:"));
										Confirm.SetMessage(destPath);
										Confirm.SetWarning(tr("WARNING: All existing saves in this path will be overwritten by extracted saves !!!"));
										
										Confirm.SetEffect(EFFECT_FADE, 20);
										
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
												
												Confirm.SetEffect(EFFECT_FADE, -20);
												
												while(Confirm.GetEffect() > 0) usleep(THREAD_SLEEP);
												
												MainWindow::Instance()->HaltGui();
												MainWindow::Instance()->Remove(&Confirm);
												MainWindow::Instance()->ResumeGui();
												
												if(choice == 1)
												{
													Window.SetTitle(tr("Extracting All..."));
													choice = AllSaveExtract(destPath);
													if(choice == 1)
													{
														Window.SetTitle(tr("Extract All Saves"));
														SuccesAll.SetEffect(EFFECT_FADE, 20);
														
														MainWindow::Instance()->HaltGui();
														MainWindow::Instance()->Append(&SuccesAll);
														MainWindow::Instance()->ResumeGui();
														
														while(SuccesAll.GetEffect() > 0) usleep(THREAD_SLEEP);
														
														exit = false;
														while(!exit)
														{
															if(SuccesAll.GetState() == STATE_CLICKED) {
																SuccesAll.ResetState();
																exit = true;
															}
														}
														
														SuccesAll.SetEffect(EFFECT_FADE, -20);
														
														while(SuccesAll.GetEffect() > 0) usleep(THREAD_SLEEP);
														
														MainWindow::Instance()->HaltGui();
														MainWindow::Instance()->Remove(&SuccesAll);
														MainWindow::Instance()->ResumeGui();
														
													}
													exit = true;
													choice = 0;
												}
												else if(choice == 2)
												{
													exit = true;
													choice = 0;
												}
											}
										}
									}
								}
							}
						}
						
						Window.SetTitle(tr("Manage All Saves"));
						AllSaveButtons.SetEffect(EFFECT_FADE, 20);
						
						MainWindow::Instance()->HaltGui();
						MainWindow::Instance()->Append(&AllSaveButtons);
						MainWindow::Instance()->ResumeGui();
						
						while(AllSaveButtons.GetEffect() > 0) usleep(THREAD_SLEEP);
						
						Window.Enabled();
						
						choice = 0;
					}
				}
				
				Window.SetTitle(tr("Manage All"));
				AllSaveButtons.SetEffect(EFFECT_FADE, -20);
				WhatButtons.SetEffect(EFFECT_FADE, 20);
				
				MainWindow::Instance()->HaltGui();
				MainWindow::Instance()->Append(&AllSaveButtons);
				MainWindow::Instance()->ResumeGui();
				
				while(AllSaveButtons.GetEffect() > 0) usleep(THREAD_SLEEP);
				
				MainWindow::Instance()->HaltGui();
				MainWindow::Instance()->Append(&WhatButtons);
				MainWindow::Instance()->Remove(&AllSaveButtons);
				MainWindow::Instance()->ResumeGui();
				
				while(WhatButtons.GetEffect() > 0) usleep(THREAD_SLEEP);
				
				exitManageAllFiles = false;
				choice = 0;
			}
			else if(choice == 2)
			{
				Window.SetTitle(tr("Manage All Miis"));
				AllMiiButtons.SetEffect(EFFECT_FADE, 20);
				
				MainWindow::Instance()->HaltGui();
				MainWindow::Instance()->Append(&AllMiiButtons);
				MainWindow::Instance()->ResumeGui();
				
				while(AllMiiButtons.GetEffect() > 0) usleep(THREAD_SLEEP);
	
				while(!exitManageAllFiles)
				{
					if(Window.GetState() == STATE_CLICKED)
					{
						Window.ResetState();
						exitManageAllFiles = true;
					}
					if(AllMiiButtons.GetState() == STATE_CLICKED)
					{
						Window.Disabled();
						
						choice = AllMiiButtons.GetChoice();
						AllMiiButtons.ResetState();
						
						AllMiiButtons.SetEffect(EFFECT_FADE, -20);
						
						while(AllMiiButtons.GetEffect() > 0) usleep(THREAD_SLEEP);
						
						MainWindow::Instance()->HaltGui();
						MainWindow::Instance()->Remove(&AllMiiButtons);
						MainWindow::Instance()->ResumeGui();
							
						if(choice == 1)
						{
							Window.SetTitle(tr("Install All Miis"));
							Path.SetTitle(tr("From:"));
							
							Path.SetEffect(EFFECT_FADE, 20);
							
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
									
									Path.SetEffect(EFFECT_FADE, -20);
							
									MainWindow::Instance()->HaltGui();
									MainWindow::Instance()->Append(&Path);
									MainWindow::Instance()->ResumeGui();
									
									while(Path.GetEffect() > 0) usleep(THREAD_SLEEP);
									
									MainWindow::Instance()->HaltGui();
									MainWindow::Instance()->Remove(&Path);
									MainWindow::Instance()->ResumeGui();
									
									char sourcePath[MAXPATHLEN];
									if(choice == 1) {
										snprintf(sourcePath, sizeof(sourcePath), "%s", cfg.SaveMiisPath);
									}
									else if(choice == 2) {
										snprintf(sourcePath, sizeof(sourcePath), "%s", navinfo.LastBrowsedPath);
									}
									else if(choice == 3)
									{
										if(!BrowseCustom(navinfo.LastBrowsedPath, sourcePath))
											exit = true;
									}
									else if(choice == 4) {
										exit = true;
									}
									
									choice = 0;
									
									if(exit == false) {
										
										Confirm.SetTitle(tr("From:"));
										Confirm.SetMessage(sourcePath);
										Confirm.SetWarning(tr("WARNING: All existing Miis in Wii will be overwritten by installed Miis !!!"));
										
										Confirm.SetEffect(EFFECT_FADE, 20);
										
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
												
												Confirm.SetEffect(EFFECT_FADE, -20);
												
												while(Confirm.GetEffect() > 0) usleep(THREAD_SLEEP);
												
												MainWindow::Instance()->HaltGui();
												MainWindow::Instance()->Remove(&Confirm);
												MainWindow::Instance()->ResumeGui();
												
												if(choice == 1)
												{
													Window.SetTitle(tr("Installing All..."));
													choice = AllMiiInstall(sourcePath);
													if(choice == 1)
													{
														Window.SetTitle(tr("Install All Miis"));
														SuccesAll.SetEffect(EFFECT_FADE, 20);
														
														
														MainWindow::Instance()->HaltGui();
														MainWindow::Instance()->Append(&SuccesAll);
														MainWindow::Instance()->ResumeGui();
														
														while(SuccesAll.GetEffect() > 0) usleep(THREAD_SLEEP);
														
														exit = false;
														while(!exit)
														{
															if(SuccesAll.GetState() == STATE_CLICKED) {
																SuccesAll.ResetState();
																exit = true;
															}
														}
														
														SuccesAll.SetEffect(EFFECT_FADE, -20);
														
														while(SuccesAll.GetEffect() > 0) usleep(THREAD_SLEEP);
														
														MainWindow::Instance()->HaltGui();
														MainWindow::Instance()->Remove(&SuccesAll);
														MainWindow::Instance()->ResumeGui();
														
													}
													exit = true;
													choice = 0;
												}
												else if(choice == 2)
												{
													exit = true;
													choice = 0;
												}
											}
										}
									}
								}
							}
						}
						else if(choice == 2)
						{
							Window.SetTitle(tr("Extract All Miis"));
							Path.SetTitle(tr("To:"));
							
							Path.SetEffect(EFFECT_FADE, 20);
							
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
									
									Path.SetEffect(EFFECT_FADE, -20);
									
									MainWindow::Instance()->HaltGui();
									MainWindow::Instance()->Append(&Path);
									MainWindow::Instance()->ResumeGui();
									
									while(Path.GetEffect() > 0) usleep(THREAD_SLEEP);
									
									MainWindow::Instance()->HaltGui();
									MainWindow::Instance()->Remove(&Path);
									MainWindow::Instance()->ResumeGui();
									
									char destPath[MAXPATHLEN];
									if(choice == 1) {
										snprintf(destPath, sizeof(destPath), "%s", cfg.SaveMiisPath);
									}
									else if(choice == 2) {
										snprintf(destPath, sizeof(destPath), "%s", navinfo.LastBrowsedPath);
									}
									else if(choice == 3)
									{
										if(!BrowseCustom(navinfo.LastBrowsedPath, destPath))
											exit = true;
									}
									else if(choice == 4) {
										exit = true;
									}
									
									choice = 0;
									
									if(exit == false) {
										
										Confirm.SetTitle(tr("To:"));
										Confirm.SetMessage(destPath);
										Confirm.SetWarning(tr("WARNING: All existing Miis in this path will be overwritten by extracted Miis !!!"));
										
										Confirm.SetEffect(EFFECT_FADE, 20);
										
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
												
												Confirm.SetEffect(EFFECT_FADE, -20);
												
												while(Confirm.GetEffect() > 0) usleep(THREAD_SLEEP);
												
												MainWindow::Instance()->HaltGui();
												MainWindow::Instance()->Remove(&Confirm);
												MainWindow::Instance()->ResumeGui();
												
												if(choice == 1)
												{
													Window.SetTitle(tr("Extracting All..."));
													choice = AllMiiExtract(destPath);
													if(choice == 1)
													{
														Window.SetTitle(tr("Extract All Miis"));
														SuccesAll.SetEffect(EFFECT_FADE, 20);
														
														MainWindow::Instance()->HaltGui();
														MainWindow::Instance()->Append(&SuccesAll);
														MainWindow::Instance()->ResumeGui();
														
														while(SuccesAll.GetEffect() > 0) usleep(THREAD_SLEEP);
														
														exit = false;
														while(!exit)
														{
															if(SuccesAll.GetState() == STATE_CLICKED) {
																SuccesAll.ResetState();
																exit = true;
															}
														}
														
														SuccesAll.SetEffect(EFFECT_FADE, -20);
														
														while(SuccesAll.GetEffect() > 0) usleep(THREAD_SLEEP);
														
														MainWindow::Instance()->HaltGui();
														MainWindow::Instance()->Remove(&SuccesAll);
														MainWindow::Instance()->ResumeGui();
														
													}
													exit = true;
													choice = 0;
												}
												else if(choice == 2)
												{
													exit = true;
													choice = 0;
												}
											}
										}
									}
								}
							}
						}
						else if(choice == 3)
						{
							Window.Enabled();
							
							Window.SetTitle(tr("Delete All Miis"));
							
							AllDeleteButtons.SetEffect(EFFECT_FADE, 20);
							
							MainWindow::Instance()->HaltGui();
							MainWindow::Instance()->Append(&AllDeleteButtons);
							MainWindow::Instance()->ResumeGui();
							
							while(AllDeleteButtons.GetEffect() > 0) usleep(THREAD_SLEEP);
							
							exit = false;
							while(!exit)
							{
								if(Window.GetState() == STATE_CLICKED)
								{
									Window.ResetState();
									exit = true;
									
									AllDeleteButtons.SetEffect(EFFECT_FADE, -20);
							
									MainWindow::Instance()->HaltGui();
									MainWindow::Instance()->Append(&AllDeleteButtons);
									MainWindow::Instance()->ResumeGui();
									
									while(AllDeleteButtons.GetEffect() > 0) usleep(THREAD_SLEEP);
									
									MainWindow::Instance()->HaltGui();
									MainWindow::Instance()->Remove(&AllDeleteButtons);
									MainWindow::Instance()->ResumeGui();
									
								}
								if(AllDeleteButtons.GetState() == STATE_CLICKED) {
									choice = AllDeleteButtons.GetChoice();
									AllDeleteButtons.ResetState();
									
									AllDeleteButtons.SetEffect(EFFECT_FADE, -20);
							
									MainWindow::Instance()->HaltGui();
									MainWindow::Instance()->Append(&AllDeleteButtons);
									MainWindow::Instance()->ResumeGui();
									
									while(AllDeleteButtons.GetEffect() > 0) usleep(THREAD_SLEEP);
									
									MainWindow::Instance()->HaltGui();
									MainWindow::Instance()->Remove(&AllDeleteButtons);
									MainWindow::Instance()->ResumeGui();
									
									if(choice == 1)
									{
										Window.Disabled();
										Confirm.SetTitle(tr("Stored in Wii's Nand?"));
										Confirm.SetMessage("");
										Confirm.SetWarning(tr("WARNING: All existing Miis in the Wii's memory will be deleted !!!"));
										
										Confirm.SetEffect(EFFECT_FADE, 20);
										
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
												
												Confirm.SetEffect(EFFECT_FADE, -20);
												
												while(Confirm.GetEffect() > 0) usleep(THREAD_SLEEP);
												
												MainWindow::Instance()->HaltGui();
												MainWindow::Instance()->Remove(&Confirm);
												MainWindow::Instance()->ResumeGui();
												
												if(choice == 1)
												{
													Window.SetTitle(tr("Deleting All Nand Mii..."));
													choice = DeleteAllNandMii();
													if(choice == 1)
													{
														Window.SetTitle(tr("Delete All Miis"));
														SuccesAll.SetEffect(EFFECT_FADE, 20);
														
														MainWindow::Instance()->HaltGui();
														MainWindow::Instance()->Append(&SuccesAll);
														MainWindow::Instance()->ResumeGui();
														
														while(SuccesAll.GetEffect() > 0) usleep(THREAD_SLEEP);
														
														exit = false;
														while(!exit)
														{
															if(SuccesAll.GetState() == STATE_CLICKED) {
																SuccesAll.ResetState();
																exit = true;
															}
														}
														
														SuccesAll.SetEffect(EFFECT_FADE, -20);
														
														while(SuccesAll.GetEffect() > 0) usleep(THREAD_SLEEP);
														
														MainWindow::Instance()->HaltGui();
														MainWindow::Instance()->Remove(&SuccesAll);
														MainWindow::Instance()->ResumeGui();
														
													}
													exit = true;
													choice = 0;
												}
												else if(choice == 2)
												{
													exit = true;
													choice = 0;
												}
											}
										}
										Window.Enabled();
									}
									else if(choice == 2)
									{
										Window.Disabled();
										
										Window.SetTitle(tr("Delete All Device Miis"));
										Path.SetTitle(tr("From:"));
										
										Path.SetEffect(EFFECT_FADE, 20);
										
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
												
												Path.SetEffect(EFFECT_FADE, -20);
												
												MainWindow::Instance()->HaltGui();
												MainWindow::Instance()->Append(&Path);
												MainWindow::Instance()->ResumeGui();
												
												while(Path.GetEffect() > 0) usleep(THREAD_SLEEP);
												
												MainWindow::Instance()->HaltGui();
												MainWindow::Instance()->Remove(&Path);
												MainWindow::Instance()->ResumeGui();
												
												char deletePath[MAXPATHLEN];
												if(choice == 1) {
													snprintf(deletePath, sizeof(deletePath), "%s", cfg.SaveMiisPath);
												}
												else if(choice == 2) {
													snprintf(deletePath, sizeof(deletePath), "%s", navinfo.LastBrowsedPath);
												}
												else if(choice == 3)
												{
													if(!BrowseCustom(navinfo.LastBrowsedPath, deletePath))
														exit = true;
												}
												else if(choice == 4) {
													exit = true;
												}
												
												choice = 0;
												
												if(exit == false) {
													
													Confirm.SetTitle(tr("From:"));
													Confirm.SetMessage(deletePath);
													Confirm.SetWarning(tr("WARNING: All existing Miis in this path will be deleted !!!"));
													
													Confirm.SetEffect(EFFECT_FADE, 20);
													
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
															
															Confirm.SetEffect(EFFECT_FADE, -20);
															
															while(Confirm.GetEffect() > 0) usleep(THREAD_SLEEP);
															
															MainWindow::Instance()->HaltGui();
															MainWindow::Instance()->Remove(&Confirm);
															MainWindow::Instance()->ResumeGui();
															
															if(choice == 1)
															{
																Window.SetTitle(tr("Deleting All device Miis..."));
																choice = DeleteAllDeviceMii(deletePath);
																if(choice == 1)
																{
																	Window.SetTitle(tr("Delete All Device Miis"));
																	SuccesAll.SetEffect(EFFECT_FADE, 20);
																	
																	MainWindow::Instance()->HaltGui();
																	MainWindow::Instance()->Append(&SuccesAll);
																	MainWindow::Instance()->ResumeGui();
																	
																	while(SuccesAll.GetEffect() > 0) usleep(THREAD_SLEEP);
																	
																	exit = false;
																	while(!exit)
																	{
																		if(SuccesAll.GetState() == STATE_CLICKED) {
																			SuccesAll.ResetState();
																			exit = true;
																		}
																	}
																	
																	SuccesAll.SetEffect(EFFECT_FADE, -20);
																	
																	while(SuccesAll.GetEffect() > 0) usleep(THREAD_SLEEP);
																	
																	MainWindow::Instance()->HaltGui();
																	MainWindow::Instance()->Remove(&SuccesAll);
																	MainWindow::Instance()->ResumeGui();
																	
																}
																exit = true;
																choice = 0;
															}
															else if(choice == 2)
															{
																exit = true;
																choice = 0;
															}
														}
													}
												}
											}
										}
										Window.Enabled();
									}
								}
							}
							exit = false;
						}
						
						Window.SetTitle(tr("Manage All Miis"));
						AllMiiButtons.SetEffect(EFFECT_FADE, 20);
						
						MainWindow::Instance()->HaltGui();
						MainWindow::Instance()->Append(&AllMiiButtons);
						MainWindow::Instance()->ResumeGui();
						
						while(AllMiiButtons.GetEffect() > 0) usleep(THREAD_SLEEP);
						
						Window.Enabled();
						
						choice = 0;
					}
				}
				
				Window.SetTitle(tr("Manage All"));
				AllMiiButtons.SetEffect(EFFECT_FADE, -20);
				WhatButtons.SetEffect(EFFECT_FADE, 20);
				
				MainWindow::Instance()->HaltGui();
				MainWindow::Instance()->Append(&AllMiiButtons);
				MainWindow::Instance()->ResumeGui();
				
				while(AllMiiButtons.GetEffect() > 0) usleep(THREAD_SLEEP);
				
				MainWindow::Instance()->HaltGui();
				MainWindow::Instance()->Append(&WhatButtons);
				MainWindow::Instance()->Remove(&AllMiiButtons);
				MainWindow::Instance()->ResumeGui();
				
				while(WhatButtons.GetEffect() > 0) usleep(THREAD_SLEEP);
				
				exitManageAllFiles = false;
				
			}
		}
	}
	
	Window.SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_OUT, 50);
	WhatButtons.SetEffect(EFFECT_SLIDE_TOP | EFFECT_SLIDE_OUT, 50);
	
	while(Window.GetEffect() > 0) usleep(THREAD_SLEEP);
	
	MainWindow::Instance()->HaltGui();
	MainWindow::Instance()->Remove(&Window);
	MainWindow::Instance()->Remove(&WhatButtons);
	MainWindow::Instance()->ResumeGui();
}
