/****************************************************************************
 * libwiigui Template
 * Tantric 2009
 *
 * input.cpp
 * Wii/GameCube controller management
 ***************************************************************************/
#include "../stdafx.h"
#include "video.h"
#include "../ImageOperations/Screenshot.h"
#include "../GUI/gui_trigger.h"
#include "../Settings/CSettings.h"
#include "../Controls/Application.h"
#include "input.h"

static int rumbleCount[4] = {0,0,0,0};
static int rumbleRequest[4] = {0,0,0,0};
GuiTrigger userInput[4];
extern bool shutdown;

/****************************************************************************
 * UpdatePads
 *
 * Scans pad and wpad
 ***************************************************************************/
void UpdatePads()
{
	WPAD_ScanPads();
	PAD_ScanPads();

	for(int i=3; i >= 0; i--)
	{
		memcpy(&userInput[i].wpaddata, WPAD_Data(i), sizeof(WPADData));
		userInput[i].pad.btns_d = PAD_ButtonsDown(i);
		userInput[i].pad.btns_u = PAD_ButtonsUp(i);
		userInput[i].pad.btns_h = PAD_ButtonsHeld(i);
		userInput[i].pad.stickX = PAD_StickX(i);
		userInput[i].pad.stickY = PAD_StickY(i);
		userInput[i].pad.substickX = PAD_SubStickX(i);
		userInput[i].pad.substickY = PAD_SubStickY(i);
		userInput[i].pad.triggerL = PAD_TriggerL(i);
		userInput[i].pad.triggerR = PAD_TriggerR(i);
		
		if(Settings.Rumble)
			DoRumble(i);
		
		//if((userInput[i].wpad->btns_h & Settings.Controls.ScreenshotHoldButton) && (userInput[i].wpad->btns_d & Settings.Controls.ScreenshotClickButton))
		if((userInput[i].wpaddata.btns_h & WPAD_NUNCHUK_BUTTON_Z) && (userInput[i].wpaddata.btns_d & WPAD_NUNCHUK_BUTTON_C))
			Screenshot();
	}
}

/****************************************************************************
 * ScreensaverTime
 ***************************************************************************/
static inline u32 ScreensaverTime(int setting)
{
	switch (setting)
	{
		case 0:
			return 0xFFFFFF;
		case 1:
			return 60;
		case 2:
			return 180;
		case 3:
			return 300;
		case 4:
			return 600;
		case 5:
			return 1200;
		case 6:
			return 1800;
		case 7:
			return 3600;
		default:
			break;
	}
	
	return 0xFFFFFF;
}

/****************************************************************************
  * SetWPADTimeout
 ***************************************************************************/
void SetWPADTimeout()
{
	WPAD_SetIdleTimeout(ScreensaverTime(Settings.Screensaver));
}

/****************************************************************************
 * ControlActivityTimeOut
 ***************************************************************************/
bool ControlActivityTimeout(void)
{
	u32 minTime = 0xFFFFFF;
	for(int i = 0; i < 3; ++i)
		if(Application::Instance()->pointer[i]->getLastActivCounter() < minTime)
			minTime = Application::Instance()->pointer[i]->getLastActivCounter();

	//! not very accurate but it's not required here
	return (minTime/(Settings.PAL50 ? 50 : 60) > ScreensaverTime(Settings.Screensaver));
}
/****************************************************************************
 * SetupPads
 *
 * Sets up userInput triggers for use
 ***************************************************************************/
void SetupPads()
{
	PAD_Init();
	WPAD_Init();

	// read wiimote accelerometer and IR data
	WPAD_SetDataFormat(WPAD_CHAN_ALL,WPAD_FMT_BTNS_ACC_IR);
	WPAD_SetVRes(WPAD_CHAN_ALL, screenwidth, screenheight);

	for(int i=0; i < 4; i++)
	{
		userInput[i].chan = i;
		//userInput[i].wpad = WPAD_Data(i);
	}

	SetWPADTimeout();
}

/****************************************************************************
 * ShutoffRumble
 ***************************************************************************/
void ShutoffRumble()
{
	for(int i = 0; i < 4; i++)
	{
		WPAD_Rumble(i, 0);
		rumbleCount[i] = 0;
	}
}

/****************************************************************************
 * Shutdown Pads
 ***************************************************************************/
void ShutdownPads()
{
	ShutoffRumble();
	WPAD_Flush(0);
	WPAD_Disconnect(0);
	WPAD_Shutdown();
}

/****************************************************************************
 * Request for Rumble
 ***************************************************************************/
void RequestRumble(int chan)
{
	rumbleRequest[chan] = 1;
}

/****************************************************************************
 * DoRumble
 ***************************************************************************/
void DoRumble(int i)
{
	if(rumbleRequest[i] && rumbleCount[i] == 0)
	{
		WPAD_Rumble(i, 1); //! rumble on
		rumbleRequest[i] = 0;
		rumbleCount[i] = 8;
	}
	else
	{
		if(rumbleCount[i] > 0)
			--rumbleCount[i];
		
		if(rumbleCount[i] < 4)
			WPAD_Rumble(i, 0); //! rumble off
		
		rumbleRequest[i] = 0;
	}
}

/****************************************************************************
 * Initialize Stuff
 ***************************************************************************/
extern "C" void __Wpad_PowerCallback(s32 chan UNUSED)
{
	shutdown = true;
}

int Wpad_Init(void)
{
	int ret;

	ret = WPAD_Init();
	if (ret < 0)
		return ret;

	WPAD_SetPowerButtonCallback(__Wpad_PowerCallback);

	return ret;
}

/****************************************************************************
 * check if a button is pressed
 ***************************************************************************/
bool ButtonPressed(void)
{
	int i;
	u32 buttons = 0;

	for (i = 3; i >= 0; i--)
	{
		buttons |= PAD_ButtonsDown(i);
		buttons |= WPAD_ButtonsDown(i);
	}
	
	return buttons > 0;
}

/****************************************************************************
 * check if a button is released
 ***************************************************************************/
bool ButtonReleased(void)
{
	int i;
	u32 buttons = 0;

	for (i = 3; i >= 0; i--)
	{
		buttons |= PAD_ButtonsUp(i);
		buttons |= WPAD_ButtonsUp(i);
	}
	
	return buttons > 0;
}
