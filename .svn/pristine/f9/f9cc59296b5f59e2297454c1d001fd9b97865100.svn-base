/****************************************************************************
 * libwiigui
 *
 * Tantric 2009
 *
 * ManageAllProgressBanner.cpp
 *
 * GUI class definitions
 ***************************************************************************/

#include <unistd.h>

#include "libwiigui/gui.h"
#include "manage_all/ManageAllProgressBanner.h"
#include "saves/wii_list.h"
#include "saves/utils.h"
#include "main.h"

/**
 * Constructor for the ManageAllProgressBanner class.
 */
ManageAllProgressBanner::ManageAllProgressBanner(u32 bannerToDisplay)
{
	changed = true;
	displayedBanner = bannerToDisplay;
	int i=0;
	
	banner = new GuiSaveBanner * [wiiSaveInfo.Cnt];
	for(i = 0; i < wiiSaveInfo.Cnt; i++)
	{
	    char savepath[ISFS_MAXPATH];
		u64 listTid =  wiiSaveList[i].tid;
		
		u32 high = (u32)(listTid >> 32);
		u32 low  = (u32)(listTid & 0xFFFFFFFF);
		
	    snprintf(savepath, sizeof(savepath),"/title/%08x/%08x/data/banner.bin", high, low);
	    
		u8* buffer = NULL;
	    read_isfs(savepath, &buffer);
		
	    banner[i] = new GuiSaveBanner(buffer);
		banner[i]->SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
		
	    free(buffer);
	}
	
	bannerBtn = new GuiButton(banner[displayedBanner]->GetWidth(),banner[displayedBanner]->GetHeight());
	bannerBtn->SetParent(this);
	bannerBtn->SetImage(banner[displayedBanner]);
	
	window = new GuiWindow(banner[0]->GetWidth(), banner[0]->GetHeight());
	window->SetParent(this);
	
	window->Append(bannerBtn);
	
	this->SetState(STATE_DEFAULT);
}

/**
 * Destructor for the ManageAllProgressBanner class.
 */
ManageAllProgressBanner::~ManageAllProgressBanner()
{
	window->RemoveAll();
	
	int i = 0;
	for(i = 0;i < wiiSaveInfo.Cnt; i++)
	{
	    if(banner[i])delete(banner[i]);
	    banner[i] = NULL;
	}
	
	delete bannerBtn;
	delete window;
}

void ManageAllProgressBanner::SetBannerToDisplay(u32 bannerToDisplay)
{
	displayedBanner = bannerToDisplay;
	
	changed = true;
}

/**
 * Draw the banner
 */
void ManageAllProgressBanner::Draw()
{
	if(!this->IsVisible())
		return;

	window->Draw();

	this->UpdateEffects();
}

void ManageAllProgressBanner::Update(GuiTrigger * t)
{
	if(state == STATE_DISABLED || !t)
		return;
	
	
	if(changed)
	{
		bannerBtn->SetImage(banner[displayedBanner]);
		changed = false;
	}
	
	if(updateCB)
		updateCB(this);
}
