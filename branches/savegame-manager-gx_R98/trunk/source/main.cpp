/****************************************************************************
 * libwiigui Template
 * Tantric 2009
 *
 * main.cpp
 * Basic template/demonstration of libwiigui capabilities. For a
 * full-featured app using many more extensions, check out Snes9x GX.
 ***************************************************************************/

#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ogcsys.h>
#include <unistd.h>
#include <locale.h>

#include "main.h"
#include "menu/menu.h"
#include "menu/MainWindow.h"

extern bool geckoinit;
extern bool textVideoInit;

unsigned int *xfb = NULL;

NAVINFO navinfo;
WINDOWINFO windowinfo;

GuiBGM * bgMusic = NULL;

void InitTextVideo ()
{
    gprintf("\n\nInitTextVideo ()");
    if (textVideoInit)
    {
        gprintf("...0");
        return;
    }
    VIDEO_Init();
	// get default video mode
    GXRModeObj *vmode = VIDEO_GetPreferredMode(NULL);

    // widescreen fix
    VIDEO_Configure (vmode);

    // Allocate the video buffers
    xfb = (u32 *) MEM_K0_TO_K1 (SYS_AllocateFramebuffer (vmode));

    // A console is always useful while debugging
    console_init (xfb, 20, 64, vmode->fbWidth, vmode->xfbHeight, vmode->fbWidth * 2);

    // Clear framebuffers etc.
    VIDEO_ClearFrameBuffer (vmode, xfb, COLOR_BLACK);
    VIDEO_SetNextFramebuffer (xfb);

    VIDEO_SetBlack (FALSE);
    VIDEO_Flush ();
    VIDEO_WaitVSync ();
    if (vmode->viTVMode & VI_NON_INTERLACE)
        VIDEO_WaitVSync ();

    //send console output to the gecko
    if (geckoinit)CON_EnableGecko(1, true);
    textVideoInit = true;
    gprintf("...1");

}

int main(int argc UNUSED, char *argv[] UNUSED)
{
	geckoinit = InitGecko();

	InitTextVideo();
	
	int ret = IOS_ReloadIOS(249);
	if (ret < 0)
	{
		IOS_ReloadIOS(250);
	}
	
	Sys_Init(); // Initialize wii button call back
	InitVideo(); // Initialize video
	SetupPads(); // Initialize input
	InitAudio(); // Initialize audio
	SDCard_Init(); // Initialize file system
	USBDevice_Init(); // Initialize file system
	ISFS_Initialize();// Initialize Wii file system
	
	ResetNavInfo();
	ResetWindowInfo();
	
	InitApp();
	
	bgMusic = new GuiBGM(bg_music_ogg, bg_music_ogg_size, cfg.MusicVolume);
	bgMusic->SetLoop(cfg.BgMusicLoop);
	bgMusic->Load(cfg.BgMusicPath);
	bgMusic->Play();
	
	MainWindow::Instance()->Show();
}
