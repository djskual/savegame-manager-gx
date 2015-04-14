/****************************************************************************
 * Copyright (C) 2010
 * by Dimok
 * modified by Dj_Skual
 * for SaveGame Manager Gx 2011
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
#include <string.h>
#include <malloc.h>
#include "FileExtensions.h"
#include "../Tools/StringTools.h"

CFilesExtensions::CFilesExtensions()
{
	AudioFiles = NULL;
	ImageFiles = NULL;
	ArchiveFiles = NULL;
	HomebrewFiles = NULL;
	FontFiles = NULL;
	LanguageFiles = NULL;
	ThemeFiles = NULL;
	MiiFiles = NULL;
	BinaryFiles = NULL;

	SetDefault();
}

CFilesExtensions::~CFilesExtensions()
{
	Clear();
}

void CFilesExtensions::Clear()
{
	if(AudioFiles)
		free(AudioFiles);
	AudioFiles = NULL;

	if(ImageFiles)
		free(ImageFiles);
	ImageFiles = NULL;

	if(ArchiveFiles)
		free(ArchiveFiles);
	ArchiveFiles = NULL;

	if(HomebrewFiles)
		free(HomebrewFiles);
	HomebrewFiles = NULL;

	if(FontFiles)
		free(FontFiles);
	FontFiles = NULL;

	if(LanguageFiles)
		free(LanguageFiles);
	LanguageFiles = NULL;

	if(ThemeFiles)
		free(ThemeFiles);
	ThemeFiles = NULL;

	if(MiiFiles)
		free(MiiFiles);
	MiiFiles = NULL;

	if(BinaryFiles)
		free(BinaryFiles);
	BinaryFiles = NULL;
}

void CFilesExtensions::SetDefault()
{
	Clear();

	AudioFiles = strdup(".mp3,.ogg,.wav,.pcm,.aif");
	ImageFiles = strdup(".png,.jpg,.jpeg,.jpe,.jfif,.bmp,.gif,.tga,.tif,.gd,.tpl");
	ArchiveFiles = strdup(".7z,.zip,.rar");
	HomebrewFiles = strdup(".elf,.dol");
	FontFiles = strdup(".ttf");
	LanguageFiles = strdup(".lang");
	ThemeFiles = strdup(".them");
	MiiFiles = strdup(".miigx");
	BinaryFiles = strdup(".bin");
}

void CFilesExtensions::SetAudio(const char * ext)
{
	if(AudioFiles)
		free(AudioFiles);

	if(!ext)
	{
		AudioFiles = NULL;
		return;
	}

	AudioFiles = strdup(ext);
}

void CFilesExtensions::SetImage(const char * ext)
{
	if(ImageFiles)
		free(ImageFiles);

	if(!ext)
	{
		ImageFiles = NULL;
		return;
	}

	ImageFiles = strdup(ext);
}

void CFilesExtensions::SetArchive(const char * ext)
{
	if(ArchiveFiles)
		free(ArchiveFiles);

	if(!ext)
	{
		ArchiveFiles = NULL;
		return;
	}

	ArchiveFiles = strdup(ext);
}

void CFilesExtensions::SetHomebrew(const char * ext)
{
	if(HomebrewFiles)
		free(HomebrewFiles);

	if(!ext)
	{
		HomebrewFiles = NULL;
		return;
	}

	HomebrewFiles = strdup(ext);
}

void CFilesExtensions::SetFont(const char * ext)
{
	if(FontFiles)
		free(FontFiles);

	if(!ext)
	{
		FontFiles = NULL;
		return;
	}

	FontFiles = strdup(ext);
}

void CFilesExtensions::SetLanguageFiles(const char * ext)
{
	if(LanguageFiles)
		free(LanguageFiles);

	if(!ext)
	{
		LanguageFiles = NULL;
		return;
	}

	LanguageFiles = strdup(ext);
}

void CFilesExtensions::SetThemeFiles(const char * ext)
{
	if(ThemeFiles)
		free(ThemeFiles);

	if(!ext)
	{
		ThemeFiles = NULL;
		return;
	}

	ThemeFiles = strdup(ext);
}

void CFilesExtensions::SetMiiFiles(const char * ext)
{
	if(MiiFiles)
		free(MiiFiles);

	if(!ext)
	{
		MiiFiles = NULL;
		return;
	}

	MiiFiles = strdup(ext);
}

void CFilesExtensions::SetBinaryFiles(const char * ext)
{
	if(BinaryFiles)
		free(BinaryFiles);

	if(!ext)
	{
		BinaryFiles = NULL;
		return;
	}

	BinaryFiles = strdup(ext);
}

int CFilesExtensions::CompareAudio(const char * ext)
{
	if(!ext || !AudioFiles)
		return -1;

	return strtokcmp(ext, AudioFiles, ",");
}

int CFilesExtensions::CompareImage(const char * ext)
{
	if(!ext || !ImageFiles)
		return -1;

	return strtokcmp(ext, ImageFiles, ",");
}

int CFilesExtensions::CompareArchive(const char * ext)
{
	if(!ext || !ArchiveFiles)
		return -1;

	return strtokcmp(ext, ArchiveFiles, ",");
}

int CFilesExtensions::CompareHomebrew(const char * ext)
{
	if(!ext || !HomebrewFiles)
		return -1;

	return strtokcmp(ext, HomebrewFiles, ",");
}

int CFilesExtensions::CompareFont(const char * ext)
{
	if(!ext || !FontFiles)
		return -1;

	return strtokcmp(ext, FontFiles, ",");
}

int CFilesExtensions::CompareLanguageFiles(const char * ext)
{
	if(!ext || !LanguageFiles)
		return -1;

	return strtokcmp(ext, LanguageFiles, ",");
}

int CFilesExtensions::CompareThemeFiles(const char * ext)
{
	if(!ext || !ThemeFiles)
		return -1;

	return strtokcmp(ext, ThemeFiles, ",");
}

int CFilesExtensions::CompareMiiFiles(const char * ext)
{
	if(!ext || !MiiFiles)
		return -1;

	return strtokcmp(ext, MiiFiles, ",");
}

int CFilesExtensions::CompareBinaryFiles(const char * ext)
{
	if(!ext || !BinaryFiles)
		return -1;

	return strtokcmp(ext, BinaryFiles, ",");
}
