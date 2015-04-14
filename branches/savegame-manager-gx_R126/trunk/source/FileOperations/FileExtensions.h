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
#ifndef FILE_EXTENSIONS_H_
#define FILE_EXTENSIONS_H_

class CFilesExtensions
{
	public:
		CFilesExtensions();
		~CFilesExtensions();
		void SetDefault();
		
		//!< Set the extensions
		void SetAudio(const char * ext);
		void SetImage(const char * ext);
		void SetArchive(const char * ext);
		void SetHomebrew(const char * ext);
		void SetFont(const char * ext);
		void SetLanguageFiles(const char * ext);
		void SetThemeFiles(const char * ext);
		void SetMiiFiles(const char * ext);
		void SetBinaryFiles(const char * ext);
		
		//!< Get the extensions
		const char * GetAudio() { return AudioFiles; };
		const char * GetImage() { return ImageFiles; };
		const char * GetArchive() { return ArchiveFiles; };
		const char * GetHomebrew() { return HomebrewFiles; };
		const char * GetFont() { return FontFiles; };
		const char * GetLanguageFiles() { return LanguageFiles; };
		const char * GetThemeFiles() { return ThemeFiles; };
		const char * GetMiiFiles() { return MiiFiles; };
		const char * GetBinaryFiles() { return BinaryFiles; };
		
		//!< Compare the extensions to one given extension
		int CompareAudio(const char * ext);
		int CompareImage(const char * ext);
		int CompareArchive(const char * ext);
		int CompareHomebrew(const char * ext);
		int CompareFont(const char * ext);
		int CompareLanguageFiles(const char * ext);
		int CompareThemeFiles(const char * ext);
		int CompareMiiFiles(const char * ext);
		int CompareBinaryFiles(const char * ext);
		
	protected:
		void Clear();
		char * AudioFiles;
		char * ImageFiles;
		char * ArchiveFiles;
		char * HomebrewFiles;
		char * FontFiles;
		char * LanguageFiles;
		char * ThemeFiles;
		char * MiiFiles;
		char * BinaryFiles;
};

#endif
