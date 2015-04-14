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
#include "ControlsSettingsMenu.hpp"
#include "SettingsMenu.h"

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
	: SettingsBrowser()
{
	SettingsMenu::Instance()->SetTitle(tr( "Controller Settings" ));
	
	SetupOptionName();
	show();
}

void ControlsSettingsMenu::SetupOptionName()
{
	int Idx = 0;
	
	//options->SetName(Idx++, tr("Screenshot Hold"));
	//options->SetName(Idx++, tr("Screenshot Press"));
	options->SetName(Idx++, tr("Click Button"));
	options->SetName(Idx++, tr("Click Button"));
	options->SetName(Idx++, tr("Click Button"));
	options->SetName(Idx++, tr("Back Button"));
	options->SetName(Idx++, tr("Back Button"));
	options->SetName(Idx++, tr("Back Button"));
	options->SetName(Idx++, tr("Up Button"));
	options->SetName(Idx++, tr("Up Button"));
	options->SetName(Idx++, tr("Up Button"));
	options->SetName(Idx++, tr("Down Button"));
	options->SetName(Idx++, tr("Down Button"));
	options->SetName(Idx++, tr("Down Button"));
	options->SetName(Idx++, tr("Left Button"));
	options->SetName(Idx++, tr("Left Button"));
	options->SetName(Idx++, tr("Left Button"));
	options->SetName(Idx++, tr("Right Button"));
	options->SetName(Idx++, tr("Right Button"));
	options->SetName(Idx++, tr("Right Button"));
	options->SetName(Idx++, tr("Home Button"));
	options->SetName(Idx++, tr("Home Button"));
	options->SetName(Idx++, tr("Home Button"));
	options->SetName(Idx++, tr("Parent Directory"));
	options->SetName(Idx++, tr("Parent Directory"));
	options->SetName(Idx++, tr("Parent Directory"));
	//here we can add onescroll and keyshift buttons
	
	SetOptionValues();
}

void ControlsSettingsMenu::SetOptionValues()
{
	int Idx = 0;

	//options->SetValue(Idx++, GetNunchuk_ButtonName(Settings.Controls.ScreenshotHoldButton));
	//options->SetValue(Idx++, GetNunchuk_ButtonName(Settings.Controls.ScreenshotClickButton));

	options->SetValue(Idx++, GetWPAD_ButtonName(WiiControls.ClickButton));
	options->SetValue(Idx++, GetClassic_ButtonName(ClassicControls.ClickButton));
	options->SetValue(Idx++, GetPAD_ButtonName(GCControls.ClickButton));

	options->SetValue(Idx++, GetWPAD_ButtonName(WiiControls.BackButton));
	options->SetValue(Idx++, GetClassic_ButtonName(ClassicControls.BackButton));
	options->SetValue(Idx++, GetPAD_ButtonName(GCControls.BackButton));

	options->SetValue(Idx++, GetWPAD_ButtonName(WiiControls.UpButton));
	options->SetValue(Idx++, GetClassic_ButtonName(ClassicControls.UpButton));
	options->SetValue(Idx++, GetPAD_ButtonName(GCControls.UpButton));

	options->SetValue(Idx++, GetWPAD_ButtonName(WiiControls.DownButton));
	options->SetValue(Idx++, GetClassic_ButtonName(ClassicControls.DownButton));
	options->SetValue(Idx++, GetPAD_ButtonName(GCControls.DownButton));

	options->SetValue(Idx++, GetWPAD_ButtonName(WiiControls.LeftButton));
	options->SetValue(Idx++, GetClassic_ButtonName(ClassicControls.LeftButton));
	options->SetValue(Idx++, GetPAD_ButtonName(GCControls.LeftButton));

	options->SetValue(Idx++, GetWPAD_ButtonName(WiiControls.RightButton));
	options->SetValue(Idx++, GetClassic_ButtonName(ClassicControls.RightButton));
	options->SetValue(Idx++, GetPAD_ButtonName(GCControls.RightButton));

	options->SetValue(Idx++, GetWPAD_ButtonName(WiiControls.HomeButton));
	options->SetValue(Idx++, GetClassic_ButtonName(ClassicControls.HomeButton));
	options->SetValue(Idx++, GetPAD_ButtonName(GCControls.HomeButton));

	options->SetValue(Idx++, GetWPAD_ButtonName(WiiControls.UpInDirectory));
	options->SetValue(Idx++, GetClassic_ButtonName(ClassicControls.UpInDirectory));
	options->SetValue(Idx++, GetPAD_ButtonName(GCControls.UpInDirectory));
}

void ControlsSettingsMenu::OnBrowserClick(GuiOptionBrowser * browser UNUSED, int option)
{
	switch(option)
	{
		/*case 0:
			Settings.Controls.ScreenshotHoldButton = NextNunchuk_Button(Settings.Controls.ScreenshotHoldButton);
			break;
		case 1:
			Settings.Controls.ScreenshotClickButton = NextNunchuk_Button(Settings.Controls.ScreenshotClickButton);
			break;*/
		case 0:
			WiiControls.ClickButton = NextWPAD_Button(WiiControls.ClickButton);
			break;
		case 1:
			ClassicControls.ClickButton = NextClassic_Button(ClassicControls.ClickButton);
			break;
		case 2:
			GCControls.ClickButton = NextPAD_Button(GCControls.ClickButton);
			break;
		case 3:
			WiiControls.BackButton = NextWPAD_Button(WiiControls.BackButton);
			break;
		case 4:
			ClassicControls.BackButton = NextClassic_Button(ClassicControls.BackButton);
			break;
		case 5:
			GCControls.BackButton = NextPAD_Button(GCControls.BackButton);
			break;
		case 6:
			WiiControls.UpButton = NextWPAD_Button(WiiControls.UpButton);
			break;
		case 7:
			ClassicControls.UpButton = NextClassic_Button(ClassicControls.UpButton);
			break;
		case 8:
			GCControls.UpButton = NextPAD_Button(GCControls.UpButton);
			break;
		case 9:
			WiiControls.DownButton = NextWPAD_Button(WiiControls.DownButton);
			break;
		case 10:
			ClassicControls.DownButton = NextClassic_Button(ClassicControls.DownButton);
			break;
		case 11:
			GCControls.DownButton = NextPAD_Button(GCControls.DownButton);
			break;
		case 12:
			WiiControls.LeftButton = NextWPAD_Button(WiiControls.LeftButton);
			break;
		case 13:
			ClassicControls.LeftButton = NextClassic_Button(ClassicControls.LeftButton);
			break;
		case 14:
			GCControls.LeftButton = NextPAD_Button(GCControls.LeftButton);
			break;
		case 15:
			WiiControls.RightButton = NextWPAD_Button(WiiControls.RightButton);
			break;
		case 16:
			ClassicControls.RightButton = NextClassic_Button(ClassicControls.RightButton);
			break;
		case 17:
			GCControls.RightButton = NextPAD_Button(GCControls.RightButton);
			break;
		case 18:
			WiiControls.HomeButton = NextWPAD_Button(WiiControls.HomeButton);
			break;
		case 19:
			ClassicControls.HomeButton = NextClassic_Button(ClassicControls.HomeButton);
			break;
		case 20:
			GCControls.HomeButton = NextPAD_Button(GCControls.HomeButton);
			break;
		case 21:
			WiiControls.UpInDirectory = NextWPAD_Button(WiiControls.UpInDirectory);
			break;
		case 22:
			ClassicControls.UpInDirectory = NextClassic_Button(ClassicControls.UpInDirectory);
			break;
		case 23:
			GCControls.UpInDirectory = NextPAD_Button(GCControls.UpInDirectory);
			break;
		default:
			break;
	}

	SetOptionValues();
}
