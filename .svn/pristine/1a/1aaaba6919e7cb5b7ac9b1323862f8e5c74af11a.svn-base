#ifndef _MANAGEALLPROGRESSBANNER_H_
#define _MANAGEALLPROGRESSBANNER_H_

#include "libwiigui/gui.h"
#include "libwiigui/gui_banner.h"

//! create and display list of tpl
class ManageAllProgressBanner : public GuiElement
{
	public:
		ManageAllProgressBanner(u32 bannerToDisplay);
		~ManageAllProgressBanner();
		
		void SetBannerToDisplay(u32 bannerToDisplay);
		void Draw();
		void Update(GuiTrigger * t);
	
	protected:
		bool changed;
		u32 displayedBanner;
		
		GuiWindow * window;
		GuiButton * bannerBtn;
		GuiSaveBanner ** banner;
	
};

#endif
