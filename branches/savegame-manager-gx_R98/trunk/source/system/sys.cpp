#include <stdio.h>
#include <ogcsys.h>
#include <gccore.h>
#include <stdlib.h>
#include <wiiuse/wpad.h>

#include "boot_homebrew/BootHomebrew.h"
#include "prompts/promptwindows.h"
#include "file/fileops.h"
#include "network/networkops.h"
#include "menu/MainWindow.h"
#include "main.h"

bool RebootApp()
{
    char filepath[300];
    snprintf(filepath, sizeof(filepath), "%s/boot.dol", cfg.UpdatePath);
    int ret = LoadHomebrew(filepath);
    if(ret < 0)
    {
        WindowPrompt(tr("Reboot failed"), tr("Can't load file"), tr("OK"), 0, 0, 0, false);
        return false;
    }

    AddBootArgument(filepath);

    MainWindow::Instance()->SetBootHomebrew(true);

    return true;
}

void AppExit()
{
	ShutdownPads();
	MainWindow::DestroyInstance();
	StopGX();
	ShutdownAudio();
	Resources::DestroyInstance();
	USBDevice_deInit();
	DeInit_Network();
}

u8 shutdown = 0;
u8 reset = 0;

void __Sys_ResetCallback(void)
{
	reset = 1;
}

void __Sys_PowerCallback(void)
{
	shutdown = 1;
}


void Sys_Init(void)
{
	SYS_SetResetCallback(__Sys_ResetCallback);
	SYS_SetPowerCallback(__Sys_PowerCallback);
}

void Sys_Reboot(void)
{
	AppExit();
	STM_RebootSystem();
}

#define ShutdownToDefault	0
#define ShutdownToIdle		1
#define ShutdownToStandby	2

static void _Sys_Shutdown(int SHUTDOWN_MODE)
{
	AppExit();
	
	if((CONF_GetShutdownMode() == CONF_SHUTDOWN_IDLE &&  SHUTDOWN_MODE != ShutdownToStandby) || SHUTDOWN_MODE == ShutdownToIdle) {
		s32 ret;

		ret = CONF_GetIdleLedMode();
		if(ret >= 0 && ret <= 2)
			STM_SetLedMode(ret);

		STM_ShutdownToIdle();
	} else {
		STM_ShutdownToStandby();
	}
}

void Sys_Shutdown(void)
{
	_Sys_Shutdown(ShutdownToDefault);
}

void Sys_ShutdownToIdle(void)
{
	_Sys_Shutdown(ShutdownToIdle);
}

void Sys_ShutdownToStandby(void)
{
	_Sys_Shutdown(ShutdownToStandby);
}

void Sys_LoadMenu(void)
{
	AppExit();
	/* Return to the Wii system menu */
	SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);
}

void Sys_BackToLoader(void)
{
	AppExit();
	if (*((u32*) 0x80001800))
	{
		exit(0);
	}
	// Channel Version
	Sys_LoadMenu();
}
