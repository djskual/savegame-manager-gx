/****************************************************************************
 * Copyright (C) 2010
 * by Dimok
 * Modified by Dj_Skual
 * for SaveGame Manager GX
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

#include "ControlsSettingsMenu.hpp"
#include "../../Language/gettext.h"

/*static inline u16 NextNunchuk_Button(u16 button)
{
	if(button == 0)
	{
		return 0x0001;
	}
	else if(button < 0x0002)
	{
		return button << 1;
	}
	else
	{
		return 0;
	}
}*/

static inline u16 NextWPAD_Button(u16 button)
{
	if(button == 0)
	{
		return 0x0001;
	}
	else if(button < 0x0010)
	{
		return button << 1;
	}
	else if(button < 0x0080)
	{
		return 0x0080;
	}
	else if(button < 0x1000)
	{
		return button << 1;
	}
	else
	{
		return 0;
	}
}

static inline u16 NextClassic_Button(u16 button)
{
	if(button == 0)
	{
		return 0x0001;
	}
	else if(button < 0x0080)
	{
		return button << 1;
	}
	else if(button < 0x0200)
	{
		return 0x0200;
	}
	else if(button < 0x8000)
	{
		return button << 1;
	}
	else
	{
		return 0;
	}
}

static inline u16 NextPAD_Button(u16 button)
{
	if(button == 0)
	{
		return 0x0001;
	}
	else if(button < 0x0040)
	{
		return button << 1;
	}
	else if(button < 0x0100)
	{
		return 0x0100;
	}
	else if(button < 0x1000)
	{
		return button << 1;
	}
	else
	{
		return 0;
	}
}

/*static inline const char * GetNunchuk_ButtonName(u16 button)
{
	switch(button)
	{
		case 0x0000:
			return tr("WPAD NUNCHUK NONE BUTTON");
		case 0x0001:
			return tr("WPAD NUNCHUK BUTTON Z");
		case 0x0002:
			return tr("WPAD NUNCHUK BUTTON C");
		default:
			return " ";
	}
}*/

static inline const char * GetWPAD_ButtonName(u16 button)
{
	switch(button)
	{
		case 0x0000:
			return tr("WPAD NONE BUTTON");
		case 0x0001:
			return tr("WPAD BUTTON 2");
		case 0x0002:
			return tr("WPAD BUTTON 1");
		case 0x0004:
			return tr("WPAD BUTTON B");
		case 0x0008:
			return tr("WPAD BUTTON A");
		case 0x0010:
			return tr("WPAD BUTTON MINUS");
		case 0x0080:
			return tr("WPAD BUTTON HOME");
		case 0x0100:
			return tr("WPAD BUTTON LEFT");
		case 0x0200:
			return tr("WPAD BUTTON RIGHT");
		case 0x0400:
			return tr("WPAD BUTTON DOWN");
		case 0x0800:
			return tr("WPAD BUTTON UP");
		case 0x1000:
			return tr("WPAD BUTTON PLUS");
		default:
			return " ";
	}
}

static inline const char * GetClassic_ButtonName(u16 button)
{
	switch(button)
	{
		case 0x0000:
			return tr("WPAD CLASSIC NONE BUTTON");
		case 0x0001:
			return tr("WPAD CLASSIC BUTTON UP");
		case 0x0002:
			return tr("WPAD CLASSIC BUTTON LEFT");
		case 0x0004:
			return tr("WPAD CLASSIC BUTTON ZR");
		case 0x0008:
			return tr("WPAD CLASSIC BUTTON X");
		case 0x0010:
			return tr("WPAD CLASSIC BUTTON A");
		case 0x0020:
			return tr("WPAD CLASSIC BUTTON Y");
		case 0x0040:
			return tr("WPAD CLASSIC BUTTON B");
		case 0x0080:
			return tr("WPAD CLASSIC BUTTON ZL");
		case 0x0200:
			return tr("WPAD CLASSIC BUTTON FULL R");
		case 0x0400:
			return tr("WPAD CLASSIC BUTTON PLUS");
		case 0x0800:
			return tr("WPAD CLASSIC BUTTON HOME");
		case 0x1000:
			return tr("WPAD CLASSIC BUTTON MINUS");
		case 0x2000:
			return tr("WPAD CLASSIC BUTTON FULL L");
		case 0x4000:
			return tr("WPAD CLASSIC BUTTON DOWN");
		case 0x8000:
			return tr("WPAD CLASSIC BUTTON RIGHT");
		default:
			return " ";
	}
}

static inline const char * GetPAD_ButtonName(u16 button)
{
	switch(button)
	{
		case 0x0000:
			return tr("GC PAD BUTTON NONE");
		case 0x0001:
			return tr("GC PAD BUTTON LEFT");
		case 0x0002:
			return tr("GC PAD BUTTON RIGHT");
		case 0x0004:
			return tr("GC PAD BUTTON DOWN");
		case 0x0008:
			return tr("GC PAD BUTTON UP");
		case 0x0010:
			return tr("GC PAD TRIGGER Z");
		case 0x0020:
			return tr("GC PAD TRIGGER R");
		case 0x0040:
			return tr("GC PAD TRIGGER L");
		case 0x0100:
			return tr("GC PAD BUTTON A");
		case 0x0200:
			return tr("GC PAD BUTTON B");
		case 0x0400:
			return tr("GC PAD BUTTON X");
		case 0x0800:
			return tr("GC PAD BUTTON Y");
		case 0x1000:
			return tr("GC PAD BUTTON START");
		default:
			return " ";
	}
}

ControlsSettingsMenu::ControlsSettingsMenu()
	: SettingsMenu(tr("Controller Settings"), &GuiOptions, MENU_NONE)
{
	int Idx = 0;
	
	//Options->SetName(Idx++, tr("Screenshot Hold"));
	//Options->SetName(Idx++, tr("Screenshot Press"));
	Options->SetName(Idx++, tr("Click Button"));
	Options->SetName(Idx++, tr("Click Button"));
	Options->SetName(Idx++, tr("Click Button"));
	Options->SetName(Idx++, tr("Back Button"));
	Options->SetName(Idx++, tr("Back Button"));
	Options->SetName(Idx++, tr("Back Button"));
	Options->SetName(Idx++, tr("Up Button"));
	Options->SetName(Idx++, tr("Up Button"));
	Options->SetName(Idx++, tr("Up Button"));
	Options->SetName(Idx++, tr("Down Button"));
	Options->SetName(Idx++, tr("Down Button"));
	Options->SetName(Idx++, tr("Down Button"));
	Options->SetName(Idx++, tr("Left Button"));
	Options->SetName(Idx++, tr("Left Button"));
	Options->SetName(Idx++, tr("Left Button"));
	Options->SetName(Idx++, tr("Right Button"));
	Options->SetName(Idx++, tr("Right Button"));
	Options->SetName(Idx++, tr("Right Button"));
	Options->SetName(Idx++, tr("Home Button"));
	Options->SetName(Idx++, tr("Home Button"));
	Options->SetName(Idx++, tr("Home Button"));
	Options->SetName(Idx++, tr("Parent Directory"));
	Options->SetName(Idx++, tr("Parent Directory"));
	Options->SetName(Idx++, tr("Parent Directory"));
	
	SetOptionValues();
}

void ControlsSettingsMenu::SetOptionValues()
{
	int Idx = 0;

	//Options->SetValue(Idx++, GetNunchuk_ButtonName(Settings.Controls.ScreenshotHoldButton));
	//Options->SetValue(Idx++, GetNunchuk_ButtonName(Settings.Controls.ScreenshotClickButton));

	Options->SetValue(Idx++, GetWPAD_ButtonName(WiiControls.ClickButton));
	Options->SetValue(Idx++, GetClassic_ButtonName(ClassicControls.ClickButton));
	Options->SetValue(Idx++, GetPAD_ButtonName(GCControls.ClickButton));

	Options->SetValue(Idx++, GetWPAD_ButtonName(WiiControls.BackButton));
	Options->SetValue(Idx++, GetClassic_ButtonName(ClassicControls.BackButton));
	Options->SetValue(Idx++, GetPAD_ButtonName(GCControls.BackButton));

	Options->SetValue(Idx++, GetWPAD_ButtonName(WiiControls.UpButton));
	Options->SetValue(Idx++, GetClassic_ButtonName(ClassicControls.UpButton));
	Options->SetValue(Idx++, GetPAD_ButtonName(GCControls.UpButton));

	Options->SetValue(Idx++, GetWPAD_ButtonName(WiiControls.DownButton));
	Options->SetValue(Idx++, GetClassic_ButtonName(ClassicControls.DownButton));
	Options->SetValue(Idx++, GetPAD_ButtonName(GCControls.DownButton));

	Options->SetValue(Idx++, GetWPAD_ButtonName(WiiControls.LeftButton));
	Options->SetValue(Idx++, GetClassic_ButtonName(ClassicControls.LeftButton));
	Options->SetValue(Idx++, GetPAD_ButtonName(GCControls.LeftButton));

	Options->SetValue(Idx++, GetWPAD_ButtonName(WiiControls.RightButton));
	Options->SetValue(Idx++, GetClassic_ButtonName(ClassicControls.RightButton));
	Options->SetValue(Idx++, GetPAD_ButtonName(GCControls.RightButton));

	Options->SetValue(Idx++, GetWPAD_ButtonName(WiiControls.HomeButton));
	Options->SetValue(Idx++, GetClassic_ButtonName(ClassicControls.HomeButton));
	Options->SetValue(Idx++, GetPAD_ButtonName(GCControls.HomeButton));

	Options->SetValue(Idx++, GetWPAD_ButtonName(WiiControls.UpInDirectory));
	Options->SetValue(Idx++, GetClassic_ButtonName(ClassicControls.UpInDirectory));
	Options->SetValue(Idx++, GetPAD_ButtonName(GCControls.UpInDirectory));
}

int ControlsSettingsMenu::GetMenuInternal()
{
	int ret = optionBrowser->GetClickedOption();

	if (ret < 0)
		return MENU_NONE;

	int Idx = -1;

	/*if (ret == ++Idx)
		Settings.Controls.ScreenshotHoldButton = NextNunchuk_Button(Settings.Controls.ScreenshotHoldButton);
	else if (ret == ++Idx)
		Settings.Controls.ScreenshotClickButton = NextNunchuk_Button(Settings.Controls.ScreenshotClickButton);
	else */if (ret == ++Idx)
		WiiControls.ClickButton = NextWPAD_Button(WiiControls.ClickButton);
	else if (ret == ++Idx)
		ClassicControls.ClickButton = NextClassic_Button(ClassicControls.ClickButton);
	else if (ret == ++Idx)
		GCControls.ClickButton = NextPAD_Button(GCControls.ClickButton);
	else if (ret == ++Idx)
		WiiControls.BackButton = NextWPAD_Button(WiiControls.BackButton);
	else if (ret == ++Idx)
		ClassicControls.BackButton = NextClassic_Button(ClassicControls.BackButton);
	else if (ret == ++Idx)
		GCControls.BackButton = NextPAD_Button(GCControls.BackButton);
	else if (ret == ++Idx)
		WiiControls.UpButton = NextWPAD_Button(WiiControls.UpButton);
	else if (ret == ++Idx)
		ClassicControls.UpButton = NextClassic_Button(ClassicControls.UpButton);
	else if (ret == ++Idx)
		GCControls.UpButton = NextPAD_Button(GCControls.UpButton);
	else if (ret == ++Idx)
		WiiControls.DownButton = NextWPAD_Button(WiiControls.DownButton);
	else if (ret == ++Idx)
		ClassicControls.DownButton = NextClassic_Button(ClassicControls.DownButton);
	else if (ret == ++Idx)
		GCControls.DownButton = NextPAD_Button(GCControls.DownButton);
	else if (ret == ++Idx)
		WiiControls.LeftButton = NextWPAD_Button(WiiControls.LeftButton);
	else if (ret == ++Idx)
		ClassicControls.LeftButton = NextClassic_Button(ClassicControls.LeftButton);
	else if (ret == ++Idx)
		GCControls.LeftButton = NextPAD_Button(GCControls.LeftButton);
	else if (ret == ++Idx)
		WiiControls.RightButton = NextWPAD_Button(WiiControls.RightButton);
	else if (ret == ++Idx)
		ClassicControls.RightButton = NextClassic_Button(ClassicControls.RightButton);
	else if (ret == ++Idx)
		GCControls.RightButton = NextPAD_Button(GCControls.RightButton);
	else if (ret == ++Idx)
		WiiControls.HomeButton = NextWPAD_Button(WiiControls.HomeButton);
	else if (ret == ++Idx)
		ClassicControls.HomeButton = NextClassic_Button(ClassicControls.HomeButton);
	else if (ret == ++Idx)
		GCControls.HomeButton = NextPAD_Button(GCControls.HomeButton);
	else if (ret == ++Idx)
		WiiControls.UpInDirectory = NextWPAD_Button(WiiControls.UpInDirectory);
	else if (ret == ++Idx)
		ClassicControls.UpInDirectory = NextClassic_Button(ClassicControls.UpInDirectory);
	else if (ret == ++Idx)
		GCControls.UpInDirectory = NextPAD_Button(GCControls.UpInDirectory);

	SetOptionValues();

	return MENU_NONE;
}
