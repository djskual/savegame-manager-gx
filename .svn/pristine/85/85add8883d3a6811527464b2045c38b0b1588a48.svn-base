/*
 *      loadMii loader v0.3
 *      main.c
 *      http://code.google.com/p/loadmii
 *
 *      Copyright 2009 The Lemon Man
 *      Thanks to luccax, Wiipower, Aurelio and crediar
 *      GX added by dimok (thanks to Tantric)
 *      usbGecko powered by Nuke
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

#include <gccore.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ogc/machine/processor.h>

#include "video.h"
#include "pngu.h"
#include "dolloader.h"
#include "filelist.h"
#include "cfg.h"
#include "fatmounter.h"

extern "C"
{
    extern void __exception_setreload(int t);
}

static PNGUPROP imgProp;
static IMGCTX ctx;


u8 * GetImageData(void)
{
	u8 * data = NULL;

	int ret;

	if (CONF_GetAspectRatio()) {
		switch (CONF_GetLanguage()) {
			case CONF_LANG_FRENCH:
				ctx = PNGU_SelectImageFromBuffer(bk169fr_png);
				break;
			case CONF_LANG_JAPANESE:
				ctx = PNGU_SelectImageFromBuffer(bk169jp_png);
				break;
			case CONF_LANG_SPANISH:
				ctx = PNGU_SelectImageFromBuffer(bk169sp_png);
				break;
			case CONF_LANG_ITALIAN:
				ctx = PNGU_SelectImageFromBuffer(bk169it_png);
				break;
			case CONF_LANG_DUTCH:
				ctx = PNGU_SelectImageFromBuffer(bk169du_png);
				break;
			case CONF_LANG_GERMAN:
				ctx = PNGU_SelectImageFromBuffer(bk169ge_png);
				break;
			default:
				ctx = PNGU_SelectImageFromBuffer(bk169en_png);
				break;
		}
	} else {
		switch (CONF_GetLanguage()) {
			case CONF_LANG_FRENCH:
				ctx = PNGU_SelectImageFromBuffer(bkfr_png);
				break;
			case CONF_LANG_JAPANESE:
				ctx = PNGU_SelectImageFromBuffer(bkjp_png);
				break;
			case CONF_LANG_SPANISH:
				ctx = PNGU_SelectImageFromBuffer(bksp_png);
				break;
			case CONF_LANG_ITALIAN:
				ctx = PNGU_SelectImageFromBuffer(bkit_png);
				break;
			case CONF_LANG_DUTCH:
				ctx = PNGU_SelectImageFromBuffer(bkdu_png);
				break;
			case CONF_LANG_GERMAN:
				ctx = PNGU_SelectImageFromBuffer(bkge_png);
				break;
			default:
				ctx = PNGU_SelectImageFromBuffer(bken_png);
				break;
		}
	}
	if (!ctx)
		return NULL;

	ret = PNGU_GetImageProperties(ctx, &imgProp);
	if (ret != PNGU_OK)
		return NULL;

    int len = imgProp.imgWidth * imgProp.imgHeight * 4;

    if(len%32) len += (32-len%32);
    data = (u8 *)memalign (32, len);
    ret = PNGU_DecodeTo4x4RGBA8 (ctx, imgProp.imgWidth, imgProp.imgHeight, data, 255);
	DCFlushRange(data, len);

	PNGU_ReleaseImageContext(ctx);

    return data;
}

void Background_Show(int x, int y, int z, u8 * data, int angle, int scaleX, int scaleY, int alpha)
{
	/* Draw image */
	Menu_DrawImg(x, y, z, imgProp.imgWidth, imgProp.imgHeight, data, angle, scaleX, scaleY, alpha);
    Menu_Render();
}

void fadein(u8 * imgdata)
{
	/* fadein of image */
	for(int i = 0; i < 255; i = i+10)
	{
		if(i>255) i = 255;
		Background_Show(0, 0, 0, imgdata, 0, 1, 1, i);
	}
}

void fadeout(u8 * imgdata)
{
	/* fadeoout of image */
	for(int i = 255; i > 1; i = i-7)
	{
		if(i < 0) i = 0;
		Background_Show(0, 0, 0, imgdata, 0, 1, 1, i);
	}
}

int main(int argc, char **argv)
{
	u32 cookie;
	FILE *exeFile = NULL;
	void *exeBuffer = (void *)EXECUTABLE_MEM_ADDR;
	u32 exeSize = 0;
	u32 exeEntryPointAddress = 0;
	entrypoint exeEntryPoint;
	__exception_setreload(0);

	IOS_ReloadIOS(58);

	/* int videomod */
	InitVideo();
	/* get imagedata */
	u8 * imgdata = GetImageData();
	fadein(imgdata);
	/* check devices */
	SDCard_Init();
	USBDevice_Init();

	char cfgpath[256];
	bool result = false;

	sprintf(cfgpath, "sd:/config/SaveGame_Manager_GX/SaveGame_Manager_GX.cfg");
	result = cfg_parsefile(cfgpath, &cfg_set);
	if(!result) //no cfg-File on SD: try USB:
	{
		sprintf(cfgpath, "usb:config/SaveGame_Manager_GX/SaveGame_Manager_GX.cfg");
		result = cfg_parsefile(cfgpath, &cfg_set);
	}

    if(result)
    {
        sprintf(cfgpath, "%sboot.dol", update_path);
		/* Open dol File and check exist */
        exeFile = fopen (cfgpath, "rb");
        if (exeFile==NULL)
        {
            sprintf(cfgpath, "%sboot.elf", update_path);
            exeFile = fopen (cfgpath,"rb");
        }
        if (exeFile==NULL)
			result = false;
        else
        result = true;
    }

	if(!result) // non cfg-File loaded or update_path not set
	{
		/* Open dol File and check exist */
		sprintf(cfgpath, "sd:/apps/SaveGame_Manager_GX/boot.dol");
		exeFile = fopen (cfgpath ,"rb");
		if (exeFile==NULL)
		{
			sprintf(cfgpath, "sd:/apps/SaveGame_Manager_GX/boot.elf");
			exeFile = fopen (cfgpath ,"rb");
		}
		if (exeFile==NULL)
		{
			sprintf(cfgpath, "usb:/apps/SaveGame_Manager_GX/boot.dol");
			exeFile = fopen (cfgpath ,"rb");
		}
		if (exeFile==NULL)
		{
			sprintf(cfgpath, "usb:/apps/SaveGame_Manager_GX/boot.elf");
			exeFile = fopen (cfgpath ,"rb");
		}
		if (exeFile==NULL)
		{
			sprintf(cfgpath, "sd:/apps/SaveGameManagerGX/boot.dol");
			exeFile = fopen (cfgpath ,"rb");
		}
		if (exeFile==NULL)
		{
			sprintf(cfgpath, "sd:/apps/SaveGameManagerGX/boot.elf");
			exeFile = fopen (cfgpath ,"rb");
		}
		if (exeFile==NULL)
		{
			sprintf(cfgpath, "usb:/apps/SaveGameManagerGX/boot.dol");
			exeFile = fopen (cfgpath ,"rb");
		}
		if (exeFile==NULL)
		{
			sprintf(cfgpath, "usb:/apps/SaveGameManagerGX/boot.elf");
			exeFile = fopen (cfgpath ,"rb");
		}
		// if nothing found exiting
		if (exeFile==NULL)
		{
            fadeout(imgdata);
            fclose (exeFile);
            SDCard_deInit();
            USBDevice_deInit();
            StopGX();
            free(imgdata);
			SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);
		}
	}

	fseek (exeFile, 0, SEEK_END);
	exeSize = ftell(exeFile);
	rewind (exeFile);

	if(fread (exeBuffer, 1, exeSize, exeFile) != exeSize)
	{
		fadeout(imgdata);
        fclose (exeFile);
        SDCard_deInit();
        USBDevice_deInit();
        StopGX();
        free(imgdata);
		SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);
	}
	fclose (exeFile);

	/* load entry point */
	struct __argv args;
	bzero(&args, sizeof(args));
	args.argvMagic = ARGV_MAGIC;
	args.length = strlen(cfgpath) + 2;
	args.commandLine = (char*)malloc(args.length);
	if (!args.commandLine) SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);
	strcpy(args.commandLine, cfgpath);
	args.commandLine[args.length - 1] = '\0';
	args.argc = 1;
	args.argv = &args.commandLine;
	args.endARGV = args.argv + 1;

	u8 * appboot_buff = (u8 *) malloc(app_booter_dol_size);
	if(!appboot_buff)
	{
		fadeout(imgdata);
        SDCard_deInit();
        USBDevice_deInit();
        StopGX();
        free(imgdata);
		SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);
	}

    memcpy(appboot_buff, app_booter_dol, app_booter_dol_size);

	exeEntryPointAddress = load_dol_image(appboot_buff, &args);

    if(appboot_buff)
        free(appboot_buff);

	fadeout(imgdata);
	SDCard_deInit();
	USBDevice_deInit();
	StopGX();
	free(imgdata);

	if (exeEntryPointAddress == 0)
	{
		SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);
	}

	exeEntryPoint = (entrypoint) exeEntryPointAddress;
	/* cleaning up and load dol */
	SYS_ResetSystem(SYS_SHUTDOWN, 0, 0);
	_CPU_ISR_Disable (cookie);
	__exception_closeall ();
	exeEntryPoint ();
	_CPU_ISR_Restore (cookie);
	return 0;
}
