#include <gctypes.h>
#include <ogc/system.h>
#include <wiiuse/wpad.h>
#include <uneek_fs.h>

#include "../DeviceControls/DeviceHandler.hpp"
#include "../FileOperations/fileops.h"
#include "../FileOperations/Nand.hpp"
#include "../HomebrewBoot/BootHomebrew.h"
#include "../Settings/CSettings.h"
#include "../Language/gettext.h"
#include "../Network/networkops.h"
#include "../Themes/CTheme.h"
#include "../SoundOperations/SoundHandler.hpp"
#include "../SoundOperations/audio.h"
#include "../Tools/ResourceManager.h"
#include "../Tools/ThreadedTask.hpp"
#include "../Tools/Playlog.h"
#include "../Tools/lstub.h"
#include "../Tools/gecko.h"
#include "../menu.h"
#include "video.h"

extern "C"
{
	extern s32 MagicPatches(s32);
}

//! Variables
u8 shutdown = 0;
u8 reset = 0;

void __Sys_ResetCallback(void)
{
	//! Reboot console
	reset = 1;
}

void __Sys_PowerCallback(void)
{
	//! Poweroff console
	shutdown = 1;
}

void Sys_Init(void)
{
	//! Set RESET/POWER button callback
	SYS_SetResetCallback(__Sys_ResetCallback);
	SYS_SetPowerCallback(__Sys_PowerCallback);
}

void AppCleanUp(void)
{
	gprintf("\nExiting main GUI.\n");
	
	Settings.Save();
	
	ExitGUIThreads();
	StopGX();
	
	delete btnSoundClick;
	delete btnSoundOver;
	delete bgMusic;
	delete background;
	delete bgImg;
	delete mainWindow;
	delete Receiver;
	delete SD_Port;
	for (int i = 0; i < 4; i++)
		delete pointer[i];
	
	gettextCleanUp();
	Theme::CleanUp();
	ThreadedTask::DestroyInstance();
	SoundHandler::DestroyInstance();
	DeinitNetwork();
	
	ShutdownAudio();

	ResourceManager::DestroyInstance();

	WPAD_Shutdown();
}

void ExitApp(void)
{
	AppCleanUp();
	DeviceHandler::DestroyInstance();
	USB_Deinitialize();
	
	if(Settings.PlaylogUpdate)
		Playlog::Delete(); //! Don't show SaveGame Manager GX in the Wii message board

	Nand::Shutdown();
	MagicPatches(0);
}

void Sys_CheckSneek(void)
{
	Settings.Sneek = OFF;
	if(is_neek4(false))
		Settings.Sneek = ON;
}

void Sys_Reboot(void)
{
	//! Restart console
	ExitApp();
	STM_RebootSystem();
}

#define ShutdownToDefault   0
#define ShutdownToIdle      1
#define ShutdownToStandby   2

static void _Sys_Shutdown(int SHUTDOWN_MODE)
{
	ExitApp();

	//! Poweroff console
	if ((CONF_GetShutdownMode() == CONF_SHUTDOWN_IDLE && SHUTDOWN_MODE != ShutdownToStandby) || SHUTDOWN_MODE
			== ShutdownToIdle)
	{
		s32 ret;

		//! Set LED mode
		ret = CONF_GetIdleLedMode();
		if (ret >= 0 && ret <= 2) STM_SetLedMode(ret);

		//! Shutdown to idle
		STM_ShutdownToIdle();
	}
	else
	{
		//! Shutdown to standby
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
	ExitApp();
	
	//! Return to the Wii system menu
	SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);
}

void Sys_BackToLoader(void)
{
	ExitApp();

	if (hbcStubAvailable())
		exit(0);
	
	//! Channel Version
	SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);
}

#define HBC_HAXX    0x0001000148415858LL
#define HBC_JODI    0x000100014A4F4449LL
#define HBC_1_0_7   0x00010001AF1BF516LL

void Sys_LoadHBC(void)
{
	ExitApp();

	WII_Initialize();

	int ret = WII_LaunchTitle(HBC_1_0_7);
	if(ret < 0)
		WII_LaunchTitle(HBC_JODI);
	if(ret < 0)
		WII_LaunchTitle(HBC_HAXX);

	//! Back to system menu if all fails
	SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);
}

void RebootApp(void)
{
	char AppPath[250];
	snprintf(AppPath, sizeof(AppPath), "%s/boot.dol", Settings.UpdatePath);
	BootHomebrew(AppPath);
}
