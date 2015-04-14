/****************************************************************************
 * Copyright (C) 2010
 * by Dimok
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
 *
 * for WiiXplorer 2010
 ***************************************************************************/
#include <gctypes.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <malloc.h>

#include "../SoundOperations/MusicPlayer.h"
#include "../Prompts/PromptWindows.h"
#include "../Themes/CTheme.h"
//#include "Prompts/ProgressWindow.h"
#include "../BootHomebrew/BootHomebrew.h"
#include "FileStartUp.h"
//#include "../TextOperations/TextEditor.h"
//#include "../TextOperations/PDFViewer.hpp"
//#include "../TextOperations/FontSystem.h"
//#include "../FileOperations/fileops.h"
//#include "../ImageOperations/ImageViewer.h"
//#include "../Controls/Application.h"
//#include "../FileExtensions.h"
//#include "../Tools/uncompress.h"

int FileStartUp(const char *filepath)
{
	if(!filepath)
		return -1;

	char *fileext = strrchr(filepath, '.');
	char *filename = strrchr(filepath, '/')+1;

	if(Settings.FileExtensions.CompareHomebrew(fileext) == 0)
	{
		int choice = 0;
		if(strcasecmp(fileext, ".dol") == 0)
			choice = WindowPrompt(tr("Do you want to boot:"), filename, tr("Wii Homebrew"), tr("GC Homebrew"), tr("Cancel"));
		else
			choice = WindowPrompt(tr("Do you want to boot:"), filename, tr("Yes"), tr("No"));
		if(choice)
		{
			int ret = LoadHomebrew(filepath);
			if(ret >= 0)
				AddBootArgument(filepath);

			//if(choice == 1)
				BootHomebrew();
			/*else if(choice == 2)
				BootGameCubeHomebrew();*/
		}

		return 0;
	}
	/*else if(Settings.FileExtensions.CompareImage(fileext) == 0)
	{
		int choice = WindowPrompt(filename, tr("How do you want to open the file?"), tr("ImageViewer"), tr("ImageConverter"), tr("Cancel"));
		if (choice == 1)
		{
			ImageViewer * ImageVwr = new ImageViewer(filepath);
			ImageVwr->SetAlignment(ALIGN_CENTER | ALIGN_MIDDLE);
			Application::Instance()->SetUpdateOnly(ImageVwr);
			Application::Instance()->Append(ImageVwr);
		}
		else if(choice == 2)
		{
			ImageConverterGui * ImageConv = new ImageConverterGui(filepath);
			ImageConv->SetAlignment(ALIGN_CENTER | ALIGN_MIDDLE);
			ImageConv->DimBackground(true);
			Application::Instance()->SetUpdateOnly(ImageConv);
			Application::Instance()->Append(ImageConv);
		}
	}*/
	else if(Settings.FileExtensions.CompareAudio(fileext) == 0)
	{
		//loadMusic:
		
		int choice = WindowPrompt(filename, tr("Do you want to playback this file?"), tr("Yes"), tr("Add to Playlist"), tr("Parse Directory"), tr("Cancel"));
		if(choice == 1)
		{
			MusicPlayer::Instance()->ClearList();
			MusicPlayer::Instance()->AddEntrie(filepath);
			if(!MusicPlayer::Instance()->Play(0))
				WindowPrompt(tr("Could not load file."), tr("Possible unsupported format."), tr("OK"));
		}
		else if(choice == 2)
		{
			MusicPlayer::Instance()->AddEntrie(filepath);
		}
		else if(choice == 3)
		{
			bool result = MusicPlayer::Instance()->ParsePath(filepath);
			if(!result)
				ShowError(tr("Could not parse directory."));
		}
	}
	/*else if(Settings.FileExtensions.CompareBinary(fileext) == 0)
	{
		FILE * f = fopen(filepath, "rb");
		if(!f)
			return -1;

		u32 magic = 0;
		fread(&magic, 1, 4, f);
		if(magic == 'IMD5')
		{
			fseek(f, 0, SEEK_END);
			int filesize = ftell(f);
			u8 * buffer = (u8 *) malloc(filesize);
			rewind(f);
			fread(buffer, 1, filesize, f);
			fclose(f);
			magic = CheckIMD5Type(buffer, filesize);
			free(buffer);
		}
		fclose(f);

		if(magic == 'RIFF' || magic == 'FORM' || magic == 'BNS ')
		{
			goto loadMusic;
		}

		if(magic == 0x55AA382D *//* U.8- *//*)
		{
			return ARCHIVE;
		}

		int choice = WindowPrompt(filename, tr("Uknown format. Open this file in the TextEditor?"), tr("Yes"), tr("No"));
		if(choice)
			TextEditor::LoadFile(filepath);
	}*/
	/*else if(Settings.FileExtensions.CompareArchive(fileext) == 0)
	{
		return ARCHIVE;
	}*/
	else if(Settings.FileExtensions.CompareLanguageFiles(fileext) == 0)
	{
		int choice = WindowPrompt(tr("How do you want to load this language file?"), filename, tr("Load"), tr("Text Editor"), tr("Cancel"));
		if(choice == 1)
		{
			Settings.LoadLanguage(filepath);
			return RELOAD_BROWSER;
		}
		/*else if(choice == 2)
			TextEditor::LoadFile(filepath);*/
	}
	else if(Settings.FileExtensions.CompareFont(fileext) == 0)
	{
		int choice = WindowPrompt(tr("Do you want to change the font?"), filename, tr("Yes"), tr("No"));
		if(choice)
		{
			snprintf(Settings.CustomFontPath, sizeof(Settings.CustomFontPath), "%s", filepath);
			bool result = Theme::LoadFont(Settings.CustomFontPath);
			if(result)
				WindowPrompt(tr("Fontpath changed."), tr("The new font is loaded."), tr("OK"));
			else
				WindowPrompt(tr("Fontpath changed."), tr("The new font could not be loaded."), tr("OK"));
		}
	}
	//! Those have to be made extra and put before MPlayerCE launch
	//! to launch them inside WiiXplorer.
	/*else if(Settings.FileExtensions.CompareWiiXplorerMovies(fileext) == 0)
	{
		if(ProgressWindow::Instance()->IsRunning())
		{
			int choice = WindowPrompt(tr("Currently a process is running."), tr("Running a Video could slowdown the process or freeze the app. Do you want to continue?"), tr("Yes"), tr("Cancel"));
			if(choice == 0)
				return 0;
		}
		WiiMovie * Video = new WiiMovie(filepath);
		Video->SetAlignment(ALIGN_CENTER | ALIGN_MIDDLE);
		Video->SetVolume(Settings.MusicVolume);
		Application::Instance()->Append(Video);
		Application::Instance()->SetUpdateOnly(Video);
		Video->Play();
	}*/
	/*else if(Settings.FileExtensions.CompareVideo(fileext) == 0)
	{
		int choice = WindowPrompt(tr("How do you want to launch this file?"), filename, tr("WiiMC"), tr("MPlayerCE"), tr("Cancel"));
		if(choice == 1)
		{
			 int ret = LoadHomebrew(Settings.WiiMCPath);
			 if(ret >= 0)
			 {
				 AddBootArgument(Settings.WiiMCPath);
				 CreateWiiMCArguments(filepath);
				 BootHomebrew();
			 }
		}
		else if(choice == 2)
		{
			 int ret = LoadHomebrew(Settings.MPlayerPath);
			 if(ret >= 0)
			 {
				 AddBootArgument(Settings.MPlayerPath);
				 CreateMPlayerArguments(filepath);
				 BootHomebrew();
			 }
		}
	}*/
	else
	{
		/*int choice = */WindowPrompt(filename, tr("Do you want to open this file in TextEditor?"), tr("Yes"), tr("No"));
		/*if(choice)
			TextEditor::LoadFile(filepath);*/
	}

	return 0;
}
