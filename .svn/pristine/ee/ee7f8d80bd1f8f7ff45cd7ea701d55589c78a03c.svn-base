/****************************************************************************
 * Copyright (C) 2011
 * by Dj_Skual
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
 * CFileList.hpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#ifndef _CFILELIST_HPP_
#define _CFILELIST_HPP_

#include <unistd.h>
#include <gccore.h>
#include <vector>

#define MAXJOLIET 255

#include "../Text/wstring.hpp"

typedef struct _FileStruct
{
	std::string filename;
	std::string displayname;
	bool isdir;
	bool ismii;
	bool issave;
	bool isdatabin;
	bool isNotInstalled;
	
} FileStruct;


class CFileList
{
	public:
		CFileList() { ResetFull(); };
		~CFileList() { ResetFull(); ResetFiltered(); };
		
		int SetFile(int i,
					std::string filename,
					std::string displayname,
					bool isdir,
					bool ismii,
					bool issave,
					bool isdatabin,
					bool isNotInstalled);
		int SetFilteredFile(int i,
							std::string filename,
							std::string displayname,
							bool isdir,
							bool ismii,
							bool issave,
							bool isdatabin,
							bool isNotInstalled);
		int FilterList(const wchar_t * fileFilter = NULL);
		void ResetFull();
		void ResetFiltered();
		void AddFullFile();
		void AddFilteredFile();
		void RemoveEntry(int ind);
		int GetFullCount() { return FilesFull.size(); };
		int GetFilteredCount() { return FilesFiltered.size(); };
		std::string GetFilename(int ind);
		std::string GetDisplayname(int ind);
		bool GetIsDir(int ind);
		bool GetIsMii(int ind);
		bool GetIsSave(int ind);
		bool GetIsDataBin(int ind);
		bool GetIsNotInstalled(int ind);
		void LoadTitlesFromGameTDB();
		
		void SetCurrentFilter(const wchar_t * fileFilter = NULL) { FileFilter = fileFilter; }
		const wchar_t * GetCurrentFilter() const { return FileFilter.c_str(); }
		const wchar_t * GetAvailableSearchChars() const { return AvailableSearchChars.c_str(); }
		
	protected:
		
		wString AvailableSearchChars;
		wString FileFilter;
		
		std::vector<FileStruct *> FilesFull;
		std::vector<FileStruct *> FilesFiltered;
};

extern CFileList FileList;

#endif
