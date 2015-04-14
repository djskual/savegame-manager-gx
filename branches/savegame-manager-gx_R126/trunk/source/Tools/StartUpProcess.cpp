#include <unistd.h>

#include "StartUpProcess.h"
#include "../SoundOperations/audio.h"
#include "../FileOperations/Nand.hpp"
#include "../System/video.h"
#include "../System/input.h"
#include "../System/sys.h"
#include "../System/IosLoader.h"
#include "../Settings/CSettings.h"
#include "../Saves/CSaveList.hpp"
#include "../EmuNand/CEmuList.hpp"
#include "../EmuNand/CEmuMiiList.hpp"
#include "../Themes/CTheme.h"
#include "../Miis/CMiiList.hpp"
#include "../DeviceControls/DeviceHandler.hpp"
#include "gecko.h"

#define TotalStep	11

StartUpProcess::StartUpProcess()
{
	showProgress = false;
	
	//! Load default font for the next text outputs
	Theme::LoadFont("");
	
	background = new GuiImage(screenwidth, screenheight, (GXColor) {0, 0, 0, 255});
	
	GXImageData = Resources::GetImageData("logo_SGMGX.png");
	GXImage = new GuiImage(GXImageData);
	GXImage->SetAlignment(ALIGN_CENTER, ALIGN_MIDDLE);
	GXImage->SetPosition(screenwidth/2, screenheight/2-50);
	
	titleTxt = new GuiText("Loading...", 24, (GXColor) {255, 255, 255, 255});
	titleTxt->SetAlignment(ALIGN_CENTER, ALIGN_MIDDLE);
	titleTxt->SetPosition(screenwidth/2, screenheight/2+30);
	
	ProgressBarOutlineData = Resources::GetImageData("progress_bar_startup_outline.png");
	ProgressBarEmptyData = Resources::GetImageData("progress_bar_startup_empty.png");
	ProgressBarData = Resources::GetImageData("progress_bar.png");
	ProgressBarOutline = new GuiImage(ProgressBarOutlineData);
	ProgressBarEmpty = new GuiImage(ProgressBarEmptyData);
	ProgressBar = new GuiImage(ProgressBarData);
	
	messageTxt = new GuiText(" ", 22, (GXColor) {255, 255, 255, 255});
	messageTxt->SetAlignment(ALIGN_CENTER, ALIGN_MIDDLE);
	messageTxt->SetPosition(screenwidth/2, screenheight/2+120);
}

StartUpProcess::~StartUpProcess()
{
	delete background;
	delete GXImageData;
	delete GXImage;
	delete titleTxt;
	delete ProgressBarOutlineData;
	delete ProgressBarEmptyData;
	delete ProgressBarData;
	delete ProgressBarOutline;
	delete ProgressBarEmpty;
	delete ProgressBar;
	delete messageTxt;
}

void StartUpProcess::ShowProgress(int done)
{
	showProgress = true;
	
	delete ProgressBarOutline;
	delete ProgressBarEmpty;
	delete ProgressBar;
	ProgressBarOutline = NULL;
	ProgressBarEmpty = NULL;
	ProgressBar = NULL;
	
	ProgressBarOutline = new GuiImage(ProgressBarOutlineData);
	ProgressBarOutline->SetAlignment(ALIGN_CENTER, ALIGN_MIDDLE);
	ProgressBarOutline->SetPosition(screenwidth/2, screenheight/2+75);

	ProgressBarEmpty = new GuiImage(ProgressBarEmptyData);
	ProgressBarEmpty->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	ProgressBarEmpty->SetPosition(ProgressBarOutline->GetLeft(), screenheight/2+75);
	ProgressBarEmpty->SetTileHorizontal(100);

	ProgressBar = new GuiImage(ProgressBarData);
	ProgressBar->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	ProgressBar->SetPosition(ProgressBarOutline->GetLeft(), screenheight/2+75);
	ProgressBar->SetTileHorizontal(100*done/TotalStep);
}


void StartUpProcess::ParseArguments(int argc, char *argv[])
{
	//! The arguments override
	for(int i = 0; i < argc; ++i)
	{
		if(!argv[i]) continue;
		
		gprintf("Boot argument %i: %s\n", i+1, argv[i]);
		
		char *ptr = strcasestr(argv[i], "-ios=");
		if(ptr)
		{
			int ios = atoi(ptr+strlen("-ios="));
			if(ios == 58)
				Settings.UseIOS58 = 1;
			else
				Settings.cIOS = LIMIT(ios, 200, 255);
		}
	}
}

void StartUpProcess::TextFade(int direction)
{
	if(direction > 0)
	{
		for(int i = 0; i < 255; i += direction)
		{
			messageTxt->SetAlpha(i);
			Draw();
		}
		messageTxt->SetAlpha(255);
		Draw();
	}
	else if(direction < 0)
	{
		for(int i = 255; i > 0; i += direction)
		{
			messageTxt->SetAlpha(i);
			Draw();
		}
		messageTxt->SetAlpha(0);
		Draw();
	}
}

void StartUpProcess::SetTextf(const char * format, ...)
{
	char * tmp = NULL;
	va_list va;
	va_start(va, format);
	if((vasprintf(&tmp, format, va) >= 0) && tmp)
	{
		TextFade(-40);
		gprintf(tmp);
		messageTxt->SetText(tmp);
		TextFade(40);
	}
	va_end(va);

	if(tmp)
		free(tmp);
}

bool StartUpProcess::Run(int argc, char *argv[])
{
	StartUpProcess Process;

	return Process.Execute(argc, argv);
}

bool StartUpProcess::Execute(int argc, char *argv[])
{
	ShowProgress(0);
	SetTextf("Initializing Devices\n");
	
	gprintf("\tMounting SD...%s\n", DeviceHandler::Instance()->MountSD() ? "done" : "failed");
	ShowProgress(1);
	
	gprintf("\tMounting USB...%s\n", DeviceHandler::Instance()->MountAllUSB() ? "done" : "failed");
	ShowProgress(2);
	
	SetTextf("Loading config files\n");
	gprintf("\tLoading config...%s\n", Settings.Load() ? "done" : "failed");
	gprintf("\tLoading language...%s\n", Settings.LoadLanguage(Settings.LanguageFile, CONSOLE_DEFAULT) ? "done" : "failed");
	ShowProgress(3);
	
	ParseArguments(argc, argv);
	ShowProgress(4);
	
	SetTextf("Initialize System\n");
	if(!Settings.UseIOS58 && Settings.cIOS != IOS_GetVersion())
	{
		gprintf("Loading cIOS %i\n", Settings.cIOS);
		
		DeviceHandler::DestroyInstance();
		
		//! Loading now the cios setup in the settings
		IosLoader::LoadAppCios();
		
		ShowProgress(5);
		
		DeviceHandler::Instance()->MountSD();
		DeviceHandler::Instance()->MountAllUSB();
	}
	
	ShowProgress(6);
	
	//! Init the rest of the System
	Sys_Init();
	SetupPads();
	InitAudio();
	setlocale(LC_CTYPE, "C-UTF-8");
	setlocale(LC_MESSAGES, "C-UTF-8");
	Nand::Startup();
	
	SetTextf("Loaded cIOS %i R%i\n", IOS_GetVersion(), IOS_GetRevision());
	Sys_CheckSneek();
	usleep(50000);
	
	ShowProgress(7);
	
	gprintf("Getting Saves List\n");
	gprintf("\tFound %i Saves\n", SaveList.Get());
	
	ShowProgress(8);
	
	gprintf("Getting Miis List\n");
	gprintf("\tFound %i Miis\n", MiiList.Get());
	
	ShowProgress(9);
	
	if(Settings.EmuNand && !Settings.Sneek)
	{
		gprintf("Getting EmuNand Lists\n");
		gprintf("\tFound %i EmuSaves\n", EmuList.Get());
		gprintf("\tFound %i EmuMiis\n", EmuMiiList.Get());
		
		ShowProgress(10);
	}
	
	SetTextf("Loading Theme\n");
	gprintf("\tLoading font...%s\n", Theme::LoadFont(Settings.UseSystemFont ? "" : Settings.CustomFontPath) ? "done" : "failed (using default)");
	gprintf("\tLoading theme...%s\n", Theme::Load(Settings.Theme) ? "done" : "failed (using default)");
	
	ShowProgress(11);
	
	if (!DeviceHandler::Instance()->SD_Inserted() &&
		!DeviceHandler::Instance()->USB0_Inserted() &&
		!DeviceHandler::Instance()->USB1_Inserted())
		return false;

	return true;
}

void StartUpProcess::Draw()
{
	background->Draw();
	GXImage->Draw();
	titleTxt->Draw();
	messageTxt->Draw();
	
	if(showProgress)
	{
		ProgressBarEmpty->Draw();
		ProgressBar->Draw();
		ProgressBarOutline->Draw();
	}
	
	Menu_Render();
}
