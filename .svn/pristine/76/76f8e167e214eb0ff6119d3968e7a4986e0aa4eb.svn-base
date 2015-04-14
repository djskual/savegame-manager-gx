/****************************************************************************
 * libwiigui Template
 * Tantric 2009
 *
 * input.cpp
 * Wii/GameCube controller management
 ***************************************************************************/

#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ogcsys.h>
#include <unistd.h>
#include <wiiuse/wpad.h>

#include "menu/menu.h"
#include "libwiigui/gui.h"
#include "main.h"

int rumbleRequest[4] = {0,0,0,0};
GuiTrigger userInput[4];
static int rumbleCount[4] = {0,0,0,0};

extern u8 shutdown;

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
		userInput[i].pad.btns_d = PAD_ButtonsDown(i);
		userInput[i].pad.btns_u = PAD_ButtonsUp(i);
		userInput[i].pad.btns_h = PAD_ButtonsHeld(i);
		userInput[i].pad.stickX = PAD_StickX(i);
		userInput[i].pad.stickY = PAD_StickY(i);
		userInput[i].pad.substickX = PAD_SubStickX(i);
		userInput[i].pad.substickY = PAD_SubStickY(i);
		userInput[i].pad.triggerL = PAD_TriggerL(i);
		userInput[i].pad.triggerR = PAD_TriggerR(i);
	}
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
		userInput[i].wpad = WPAD_Data(i);
	}
}

/****************************************************************************
 * ShutoffRumble
 ***************************************************************************/

void ShutoffRumble()
{
	for(int i=0; i < 4; i++)
	{
		WPAD_Rumble(i, 0);
		rumbleCount[i] = 0;
	}
}

/****************************************************************************
 * DoRumble
 ***************************************************************************/

void DoRumble(int i)
{
	if(rumbleRequest[i] && rumbleCount[i] < 3)
	{
		WPAD_Rumble(i, 1); // rumble on
		rumbleCount[i]++;
	}
	else if(rumbleRequest[i])
	{
		rumbleCount[i] = 12;
		rumbleRequest[i] = 0;
	}
	else
	{
		if(rumbleCount[i])
			rumbleCount[i]--;
		WPAD_Rumble(i, 0); // rumble off
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
 * WPAD_Stick
 *
 * Get X/Y value from Wii Joystick (classic, nunchuk) input
 ***************************************************************************/

s8 WPAD_Stick(u8 chan, u8 right, int axis)
{
	float mag = 0.0;
	float ang = 0.0;
	WPADData *data = WPAD_Data(chan);

	switch (data->exp.type)
	{
		case WPAD_EXP_NUNCHUK:
		case WPAD_EXP_GUITARHERO3:
			if (right == 0)
			{
				mag = data->exp.nunchuk.js.mag;
				ang = data->exp.nunchuk.js.ang;
			}
			break;

		case WPAD_EXP_CLASSIC:
			if (right == 0)
			{
				mag = data->exp.classic.ljs.mag;
				ang = data->exp.classic.ljs.ang;
			}
			else
			{
				mag = data->exp.classic.rjs.mag;
				ang = data->exp.classic.rjs.ang;
			}
			break;

		default:
			break;
	}

	/* calculate x/y value (angle need to be converted into radian) */
	if (mag > 1.0) mag = 1.0;
	else if (mag < -1.0) mag = -1.0;
	double val;

	if(axis == 0) // x-axis
		val = mag * sin((PI * ang)/180.0f);
	else // y-axis
		val = mag * cos((PI * ang)/180.0f);

	return (s8)(val * 128.0f);
}

/****************************************************************************
 * Initialize Stuff
 ***************************************************************************/
void __Wpad_PowerCallback(s32 chan UNUSED)
{
	shutdown = 1;
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

void WPad_SetIdleTime(u32 seconds) {
    /*Set idle time for wiimote*/
    WPAD_SetIdleTimeout(seconds);
}

bool IsWpadConnected() {
    int i = 0;
    u32 test = 0;
    int notconnected = 0;
    for (i=3; i >= 0; i--) {
        if (WPAD_Probe(i, &test) == WPAD_ERR_NO_CONTROLLER) {
            notconnected++;
        }
    }
    if (notconnected < 4)
        return true;
    else
        return false;
}

u32 ButtonsPressed(void) {

    int i;
    u32 buttons = 0;

    WPAD_ScanPads();
    PAD_ScanPads();

    for (i=3; i >= 0; i--) {
        buttons |= PAD_ButtonsDown(i);
        buttons |= WPAD_ButtonsDown(i);
    }
    return buttons;

    /*  Don't remove this commented out code it might be useful for checking which buttons were pressed/hold

        if(buttons & (PAD_BUTTON_LEFT | PAD_BUTTON_RIGHT | PAD_BUTTON_DOWN | PAD_BUTTON_UP
                        | PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_X | PAD_BUTTON_Y | PAD_BUTTON_MENU
                        | PAD_BUTTON_START | WPAD_BUTTON_2 | WPAD_BUTTON_1
                        | WPAD_BUTTON_B | WPAD_BUTTON_A | WPAD_BUTTON_MINUS
                        | WPAD_BUTTON_HOME | WPAD_BUTTON_LEFT | WPAD_BUTTON_RIGHT
                        | WPAD_BUTTON_DOWN | WPAD_BUTTON_UP | WPAD_BUTTON_PLUS
                        | WPAD_NUNCHUK_BUTTON_Z | WPAD_NUNCHUK_BUTTON_C
                        | WPAD_CLASSIC_BUTTON_UP | WPAD_CLASSIC_BUTTON_LEFT
                        | WPAD_CLASSIC_BUTTON_ZR | WPAD_CLASSIC_BUTTON_X
                        | WPAD_CLASSIC_BUTTON_A | WPAD_CLASSIC_BUTTON_Y
                        | WPAD_CLASSIC_BUTTON_B | WPAD_CLASSIC_BUTTON_ZL
                        | WPAD_CLASSIC_BUTTON_FULL_R | WPAD_CLASSIC_BUTTON_PLUS
                        | WPAD_CLASSIC_BUTTON_HOME | WPAD_CLASSIC_BUTTON_MINUS
                        | WPAD_CLASSIC_BUTTON_FULL_L | WPAD_CLASSIC_BUTTON_DOWN
                        | WPAD_CLASSIC_BUTTON_RIGHT | WPAD_GUITAR_HERO_3_BUTTON_STRUM_UP
                        | WPAD_GUITAR_HERO_3_BUTTON_YELLOW | WPAD_GUITAR_HERO_3_BUTTON_GREEN
                        | WPAD_GUITAR_HERO_3_BUTTON_BLUE | WPAD_GUITAR_HERO_3_BUTTON_RED
                        | WPAD_GUITAR_HERO_3_BUTTON_ORANGE | WPAD_GUITAR_HERO_3_BUTTON_PLUS
                        | WPAD_GUITAR_HERO_3_BUTTON_MINUS | WPAD_GUITAR_HERO_3_BUTTON_STRUM_DOWN)
          )
    */

}

