/****************************************************************************
 * Copyright (C) 2010
 * by Dimok
 * modified by Dj_Skual
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
 * HomebreBrowser.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include "HomebrewBrowser.hpp"
#include "BootHomebrew.h"
#include "HomebrewXML.h"
#include "../Tools/ShowError.h"
#include "../Tools/StringTools.h"
#include "../Prompts/TitleBrowser.hpp"
#include "../Prompts/HomebrewPrompt.hpp"
#include "../FileOperations/fileops.h"
#include "../Language/gettext.h"
#include "../Themes/CTheme.h"
#include "../Text/wstring.hpp"

HomebrewBrowser::HomebrewBrowser()
	: FlyingButtonsMenu(tr( "Homebrew Launcher" ))
{
	HomebrewList = new DirList(Settings.HomebrewPath, ".dol,.elf", DirList::Files | DirList::Dirs | DirList::CheckSubfolders);
	HomebrewFilter();
	
	channelImgData = Resources::GetImageData("button_channel.png");
	channelBtnImg = new GuiImage(channelImgData);
	channelBtnImg->SetWidescreen(Settings.Widescreen);
	channelBtn = new GuiButton(channelBtnImg->GetWidth(), channelBtnImg->GetHeight());
	channelBtn->SetPosition(240, 400);
	channelBtn->SetImage(channelBtnImg);
	channelBtn->SetSoundOver(btnSoundOver);
	channelBtn->SetSoundClick(btnSoundClick);
	channelBtn->SetEffectGrow();
	channelBtn->SetTrigger(trigA);
	if (Settings.GodMode || !(Settings.ParentalBlocks & BLOCK_TITLE_LAUNCHER))
		Append(channelBtn);

	MainButtonDesc.resize(HomebrewList->GetFilecount());
	MainButtonDescOver.resize(HomebrewList->GetFilecount());

	for(u32 i = 0; i < 4; ++i)
	{
		IconImgData[i] = NULL;
		IconImg[i] = NULL;
	}

	for(int i = 0; i < HomebrewList->GetFilecount(); ++i)
	{
		MainButtonDesc[i] = new GuiText((char *) NULL, 18, thColor("r=0 g=0 b=0 a=255 - flyingbutton menu mainbutton text color"));
		MainButtonDesc[i]->SetMaxWidth(MainButtonImgData->GetWidth() - 150, DOTTED);
		MainButtonDesc[i]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
		MainButtonDesc[i]->SetPosition(148, 15);
		
		MainButtonDescOver[i] = new GuiText((char *) NULL, 18, thColor("r=0 g=0 b=0 a=255 - flyingbutton menu mainbutton text color"));
		MainButtonDescOver[i]->SetMaxWidth(MainButtonImgData->GetWidth() - 150, SCROLL_HORIZONTAL);
		MainButtonDescOver[i]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
		MainButtonDescOver[i]->SetPosition(148, 15);
	}
}

HomebrewBrowser::~HomebrewBrowser()
{
	HaltGui();
	delete HomebrewList;

	Remove(channelBtn);
	delete channelImgData;
	delete channelBtnImg;
	delete channelBtn;

	for(u32 i = 0; i < DISPLAY_BUTTONS; ++i)
	{
		if(IconImgData[i])
			delete IconImgData[i];
		if(IconImg[i])
			delete IconImg[i];
	}
	
	for(u32 i = 0; i < MainButtonDesc.size(); ++i)
	{
		delete MainButtonDesc[i];
		delete MainButtonDescOver[i];
		MainButton[i]->SetLabel(NULL, 1);
		MainButton[i]->SetLabelOver(NULL, 1);
		MainButton[i]->SetIcon(NULL);
	}
}

int HomebrewBrowser::Run()
{
	HomebrewBrowser * Menu = new HomebrewBrowser();
	mainWindow->Append(Menu);

	Menu->ShowMenu();

	int returnMenu = MENU_NONE;

	while((returnMenu = Menu->MainLoop()) == MENU_NONE);

	delete Menu;

	return returnMenu;
}

void HomebrewBrowser::HomebrewFilter()
{
	for(int i = 0; i < HomebrewList->GetFilecount(); i++)
	{
		if(strtokcmp(HomebrewList->GetFilename(i), "boot.dol,boot.elf", ",") == 0)
			continue;
		else
		{
			std::string hbPath(HomebrewList->GetFilepath(i));
			size_t pos = hbPath.rfind('/');
			if(pos != std::string::npos && pos < hbPath.size()-1)
				hbPath.erase(pos+1);
			
			bool exit = false;
			for(int j = 0; j < HomebrewList->GetFilecount() && !exit; j++)
			{
				if(i == j)
					continue;
				
				std::string hbPath2(HomebrewList->GetFilepath(j));
				size_t pos = hbPath2.rfind('/');
				if(pos != std::string::npos && pos < hbPath2.size()-1)
					hbPath2.erase(pos+1);
				
				if(!hbPath.compare(hbPath2))
				{
					if(!strtokcmp(HomebrewList->GetFilename(j), "boot.dol,boot.elf", ","))
						HomebrewList->RemoveEntrie(i);
					else
						HomebrewList->RemoveEntrie(j);
					
					exit = true;
					i--;
				}
			}
		}
	}
}

void HomebrewBrowser::AddMainButtons()
{
	HaltGui();

	for(u32 i = 0; i < 4; ++i)
	{
		if(IconImgData[i])
			delete IconImgData[i];
		if(IconImg[i])
			delete IconImg[i];
		IconImgData[i] = NULL;
		IconImg[i] = NULL;
	}

	for(u32 i = 0; i < MainButton.size(); ++i)
		MainButton[i]->SetIcon(NULL);

	int FirstItem = currentPage*DISPLAY_BUTTONS;

	for(int i = FirstItem, n = 0; i < (int) MainButton.size() && i < FirstItem+DISPLAY_BUTTONS; ++i, ++n)
	{
		std::string iconpath = HomebrewList->GetFilepath(i);
		size_t pos = iconpath.rfind('/');
		if(pos != std::string::npos && pos < iconpath.size()-1)
			iconpath.erase(pos+1);
		iconpath += "icon.png";

		IconImgData[n] = new GuiImageData(iconpath.c_str());
		IconImg[n] = new GuiImage(IconImgData[n]);
		IconImg[n]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
		IconImg[n]->SetPosition(12, 0);
		IconImg[n]->SetScale(0.95);
		MainButton[i]->SetIcon(IconImg[n]);
	}

	FlyingButtonsMenu::AddMainButtons();
}

void HomebrewBrowser::SetupMainButtons()
{
	HomebrewXML MetaXML;

	for(int i = 0; i < HomebrewList->GetFilecount(); ++i)
	{
		const char * HomebrewName = NULL;
		std::string metapath = HomebrewList->GetFilepath(i);
		size_t pos = metapath.rfind('/');
		if(pos != std::string::npos && pos < metapath.size()-1)
			metapath.erase(pos+1);
		metapath += "meta.xml";

		if (MetaXML.LoadHomebrewXMLData(metapath.c_str()) > 0)
		{
			HomebrewName = MetaXML.GetName();
			MainButtonDesc[i]->SetText(MetaXML.GetShortDescription());
			MainButtonDescOver[i]->SetText(MetaXML.GetShortDescription());
		}
		else
		{
			const char * shortpath = HomebrewList->GetFilepath(i);
			const char * ptr = shortpath;
			const char * ptr2 = NULL;
			while(*ptr != '\0')
			{
				if(*ptr == '/')
				{
					shortpath = ptr2;
					ptr2 = ptr;
				}

				++ptr;
			}
			if(!shortpath && ptr2)
				shortpath = ptr2;
			else if(!shortpath)
				shortpath = HomebrewList->GetFilename(i);

			HomebrewName = shortpath;
			MainButtonDesc[i]->SetText(" ");
			MainButtonDescOver[i]->SetText(" ");
		}

		SetMainButton(i, HomebrewName, MainButtonImgData, MainButtonImgOverData);

		MainButtonTxt[i]->SetFontSize(18);
		MainButtonTxt[i]->SetMaxWidth(MainButtonImgData->GetWidth() - 150, DOTTED);
		MainButtonTxt[i]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
		MainButtonTxt[i]->SetPosition(148, -12);
		MainButton[i]->SetLabel(MainButtonDesc[i], 1);
		MainButton[i]->SetLabelOver(MainButtonDescOver[i], 1);
	}
}

int HomebrewBrowser::MainLoop()
{
	Receiver->CheckIncoming();
	
	if(channelBtn->GetState() == STATE_CLICKED)
	{
		GuiWindow * parent = (GuiWindow *) parentElement;
		if(parent) parent->SetState(STATE_DISABLED);
		
		titleBrowser();
		
		if(parent) parent->SetState(STATE_DEFAULT);
		
		channelBtn->ResetState();
	}

	return FlyingButtonsMenu::MainLoop();
}

//! Callback for MainButton clicked
void HomebrewBrowser::MainButtonClicked(int button)
{
	HomebrewXML MetaXML;
	std::string metapath = HomebrewList->GetFilepath(button);
	size_t pos = metapath.rfind('/');
	if(pos != std::string::npos && pos < metapath.size()-1)
		metapath.erase(pos+1);
	metapath += "meta.xml";
	MetaXML.LoadHomebrewXMLData(metapath.c_str());

	u64 filesize = HomebrewList->GetFilesize(button);

	wString HomebrewName(MainButtonTxt[button]->GetText());

	HomebrewPrompt *HBCWindowPrompt = new HomebrewPrompt(HomebrewName.toUTF8().c_str(),
														 MetaXML.GetCoder(),
														 MetaXML.GetVersion(),
														 MetaXML.GetReleasedate(),
														 MetaXML.GetLongDescription(),
														 IconImgData[button % 4],
														 filesize);

	mainWindow->SetState(STATE_DISABLED);
	mainWindow->Append(HBCWindowPrompt);

	int choice = HBCWindowPrompt->MainLoop();

	delete HBCWindowPrompt;

	mainWindow->SetState(STATE_DEFAULT);

	if (choice == 1)
	{
		u8 *buffer = NULL;
		u64 filesize = 0;
		LoadFileToMem(HomebrewList->GetFilepath(button), &buffer, &filesize);
		if(!buffer)
		{
			ShowError(tr("Not enough memory."));
			return;
		}
		FreeHomebrewBuffer();
		CopyHomebrewMemory(buffer, 0, filesize);
		
		AddBootArgument(HomebrewList->GetFilepath(button));
		
		for(u32 i = 0; i < MetaXML.GetArguments().size(); ++i)
		{
			AddBootArgument(MetaXML.GetArguments().at(i).c_str());
		}
		
		BootHomebrewFromMem();
	}
}
